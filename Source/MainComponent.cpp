#include <nlohmann/json.hpp>
#include "MainComponent.h"
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

void MainComponent::mouseDown(const juce::MouseEvent& event)
{   

}

void MainComponent::openNewWindow(const juce::String& symbol)
{
    
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
            flasher.flash(g, price, 2, 0, rowNumber); // Flasher'ý burada kullanýyoruz.
        }
    g.setColour(getLookAndFeel().findColour(juce::ListBox::backgroundColourId));
    g.fillRect(width - 1, 0, 1, height);

        ColourChangeButton.setButtonText(model->getSymbols().at(0));
        ColourChangeButton.setBounds(x, y, width - 4, height);
        addAndMakeVisible(ColourChangeButton);
        /*x += 8;
        y += 35;*/
        /*ColourChangeButton1.setButtonText(model->getSymbols().at(1));
        ColourChangeButton1.setBounds(x, 60, width - 4, height);
        addAndMakeVisible(ColourChangeButton1);
        
        ColourChangeButton2.setButtonText(model->getSymbols().at(2));
        ColourChangeButton2.setBounds(x, 85, width - 4, height);
        addAndMakeVisible(ColourChangeButton2);*/

           
        
    
    ColourChangeButton.onClick = [this,rowNumber] {
        open_new_window(rowNumber);
        //juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "sa býlader", "as");

    };
    

}
void MainComponent::open_new_window(int rowNumber) {
    //[{"e":"24hrMiniTicker","E":1691754449551,"s":"BTCUSDT","c":"29422.00000000","o":"29515.26000000","h":"29738.00000000","l":"29320.20000000","v":"23376.93902000","q":"689008713.92773180"},
    //{"e":"24hrMiniTicker","E":1691754449545,"s":"XMRBTC","c":"0.00536700","o":"0.00524500","h":"0.00536800","l":"0.00524500","v":"25657.93100000","q":"136.05164730"}]
    
    
    
    juce::DocumentWindow* cleanWindow = new juce::DocumentWindow("Clean Window",
        juce::Colours::grey,
        juce::DocumentWindow::allButtons);

    if (cleanWindow != nullptr)
    {
        cleanWindow->closeButtonPressed(); // Eðer varsa, mevcut pencereyi kapat
    }
    auto data = (model->getData());
    auto price = juce::String(model->getPrices().at(rowNumber));
    auto symbol = juce::String(model->getSymbols().at(rowNumber));
    auto open_price = juce::String(model->getOpen().at(rowNumber));
    auto high_price = juce::String(model->getHigh().at(rowNumber));
    auto low_price = juce::String(model->getLow().at(rowNumber));

    cleanWindow->setSize(600, 600);
    cleanWindow->setVisible(true);
    juce::Label* label = new juce::Label();
    label->setText("Clicked Symbol: " + symbol + "\n" + "Price: " + price + "\n" + "Open Price: " + open_price + "\n" + "High Price: " + high_price + "\n" + "Low Price: " + low_price, juce::NotificationType::dontSendNotification);
    
    label->setBounds(10, 10, 600, 600);

    cleanWindow->setContentOwned(label, false); // Ýlk etiketi pencereye ekle
    cleanWindow->setVisible(true);
   
    
        

}











