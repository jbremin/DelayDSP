/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"

//==============================================================================
/**
*/
class DelayDSPAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DelayDSPAudioProcessorEditor (DelayDSPAudioProcessor&);
    ~DelayDSPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayDSPAudioProcessor& audioProcessor;
    
    RotaryKnob gainKnob      { "Gain", audioProcessor.apvts, gainParamID };
    RotaryKnob mixKnob       { "Mix", audioProcessor.apvts, mixParamID };
    RotaryKnob delayTimeKnob { "Time", audioProcessor.apvts, delayTimeParamID };
    
    juce::GroupComponent delayGroup, feedbackGroup, outputGroup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayDSPAudioProcessorEditor)
};
