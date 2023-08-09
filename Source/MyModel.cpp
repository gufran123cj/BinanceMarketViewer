#include "MyModel.h"
#include <nlohmann/json.hpp>
#include <juce_graphics/juce_graphics.h>


/*void MyModel::run()
{
	MyAsyncHttpSession ses;
	ses.run([this](const std::string& res)
        {
            response = res;
            parseresponse(response);
            
           if (view)              
                view->update();
                   
            
        });

}*/

//sembole çift týkla yeni sayfa aç (boþ pencere__include*  o sembolün bilgileri)
//robotun guisini oluþturucaz (baþlat, durdur, deðeri güncelle)
//button kullancaz(robotu baþlat (robot algoritmasý olucak)_bu robot alým yapýcak baþlattýðýmýz deðerden satarýz)
//symbol için sýralama ekle


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
        int count = 0;
        for (const auto& item : parsed_json) {

                if (item.is_object()) {
                    
                        symbols.push_back(item["s"]);
                        prices.push_back(item["c"]);
                }
                else {
                    std::cerr << "Error: One of the items is not a JSON object." << std::endl;
                    return;
                }
        }
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
