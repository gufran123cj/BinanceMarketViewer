#include "newwindow.h"



void newwindow::CryptoDetailWindow(const juce::String& cryptoName, const juce::String& cryptoPrice)
{
    juce::DocumentWindow(cryptoName, juce::Colours::white, juce::DocumentWindow::allButtons);
    {

        setContentOwned(new juce::Label("CryptoDetailLabel", "Crypto Name: " + cryptoName + "\nPrice: " + cryptoPrice), true);
        setResizable(true, true);
        centreWithSize(400, 400);
        setVisible(true);
        juce::Component* tableComponent = new juce::Component();
        tableComponent->setSize(400, 300);
        tableComponent->addAndMakeVisible(table);
        setContentOwned(tableComponent, true);
        createTable();
    }
}

void newwindow::createTable()
{
}

void newwindow::closeButtonPressed()
{
    delete this;
}


