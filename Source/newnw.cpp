#include "newnw.h"
#include "MainComponent.h"
#include "MyModel.h"
#include "map";


newnw::newnw(juce::String clickedSymbol, std::shared_ptr<MyModel> aModel) :
    juce::DocumentWindow(clickedSymbol, juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
        juce::DocumentWindow::allButtons)
{
    this->model = aModel;
    setSize(800, 800);
    /*model->addDataListener(this);*/

#if JUCE_IOS || JUCE_ANDROID
    setFullScreen(true);
#else
    setResizable(true, true);
    centreWithSize(getWidth(), getHeight());
#endif

    setVisible(true);
    createTable();
    setUsingNativeTitleBar(true);
    setContentOwned(&table, true);


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
        100,
        100, 500,
        juce::TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn(juce::String("PRICE"),
        2,
        100,
        100, 500,
        juce::TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn(juce::String("OPEN PRICE"),
        3,
        100,
        100, 500,
        juce::TableHeaderComponent::defaultFlags);

    table.getHeader().addColumn(juce::String("LOW PRICE"),
        4,
        100,
        100, 500,
        juce::TableHeaderComponent::defaultFlags);

    table.getHeader().addColumn(juce::String("HIGH PRICE"),
        5,
        100,
        100, 500,
        juce::TableHeaderComponent::defaultFlags);

    table.getHeader().addColumn(juce::String("TOTAL TRADE BASE"),
        6,
        100,
        100, 500,
        juce::TableHeaderComponent::defaultFlags);

    table.getHeader().addColumn(juce::String("TOTAL QUOTE BASE"),
        7,
        100,
        100, 500,
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

