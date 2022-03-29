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

    auto bounds = Rectangle<float>(x, y, width, height * 0.7);

    g.setColour(juce::Colours::black);
    Path outline;
    float size = bounds.getHeight();
    outline.addEllipse(bounds.getCentreX() - size / 2.f, bounds.getY(), size, size);
    g.fillPath(outline);


    auto center = bounds.getCentre();
    auto radius = 30.f;

    Path p;

    Rectangle<float> r;


    // Button
    r.setLeft(center.getX() - 1.7);
    r.setRight(center.getX() + 1.7);
    r.setTop(bounds.getY());
    r.setBottom(center.getY());

    p.addRectangle(r);
    p.addEllipse(center.getX() - radius / 2.f, center.getY() - radius / 2.f, radius, radius);

    jassert(rotaryStartAngle <= rotaryEndAngle);

    auto sliderAngleInRadians = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    p.applyTransform(AffineTransform().rotated(sliderAngleInRadians, center.getX(), center.getY()));

    g.setColour(Colours::darkgrey);
    g.fillPath(p);

    g.setColour(Colours::antiquewhite);
    for (int i = 0; i < 7; ++i)
    {

        Path indicator;
        Rectangle<float> indRect;


        indRect.setLeft(center.getX() - 1.2f);
        indRect.setRight(center.getX() + 1.2f);
        indRect.setTop(center.getY() - size / 2.f - 6.f);
        indRect.setBottom(center.getY() - size / 2.f);
        indicator.addRectangle(indRect);
        float radians = rotaryStartAngle + ((float)i / 6.f) * (rotaryEndAngle - rotaryStartAngle);
        indicator.applyTransform(AffineTransform().rotated(radians, center.getX(), center.getY()));
        g.fillPath(indicator);
    }

    this->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    g.setColour(Colours::black);
    g.setFont(10.f);
    auto strWidth = g.getCurrentFont().getStringWidth(juce::String(this->getValue()));
    g.drawSingleLineText(juce::String(this->getValue()), center.getX() - strWidth/2.f, center.getY() + 4.f);
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


    Font font(getTextHeight(), Font::FontStyleFlags::bold);
    g.setFont(font);
    auto strWidth = g.getCurrentFont().getStringWidth(getDisplayString());

    auto bounds = getLocalBounds();
    Rectangle<float> labelRect;

    auto height = getTextHeight() + 4;
    labelRect.setSize(strWidth + 4, height);
    labelRect.setCentre(bounds.getCentreX(), bounds.getBottom() - height);

    g.setColour(juce::Colours::black);
    g.drawFittedText(getDisplayString(), labelRect.toNearestInt(), Justification::centred, 1);
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