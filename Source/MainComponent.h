#pragma once
#include <JuceHeader.h>
#include <memory>
#include "MyViewInterface.h"
#include "MyModel.h"
#include "Flasher.h"
#include "newnw.h"

class MainComponent :
    public juce::Component,
    public juce::TableListBoxModel,
    public juce::AsyncUpdater,
    public MyViewInterface,
    public juce::MouseListener
    

{
public:
    MainComponent(std::shared_ptr<MyModel> aModel);
    ~MainComponent();
    void paint(juce::Graphics&) override;
    void resized() override;
    void sortOrderChanged(int newSortColumnId, bool isForwards);
    void cellDoubleClicked(int rowNumber, int columnId, const juce::MouseEvent&);

    virtual void handleAsyncUpdate() override;
    virtual void update() override;
    juce::Label* label;


protected:

private:
    Flasher flasher;
    std::shared_ptr<MyModel> model;
    juce::TableListBox table;
    juce::TableListBox newwindow;
    juce::Font font{ 14.0f };   
    // Inherited via TableListBoxModel
    // Inherited via Timer
    virtual int getNumRows() override;
    virtual void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    virtual void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

        
};
