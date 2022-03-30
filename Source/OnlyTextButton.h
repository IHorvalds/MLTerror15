#pragma once

#include <JuceHeader.h>

class OnlyTextButton : public juce::TextButton
{
public:
    OnlyTextButton();
    ~OnlyTextButton() = default;
    juce::Font m_font;
    juce::Colour m_colour;

    void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OnlyTextButton)
};
