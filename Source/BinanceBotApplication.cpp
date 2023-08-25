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

void BinanceBotApplication::run()
{
    asio::ip::tcp::resolver resolver(ioContext);
    asio::ip::tcp::resolver::query query("api.binance.com", "443");
    auto endpoints = resolver.resolve(query);

    asio::ssl::context sslContext(asio::ssl::context::sslv23_client);
    asio::ssl::stream<asio::ip::tcp::socket> sslSocket(ioContext, sslContext);
    asio::connect(sslSocket.lowest_layer(), endpoints);

    sslSocket.handshake(asio::ssl::stream_base::client);

    // API iste�i olu�tur
    std::string queryString = "symbol=BTCUSDT";
    std::string timestamp = std::to_string(std::time(nullptr));
    std::string signature = createSignature(queryString + "&timestamp=" + timestamp);

    std::string request = "POST /api/v3/ticker/price?" + queryString + "&timestamp=" + timestamp + "&signature=" + signature + " HTTP/1.1\r\n"
        "Host: api.binance.com\r\n"
        "Connection: close\r\n"
        "\r\n";

    asio::write(sslSocket, asio::buffer(request));

    std::string response;
    char buffer[1024];
    while (std::size_t bytes = asio::read(sslSocket, asio::buffer(buffer, sizeof(buffer))))
    {
        response.append(buffer, bytes);
    }

    std::cout << "API Response:\n" << response << std::endl;
    startTrading();
}
void BinanceBotApplication::startTrading()
{
    while (true)
    {
        //double currentPrice = getLatestPrice(); 

        //// �imdilik 100 al�m de�eri t�klanansembolun 5 t�k kademesine g�re al�n�cak 
        //if (currentPrice < 100.0)
        //{
        //    buyAsset(currentPrice);
        //}
        //else if (currentPrice > 110.0)
        //{
        //    sellAsset(currentPrice);
        //}

        // Belirli bir s�re sonra tekrar kontrol etcek e�kilde ayarla
    }
}

std::string BinanceBotApplication::createSignature(const std::string& payload) const
{
    unsigned char hmacResult[EVP_MAX_MD_SIZE];
    unsigned int hmacResultLength;

    HMAC(
        EVP_sha256(),
        secretKey.c_str(), static_cast<int>(secretKey.size()),
        reinterpret_cast<const unsigned char*>(payload.c_str()), payload.size(),
        hmacResult, &hmacResultLength);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < hmacResultLength; ++i)
    {
        ss << std::setw(2) << static_cast<int>(hmacResult[i]);
    }

    return ss.str();
}
double getLatestPrice()
{
    // Binance API'den veriyi al

    // API yan�t�n� analiz ederek g�ncel fiyat� ��kartma i�lemi
    return 0;
}

void buyAsset(double price)
{
    // Al�m i�lemi yapma kodunu burada ger�ekle�tirin
    // API ile uygun �ekilde sipari� olu�turmal�s�n�z
    // �rne�in: createBuyOrder(amount, price);
}

void sellAsset(double price)
{
    // Sat�m i�lemi yapma kodunu burada ger�ekle�tirin
    // API ile uygun �ekilde sipari� olu�turmal�s�n�z
    // �rne�in: createSellOrder(amount, price);
}
