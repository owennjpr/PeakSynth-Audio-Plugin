/*
  ==============================================================================

    Name: PluginEditor.h
    Author: Owen Prendergast
    Description: The top level graphical editor for this plugin, initializes the
                 audio player and param control components which handle the bulk of
                 the actual UI

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
