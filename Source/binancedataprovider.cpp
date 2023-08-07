#include "binancedataprovder.h"
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <boost/beast/http.hpp>




int binancedataproivder::run(int argc, char** argv){
    try
    {
        
        // Check command line arguments.
        if (argc != 4)
        {
            std::cerr <<
                "Usage: websocket-client-sync-ssl <host> <port> <text>\n" <<
                "Example:\n" <<
                "    websocket-client-sync-ssl echo.websocket.org 443 \"Hello, world!\"\n";
            return EXIT_FAILURE;
        }
        std::string host = argv[1];
        auto const  port = argv[2];
        auto const  text = argv[3];

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

        // Perform the websocket handshake
        ws.handshake(host, "/");
       
        // Send the message
        ws.write(net::buffer(std::string(text)));

        // This buffer will hold the incoming message

        while (true) {
            beast::flat_buffer buffer;
            ws.read(buffer);
            std::string message = beast::buffers_to_string(buffer.data());
            parseWebSocketMessage(message);
        }

        // Close the WebSocket connection
        //ws.close(websocket::close_code::normal);

        // If we get here then the connection is closed gracefully

        // The make_printable() function helps print a ConstBufferSequence
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
