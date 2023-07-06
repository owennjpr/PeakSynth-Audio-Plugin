/*
  ==============================================================================

    AudioPlayerComponent.cpp
    Created: 5 Jul 2023 5:52:13pm
    Author:  Owen Prendergast

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioPlayerComponent.h"

//==============================================================================
AudioPlayerComponent::AudioPlayerComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

AudioPlayerComponent::~AudioPlayerComponent()
{
}

void AudioPlayerComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("AudioPlayerComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void AudioPlayerComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
