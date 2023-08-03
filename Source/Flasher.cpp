#include "Flasher.h"
#include <juce_graphics/juce_graphics.h>



Flasher::Flasher() : flashing(false), numFlashesRemaining(0), flashInterval(0), flashDuration(500), lastFlashTime(juce::Time::getCurrentTime())
{

}


void Flasher::start(int numFlashes, int flashIntervalMs)
{
    if (numFlashes > 0 && flashIntervalMs > 0)
    {
        numFlashesRemaining = numFlashes * 2; // Yan�p s�nmelerin say�s� �ift olacak �ekilde ayarlan�r
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
    tempdata.push_back(newData);//�nceki verileri tuutyoruz
}



void Flasher::flash(juce::Graphics& g, const juce::String& text, int x, int y,int row)
{
    juce::Time currentTime = juce::Time::getCurrentTime();
    int count = 0;
        if ((currentTime - lastFlashTime).inMilliseconds() < flashDuration)
        {
            g.setColour(juce::Colours::green);
        }
        else
        {
            // Verilere g�re renk de�i�tirme
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
            count++;

                
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
//aray�z�n kitlenmemsi i�in bir �ey bak�lcak
