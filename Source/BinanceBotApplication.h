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
#include <nlohmann/json.hpp>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class BinanceBotApplication : public ResponseParser
{
public:

    BinanceBotApplication(const std::string& apiKey, const std::string& secretKey);

    void sendOrderBookResponse();

    void parseOrderBookResponse(const std::string &apiResponse);

    void sendTradeFeeRequest();

    void parseTradeFeeRequest(std::vector<std::string> TradeFee);

    std::string deleteNewOrder();
    std::string testNewOrder();
    std::string hmac_sha256(const std::string& data, const std::string& key);
   
private:
    std::string finalprice;
    std::vector<std::string> orderBookData;
    std::vector<std::string> TradeFee;
    using json = nlohmann::json;
    juce::String selectedSymbol;
    // The io_context is required for all I/O
    net::io_context ioc;
    // The SSL context is required, and holds certificates
    ssl::context ctx{ssl::context::tlsv12_client};
    juce::String symbol;
    std::string apiKey;
    std::string secretKey;
   

    // Inherited via ResponseParser
    virtual void parseResponse(const std::string&) override;

};
#endif // !BinanceBotApplication_h

