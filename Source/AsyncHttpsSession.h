/*
  ==============================================================================

    AsyncHttpsSession.h
    Created: 28 Aug 2023 9:37:11pm
    Author:  admin

  ==============================================================================
*/
#pragma once
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP SSL client, asynchronous
//
//------------------------------------------------------------------------------

#include "root_certificates.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost/url.hpp>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class ResponseParser
{
public:
    virtual void parseResponse(const std::string&) = 0;
};
//------------------------------------------------------------------------------

// Performs an HTTP GET and prints the response
class AsyncHttpsSession : public std::enable_shared_from_this<AsyncHttpsSession>
{
    tcp::resolver resolver_;
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_; // (Must persist between reads)
    http::request<http::empty_body> req_;
    http::response<http::string_body> res_;
    ResponseParser* parser;

public:
    
        explicit AsyncHttpsSession(
            net::any_io_executor ex,
            ResponseParser* parser,
            ssl::context& ctx);
    static boost::url make_url(boost::url_view base_api, boost::url_view method) {
        assert(!method.is_path_absolute());
        assert(base_api.data()[base_api.size() - 1] == '/');

        boost::urls::error_code ec;
        boost::url              url;
        resolve(base_api, method, url);
        return url;
    }

    // Start the asynchronous operation
    void
        run(boost::url url_, http::verb method, std::unordered_map<std::string, std::string> headers);

    void
        on_resolve(
            beast::error_code ec,
            tcp::resolver::results_type results);

    void
        on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type);

    void
        on_handshake(beast::error_code ec);

    void
        on_write(
            beast::error_code ec,
            std::size_t bytes_transferred);

    void
        on_read(
            beast::error_code ec,
            std::size_t bytes_transferred);

    void
        on_shutdown(beast::error_code ec);

    std::string getResponseBody();
};

//------------------------------------------------------------------------------