/*
  ==============================================================================

    SynthFilterVoice.cpp
    Created: 26 Jun 2023 5:44:40pm
    Author:  Owen Prendergast

  ==============================================================================
*/

#include "SynthFilterVoice.h"

bool SynthFilterVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
    
}

void SynthFilterVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
//    DBG("NOTE STARTING");
//    noteActive = true;
    adsr.noteOn();
    currentNote =juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
//    *myFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber), q, gainFactor);
    

}

void SynthFilterVoice::stopNote (float velocity, bool allowTailOff)
{
//    DBG("note stopping");
    //noteActive = false;
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

void SynthFilterVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
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
    
    //*myFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 1000, 0.5, 2);

}

void SynthFilterVoice::renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);
    
    if (! isVoiceActive()) {
        //DBG("inactive voice");
        return;
    }
    
    synthBuffer.setSize (outputBuffer.getNumChannels(), numSamples, false, false, true);
    adsr.applyEnvelopeToBuffer (synthBuffer, 0, numSamples);
    synthBuffer.clear();
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        synthBuffer.copyFrom(channel, 0, outputBuffer, channel, startSample, numSamples);
    }
    
    juce::dsp::AudioBlock<float> block { synthBuffer };
    updateFilter(adsr.getNextSample());
    myFilter.process(juce::dsp::ProcessContextReplacing<float> (block));
    
    //    auto block = juce::dsp::AudioBlock<float>(outputBuffer).getSubBlock(startSample, numSamples);
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.copyFrom (channel, startSample, synthBuffer, channel, 0, numSamples);
        
        if (! adsr.isActive()) {
            clearCurrentNote();
            currentNote = 0;
        }
    }
}

void SynthFilterVoice::update(const float newGain, const float newQ, const float a, const float d, const float s, const float r) {
//    DBG("UPDATING");
    gainFactor = newGain;
    q = newQ;
    adsr.update(a, d, s, r);
}

void SynthFilterVoice::updateFilter(const float adsrFactor) {
    if (currentNote != 0.0f) {
//        DBG(adsrFactor);
        float trueGain = std::fmax(1.0f, adsrFactor*gainFactor);
        
        *myFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), currentNote, q, trueGain);
    }
}

