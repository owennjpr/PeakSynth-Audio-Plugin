/*
  ==============================================================================

    FilterADSR.h
    Created: 1 Jul 2023 11:16:46am
    Author:  Owen Prendergast
    Description: Simple ADSR class used by the SynthFilterVoice

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterADSR : public juce::ADSR
{
public:
    void update (const float attack, const float decay, const float sustain, const float release);
    
private:
    juce::ADSR::Parameters adsrParams;
    JUCE_LEAK_DETECTOR(FilterADSR);
};
