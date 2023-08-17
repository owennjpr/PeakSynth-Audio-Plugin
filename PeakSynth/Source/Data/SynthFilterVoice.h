/*
  ==============================================================================

    SynthFilterVoice.h
    Created: 26 Jun 2023 5:44:40pm
    Author:  Owen Prendergast
    Description: inheriting from SynthesiserVoice, this class handles the actual IIR
                 filtering for the plugin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "FilterADSR.h"

class SynthFilterVoice : public juce::SynthesiserVoice
{
public:
    // necessary SynthesiserVoice methods
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    
    // other public methods
    void update(const float newGain, const float newQ, const float a, const float d, const float s, const float r);

private:
    // private methods
    void updateFilter(const float adsrFactor);
    
    // private variables
    using Filter = juce::dsp::IIR::Filter<float>;
    juce::dsp::ProcessorDuplicator<Filter, juce::dsp::IIR::Coefficients<float>> myFilter;
    
    juce::AudioBuffer<float> synthBuffer;
    FilterADSR adsr;
    
    float gainFactor;
    float q;
    float currentNote;

    bool isPrepared { false };
    
    JUCE_LEAK_DETECTOR(SynthFilterVoice);

};
