/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ProtectYourEars.h"

//==============================================================================
DelayDSPAudioProcessor::DelayDSPAudioProcessor() :
    AudioProcessor(
        BusesProperties()
           .withInput("Input", juce::AudioChannelSet::stereo(), true)
           .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ),
    params(apvts)
{
    // do nothing
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
    params.prepareToPlay(sampleRate);
    params.reset();
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32(samplesPerBlock);
    spec.numChannels = 2;
    
    delayLine.prepare(spec);
    
    double numSamples = Parameters::maxDelayTime / 1000.0 * sampleRate;
    int maxDelayInSamples = int(std::ceil(numSamples));
    delayLine.setMaximumDelayInSamples(maxDelayInSamples);
    delayLine.reset();
    feedbackL = 0.0f;
    feedbackR = 0.0f;

}

void DelayDSPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayDSPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto mono = juce::AudioChannelSet::mono();
    const auto stereo = juce::AudioChannelSet::stereo();
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();
    
    if (mainIn == mono && mainOut == mono) { return true; }
    if (mainIn == mono && mainOut == stereo) { return true; }
    if (mainIn == stereo && mainOut == stereo) { return true; }
    
    return false;
}

void DelayDSPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    params.update();
    
    float sampleRate = float(getSampleRate());

    auto mainInput = getBusBuffer(buffer, true, 0);
    auto mainInputChannels = mainInput.getNumChannels();
    auto isMainInputStereo = mainInputChannels > 1;
    const float* inputDataL = mainInput.getReadPointer(0);
    const float* inputDataR = mainInput.getReadPointer(isMainInputStereo ? 1 : 0);
    
    auto mainOutput = getBusBuffer(buffer, false, 0);
    auto mainOutputChannels = mainOutput.getNumChannels();
    auto isMainOutputStereo = mainOutputChannels > 1;
    float* outputDataL = mainOutput.getWritePointer(0);
    float* outputDataR = mainOutput.getWritePointer(isMainOutputStereo ? 1 : 0);


    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        params.smoothen();
        
        float delayInSamples = params.delayTime / 1000.0f * sampleRate;
        delayLine.setDelay(delayInSamples);
        
        float dryL = inputDataL[sample];
        float dryR = inputDataR[sample];
        
        // convert stereo to mono
        float mono = (dryL + dryR) * 0.5f;
        // push the mono signal into the delay line
        delayLine.pushSample(0, mono*params.panL + feedbackR);
        delayLine.pushSample(1, mono*params.panR + feedbackL);
        
        float wetL = delayLine.popSample(0);
        float wetR = delayLine.popSample(1);
        
        feedbackL = wetL * params.feedback;
        feedbackR = wetR * params.feedback;
        
        float mixL = dryL + wetL * params.mix;
        float mixR = dryR + wetR * params.mix;
        
        outputDataL[sample] = mixL * params.gain;
        outputDataR[sample] = mixR * params.gain;
    }
    #if JUCE_DEBUG
    protectYourEars(buffer);
    #endif
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
    copyXmlToBinary(*apvts.copyState().createXml(), destData);

    //DBG(apvts.copyState().toXmlString());
}

void DelayDSPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayDSPAudioProcessor();
}
