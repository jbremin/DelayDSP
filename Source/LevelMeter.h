/*
  ==============================================================================

    LevelMeter.h
    Created: 10 Sep 2024 10:15:39pm
    Author:  Johan Bremin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class LevelMeter : public juce::Component, private juce::Timer
{
public:
    LevelMeter(std::atomic<float>& measurementL, std::atomic<float>& measurementR);
    
    ~LevelMeter() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    int positionForLevel(float dbLevel) const noexcept
    {
        return int(std::round(juce::jmap(dbLevel, maxdB, mindB, maxPos, minPos)));
    }
    void drawLevel(juce::Graphics& g, float level, int x, int width);
    void timerCallback() override;
    
    std::atomic<float>& measurementL;
    std::atomic<float>& measurementR;
    static constexpr float maxdB = 6.0f;
    static constexpr float mindB = -60.0f;
    static constexpr float stepdB = 6.0f;
    float maxPos = 0.0f;
    float minPos = 0.0f;
    float dbLevelL;
    float dbLevelR;
    static constexpr float clampdB = -120.0f;
    static constexpr float clampLevel = 0.000001f; // // -120 dB
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
