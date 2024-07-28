/*
  ==============================================================================

    DSP.h
    Created: 28 Jul 2024 8:41:19pm
    Author:  Johan Bremin

  ==============================================================================
*/

#pragma once

#include <cmath>

inline void panningEqualPower(float panning, float& left, float& right)
{
    float x = 0.7853981633974483f * (panning + 1.0f);
    left = std::cos(x);
    right = std::sin(x);
}
