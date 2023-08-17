/*
  ==============================================================================

    ParamControlComponent.h
    Created: 26 Jun 2023 2:05:45pm
    Author:  Owen Prendergast
    Description: Implementation for ParamControl Component, a graphical class which
                 creates the UI for the left side of the plugin including the title,
                 adsr knobs and gain, q, and voice selection parameters.
 
  ==============================================================================
*/


#include <JuceHeader.h>
#include "ParamControlComponent.h"

//==============================================================================
ParamControlComponent::ParamControlComponent(juce::AudioProcessorValueTreeState& apvts)
{
    // initializing attachments for all apvts associated parameter sliders
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    attackAttachment = std::make_unique<SliderAttachment>(apvts, "attack", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "decay", decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "sustain", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "release", releaseSlider);
    gainAttachment = std::make_unique<SliderAttachment>(apvts, "filter gain", gainSlider);
    qAttachment = std::make_unique<SliderAttachment>(apvts, "filter q", qSlider);
    
    // setting font look and feel for title
    labelFont.getDefaultMonospacedFontName();
    labelFont.setTypefaceStyle("Bold");
    
    // adding the title
    addAndMakeVisible(title);
    title.setText("Peak", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::topLeft);
    title.setFont(juce::Font (60.0f, juce::Font::bold));
    title.setColour(juce::Label::textColourId, juce::Colour (38,40,46));
    
    addAndMakeVisible(title2);
    title2.setText("Synth", juce::dontSendNotification);
    title2.setJustificationType(juce::Justification::topLeft);
    title2.setFont(juce::Font (60.0f, juce::Font::bold));
    title2.setColour(juce::Label::textColourId, juce::Colour (94,126,98));

    // setting the slider styles for each parameter
    setADSRSliderParams(attackSlider, attackLabel, "Attack");
    setADSRSliderParams(decaySlider, decayLabel, "Decay");
    setADSRSliderParams(sustainSlider, sustainLabel, "Sustain");
    setADSRSliderParams(releaseSlider, releaseLabel, "Release");
    
    setHoriSliderParams(gainSlider, gainLabel, "Gain Factor");
    setHoriSliderParams(qSlider, qLabel, "Q");
    
    // initializing the voice combo box attachment
    voicesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "voices", voicesBox);
    
    int initCount = apvts.getRawParameterValue("voices")->load();
    if (initCount == 0) {
        initCount = 1;
    } else if (initCount == 1) {
        initCount = 4;
    } else if (initCount == 2) {
        initCount = 8;
    }

    // setting the combo box style
    voicesBox.setTextWhenNothingSelected(std::to_string(initCount));
    voicesBox.addItem("1", 1);
    voicesBox.addItem("4", 2);
    voicesBox.addItem("8", 3);
    addAndMakeVisible(voicesBox);
    
    addAndMakeVisible(voicesLabel);
    voicesLabel.setText("Num Voices", juce::dontSendNotification);
    voicesLabel.attachToComponent(&voicesBox, false);
    voicesLabel.setJustificationType(juce::Justification::centredTop);
    voicesLabel.setFont(labelFont);
    voicesLabel.setColour(juce::Label::textColourId, juce::Colour (38,40,46));

}

ParamControlComponent::~ParamControlComponent()
{
}

/* drawing the component */
void ParamControlComponent::paint (juce::Graphics& g)
{
    // simple bound coloring
    g.fillAll (juce::Colour (223, 239, 211));
    g.setColour (juce::Colour (155,184,146));
    g.drawRect (getLocalBounds(), 1);
}

/* handling bounds for the component*/
void ParamControlComponent::resized()
{
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth()/2;
    const auto sliderHeight = sliderWidth - 50;
    const auto sliderStartX = 0;
    const auto horizontalSliderStartX = 75;
    const auto sliderStartY = bounds.getHeight() - 165;
    
    
    sustainSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    attackSlider.setBounds(sliderStartX, sliderStartY - sliderHeight - 3*padding, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY - sliderHeight  - 3*padding, sliderWidth, sliderHeight);

    gainSlider.setBounds(horizontalSliderStartX, 100, bounds.getWidth() - 150, 30);
    qSlider.setBounds(horizontalSliderStartX, gainSlider.getBottom() + padding, bounds.getWidth()  - 150, 30);

    voicesBox.setBounds(bounds.getRight() - 75, gainSlider.getBottom() + padding, 75, 30);
    
    title.setBounds(10, 10, 150, 100);
    title2.setBounds(title.getRight() - 25, 10, 150, 100);

}

/* initializing the style for the ADSR rotary dial sliders */
void ParamControlComponent::setADSRSliderParams(juce::Slider& slider, juce::Label& label, std::string labelText)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 25);
    slider.setTextBoxIsEditable(true);
    slider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour (94,126,98));
    slider.setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colour (94,126,98));
    slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour (94,126,98));

    addAndMakeVisible(slider);
    
    addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.attachToComponent(&slider, false);
    label.setJustificationType(juce::Justification::centredTop);
    label.setFont(labelFont);
    label.setColour(juce::Label::textColourId, juce::Colour (38,40,46));


}

/* initializing the style for the horizontal sliders used in this component */
void ParamControlComponent::setHoriSliderParams(juce::Slider& slider, juce::Label& label, std::string labelText)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 50, 25);
    slider.setTextBoxIsEditable(true);
    slider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour (94,126,98));
    slider.setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colour (94,126,98));
    slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour (94,126,98));

    addAndMakeVisible(slider);
    
    addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.attachToComponent(&slider, true);
    label.setJustificationType(juce::Justification::centredRight);
    label.setFont(labelFont);
    label.setColour(juce::Label::textColourId, juce::Colour (38,40,46));

}
