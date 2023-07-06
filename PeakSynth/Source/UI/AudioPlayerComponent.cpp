/*
  ==============================================================================

    AudioPlayerComponent.cpp
    Created: 25 Jun 2023 10:57:20am
    Author:  Owen Prendergast

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioPlayerComponent.h"

//==============================================================================
AudioPlayerComponent::AudioPlayerComponent(AudioPlayerData* inputData, juce::AudioProcessorValueTreeState& apvts)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open File");
    openButton.onClick = [this] {openClicked();};
    openButton.setColour (juce::TextButton::buttonColourId, juce::Colour (94,126,98));


    addAndMakeVisible (&pauseButton);
    pauseButton.setButtonText (" ");
    //pauseButton.setColour (juce::TextButton::buttonColourId, juce::Colours::grey);
    pauseButton.onClick = [this] { pausePlayClicked(); };
    pauseButton.setEnabled (false);
    
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "file gain", gainSlider);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 50, 25);
    gainSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour (94,126,98));
    gainSlider.setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colour (94,126,98));
    gainSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour (94,126,98));
    
    addAndMakeVisible(gainSlider);

    
//    playerData->pauseWithDAW = false;
//    playheadAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "auto pause", playheadButton);
    playheadButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour (223, 239, 211));
    playheadButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour (94,126,98));
    playheadButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colour (38,40,46));

    playheadButton.onClick = [this] { playheadButtonClicked(); };

    addAndMakeVisible(playheadButton);

    playerData = inputData;
    
    startTimer(250);
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

    g.fillAll (juce::Colour (223, 239, 211));   // clear the background

    g.setColour (juce::Colour (155,184,146));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    juce::Rectangle<int> thumbnailBounds (10, 100, getWidth() - 20, getHeight() - 120);
    if (playerData->getFileState(found)) {
        playheadButton.setEnabled(true);
        playheadButton.setButtonText("Pause With DAW");

        if (!playerData->pauseWithDAW) {
            pauseButton.setEnabled (true);
        }
        if (playerData->getFileState(paused)) {
            pauseButton.setButtonText ("Play");
        } else {
            pauseButton.setButtonText ("Pause");
        }
        pauseButton.setColour (juce::TextButton::buttonColourId, juce::Colour (94,126,98));
        paintThumbWithFile(g, thumbnailBounds);

    } else {
        pauseButton.setEnabled (false);
        playheadButton.setEnabled(false);
        playheadButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour (223, 239, 211));
        playheadButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colour (38,40,46));

        playerData->pauseWithDAW = false;
        playheadButton.setButtonText(" ");
        pauseButton.setButtonText (" ");
        pauseButton.setColour (juce::TextButton::buttonColourId, juce::Colour (223, 239, 211));
        paintThumbNoFile(g, thumbnailBounds);

    }

}

void AudioPlayerComponent::paintThumbWithFile(juce::Graphics& g, juce::Rectangle<int>& thumbnailBounds) {
    g.setColour (juce::Colour (155,184,146));
    g.fillRect (thumbnailBounds);

    g.setColour (juce::Colour (94,126,98));
    auto audioLength = (float) playerData->getThumbnail()->getTotalLength();
    playerData->getThumbnail()->drawChannels (g, thumbnailBounds, 0.0, audioLength, 1.0f);
    g.setColour (juce::Colour (223, 239, 211));
    auto audioPosition = (float) playerData->getTSource()->getCurrentPosition();
    auto drawPosition = (audioPosition / audioLength) * (float) thumbnailBounds.getWidth() + (float) thumbnailBounds.getX();
    g.drawLine (drawPosition, (float) thumbnailBounds.getY(), drawPosition, (float) thumbnailBounds.getBottom(), 2.0f);
}

void AudioPlayerComponent::paintThumbNoFile(juce::Graphics& g, juce::Rectangle<int>& thumbnailBounds) {
    g.setColour(juce::Colour (155,184,146));
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colour (38,40,46));
    g.drawFittedText ("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}


void AudioPlayerComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    openButton.setBounds (10, 10, getWidth() - 20, 20);
    pauseButton.setBounds (10, 40, getWidth() - 20, 20);
    gainSlider.setBounds(10, 70, getWidth() - 100, 20);
    playheadButton.setBounds(getWidth() - 90, 70, 80, 20);

}

void AudioPlayerComponent::openClicked() {
    playerData->getNewFile();
}


void AudioPlayerComponent::pausePlayClicked() {
    playerData->pauseOrPlay();
}

void AudioPlayerComponent::playheadButtonClicked() {
    //DBG("HI");
    playerData->pauseWithDAW = !(playerData->pauseWithDAW);
    if (playerData->pauseWithDAW) {
        playheadButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour (94,126,98));
        playheadButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
        pauseButton.setEnabled (false);

    } else {
        playheadButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour (223, 239, 211));
        playheadButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colour (38,40,46));
        pauseButton.setEnabled (true);
    }
    repaint();
}

void AudioPlayerComponent::timerCallback()
{
    repaint();
}

void AudioPlayerComponent::changeListenerCallback (juce::ChangeBroadcaster *source) {
    if (source == playerData->getThumbnail()) {
        repaint();
    }
}
