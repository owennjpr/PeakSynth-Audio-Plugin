/*
  ==============================================================================

    ParamControlComponent.h
    Created: 26 Jun 2023 2:05:45pm
    Author:  Owen Prendergast
    Description: Header file for ParamControl Component, a graphical class which
                 creates the UI for the left side of the plugin including the title,
                 adsr knobs and gain, q, and voice selection parameters.
 
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ParamControlComponent  : public juce::Component
{
public:
    ParamControlComponent(juce::AudioProcessorValueTreeState& apvts);
    ~ParamControlComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    //declaring sliders
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::Slider gainSlider;
    juce::Slider qSlider;
    
   
    //declaring labels
    juce::Font labelFont;
    
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;
    juce::Label gainLabel;
    juce::Label qLabel;
    juce::Label voicesLabel;
    juce::Label title;
    juce::Label title2;
    
    juce::ComboBox voicesBox;
    
    void setADSRSliderParams(juce::Slider& slider, juce::Label& label, std::string labelText);
    void setHoriSliderParams(juce::Slider& slider, juce::Label& label, std::string labelText);

    // declaring attachments
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    std::unique_ptr<SliderAttachment> gainAttachment;
    std::unique_ptr<SliderAttachment> qAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> voicesAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamControlComponent)
};
