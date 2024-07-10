/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayDSPAudioProcessor::DelayDSPAudioProcessor() : AudioProcessor(
                                                   BusesProperties()
                                                        .withInput("Input", juce::AudioChannelSet::stereo(), true)
                                                        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
                                                   )
{
    auto* param = apvts.getParameter(gainParamID.getParamID());
    gainParam = dynamic_cast<juce::AudioParameterFloat*>(param);
}

DelayDSPAudioProcessor::~DelayDSPAudioProcessor()
{
}

//==============================================================================
const juce::String DelayDSPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayDSPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayDSPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayDSPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayDSPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayDSPAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayDSPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayDSPAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayDSPAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayDSPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayDSPAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DelayDSPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayDSPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

void DelayDSPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    float gainInDecibels = gainParam->get();
    float gain = juce::Decibels::decibelsToGain(gainInDecibels);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            channelData[sample] *= gain;
        }
    }
}

//==============================================================================
bool DelayDSPAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayDSPAudioProcessor::createEditor()
{
    return new DelayDSPAudioProcessorEditor (*this);
}

//==============================================================================
void DelayDSPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayDSPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayDSPAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout DelayDSPAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(gainParamID, "Output Gain", juce::NormalisableRange<float> { -12.0f, 12.0f }, 0.0f));
    
    return layout;
}
