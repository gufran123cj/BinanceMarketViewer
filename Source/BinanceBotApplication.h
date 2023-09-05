#pragma once
#ifndef BinanceBotApplication_h
#define BinanceBotApplication_h
#include <vector>
#include <chrono>
#include "AsyncHttpsSession.h"
#include "newnw.h"
#include <cstdlib>
#include <iostream>
#include <string>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class BinanceBotApplication
{
public:

    BinanceBotApplication(const std::string& apiKey, const std::string& secretKey);

    std::string placeOrder(const std::string& symbol, const std::string& side, double quantity, double price);
    std::string testNewOrder();


    size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata);
    std::string hmac_sha256(const std::string& data, const std::string& key);
   
private:
    // The io_context is required for all I/O
    net::io_context ioc;
    // The SSL context is required, and holds certificates
    ssl::context ctx{ssl::context::tlsv12_client};
    juce::String symbol;
    std::string apiKey;
    std::string secretKey;
   
};
#endif // !BinanceBotApplication_h

