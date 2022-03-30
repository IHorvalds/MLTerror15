#pragma once

#include <JuceHeader.h>

class RotaryParameter : public juce::Component
{
public:

    RotaryParameter() {}
    ~RotaryParameter() = default;

    float m_normalisedValue = 0.f;
    void paint(Graphics& g) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryParameter)
};