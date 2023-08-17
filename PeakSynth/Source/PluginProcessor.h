/*
  ==============================================================================

    Name: PluginProcessor.h
    Author: Owen Prendergast
    Description: The core processing for this audio plugin, the top level class
 
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
    // JUCE built in public methods
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
    
    
    // public variables
    AudioPlayerData* getFilePlayer();
    juce::AudioProcessorValueTreeState apvts;

private:
    
    //private methods
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void updateVoices(float newGain, float newQ, float a, float d, float s, float r);
    void updateVoiceCount(float newValue);
    
    //private variables
    enum stateIndex { found, paused };
    AudioPlayerData AudioFilePlayer;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    juce::dsp::Gain<float> fileGain;
    juce::dsp::Limiter<float> hardClipper;
    juce::Synthesiser synth;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakSynthAudioProcessor)
};
