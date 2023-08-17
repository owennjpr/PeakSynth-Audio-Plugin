/*
  ==============================================================================

    SynthFilterVoice.cpp
    Created: 26 Jun 2023 5:44:40pm
    Author:  Owen Prendergast
    Description: inheriting from SynthesiserVoice, this class handles the actual IIR
              filtering for the plugin.

  ==============================================================================
*/

#include "SynthFilterVoice.h"

bool SynthFilterVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
    
}

/* when a note is pressed */
void SynthFilterVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    adsr.noteOn();
    currentNote =juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
}

/* when a note is released */
void SynthFilterVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    
    if (!allowTailOff || ! adsr.isActive()) {
        clearCurrentNote();
        currentNote = 0;
    }
    
}

void SynthFilterVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void SynthFilterVoice::pitchWheelMoved (int newPitchWheelValue) {
    
}

/* the instance of the prepare to play method for this synth voice */
void SynthFilterVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    // do not want to prepare the same voice multiple times
    if (!isPrepared) {
        juce::dsp::ProcessSpec spec;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = outputChannels;
        spec.sampleRate = sampleRate;
        adsr.setSampleRate(sampleRate);
        
        myFilter.prepare(spec);
        myFilter.reset();

        isPrepared = true;
    }
}

/* the actual audio processing for the voice */
void SynthFilterVoice::renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    // cannot render next audio block if the voice has not been prepared
    jassert(isPrepared);
    
    if (! isVoiceActive()) {
        return;
    }
    
    // copying data over into the synth buffer
    
    synthBuffer.setSize (outputBuffer.getNumChannels(), numSamples, false, false, true);
    adsr.applyEnvelopeToBuffer (synthBuffer, 0, numSamples);
    synthBuffer.clear();
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        synthBuffer.copyFrom(channel, 0, outputBuffer, channel, startSample, numSamples);
    }
    
    juce::dsp::AudioBlock<float> block { synthBuffer };
    updateFilter(adsr.getNextSample());
    myFilter.process(juce::dsp::ProcessContextReplacing<float> (block));
    
    // copying back into the output buffer
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.copyFrom (channel, startSample, synthBuffer, channel, 0, numSamples);
        
        if (! adsr.isActive()) {
            clearCurrentNote();
            currentNote = 0;
        }
    }
}

/* update method for all of the voice's parameters*/
void SynthFilterVoice::update(const float newGain, const float newQ, const float a, const float d, const float s, const float r)
{
    gainFactor = newGain;
    q = newQ;
    adsr.update(a, d, s, r);
}

/* updating the level of filter gain every render call to match with the adsr factor */
void SynthFilterVoice::updateFilter(const float adsrFactor)
{
    if (currentNote != 0.0f) {
        float trueGain = std::fmax(1.0f, adsrFactor*gainFactor);
        
        *myFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), currentNote, q, trueGain);
    }
}

