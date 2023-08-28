#pragma once
#ifndef BinanceBotApplication_h
#define BinanceBotApplication_h
#include <vector>
#include <chrono>
#include <openssl/hmac.h>
#include <nlohmann/json.hpp>
#include "root_certificates.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class BinanceBotApplication
{
public:
	BinanceBotApplication(boost::asio::io_context& ioContext, const std::string& apiKey, const std::string& secretKey);

    std::string placeOrder(const std::string& symbol, const std::string& side, double quantity, double price);
    std::string post(const std::string& url, const std::string& data, const std::vector<std::string>& headers);

    size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata);
    std::string hmac_sha256(const std::string& data, const std::string& key);
   
private:
    
    boost::asio::io_context& ioContext;
    std::string apiKey;
    std::string secretKey;
   
};
#endif // !BinanceBotApplication_h

