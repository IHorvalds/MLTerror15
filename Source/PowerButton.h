#pragma once

#include <JuceHeader.h>

class PowerButton : public juce::ToggleButton
{
public:

    PowerButton(juce::Image onImage, juce::Image offImage);
    PowerButton();
    ~PowerButton() = default;

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:

    juce::Image onImage, offImage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PowerButton)
};