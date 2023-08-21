#include "MainComponent.h"
#include <string.h>

// juce timer
MainComponent::MainComponent(std::shared_ptr<MyModel> aModel)
{
    model = aModel;
    setSize(1000, 800);
    addAndMakeVisible(table);
    table.setModel(this);
    
    table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
    table.setOutlineThickness(1);
    // symbol sutunu
    table.getHeader().addColumn(juce::String("symbol"),
        1,
        200,
        50, 400,
        juce::TableHeaderComponent::defaultFlags);
    // price sutunu
    table.getHeader().addColumn(juce::String("price"),
        2,
        200,
        50, 400,
        juce::TableHeaderComponent::defaultFlags);
    model->run();
}
MainComponent::~MainComponent()
{
    model.reset();
}
void MainComponent::paint(juce::Graphics& g)
{
}
void MainComponent::resized()
{
    table.setBoundsInset(juce::BorderSize<int>(8));
}
void MainComponent::cellDoubleClicked(int rowNumber, int columnId, const juce::MouseEvent& e)
{

    

    std::map<std::string, MarketData> dataMap = model->data; 
    auto it = std::next(dataMap.begin(), rowNumber);

    const std::string& selectedSymbol = it->first;
    const MarketData& selectedData = it->second;  
   
    newnw* newWindow = new newnw(selectedSymbol, model);
   
}



void MainComponent::handleAsyncUpdate()
{
    table.updateContent();
    repaint();
}
void MainComponent::update()
{
    triggerAsyncUpdate();
}

int MainComponent::getNumRows()
{
    return model->data.size();
}
void MainComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    auto alternateColour = getLookAndFeel().findColour(juce::ListBox::backgroundColourId)
        .interpolatedWith(getLookAndFeel().findColour(juce::ListBox::textColourId), 0.03f);
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);
    else if (rowNumber % 2)
        g.fillAll(alternateColour);

}
void MainComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(getLookAndFeel().findColour(juce::ListBox::textColourId));
    g.setFont(font);

     size_t counter = 0;
     auto map_it = model->data.begin();
     std::advance(map_it, rowNumber);

    if (columnId == 1)
    {
        g.drawText(map_it->first, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    else if (columnId == 2)
    {
        g.drawText(map_it->second.price, 2, 0, width - 4, height, juce::Justification::centredLeft, true);      
        flasher.setNewData(map_it->second.price);
        flasher.flash(g, map_it->second.price, 2, 0, rowNumber);
    }
    g.setColour(getLookAndFeel().findColour(juce::ListBox::backgroundColourId));
    g.fillRect(width - 1, 0, 1, height);

}
