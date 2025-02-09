#include "newnw.h"
#include "MainComponent.h"
#include "MyModel.h"
#include "map";
#include "BinanceBotApplication.h"
#include "AsyncHttpsSession.h"

newnw::newnw(juce::String clickedSymbol, std::shared_ptr<MyModel> aModel, std::vector<std::string> pricehistory, std::string lastprice) :
    juce::DocumentWindow(clickedSymbol, juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
        juce::DocumentWindow::allButtons), selectedSymbol(clickedSymbol), pricehistory1(pricehistory),lastprice1(lastprice)
{
    this->model = aModel;
    group.addAndMakeVisible(button);
    group.addAndMakeVisible(button1);
    group.addAndMakeVisible(button2);
    group.addAndMakeVisible(table);
    /////////////////////////////////
    button.setButtonText("START");
    button1.setButtonText("STOP");
    button2.setButtonText("ENTER");
    

    button.onClick = [this]() {

        //model->bot.sendTradeFeeRequest();
        model->bot.sendOrderBookResponse();
        //model->bot.testNewOrder(/*lastprice1*/);

        return 0;
    };

    button1.onClick = [this]() {
        /*bot.deleteNewOrder();
        return 0;*/
    };

    button2.onClick = [this]() {

    };
    setSize(720, 300);
    model->addDataListener(this);

#if JUCE_IOS || JUCE_ANDROID
    setFullScreen(true);
#else
    setResizable(true, true);
    centreWithSize(getWidth(), getHeight());
#endif

    setVisible(true);
    createTable();
    setUsingNativeTitleBar(true);
    setContentOwned(&group, false);

}


void newnw::onDataReceived(std::map<std::string, MarketData>& data)
{
    model->data = data;
    update();

}

void newnw::resized()
{
    group.setBoundsInset(juce::BorderSize<int>(8));
    table.setBoundsRelative(0,0,1,0.5);
    button.setBoundsRelative(0, 0.5, 0.3, 0.5);
    button1.setBoundsRelative(0.3, 0.5, 0.3, 0.5);
    button2.setBoundsRelative(0.6, 0.5, 0.3, 0.5);

}

void newnw::handleAsyncUpdate()
{
	table.updateContent();
    juce::DocumentWindow::repaint();
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


void newnw::paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected)
{
}

int newnw::getNumRows()
{
    if (model->data.find(selectedSymbol.toStdString()) != model->data.end()) {
    
        return 1;
    }
    return 0;
}
void newnw::Stackprice() {
    const MarketData& rowData = model->data[selectedSymbol.toStdString()];
    std::string newPrice = rowData.price;
    lastprice1 = newPrice;
    if (std::find(pricehistory1.begin(), pricehistory1.end(), newPrice) == pricehistory1.end()) {
        pricehistory1.push_back(newPrice);
        std::sort(pricehistory1.rbegin(), pricehistory1.rend());
    }
}


void newnw::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.fillAll(juce::Colours::white);

    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);

    g.setColour(juce::Colours::black);
    g.setFont(height * 0.7f);

    if (model->data.find(selectedSymbol.toStdString()) != model->data.end()) {
        const MarketData& rowData = model->data[selectedSymbol.toStdString()];

        switch (columnId)
        {
        case 1:
            g.drawText(selectedSymbol, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            break;
        case 2:
            g.drawText(juce::String(rowData.price), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            flasher.setNewData(rowData.price);
            flasher.flash(g, rowData.price, 2, 0, rowNumber);
            break;
        case 3:
            g.drawText(juce::String(rowData.open_price), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            flasher.setNewData(rowData.open_price);
            flasher.flash(g, rowData.open_price, 2, 0, rowNumber);
            break;
        case 4:
            g.drawText(juce::String(rowData.low_price), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            flasher.setNewData(rowData.low_price);
            flasher.flash(g, rowData.low_price, 2, 0, rowNumber);
            break;
        case 5:
            g.drawText(juce::String(rowData.high_price), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            flasher.setNewData(rowData.high_price);
            flasher.flash(g, rowData.high_price, 2, 0, rowNumber);
            break;
        case 6:
            g.drawText(juce::String(rowData.total_trade_base), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            flasher.setNewData(rowData.total_trade_base);
            flasher.flash(g, rowData.total_trade_base, 2, 0, rowNumber);
            break;
        case 7:
            g.drawText(juce::String(rowData.total_trade_quote), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            flasher.setNewData(rowData.total_trade_quote);
            flasher.flash(g, rowData.total_trade_quote, 2, 0, rowNumber);
            break;
        default:
            break;
        }
    }
    Stackprice();
    
}



