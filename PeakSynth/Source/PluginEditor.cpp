/*
  ==============================================================================

    Name: PluginEditor.h
    Author: Owen Prendergast
    Description: The top level graphical editor for this plugin, initializes the
                 audio player and param control components which handle the bulk of
                 the actual UI

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PeakSynthAudioProcessorEditor::PeakSynthAudioProcessorEditor (PeakSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), audioPlayerComp(audioProcessor.getFilePlayer(), audioProcessor.apvts), paramControlComp(audioProcessor.apvts)
{
    setSize (900, 600);
    
    // setting color palette with look and feel
    getLookAndFeel().setColour (juce::Slider::thumbColourId, juce::Colour (38,40,46));
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colour (155,184,146));
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colour (94,126,98));
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colour (94,126,98));
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour (155,184,146));
    getLookAndFeel().setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour (38,40,46));
    getLookAndFeel().setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour (155,184,146));
    
    // generating the components
    addAndMakeVisible(audioPlayerComp);
    addAndMakeVisible(paramControlComp);
}

PeakSynthAudioProcessorEditor::~PeakSynthAudioProcessorEditor()
{
}

//==============================================================================
void PeakSynthAudioProcessorEditor::paint (juce::Graphics& g)
{

}

void PeakSynthAudioProcessorEditor::resized()
{
    // each component gets half the window
    audioPlayerComp.setBounds(getWidth()/2, 0, getWidth()/2, getHeight());
    paramControlComp.setBounds(0, 0, getWidth()/2, getHeight());
}
