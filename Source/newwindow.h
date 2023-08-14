#pragma once
#include <JuceHeader.h>
#include <juce_graphics/juce_graphics.h>
#include "MyModel.h"
class newwindow :
	public juce::DocumentWindow,
	public juce::TableListBoxModel
{
public:
	void CryptoDetailWindow(const juce::String& cryptoName, const juce::String& cryptoPrice);
	void createTable();
	void closeButtonPressed() override;
private:
	std::shared_ptr<MyModel> model;
	juce::TableListBox table;
	juce::Label label;

};