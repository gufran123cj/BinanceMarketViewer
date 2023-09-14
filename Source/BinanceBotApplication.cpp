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
    std::thread([this] {
        while (true)
        {
            ioc.run();

        }
    }).detach();

}

void BinanceBotApplication::sendOrderBookResponse() {
    
    auto ses = std::make_shared<AsyncHttpsSession>(net::make_strand(ioc),
        ctx);

    boost::url_view base_api{"https://api.binance.com/api/v3/"};
    ses->parser = this;
    ses->type = responseType::orderbook;

    
    std::string selectedSymbol = "PEPEUSDT"; 

    boost::url url_("depth");

    std::unordered_map<std::string, std::string> header;


    url_.params().append({ "symbol", selectedSymbol });
    ses->run(AsyncHttpsSession::make_url(base_api, url_), http::verb::get, header);

   
}

void BinanceBotApplication::parseOrderBookResponse(const std::string& apiResponse) {
    try {
        json jsonResponse = json::parse(apiResponse);

        if (!jsonResponse.is_object()) {
            std::cerr << "Error: The JSON is not an object." << std::endl;
            return;
        }

        // Check if "bids" and "asks" fields exist and are arrays
        if (jsonResponse.contains("bids") && jsonResponse.contains("asks") &&
            jsonResponse["bids"].is_array() && jsonResponse["asks"].is_array()) {

            // Extract and process bid and ask data
            

            for (const auto& bid : jsonResponse["bids"]) {
                if (bid.is_array() && bid.size() >= 2) {
                    bids.push_back({ bid[0].get<std::string>()});
                }
            }

            for (const auto& ask : jsonResponse["asks"]) {
                if (ask.is_array() && ask.size() >= 2) {
                    asks.push_back({ ask[0].get<std::string>()});
                }
            }
        }
        else {
            std::cerr << "Error: The 'bids' and/or 'asks' fields are missing or not arrays." << std::endl;
        }
    }
    catch (const json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}
void BinanceBotApplication::sendTradeFeeRequest() {
    auto ses = std::make_shared<AsyncHttpsSession>(net::make_strand(ioc),
        ctx);

    boost::url_view base_api{"https://api.binance.com/sapi/v1/asset/"};
    ses->parser = this;
    ses->type = responseType::tradefee;

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



}
void BinanceBotApplication::parseTradeFeeRequest(const std::string& apiResponse) {
    try {
        json jsonData = json::parse(apiResponse);

         makerCommission = jsonData[0]["makerCommission"];
         takerCommission = jsonData[0]["takerCommission"];

    }
    catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << std::endl;
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
std::string BinanceBotApplication::testNewOrder(/*std::string lastprice*/)
{
    makerCommission;//0.001,deftere ekliceksek ise
    takerCommission;//0.001,hızlı işlemlerde

    bids;//sell the stock vektör
    asks;//buy the stock vektör
    auto ses = std::make_shared<AsyncHttpsSession>(
        net::make_strand(ioc),
        ctx
    );
    //lastprice;//işlem yapılmak için seçilen değer
    boost::url_view base_api{"https://api.binance.com/api/v3/"};

    boost::url url_("order");

    std::unordered_map<std::string, std::string> header;
    std::string selectedSymbol = "PEPEUSDT";

    header.insert({ "X-MBX-APIKEY",apiKey });
    url_.params().append({ "symbol", selectedSymbol });


    std::string tempprice = "0.00000067";//şimdilik değeri
    std::string mcom = "0.001";
    std::string tcom = "0.001";
    int index = -1;
    int index1 = -1;
    double clickedprice = std::stod(tempprice);    
    double makerCommission = std::stod(mcom);
    double takerCommission = std::stod(tcom);

    int quantity = -1;

    //for buy
    for (size_t i = 0; i < bids.size(); ++i) {
        if (tempprice == bids[i]) {
            index = static_cast<int>(i);
            break;
        }
    }
    //for sell
    for (size_t i = 0; i < asks.size(); ++i) {
        if (tempprice == asks[i]) {
            index1 = static_cast<int>(i);
            break;
        }
    }
    if (index != -1) {
        int finalpriceID = index;
        int targetIndex = finalpriceID + 5;
        std::string finalprice = bids[targetIndex];
        if (targetIndex < bids.size() ) {
            double finalvalue = std::stod(finalprice);
            double x = clickedprice - finalvalue;
            if (x > 0) {
                quantity = (takerCommission / x) + 2;
                double kar = (x * quantity) - takerCommission;
                if ((x * quantity) > takerCommission){
                    std::string quantitystr = std::to_string(quantity);
                    url_.params().append({ "side", "SELL" });
                    url_.params().append({ "quantity", quantitystr });
                    OutputDebugString("Satim");
                    OutputDebugString("kar");
                }
            }
            else {
                std::cout << "The condition (x * quantity) > makerCommission cannot be satisfied." << std::endl;
            }

           
        }
       
    }
    if(index1!=-1){
        int finalpriceID = index1;
        int targetIndex = finalpriceID + 5;
        finalprice = asks[targetIndex];
        if (targetIndex < asks.size()  ) {

            double finalvalue = std::stod(finalprice);
            double x = finalvalue - clickedprice;
            if (x > 0) {
                quantity = takerCommission / x;
                double kar = (x * quantity) - takerCommission;
                if ((x * quantity) > takerCommission) {
                    std::string quantitystr = std::to_string(quantity);
                    url_.params().append({ "side", "BUY" });
                    url_.params().append({ "quantity", quantitystr });
                    OutputDebugString("alım");
                    OutputDebugString("kar");
                }

            }
            else {
                std::cout << "The condition (x * quantity) > makerCommission cannot be satisfied." << std::endl;
            }


        }
    }


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

void BinanceBotApplication::parseResponse(const std::string& response,responseType type)
{
    switch (type)
    {
    case responseType::tradefee:
        parseTradeFeeRequest(response);

        break;
    case responseType::orderbook:
        parseOrderBookResponse(response);

        break;
    default:
        break;
    }

}












