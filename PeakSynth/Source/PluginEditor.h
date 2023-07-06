/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AudioPlayerComponent.h"
#include "UI/ParamControlComponent.h"

//==============================================================================
/**
*/
class PeakSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PeakSynthAudioProcessorEditor (PeakSynthAudioProcessor&);
    ~PeakSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PeakSynthAudioProcessor& audioProcessor;
    
    AudioPlayerComponent audioPlayerComp;
    ParamControlComponent paramControlComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakSynthAudioProcessorEditor)
};
