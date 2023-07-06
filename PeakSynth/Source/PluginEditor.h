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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PeakSynthAudioProcessor& audioProcessor;
    AudioPlayerComponent audioPlayerComp;
    ParamControlComponent paramControlComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakSynthAudioProcessorEditor)
};
