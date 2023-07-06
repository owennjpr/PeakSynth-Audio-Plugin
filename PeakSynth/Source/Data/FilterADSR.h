/*
  ==============================================================================

    FilterADSR.h
    Created: 1 Jul 2023 11:16:46am
    Author:  Owen Prendergast

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
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(filterADSR);
    JUCE_LEAK_DETECTOR(FilterADSR);

};
