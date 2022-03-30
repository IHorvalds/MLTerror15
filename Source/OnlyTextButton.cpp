#include "OnlyTextButton.h"

OnlyTextButton::OnlyTextButton() : juce::TextButton()
{
}

void OnlyTextButton::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{

    this->setPaintingIsUnclipped(true);
    auto bounds = getLocalBounds();
    auto shadow = juce::DropShadow(juce::Colour(0x88000000), 11, Point<int>(3, 4));

    juce::Path textPath;

    juce::GlyphArrangement glyphs;
    glyphs.addFittedText(this->m_font, this->getButtonText(), (float)bounds.getX(), (float)bounds.getY(),
                            (float)bounds.getWidth(), (float)bounds.getHeight(),
                            juce::Justification::centredLeft, 1);
    glyphs.createPath(textPath);
    
    g.setColour(this->m_colour);
    g.fillPath(textPath);
    shadow.drawForPath(g, textPath);
}
