#pragma once
#ifndef data_listener_h
#define data_listener_h

#include <string>
#include <map>
struct MarketData {
	std::string symbol;
	std::string price;
	std::string open_price;
	std::string high_price;
	std::string low_price;
	std::string total_trade_base;
	std::string total_trade_quote;
};

class data_listener {
public: 
    virtual void onDataReceived(std::map<std::string, MarketData>& data) = 0;
    
};
#endif // !1
