#include "BinanceBotApplication.h"
#include <iostream>
#include <string>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

namespace asio = boost::asio;

BinanceBotApplication::BinanceBotApplication(const std::string& apiKey, const std::string& secretKey)
    : apiKey(apiKey), secretKey(secretKey)
{

    // This holds the root certificate used for verification
    load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_peer);

}

void BinanceBotApplication::sendOrderBookResponse() {
    auto instance = new AsyncHttpsSession(
        net::make_strand(ioc), dynamic_cast<ResponseParser*>(this),
        ctx );

    auto ses = std::shared_ptr<AsyncHttpsSession>(instance);

    boost::url_view base_api{"https://api.binance.com/api/v3/"};

    
    std::string selectedSymbol = "PEPEUSDT"; 

    boost::url url_("depth");

    std::unordered_map<std::string, std::string> header;

    header.insert({ "X-MBX-APIKEY", apiKey });

    url_.params().append({ "symbol", selectedSymbol });
    //hata
    ses->run(AsyncHttpsSession::make_url(base_api, url_), http::verb::get, header);
    
   
}

void BinanceBotApplication::parseOrderBookResponse(const std::string &apiResponse) {
        try {
            json jsonResponse = json::parse(apiResponse);

            json bids = jsonResponse["bids"];
            json asks = jsonResponse["asks"];

            for (const auto& bid : bids) {
                double price = bid[0]; 
                double quantity = bid[1]; 

                
            }

            for (const auto& ask : asks) {
                double price = ask[0]; 
                double quantity = ask[1];              
            }
        }
        catch (const json::exception& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
        }
    

}
void BinanceBotApplication::sendTradeFeeRequest() {
    auto ses = std::make_shared<AsyncHttpsSession>(
        net::make_strand(ioc),
        ctx
    );

    boost::url_view base_api{"https://api.binance.com/sapi/v1/asset/"};


    std::string selectedSymbol = "PEPEUSDT";
    boost::url url_("tradeFee");
    std::unordered_map<std::string, std::string> header;
    header.insert({ "X-MBX-APIKEY", apiKey });
    url_.params().append({ "symbol", selectedSymbol });
    std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    url_.params().append({ "timestamp", timestamp });

    auto signature = hmac_sha256(url_.query(), secretKey);
    url_.params().append({ "signature", signature });
    ses->run(AsyncHttpsSession::make_url(base_api, url_), http::verb::get, header);
    //BOÞ GELÝYOR   
    std::string apiResponse = ses->getResponseBody();
    TradeFee.push_back(apiResponse);

    parseTradeFeeRequest(TradeFee);


}
void BinanceBotApplication::parseTradeFeeRequest(std::vector<std::string> TradeFee) {
    try {
        for (const std::string& apiResponse : TradeFee) {
        json jsonResponse = json::parse(apiResponse);
        double makerCommission = jsonResponse["makerCommission"];
        double takerCommission = jsonResponse["takerCommission"];

            
        
        }
    }
    catch (const json::exception& e) {
        // Handle JSON parsing errors
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }

}

std::string BinanceBotApplication::deleteNewOrder() {
    auto ses = std::make_shared<AsyncHttpsSession>(
        net::make_strand(ioc),
        ctx
    );
    boost::url_view base_api{"https://api.binance.com/api/v3/"};

    boost::url url_("delete");

    std::unordered_map<std::string, std::string> header;
    std::string selectedSymbol = "PEPEUSDT";

    header.insert({ "X-MBX-APIKEY",apiKey });


    url_.params().append({ "symbol", selectedSymbol });
    std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    url_.params().append({ "timestamp", timestamp });
    auto signature = hmac_sha256(url_.query(), secretKey);
    url_.params().append({ "signature", signature });
    ses->run(AsyncHttpsSession::make_url(base_api, url_), http::verb::delete_, header);
    OutputDebugString("DURDUR");

    // Run the I/O service. The call will return when
    // the get operation is complete.
    ioc.run();

    return "";

}
std::string BinanceBotApplication::testNewOrder()
{
    auto ses = std::make_shared<AsyncHttpsSession>(
        net::make_strand(ioc),
        ctx
    );
    boost::url_view base_api{"https://api.binance.com/api/v3/"};

    boost::url url_("order");

    std::unordered_map<std::string, std::string> header;
    std::string selectedSymbol = "PEPEUSDT";

    header.insert({ "X-MBX-APIKEY",apiKey });
    //std::string newPrice = rowData.price;
    url_.params().append({ "symbol", selectedSymbol });
    url_.params().append({ "side", "SELL" });
    url_.params().append({ "type", "MARKET" });
    url_.params().append({ "quantity", "1" });
    std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    url_.params().append({ "timestamp", timestamp });

    auto signature = hmac_sha256(url_.query(), secretKey);
    url_.params().append({ "signature", signature });

    ses->run(AsyncHttpsSession::make_url(base_api, url_), http::verb::post, header);

    // Run the I/O service. The call will return when
    // the get operation is complete.
    ioc.run();

    return "";
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

void BinanceBotApplication::parseResponse(const std::string& response)
{
    parseOrderBookResponse(response);
}









