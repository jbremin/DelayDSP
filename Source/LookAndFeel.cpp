/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 19 Jul 2024 11:38:30pm
    Author:  Johan Bremin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LookAndFeel.h"

//==============================================================================
LookAndFeel::LookAndFeel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

LookAndFeel::~LookAndFeel()
{
}

void LookAndFeel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("LookAndFeel", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void LookAndFeel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
