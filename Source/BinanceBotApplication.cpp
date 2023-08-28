#include "BinanceBotApplication.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

namespace asio = boost::asio;

BinanceBotApplication::BinanceBotApplication(boost::asio::io_context& ioContext, const std::string& apiKey, const std::string& secretKey)
    : ioContext(ioContext), apiKey(apiKey), secretKey(secretKey)
{
}
/*int BinanceBotApplication::getTickerPrice(const std::string& symbol)
{
    std::string url = "https://api.binance.com/api/v3/ticker/price?symbol=" + symbol;
    std::string response = get(url);
    return json::parse(response);
}

int BinanceBotApplication::getOrderBook(const std::string& symbol, int limit)
{
    std::string url = "https://api.binance.com/api/v3/depth?symbol=" + symbol + "&limit=" + std::to_string(limit);
    std::string response = get(url);
    return json::parse(response);
}*/

std::string BinanceBotApplication::placeOrder(const std::string& symbol, const std::string& side, double quantity, double price)
{
    std::string url = "https://api.binance.com/api/v3/order";
    std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    std::string query = "symbol=" + symbol + "&side=" + side + "&type=LIMIT&timeInForce=GTC&quantity=" + std::to_string(quantity) + "&price=" + std::to_string(price) + "&timestamp=" + timestamp;
    std::string signature = hmac_sha256(query, apiKey);
    std::string response = post(url, query + "&signature=" + signature, { "X-MBX-APIKEY: " + secretKey });
    return response;
}
std::string BinanceBotApplication::post(const std::string& url, const std::string& data, const std::vector<std::string>& headers)
{
    asio::io_context io_context;
    asio::ip::tcp::resolver resolver(io_context);
    asio::ip::tcp::socket socket(io_context);

    // Resolve endpoint
    auto endpoints = resolver.resolve(url, "http");
    asio::connect(socket, endpoints);

    // Prepare HTTP request
    beast::http::request<beast::http::string_body> request(beast::http::verb::post, url, 11);
    request.body() = data;
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.set(beast::http::field::content_type, "application/x-www-form-urlencoded");
    request.prepare_payload();

    // Send HTTP request
    beast::http::write(socket, request);

    // Receive HTTP response
    beast::flat_buffer buffer;
    beast::http::response<beast::http::string_body> response;
    beast::http::read(socket, buffer, response);

    socket.shutdown(asio::ip::tcp::socket::shutdown_both);
    socket.close();

    return response.body();
}
size_t BinanceBotApplication::writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    std::string* response = (std::string*)userdata;
    response->append(ptr, size * nmemb);
    return size * nmemb;
}

std::string BinanceBotApplication::hmac_sha256(const std::string& data, const std::string& key)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    HMAC_CTX* hmac = HMAC_CTX_new();
    HMAC_Init_ex(hmac, key.c_str(), key.length(), EVP_sha256(), NULL);
    HMAC_Update(hmac, (const unsigned char*)data.c_str(), data.length());
    unsigned int length = SHA256_DIGEST_LENGTH;
    HMAC_Final(hmac, hash, &length);
    HMAC_CTX_free(hmac);
    std::stringstream ss;
    ss << std::hex;
    for (unsigned char i : hash)
    {
        ss << std::setw(2) << std::setfill('0') << (int)i;
    }
    return ss.str();
}









