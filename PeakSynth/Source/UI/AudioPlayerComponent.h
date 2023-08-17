/*
  ==============================================================================

    AudioPlayerComponent.h
    Created: 25 Jun 2023 10:57:20am
    Author:  Owen Prendergast
    Description: The header file for the audio file player graphical component which
                 displays file controller buttons and an audio thumbnail.

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
    // constructor and deconstructor
    AudioPlayerComponent(AudioPlayerData* inputData, juce::AudioProcessorValueTreeState& apvts);
    ~AudioPlayerComponent() override;

    // other public methods
    void paint (juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback (juce::ChangeBroadcaster *source) override;


private:
    
    // private methods
    void timerCallback() override;
    void openClicked();
    void pausePlayClicked();
    void playheadButtonClicked();
    void paintThumbWithFile(juce::Graphics& g, juce::Rectangle<int>& thumbnailBounds);
    void paintThumbNoFile(juce::Graphics& g, juce::Rectangle<int>& thumbnailBounds);
    
    //private variables, enums, definitions
    enum stateIndex { found, paused };

    juce::TextButton openButton;
    juce::TextButton pauseButton;
        juce::Slider gainSlider;
    juce::TextButton playheadButton;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment;
    
    AudioPlayerData* playerData;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerComponent)
};
