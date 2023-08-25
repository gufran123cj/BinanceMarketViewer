#include "MyModel.h"
#include <nlohmann/json.hpp>
#include <juce_graphics/juce_graphics.h>
#include "data_listener.h"
//TASK---
//button kullancaz(robotu ba�lat (robot algoritmas� olucak)_bu robot al�m yap�cak ba�latt���m�z de�erden satar�z)
//take profit order diye class olu�turuun
//spot account bak
// PROBLEMS---
//coin penceresini kapat�nca b�t�n guiler kapan�yor
//symbole t�kland���nda de�erler s�ral�ncak 
//flasher class�na bak patlam�� olabilir




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

        
    }
    catch (const json::exception& e) {
        std::cerr << "Error: JSON parsing failed. Reason: " << e.what() << std::endl;
        return;
    }
   
    
}
