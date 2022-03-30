#include "RotaryParameter.h"

void RotaryParameter::paint(Graphics& g)
{
    using namespace juce;
    this->setPaintingIsUnclipped(true);

    auto startAngle = degreesToRadians(180.f + 45.f);
    auto endAngle = MathConstants<float>::twoPi + degreesToRadians(180.f - 45.f);

    auto bounds = getLocalBounds();
    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());
    bounds.setSize(size, size);

    g.setColour(juce::Colour(0xff252525));
    Path outline;
    outline.addEllipse(bounds.getCentreX() - size / 2.f, bounds.getY(), size, size);
    g.fillPath(outline);

    //======
    auto center = bounds.getCentre();

    Path p;

    Rectangle<float> r;


    // Button
    float buttonWidth = size * 0.097f / 2.f; // Constants from the Reference Design
    r.setLeft(center.getX() - buttonWidth);
    r.setRight(center.getX() + buttonWidth);
    r.setTop(bounds.getY());
    r.setBottom(bounds.getY() + size * 0.21f);

    p.addRectangle(r);

    jassert(startAngle <= endAngle);

    auto sliderAngleInRadians = startAngle + this->m_normalisedValue * (endAngle - startAngle);

    p.applyTransform(AffineTransform().rotated(sliderAngleInRadians, center.getX(), center.getY()));

    g.setColour(Colour(0xffc4c4c4));
    g.fillPath(p);
}
