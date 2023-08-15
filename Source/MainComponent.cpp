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
   
    juce::String selectedPrice = model->getPrices().at(rowNumber);
    juce::String selectedSymbol = model->getSymbols().at(rowNumber);
    juce::String clickedSymbol = selectedSymbol;
    std::thread([this, rowNumber, selectedSymbol]() {
        juce::MessageManager::callAsync([this, rowNumber, selectedSymbol]() {
            open_new_window(rowNumber, selectedSymbol);
            });
        }).detach();
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
    return model->getSymbols().size();
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
    
    auto price = juce::String(model->getPrices().at(rowNumber));
    auto symbol = juce::String(model->getSymbols().at(rowNumber));

    
        if (columnId == 1)
        {
            g.drawText(symbol, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        }
        else if (columnId == 2)
        {
            g.drawText(price, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            flasher.setNewData(price);
            flasher.flash(g, price, 2, 0, rowNumber); 
        }
    g.setColour(getLookAndFeel().findColour(juce::ListBox::backgroundColourId));
    g.fillRect(width - 1, 0, 1, height);
}
void MainComponent::open_new_window(int rowNumber, juce::String clickedSymbol) {

    juce::DocumentWindow* cleanWindow = new juce::DocumentWindow(clickedSymbol + " COIN",
        juce::Colours::grey,
        juce::DocumentWindow::allButtons);

    if (cleanWindow != nullptr) {
        cleanWindow->setSize(600, 600);
        cleanWindow->setVisible(true);

        juce::Label* label = new juce::Label();
        label->setBounds(10, 10, 600, 600);
        cleanWindow->setContentOwned(label, false); 
        cleanWindow->setVisible(true);

        std::thread([this, label, rowNumber, clickedSymbol]() {
            while (true) {
                std::vector<std::string> parsedData = model->getParsed();
                std::vector<std::string> symbols = model->getSymbols();
                std::vector<std::string> prices = model->getPrices();
                std::vector<std::string> openPrices = model->getOpen();
                std::vector<std::string> highPrices = model->getHigh();
                std::vector<std::string> lowPrices = model->getLow();

                for (int i = 0; i < symbols.size(); ++i) {
                    if (symbols[i] == clickedSymbol) {
                        juce::String symbol = symbols[i];
                        juce::String price = prices[i];
                        juce::String open_price = openPrices[i];
                        juce::String high_price = highPrices[i];
                        juce::String low_price = lowPrices[i];
                        flasher.setNewData(price);
                        juce::MessageManager::callAsync([label, symbol, price, open_price, high_price, low_price]() {
                            label->setText("Clicked Symbol: " + symbol + "\n" +
                                "Price: " + price + "\n" +
                                "Open Price: " + open_price + "\n" +
                                "High Price: " + high_price + "\n" +
                                "Low Price: " + low_price, juce::NotificationType::dontSendNotification);
                            });

                        break;
                    }
                }

                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            }).detach();
    }
}













