#pragma once

#include <JuceHeader.h>

class RotarySlider : public juce::Slider
{
public:
    RotarySlider();
    RotarySlider(juce::RangedAudioParameter* rap);
    ~RotarySlider() override;

    void paint(juce::Graphics&) override;
    juce::Rectangle<int> getSliderBounds() const;
    juce::String getDisplayString() const;
    int getTextHeight() const { return 14; }

private:
    juce::RangedAudioParameter* m_param;

    void drawSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotarySlider)
};

