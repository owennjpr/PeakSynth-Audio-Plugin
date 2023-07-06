/*
  ==============================================================================

    AudioPlayerComponent.h
    Created: 5 Jul 2023 5:52:13pm
    Author:  Owen Prendergast

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AudioPlayerComponent  : public juce::Component
{
public:
    AudioPlayerComponent();
    ~AudioPlayerComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerComponent)
};
