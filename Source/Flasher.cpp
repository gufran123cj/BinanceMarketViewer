#include "Flasher.h"
#include <juce_graphics/juce_graphics.h>



Flasher::Flasher() : flashing(false), numFlashesRemaining(0), flashInterval(0), flashDuration(500), lastFlashTime(juce::Time::getCurrentTime())
{

}
void Flasher::start(int numFlashes, int flashIntervalMs)
{
    if (numFlashes > 0 && flashIntervalMs > 0)
    {
        numFlashesRemaining = numFlashes * 2; // Yanýp sönmelerin sayýsý çift olacak þekilde ayarlanýr
        flashInterval = flashIntervalMs;
        flashing = true;
        flashStartTime = juce::Time::getCurrentTime();
        juce::Timer::startTimer(flashInterval);
    }
}
void Flasher::stop()
{
    juce::Timer::stopTimer();
    flashing = false;
}
bool Flasher::isFlashing() const
{
    return flashing;
}
void resetLastFlashTime()
{
    juce::Time lastFlashTime = juce::Time::getCurrentTime();
}
void Flasher::setNewData(const juce::String& newData)
{
    currentData = newData;
    tempdata.push_back(newData);
}
void Flasher::flash(juce::Graphics& g, const juce::String& text, int x, int y, int row)
{

    // Verilere göre renk deðiþtirme
    if (currentData > tempdata.at(row)) {
        g.setColour(juce::Colours::green);
        g.setOpacity(0.3);
        g.fillRect(x, y, 100, 20);
        g.drawText(currentData, getLocalBounds(), juce::Justification::centredLeft, true);

    }

    else if (currentData < tempdata.at(row)) {
        g.setColour(juce::Colours::red);
        g.setOpacity(0.3);
        g.fillRect(x, y, 100, 20);
        g.drawText(currentData, getLocalBounds(), juce::Justification::centredLeft, true);

    }
}

void Flasher::timerCallback()
{
    auto currentTime = juce::Time::getCurrentTime();
    auto elapsedTime = currentTime.toMilliseconds() - flashStartTime.toMilliseconds();

    bool visible = (elapsedTime / flashInterval) % 2 == 0;

    if (numFlashesRemaining > 0)
    {
        numFlashesRemaining--;

        if (numFlashesRemaining == 0)
            stop();
    }

    repaint();
}
