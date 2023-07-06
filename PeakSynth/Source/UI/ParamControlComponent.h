/*
  ==============================================================================

    ParamControlComponent.h
    Created: 5 Jul 2023 5:51:54pm
    Author:  Owen Prendergast

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
    ParamControlComponent();
    ~ParamControlComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamControlComponent)
};
