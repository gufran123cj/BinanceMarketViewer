#pragma once
#ifndef mymodel_h
#define mymodel_h


#include <string>
#include <JuceHeader.h>
#include "MyAsyncHttpSession.h"
#include "MyViewInterface.h"
#include "data_provider.h"
#include "data_listener.h"
#include "BinanceBotApplication.h"


class MyModel:
	public juce::Timer,
	public juce::Component
{
	
public:
	
	MyModel() : view(nullptr)
		, data(),bot(apiKey, secretKey)
	{
		//startTimer(1000);

	}
	~MyModel()
	{
	}

	void setView(MyViewInterface* aView)
	{
		if(aView)
			view = aView;
	}
	
	void run();
	void onDataReceived(const std::string& message);
	void addDataListener(data_listener* listener);

	std::vector<data_listener*> listeners;
	std::map<std::string, MarketData>data;
	
	std::string response;
	std::string message;
	MyViewInterface* view;
	std::string apiKey = "5ybw5ipsGy3vKqr5iDwL7mnk04mf10Xz2frAiVPfWAj00v6LDjusXeSdxWHZVa9m";
	std::string secretKey = "hXRGVF8JZ67p0yYL5Qm7XNc4atEHHQVtNTQvGjeYs4TenPijvXiO3oBt905k39Ex";
	BinanceBotApplication bot;

private:

	juce::TableListBox table;
	juce::Font font{ 14.0f };
	std::vector<std::string> parsedElements;
	std::vector<std::string> symbols;
	std::vector<std::string> prices;
	std::vector<std::string> open_prices;
	std::vector<std::string> high_price;
	std::vector<std::string> low_price;
	std::vector<std::string> total_trade_base;
	std::vector<std::string> total_trade_quote;
	std::vector<int> ids;
	
	virtual void timerCallback() override;
	void parseresponse(std::string response);



	
};

#endif // !mymodel_h