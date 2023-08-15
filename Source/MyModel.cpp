#include "MyModel.h"
#include <nlohmann/json.hpp>
#include <juce_graphics/juce_graphics.h>
//sembole çift týkla yeni sayfa aç (boþ pencere__include*  o sembolün bilgileri)(completed)
//robotun guisini oluþturucaz (baþlat, durdur, deðeri güncelle)
//button kullancaz(robotu baþlat (robot algoritmasý olucak)_bu robot alým yapýcak baþlattýðýmýz deðerden satarýz)
//symbol için sýralama ekle(completed)




void MyModel::run() {
    data_provider ses;

    ses.run([this](const std::string& res) {
        
        message = res;       
        parseresponse(message);

        if (view) {
            view->update();  
        }
        });
}

void MyModel::timerCallback()
{
    run();
}
void MyModel::parseresponse(std::string response)
{
    using json = nlohmann::json;
    try {
        // JSON parsing
        json parsed_json = json::parse(response);
        if (!parsed_json.is_array()) {
            std::cerr << "Error: The JSON is not an array of objects." << std::endl;
            return;
        }
        // Parse JSON and update the vectors
        
        symbols.clear();
        prices.clear();
        open_prices.clear();
        high_price.clear();
        low_price.clear();
        total_trade_base.clear();
        total_trade_quote.clear();
        data.push_back(response);
        int count = 0;
        for (const auto& item : parsed_json) {

                if (item.is_object()) {
                        
                        symbols.push_back(item["s"]);
                        prices.push_back(item["c"]);
                        open_prices.push_back(item["o"]);
                        high_price.push_back(item["h"]);
                        low_price.push_back(item["l"]);
                        total_trade_base.push_back(item["v"]);
                        total_trade_quote.push_back(item["q"]);
                        std::string parsedElement = R"({"e":")" + item["e"].get<std::string>() + R"(","E":)" +
                            std::to_string(item["E"].get<long>()) + R"(,"s":")" +
                            item["s"].get<std::string>() + R"(","c":")" +
                            item["c"].get<std::string>() + R"(","o":")" +
                            item["o"].get<std::string>() + R"(","h":")" +
                            item["h"].get<std::string>() + R"(","l":")" +
                            item["l"].get<std::string>() + R"(","v":")" +
                            item["v"].get<std::string>() + R"(","q":")" +
                            item["q"].get<std::string>() + "\"}";

                        parsedElements.push_back(parsedElement);
                }
                else {
                    std::cerr << "Error: One of the items is not a JSON object." << std::endl;
                    return;
                }
        }
        
        // Sembol adýna göre verileri sýrala
        std::vector<size_t> indices(symbols.size());
        for (size_t i = 0; i < symbols.size(); ++i) {
            indices[i] = i;
        }

        auto customSort = [&](size_t i, size_t j) {
            return symbols[i] < symbols[j];
        };

        std::sort(indices.begin(), indices.end(), customSort);

        std::vector<std::string> sortedSymbols(symbols.size());
        std::vector<std::string> sortedPrices(prices.size());
        std::vector<int> sortedIds(ids.size());

        for (size_t i = 0; i < symbols.size(); ++i) {
            size_t index = indices[i];
            sortedSymbols[i] = symbols[index];
            sortedPrices[i] = prices[index];
        }

        symbols = std::move(sortedSymbols);
        prices = std::move(sortedPrices);

    }
    catch (const json::exception& e) {
        std::cerr << "Error: JSON parsing failed. Reason: " << e.what() << std::endl;
        return;
    }
   
    ids.clear();
    for (size_t i = 0; i < symbols.size() && i < prices.size(); ++i)
    {
        ids.push_back(i + 1);
    }
}


const std::vector<std::string>& MyModel::getOpen() const
{
    return open_prices;
}
const std::vector<std::string>& MyModel::getHigh() const
{
    return high_price;
}
const std::vector<std::string>& MyModel::getLow() const
{
    return low_price;
}

//void MyModel::addListener(data_listener aListener)
//{
//
//}
const std::vector<std::string>& MyModel::getParsed() const
{
    return parsedElements;
}

const std::vector<std::string>& MyModel::getSymbols() const
{
    return symbols;
}

const std::vector<std::string>& MyModel::getPrices() const
{
    return prices;
}
const std::vector<int>& MyModel::getIds() const
{
    return ids;
}

const std::vector<std::string>& MyModel::getData() const
{
    return data;
}
