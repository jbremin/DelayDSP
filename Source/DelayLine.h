/*
  ==============================================================================

    DelayLine.h
    Created: 26 Aug 2024 2:48:15pm
    Author:  Johan Bremin

  ==============================================================================
*/

#pragma once

#include <memory>

class DelayLine
{
public:
    void setMaximumDelayInSamples(int maxLengthInSamples);
    void reset() noexcept;
    
    void write(float input) noexcept;
    float read(float delayInSamples) const noexcept;
    
    int getBufferLength() const noexcept
    {
        return bufferLength;
    }
private:
    std::unique_ptr<float[]> buffer;
    int bufferLength = 0;
    int writeIndex = 0;
};
