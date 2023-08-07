#include "data_provider.h"
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <boost/beast/http.hpp>




int data_provider::run(std::function <void(const std::string& message)> cb){
    try
    {
        
       
        std::thread([this,cb]() {
            std::string host = "stream.binance.com";
            auto const  port = "443";
            //auto const  text = argv[3];

            // The io_context is required for all I/O
            net::io_context ioc;

            // The SSL context is required, and holds certificates
            ssl::context ctx{ssl::context::tlsv12_client};

            // This holds the root certificate used for verification
            load_root_certificates(ctx);

            // These objects perform our I/O
            tcp::resolver resolver{ioc};
            websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};

            // Look up the domain name
            auto const results = resolver.resolve(host, port);

            // Make the connection on the IP address we get from a lookup
            auto ep = net::connect(get_lowest_layer(ws), results);

            // Set SNI Hostname (many hosts need this to handshake successfully)
            if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str()))
                throw beast::system_error(
                    beast::error_code(
                        static_cast<int>(::ERR_get_error()),
                        net::error::get_ssl_category()),
                    "Failed to set SNI Hostname");

            // Update the host_ string. This will provide the value of the
            // Host HTTP header during the WebSocket handshake.
            // See https://tools.ietf.org/html/rfc7230#section-5.4
            host += ':' + std::to_string(ep.port());

            // Perform the SSL handshake
            ws.next_layer().handshake(ssl::stream_base::client);

            // Set a decorator to change the User-Agent of the handshake
            ws.set_option(websocket::stream_base::decorator(
                [](websocket::request_type& req)
                {
                    req.set(http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-client-coro");
                }));

            // Create a WebSocket connection for each symbol
            std::vector<beast::websocket::stream<beast::tcp_stream>> websockets;
            // Perform the websocket handshake
            ws.handshake(host, "/ws/!miniTicker@arr");
            while (true)
            {
                beast::flat_buffer buffer;
                // Read a message into our buffer
                ws.read(buffer);
                std::string message = beast::buffers_to_string(buffer.data());
                cb(message);

            }
            
            
            }).detach();//bitmemesini beklemiyor 
      
      
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
