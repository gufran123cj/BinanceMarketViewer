#pragma once
#include "data_listener.h"
#include <JuceHeader.h>
#include "MyViewInterface.h"
#include "Flasher.h"




class MyModel;

class newnw :
	public juce::DocumentWindow,
	public data_listener, 
	public juce::TableListBoxModel,
	public juce::AsyncUpdater,
	public MyViewInterface

{
public:
	newnw(juce::String clickcedsymbol, std::shared_ptr<MyModel> aModel);
	virtual void onDataReceived(std::map<std::string, MarketData>& data) override;
	juce::Label* label;
	void resized() override;
	virtual void handleAsyncUpdate() override;
	virtual void update() override;
	void createTable();
	void closeButtonPressed() override;
private:
	std::shared_ptr<MyModel> model;
	Flasher flasher;
	juce::TextButton button;
	juce::TextButton button1;
	juce::TextButton button2;
	juce::String selectedSymbol;
	juce::TableListBox table;
	juce::Font font{ 10.0f };
	virtual void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
	virtual int getNumRows() override;
	virtual void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	
};

