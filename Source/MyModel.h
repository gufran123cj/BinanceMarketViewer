#pragma once
#include <string>
#include <JuceHeader.h>
#include "MyAsyncHttpSession.h"
#include "MyViewInterface.h"
class MyModel:
	public juce::Timer,
	public juce::Component

{
public:
	MyModel(): view(nullptr)
	{
		startTimer(1000);

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
	const std::vector<std::string>& getSymbols() const;
	const std::vector<std::string>& getPrices() const;
	const std::vector<int>& getIds() const;
	std::string response;
	MyViewInterface* view;

private:
	juce::TableListBox table;
	juce::Font font{ 14.0f };
	std::vector<std::string> symbols;
	std::vector<std::string> prices;
	std::vector<int> ids;
	
	virtual void timerCallback() override;
	void parseresponse(std::string response);
	
};

