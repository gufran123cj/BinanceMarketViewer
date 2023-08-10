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
    //bool flag = true;
    //if (flag) // Eðer sadece belirli bir sütuna týklanýyorsa
    //{
    //    // Yeni pencereyi aç
    //    juce::DialogWindow::LaunchOptions options;
    //    options.dialogTitle = "Pencere Baþlýðý";
    //    options.dialogBackgroundColour = juce::Colours::white;
    //    options.escapeKeyTriggersCloseButton = true;
    //    options.useNativeTitleBar = true;
    //    options.resizable = true;
    //    options.content.setOwned(new juce::Label("Pencere Ýçeriði", "Týklanan sembol: "));

    //    juce::Component* parentComponent = this;
    //    juce::Rectangle<int> areaBounds = getLocalBounds().withSize(400, 300); // Pencerenin boyutunu ayarlayýn
    //    juce::DialogWindow::showDialog(juce::String(), parentComponent, nullptr, juce::Colours::lightgrey, true);
    //}
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
    for (int i = 0; i < 4; ++i)
    {
        ColourChangeButton.setButtonText(symbol);
        ColourChangeButton.setBounds(x, y, width - 4, height);
        addAndMakeVisible(ColourChangeButton);
        x += 8;
        y += 35;
    }
    
    
    ColourChangeButton.onClick = [this] {

        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "sa býlader", "as");

    };
}





