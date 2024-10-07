/*
  ==============================================================================

    Measurement.h
    Created: 7 Oct 2024 9:30:43pm
    Author:  Johan Bremin

  ==============================================================================
*/

#pragma once

#include <atomic>

struct Measurement
{
    void reset() noexcept
    {
        value.store(0.0f);
    }
    
    void updateIfGreater(float newValue) noexcept
    {
        auto oldValue = value.load();
        while (newValue > oldValue &&
               !value.compare_exchange_weak(oldValue, newValue));
    }
    
    float readAndReset() noexcept
    {
        return value.exchange(0.0f);
    }
    
    std::atomic<float> value;
};
