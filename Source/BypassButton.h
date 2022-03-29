#pragma once

#include <JuceHeader.h>

class BypassButton : public juce::ToggleButton
{

public:

    BypassButton(juce::Colour onColour, juce::Colour offColour);
    BypassButton();
    ~BypassButton();

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:

    juce::Colour m_onColour = juce::Colours::orangered, m_offColour = juce::Colours::lightgrey;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BypassButton)
};