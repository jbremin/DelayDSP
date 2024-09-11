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
#include "LookAndFeel.h"
#include "LevelMeter.h"

//==============================================================================
/**
*/
class DelayDSPAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                      private juce::AudioProcessorParameter::Listener
{
public:
    DelayDSPAudioProcessorEditor (DelayDSPAudioProcessor&);
    ~DelayDSPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override { }
    void updateDelayKnobs(bool tempoSyncActive);
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayDSPAudioProcessor& audioProcessor;
    
    RotaryKnob gainKnob      { "Gain", audioProcessor.apvts, gainParamID, true };
    RotaryKnob mixKnob       { "Mix", audioProcessor.apvts, mixParamID };
    RotaryKnob delayTimeKnob { "Time", audioProcessor.apvts, delayTimeParamID };
    RotaryKnob feedbackKnob  { "Feedback", audioProcessor.apvts, feedbackParamID, true };
    RotaryKnob stereoKnob    { "Stereo", audioProcessor.apvts, stereoParamID, true };
    RotaryKnob lowCutKnob    { "Low Cut", audioProcessor.apvts, lowCutParamID };
    RotaryKnob highCutKnob   { "High Cut", audioProcessor.apvts, highCutParamID };
    RotaryKnob delayNoteKnob { "Note", audioProcessor.apvts, delayNoteParamID };

    juce::TextButton tempoSyncButton;
    
    juce::AudioProcessorValueTreeState::ButtonAttachment tempoSyncAttachment {
        audioProcessor.apvts, tempoSyncParamID.getParamID(), tempoSyncButton
    };
    
    juce::GroupComponent delayGroup, feedbackGroup, outputGroup;
    
    LevelMeter meter;
    
    MainLookAndFeel mainLF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayDSPAudioProcessorEditor)
};
