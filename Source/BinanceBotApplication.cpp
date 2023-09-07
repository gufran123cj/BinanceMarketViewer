#include "BinanceBotApplication.h"
#include <iostream>
#include <string>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>

namespace asio = boost::asio;

BinanceBotApplication::BinanceBotApplication(const std::string& apiKey, const std::string& secretKey, juce::String clickedSymbol, std::vector<std::string> pricehistory1, std::string lastprice1)
    : apiKey(apiKey), secretKey(secretKey), selectedSymbol(clickedSymbol),price(pricehistory1),finalprice(lastprice1)
{

    // This holds the root certificate used for verification
    load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_peer);

}


std::string BinanceBotApplication::placeOrder(const std::string& symbol, const std::string& side, double quantity, double price)
{
    std::string url = "https://api.binance.com/api/v3/order";
    std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    std::string query = "symbol=" + symbol + "&side=" + side + "&type=LIMIT&timeInForce=GTC&quantity=" + std::to_string(quantity) + "&price=" + std::to_string(price) + "&timestamp=" + timestamp;
    std::string signature = hmac_sha256(query, apiKey);
    std::string response; //= post(url, query + "&signature=" + signature, { "X-MBX-APIKEY: " + secretKey });
    return response;
}
std::string BinanceBotApplication::testNewOrder()
{

    // Launch the asynchronous operation
    // The session is constructed with a strand to
    // ensure that handlers do not execute concurrently.
    auto ses = std::make_shared<AsyncHttpsSession>(
        net::make_strand(ioc),
        ctx
    );
    boost::url_view base_api{"https://api.binance.com/api/v3/"};

    boost::url url_("order");

    std::unordered_map<std::string, std::string> header;

    header.insert({ "X-MBX-APIKEY",apiKey });
    //std::string newPrice = rowData.price;
    int index = -1;
    for (size_t i = 0; i < price.size(); ++i) {
        if (finalprice == price[i]) {
            index = static_cast<int>(i);
            break;
        }
    }
    if (index != -1) {
        int finalpriceID = index;
        int targetIndex = finalpriceID + 5;
        int targetIndex1 = finalpriceID - 5;
        std::string stringsymbol = selectedSymbol.toStdString();
        // Eðer hedef indeks pricehistory vektörünün sýnýrlarý içindeyse ve hedef deðer ile newPrice aynýysa satým yap
        if (targetIndex < price.size() && finalprice == price[targetIndex]) {
            url_.params().append({ "symbol", stringsymbol });
            url_.params().append({ "side", "SELL" });
            OutputDebugString("Satim");
        }
        if (targetIndex1 < price.size() && finalprice == price[targetIndex1]) {
            url_.params().append({ "symbol", stringsymbol });
            url_.params().append({ "side", "BUY" });
            OutputDebugString("alým");
        }
    }
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









