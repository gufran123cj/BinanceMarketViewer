/*
  ==============================================================================

    AsyncHttpsSession.cpp
    Created: 28 Aug 2023 9:37:12pm
    Author:  admin

  ==============================================================================
*/

#include "AsyncHttpsSession.h"

// Report a failure
void
failAsyncHttpsSession(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

AsyncHttpsSession::AsyncHttpsSession(net::any_io_executor ex, ssl::context& ctx)
    : resolver_(ex)
    , stream_(ex, ctx)
{
}

// Start the asynchronous operation

void AsyncHttpsSession::run(boost::url url_, http::verb method, std::unordered_map<std::string, std::string> headers)
{
    std::string const host(url_.host());
    std::string const service = url_.has_port() //
        ? url_.port()
        : (url_.scheme_id() == boost::urls::scheme::https) //
        ? "https"
        : "http";
    url_.remove_origin(); // becomes req_.target()
    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(stream_.native_handle(), host.c_str()))
    {
        beast::error_code ec{ static_cast<int>(::ERR_get_error()), net::error::get_ssl_category() };
        std::cerr << ec.message() << "\n";
        return;
    }


    // Set up an HTTP GET request message
    req_.version(11);
    req_.method(method);
    req_.target(url_);
    for (auto& header : headers)
    {
        req_.set(header.first, header.second);
    }
    req_.set(http::field::host, host);
    req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Look up the domain name
    resolver_.async_resolve(
        host,
        service,
        beast::bind_front_handler(
            &AsyncHttpsSession::on_resolve,
            shared_from_this()));
}

void AsyncHttpsSession::on_resolve(beast::error_code ec, tcp::resolver::results_type results)
{
    if (ec)
        return failAsyncHttpsSession(ec, "resolve");

    // Set a timeout on the operation
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(stream_).async_connect(
        results,
        beast::bind_front_handler(
            &AsyncHttpsSession::on_connect,
            shared_from_this()));
}

void AsyncHttpsSession::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
{
    if (ec)
        return failAsyncHttpsSession(ec, "connect");

    // Perform the SSL handshake
    stream_.async_handshake(
        ssl::stream_base::client,
        beast::bind_front_handler(
            &AsyncHttpsSession::on_handshake,
            shared_from_this()));
}

void AsyncHttpsSession::on_handshake(beast::error_code ec)
{
    if (ec)
        return failAsyncHttpsSession(ec, "handshake");

    // Set a timeout on the operation
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    // Send the HTTP request to the remote host
    http::async_write(stream_, req_,
        beast::bind_front_handler(
            &AsyncHttpsSession::on_write,
            shared_from_this()));
}

void AsyncHttpsSession::on_write(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return failAsyncHttpsSession(ec, "write");

    // Receive the HTTP response
    http::async_read(stream_, buffer_, res_,
        beast::bind_front_handler(
            &AsyncHttpsSession::on_read,
            shared_from_this()));
}

void AsyncHttpsSession::on_read(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return failAsyncHttpsSession(ec, "read");

    // Write the message to standard out
    std::cout << res_ << std::endl;
    OutputDebugString(res_.body().c_str());
    // calisir hale geldi istek, bakiye yetersiz oldugu icin basarisiz oldu
    // cok ucuz bi coin bulup onda test edebilirsin
    // {"code":-2010,"msg":"Account has insufficient balance for requested action."}
    // 
    // Set a timeout on the operation
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    // Gracefully close the stream
    stream_.async_shutdown(
        beast::bind_front_handler(
            &AsyncHttpsSession::on_shutdown,
            shared_from_this()));
}

void AsyncHttpsSession::on_shutdown(beast::error_code ec)
{
    if (ec == net::error::eof)
    {
        // Rationale:
        // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
        ec = {};
    }
    if (ec)
        return failAsyncHttpsSession(ec, "shutdown");

    // If we get here then the connection is closed gracefully
}
