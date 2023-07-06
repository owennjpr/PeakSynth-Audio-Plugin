/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PeakSynthAudioProcessorEditor::PeakSynthAudioProcessorEditor (PeakSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), audioPlayerComp(audioProcessor.getFilePlayer(), audioProcessor.apvts), paramControlComp(audioProcessor.apvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (900, 600);
    
    getLookAndFeel().setColour (juce::Slider::thumbColourId, juce::Colour (38,40,46));
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colour (155,184,146));
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colour (94,126,98));
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colour (94,126,98));
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour (155,184,146));
    getLookAndFeel().setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour (38,40,46));
    getLookAndFeel().setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour (155,184,146));

//    getLookAndFeel().setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour (38,40,46));
//    getLookAndFeel().setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour (38,40,46));
//    getLookAndFeel().setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colour (38,40,46));
//    getLookAndFeel().setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colour (38,40,46));

    
    addAndMakeVisible(audioPlayerComp);
    addAndMakeVisible(paramControlComp);
}

PeakSynthAudioProcessorEditor::~PeakSynthAudioProcessorEditor()
{
}

//==============================================================================
void PeakSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void PeakSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    audioPlayerComp.setBounds(getWidth()/2, 0, getWidth()/2, getHeight());
    paramControlComp.setBounds(0, 0, getWidth()/2, getHeight());
}
