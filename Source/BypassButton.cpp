#include "BypassButton.h"

BypassButton::BypassButton(juce::Colour onColour, juce::Colour offColour) :
    juce::ToggleButton(),
    m_onColour(onColour), m_offColour(offColour)
{
}

BypassButton::BypassButton() : juce::ToggleButton()
{
}

BypassButton::~BypassButton()
{
}

void BypassButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    using namespace juce;

    auto bounds = this->getLocalBounds().toFloat();


    Path p;
    float size = jmin(bounds.getWidth(), bounds.getHeight());
    p.addEllipse(bounds.getCentreX() - size / 2.f, bounds.getCentreY() - size / 2.f, size, size);

    g.setColour(this->getToggleState() ? this->m_onColour : this->m_offColour);
    g.fillPath(p);

}