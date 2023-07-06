/*
  ==============================================================================

    AudioPlayerComponent.h
    Created: 25 Jun 2023 10:57:20am
    Author:  Owen Prendergast

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Data/AudioPlayerData.h"
//==============================================================================
/*
*/
class AudioPlayerComponent  : public juce::Component, public juce::ChangeListener, private juce::Timer

{
public:
    AudioPlayerComponent(AudioPlayerData* inputData, juce::AudioProcessorValueTreeState& apvts);
    ~AudioPlayerComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback (juce::ChangeBroadcaster *source) override;


private:
    enum stateIndex { found, paused };

    void timerCallback() override;

    juce::TextButton openButton;
    juce::TextButton pauseButton;

    void openClicked();
    void pausePlayClicked();
    void playheadButtonClicked();
    void paintThumbWithFile(juce::Graphics& g, juce::Rectangle<int>& thumbnailBounds);
    void paintThumbNoFile(juce::Graphics& g, juce::Rectangle<int>& thumbnailBounds);
    
    juce::Slider gainSlider;
    juce::TextButton playheadButton;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment;
    
    AudioPlayerData* playerData;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerComponent)
};
