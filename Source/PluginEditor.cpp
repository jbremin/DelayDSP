/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

DelayDSPAudioProcessorEditor::DelayDSPAudioProcessorEditor (DelayDSPAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p), meter(p.levelL, p.levelR)
{
    delayGroup.setText("Delay");
    delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    delayGroup.addAndMakeVisible(delayTimeKnob);
    delayGroup.addChildComponent(delayNoteKnob);
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
    outputGroup.addAndMakeVisible(meter);
    addAndMakeVisible(outputGroup);
    
    tempoSyncButton.setButtonText("Sync");
    tempoSyncButton.setClickingTogglesState(true);
    tempoSyncButton.setBounds(0, 0, 70, 27);
    tempoSyncButton.setLookAndFeel(ButtonLookAndFeel::get());
    delayGroup.addAndMakeVisible(tempoSyncButton);
    
    auto bypassIcon = juce::ImageCache::getFromMemory(BinaryData::Bypass_png, BinaryData::Bypass_pngSize);
    bypassButton.setClickingTogglesState(true);
    bypassButton.setBounds(0, 0, 20, 20); bypassButton.setImages(
                false, true, true,
                bypassIcon, 1.0f, juce::Colours::white,
                bypassIcon, 1.0f, juce::Colours::white,
                bypassIcon, 1.0f, juce::Colours::grey, 0.0f);
    addAndMakeVisible(bypassButton);
    
    setLookAndFeel (&mainLF);
        
    setSize(500, 330);
    
    updateDelayKnobs(audioProcessor.params.tempoSyncParam->get());
    audioProcessor.params.tempoSyncParam->addListener(this);

    
}

DelayDSPAudioProcessorEditor::~DelayDSPAudioProcessorEditor()
{
    audioProcessor.params.tempoSyncParam->removeListener(this);
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
        delayGroup.addAndMakeVisible(delayNoteKnob);
    
        feedbackGroup.setBounds(delayGroup.getRight() + 10, y,
                                outputGroup.getX() - delayGroup.getRight() - 20, height);
        feedbackKnob.setTopLeftPosition(20, 20);
        stereoKnob.setTopLeftPosition(feedbackKnob.getRight() + 20, 20);
    
        lowCutKnob.setTopLeftPosition(feedbackKnob.getX(), feedbackKnob.getBottom() + 10);
        highCutKnob.setTopLeftPosition(lowCutKnob.getRight() + 20, lowCutKnob.getY());
    
        delayTimeKnob.setTopLeftPosition(20, 20);
        mixKnob.setTopLeftPosition(20, 20);
        gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);
    
        tempoSyncButton.setTopLeftPosition(20, delayTimeKnob.getBottom() + 10);
        delayNoteKnob.setTopLeftPosition(delayTimeKnob.getX(), delayTimeKnob.getY());
    
        meter.setBounds(outputGroup.getWidth() - 45, 30, 30, gainKnob.getBottom() - 30);
        
        bypassButton.setTopLeftPosition(bounds.getRight() - bypassButton.getWidth() - 10, 10);
}

void DelayDSPAudioProcessorEditor::parameterValueChanged(int, float value)
{
    if (juce::MessageManager::getInstance()->isThisTheMessageThread()) { updateDelayKnobs(value != 0.0f);
    } else {
        juce::MessageManager::callAsync([this, value]
        {
            updateDelayKnobs(value != 0.0f);
        });
    }
}


void DelayDSPAudioProcessorEditor::updateDelayKnobs(bool tempoSyncActive)
{
    delayTimeKnob.setVisible(!tempoSyncActive);
    delayNoteKnob.setVisible(tempoSyncActive);
}
