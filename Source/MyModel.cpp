#include "MyModel.h"
#include <nlohmann/json.hpp>
#include <juce_graphics/juce_graphics.h>
#include "data_listener.h"
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

void MyModel::onDataReceived(const std::string& message)
{
    parseresponse(message);
    if (view)
        view->update();
}

void MyModel::addDataListener(data_listener* listener)
{
    listeners.push_back(listener);
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
        int count = 0;
        for (const auto& item : parsed_json) {

                if (item.is_object()) {
                    const std::string symbol = item["s"];
                    data[symbol].price = item["c"];
                    data[symbol].low_price = item["l"];
                    data[symbol].high_price = item["h"];
                    data[symbol].open_price = item["o"];
                    data[symbol].total_trade_quote = item["q"];
                    data[symbol].total_trade_base = item["v"];
                }
                else {
                    std::cerr << "Error: One of the items is not a JSON object." << std::endl;
                    return;
                }
        }

        for (auto& i : listeners)
        {
            
                i->onDataReceived(data);
        }
        /*std::vector<size_t> indices(symbols.size());
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
        prices = std::move(sortedPrices);*/

        
    }
    catch (const json::exception& e) {
        std::cerr << "Error: JSON parsing failed. Reason: " << e.what() << std::endl;
        return;
    }
   
    
}
