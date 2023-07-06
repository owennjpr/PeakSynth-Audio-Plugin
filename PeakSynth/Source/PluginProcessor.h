/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Data/AudioPlayerData.h"
#include "Data/SynthFilterVoice.h"
#include "Data/SynthSound.h"


//==============================================================================
/**
*/
class PeakSynthAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    PeakSynthAudioProcessor();
    ~PeakSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    AudioPlayerData* getFilePlayer();
    juce::AudioProcessorValueTreeState apvts;

private:
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void updateVoices(float newGain, float newQ, float a, float d, float s, float r);
    void updateVoiceCount(float newValue);
    enum stateIndex { found, paused };

    AudioPlayerData AudioFilePlayer;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    using Filter = juce::dsp::IIR::Filter<float>;
    
    juce::dsp::ProcessorDuplicator<Filter, juce::dsp::IIR::Coefficients<float>> myFilter;
    juce::dsp::Gain<float> fileGain;
    juce::dsp::Limiter<float> hardClipper;
    juce::Synthesiser synth;
//    float *leaker;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakSynthAudioProcessor)
};
