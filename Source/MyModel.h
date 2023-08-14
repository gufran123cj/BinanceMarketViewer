#pragma once
#include <string>
#include <JuceHeader.h>
#include "MyAsyncHttpSession.h"
#include "MyViewInterface.h"
#include "data_provider.h"
#include "data_listener.h"
//buraya include edicek
class MyModel:
	public juce::Timer,
	public juce::Component
	 

{

public:
	MyModel(): view(nullptr)
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
	const std::vector<std::string>& getParsed() const;
	const std::vector<std::string>& getSymbols() const;
	const std::vector<std::string>& getPrices() const;
	const std::vector<int>& getIds() const;
	const std::vector<std::string>& getData() const;
	const std::vector<std::string>& getOpen() const;
	const std::vector<std::string>& getHigh() const;
	const std::vector<std::string>& getLow() const;
	/*void addListener(data_listener aListener);*/
	std::string response;
	std::string message;
	MyViewInterface* view;

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
	std::vector<std::string> data;
	std::vector<int> ids;
	
	virtual void timerCallback() override;
	void parseresponse(std::string response);



	
};

