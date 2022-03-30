#include "RotarySlider.h"
//==============================================================================
RotarySlider::RotarySlider() :
    juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxAbove)
{
    m_param = nullptr;
}

RotarySlider::RotarySlider(juce::RangedAudioParameter* rap) : m_param(rap), juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow)
{
}

RotarySlider::~RotarySlider()
{
}

void RotarySlider::drawSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle)
{
    using namespace juce;
    this->setPaintingIsUnclipped(true);

    auto bounds = Rectangle<float>(x, y, width, height);

    g.setColour(juce::Colour(0xff252525));
    Path outline;
    float size = bounds.getHeight();
    outline.addEllipse(bounds.getCentreX() - size / 2.f, bounds.getY(), size, size);
    g.fillPath(outline);


    auto center = bounds.getCentre();

    Path p;

    Rectangle<float> r;


    // Button
    float buttonWidth = width * 0.097f / 2.f; // Constants from the Reference Design
    r.setLeft(center.getX() - buttonWidth);
    r.setRight(center.getX() + buttonWidth);
    r.setTop(bounds.getY());
    r.setBottom(bounds.getY() + width * 0.21f);

    p.addRectangle(r);

    jassert(rotaryStartAngle <= rotaryEndAngle);

    auto sliderAngleInRadians = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    p.applyTransform(AffineTransform().rotated(sliderAngleInRadians, center.getX(), center.getY()));

    g.setColour(juce::Colour(0xffc4c4c4));
    g.fillPath(p);

    this->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
}

void RotarySlider::paint(juce::Graphics& g)
{
    using namespace juce;

    setPaintingIsUnclipped(true);

    auto startAngle = degreesToRadians(180.f + 45.f);
    auto endAngle = MathConstants<float>::twoPi + degreesToRadians(180.f - 45.f);

    auto range = getRange();

    auto sliderBounds = getSliderBounds();

    this->drawSlider(g,
        sliderBounds.getX(), sliderBounds.getY(),
        sliderBounds.getWidth(),
        sliderBounds.getHeight(),
        valueToProportionOfLength(getValue()),
        startAngle,
        endAngle);
}

juce::String RotarySlider::getDisplayString() const
{
    return m_param->paramID;
}

juce::Rectangle<int> RotarySlider::getSliderBounds() const
{
    auto bounds = getLocalBounds();
    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());

    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentre());

    return r;
}