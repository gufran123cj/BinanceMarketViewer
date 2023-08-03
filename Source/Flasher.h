#pragma once
#include <JuceHeader.h>
#include <juce_gui_basics/juce_gui_basics.h>

class Flasher :
    public juce::Component,
    public juce::Timer

{
public:
    Flasher();
    void start(int numFlashes, int flashIntervalMs);
    void stop();
    bool isFlashing() const;
    void resetLastFlashTime();
    void setNewData(const juce::String& newData);
    void flash(juce::Graphics& g, const juce::String& text, int x, int y,int row);

private:
    bool flashing;
    std::vector<juce::String> tempdata;
    int numFlashesRemaining;
    int flashInterval;
    juce::Time flashStartTime;
    virtual void timerCallback();
    int flashDuration; // Geçici renklendirme süresi (milisaniye cinsinden)
    juce::Time lastFlashTime;
    int previousPrice = 0;
    juce::String currentData;
     // Flasher'ýn göreceði güncel veri

};
//güncellenen satýrlar flash olucak
//index tutan dizi reng deðiþtirme satýra göre 