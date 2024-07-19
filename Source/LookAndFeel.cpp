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
RotaryKnobLookAndFeel::RotaryKnobLookAndFeel()
{
    setColour(juce::Label::textColourId, Colors::Knob::label);
    setColour(juce::Slider::textBoxTextColourId, Colors::Knob::label);
}

void RotaryKnobLookAndFeel::drawRotarySlider( juce::Graphics& g, int x, int y, int width,
                                            [[maybe_unused]] int height,float sliderPos,
                                            float rotaryStartAngle, float rotaryEndAngle,
                                             juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int>(x, y, width, width).toFloat();
    auto knobRect = bounds.reduced(10.0f, 10.0f);
    
    auto path = juce::Path();
    path.addEllipse(knobRect);
    dropShadow.drawForPath(g, path);
    
    g.setColour(Colors::Knob::outline);
    g.fillEllipse(knobRect);
    
    auto innerRect = knobRect.reduced(2.0f, 2.0f);
    auto gradient = juce::ColourGradient(
        Colors::Knob::gradientTop, 0.0f, innerRect.getY(),
        Colors::Knob::gradientBottom, 0.0f, innerRect.getBottom(), false);
    g.setGradientFill(gradient);
    g.fillEllipse(innerRect);
    
    auto center = bounds.getCentre();
    auto radius = bounds.getWidth() / 2.0f;
    auto lineWidth = 3.0f;
    auto arcRadius = radius - lineWidth/2.0f;
    
    juce::Path backgroundArc;
    backgroundArc.addCentredArc(center.x, center.y, arcRadius, arcRadius,
                                0.0f, rotaryStartAngle, rotaryEndAngle, true);
    
    auto strokeType = juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved,
                                           juce::PathStrokeType::rounded);
    
    g.setColour(Colors::Knob::trackBackground);
    g.strokePath(backgroundArc, strokeType);
    
    auto dialRadius = innerRect.getHeight()/2.0f - lineWidth;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    juce::Point<float> dialStart(center.x + 10.0f * std::sin(toAngle), center.y - 10.0f * std::cos(toAngle));
    juce::Point<float> dialEnd(center.x + dialRadius * std::sin(toAngle),
                               center.y - dialRadius * std::cos(toAngle));
    
    juce::Path dialPath;
    dialPath.startNewSubPath(dialStart);
    dialPath.lineTo(dialEnd);
    g.setColour(Colors::Knob::dial);
    g.strokePath(dialPath, strokeType);
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
