#include "newnw.h"
#include "MainComponent.h"
#include "MyModel.h"


newnw::newnw(std::string clickcedsymbol, std::shared_ptr<MyModel> aModel) :
    juce::DocumentWindow(clickcedsymbol + " COIN",
        juce::Colours::grey,
        juce::DocumentWindow::allButtons)
{

    juce::Label* label = new juce::Label("CryptoDetailLabel", "Crypto Name: ");
    label->setSize(400, 100);
    label->setJustificationType(juce::Justification::centred);
    this->model = aModel;
    model->addDataListener(this);
    setContentOwned(label, true);

    setSize(600, 200);
    centreWithSize(getWidth(), getHeight());
    setVisible(true);
    createTable();
    
}

void newnw::onDataReceived(std::map<std::string, MarketData>& data)
{

}



void newnw::paint(juce::Graphics&)
{
}

void newnw::resized()
{
	table.setBoundsInset(juce::BorderSize<int>(8));

}

void newnw::handleAsyncUpdate()
{
	table.updateContent();
	repaint();
}

void newnw::update()
{
	triggerAsyncUpdate();

}

void newnw::createTable()
{
    table.setModel(this);
    table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
    table.setOutlineThickness(1);
    table.getHeader().addColumn(juce::String("SYMBOL"),
        1,
        50,
        50, 350,
        juce::TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn(juce::String("PRICE"),
        2,
        50,
        50, 350,
        juce::TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn(juce::String("OPEN PRICE"),
        3,
        50,
        50, 350,
        juce::TableHeaderComponent::defaultFlags);

    table.getHeader().addColumn(juce::String("LOW PRICE"),
        4,
        50,
        50, 350,
        juce::TableHeaderComponent::defaultFlags);

    table.getHeader().addColumn(juce::String("HIGH PRICE"),
        5,
        50,
        50, 350,
        juce::TableHeaderComponent::defaultFlags);

    table.getHeader().addColumn(juce::String("TOTAL TRADE BASE"),
        6,
        50,
        50, 350,
        juce::TableHeaderComponent::defaultFlags);

    table.getHeader().addColumn(juce::String("TOTAL QUOTE BASE"),
        7,
        50,
        50, 350,
        juce::TableHeaderComponent::defaultFlags);

}

void newnw::closeButtonPressed()
{
    delete this;
}


int newnw::getNumRows()
{
    return model->data.size();
}

void newnw::paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected)
{
}

void newnw::paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{

}
