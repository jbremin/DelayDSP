/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayDSPAudioProcessorEditor::DelayDSPAudioProcessorEditor (DelayDSPAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    delayGroup.setText("Delay");
    delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    delayGroup.addAndMakeVisible(delayTimeKnob);
    addAndMakeVisible(delayGroup);
    
    feedbackGroup.setText("Feedback");
    feedbackGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    feedbackGroup.addAndMakeVisible(feedbackKnob);
    feedbackGroup.addAndMakeVisible(stereoKnob);
    feedbackGroup.addAndMakeVisible(lowCutKnob);
    feedbackGroup.addAndMakeVisible(highCutKnob);
    addAndMakeVisible(feedbackGroup);
    
    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    outputGroup.addAndMakeVisible(gainKnob);
    outputGroup.addAndMakeVisible(mixKnob);
    addAndMakeVisible(outputGroup);
    
    setLookAndFeel (&mainLF);
        
    setSize(500, 330);
}

DelayDSPAudioProcessorEditor::~DelayDSPAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void DelayDSPAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto noise = juce::ImageCache::getFromMemory( BinaryData::Noise_png, BinaryData::Noise_pngSize);
    auto fillType = juce::FillType(noise, juce::AffineTransform::scale(0.5f)); g.setFillType(fillType);
    g.fillRect(getLocalBounds());
    
    auto rect = getLocalBounds().withHeight(40); g.setColour(Colors::header); g.fillRect(rect);
    auto image = juce::ImageCache::getFromMemory( BinaryData::Logo_png, BinaryData::Logo_pngSize);
    int destWidth = image.getWidth() / 2; int destHeight = image.getHeight() / 2; g.drawImage(image,
                    getWidth() / 2 - destWidth / 2, 0, destWidth, destHeight,
                    0, 0, image.getWidth(), image.getHeight());
}

void DelayDSPAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    int y = 50;
    int height = bounds.getHeight() - 60; // Position the groups
        delayGroup.setBounds(10, y, 110, height);
        outputGroup.setBounds(bounds.getWidth() - 160, y, 150, height);
    
        feedbackGroup.setBounds(delayGroup.getRight() + 10, y,
                                outputGroup.getX() - delayGroup.getRight() - 20, height);
        feedbackKnob.setTopLeftPosition(20, 20);
        
        stereoKnob.setTopLeftPosition(feedbackKnob.getRight() + 20, 20);
    
        lowCutKnob.setTopLeftPosition(feedbackKnob.getX(), feedbackKnob.getBottom() + 10);
        highCutKnob.setTopLeftPosition(lowCutKnob.getRight() + 20, lowCutKnob.getY());
    
        delayTimeKnob.setTopLeftPosition(20, 20);
        mixKnob.setTopLeftPosition(20, 20);
        gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);
}
