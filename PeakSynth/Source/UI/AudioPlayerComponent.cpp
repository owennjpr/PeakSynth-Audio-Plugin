/*
  ==============================================================================

    AudioPlayerComponent.h
    Created: 25 Jun 2023 10:57:20am
    Author:  Owen Prendergast
    Description: The implementation for the audio file player graphical component
                 which displays file controller buttons and an audio thumbnail.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioPlayerComponent.h"

// constructor
AudioPlayerComponent::AudioPlayerComponent(AudioPlayerData* inputData, juce::AudioProcessorValueTreeState& apvts)
{
    //creating the open file button
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open File");
    openButton.onClick = [this] {openClicked();};
    openButton.setColour (juce::TextButton::buttonColourId, juce::Colour (94,126,98));

    // creating the pause playback button
    addAndMakeVisible (&pauseButton);
    pauseButton.setButtonText (" ");
    pauseButton.onClick = [this] { pausePlayClicked(); };
    pauseButton.setEnabled (false);
    
    // creating a gain slider
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "file gain", gainSlider);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 50, 25);
    gainSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour (94,126,98));
    gainSlider.setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colour (94,126,98));
    gainSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour (94,126,98));
    
    addAndMakeVisible(gainSlider);

    // creating the button for matching playback with daw
    playheadButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour (223, 239, 211));
    playheadButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour (94,126,98));
    playheadButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colour (38,40,46));
    playheadButton.onClick = [this] { playheadButtonClicked(); };
    addAndMakeVisible(playheadButton);

    playerData = inputData;
    
    // starting timer callback for the audio file progress playhead
    startTimer(50);
}

// deconstructor
AudioPlayerComponent::~AudioPlayerComponent()
{
}

/* drawing the component */
void AudioPlayerComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (223, 239, 211));

    g.setColour (juce::Colour (155,184,146));
    g.drawRect (getLocalBounds(), 1);
    
    juce::Rectangle<int> thumbnailBounds (10, 100, getWidth() - 20, getHeight() - 120);
    
    // must repaint if the file state has changed
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

/* painting the thumbnail with a selected file */
void AudioPlayerComponent::paintThumbWithFile(juce::Graphics& g, juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour (juce::Colour (155,184,146));
    g.fillRect (thumbnailBounds);

    g.setColour (juce::Colour (94,126,98));
    auto audioLength = (float) playerData->getThumbnail()->getTotalLength();
    playerData->getThumbnail()->drawChannels (g, thumbnailBounds, 0.0, audioLength, 1.0f);
    g.setColour (juce::Colour (223, 239, 211));
    
    // adding a playhead for the thumbnail
    auto audioPosition = (float) playerData->getTSource()->getCurrentPosition();
    auto drawPosition = (audioPosition / audioLength) * (float) thumbnailBounds.getWidth() + (float) thumbnailBounds.getX();
    g.drawLine (drawPosition, (float) thumbnailBounds.getY(), drawPosition, (float) thumbnailBounds.getBottom(), 2.0f);
}

/* painting the empty thumbnail */
void AudioPlayerComponent::paintThumbNoFile(juce::Graphics& g, juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour(juce::Colour (155,184,146));
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colour (38,40,46));
    g.drawFittedText ("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

/* setting component dimensions */
void AudioPlayerComponent::resized()
{
    openButton.setBounds (10, 10, getWidth() - 20, 20);
    pauseButton.setBounds (10, 40, getWidth() - 20, 20);
    gainSlider.setBounds(10, 70, getWidth() - 100, 20);
    playheadButton.setBounds(getWidth() - 90, 70, 80, 20);

}

/* function call for when the open file button is clicked */
void AudioPlayerComponent::openClicked()
{
    playerData->getNewFile();
}

/* function call for when the pause/play  button is clicked */
void AudioPlayerComponent::pausePlayClicked()
{
    playerData->pauseOrPlay();
}

/* function call for when the match playhead button is clicked */
void AudioPlayerComponent::playheadButtonClicked()
{
    // toggle it and redraw
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

void AudioPlayerComponent::changeListenerCallback (juce::ChangeBroadcaster *source)
{
    if (source == playerData->getThumbnail()) {
        repaint();
    }
}
