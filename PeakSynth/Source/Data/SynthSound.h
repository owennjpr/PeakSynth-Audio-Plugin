/*
  ==============================================================================

    SynthSound.h
    Created: 26 Jun 2023 5:54:29pm
    Author:  Owen Prendergast
    Description: creating a standard Synthesiser Sound to be used by the synthesiser
                 object declared in pluginProcessor

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthSound :  public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override {return true; }
    bool appliesToChannel (int midiChannel) override {return true; }
};
