#include "PowerButton.h"

PowerButton::PowerButton(juce::Image onImage, juce::Image offImage)
{
    this->onImage = onImage;
    this->offImage = offImage;
}

PowerButton::PowerButton()
{
    this->onImage = juce::ImageCache::getFromMemory(assets::Switch_png, assets::Switch_pngSize);
    this->offImage = this->onImage.createCopy();
}

void PowerButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = this->getLocalBounds().toFloat();

    juce::AffineTransform transform = this->getToggleState() ? juce::AffineTransform::verticalFlip(bounds.getHeight()).followedBy(juce::AffineTransform::translation(0.f, 20.f)) : juce::AffineTransform::identity;
    g.addTransform(transform);
    g.drawImage(this->onImage, bounds, this->getToggleState() ? juce::RectanglePlacement::yBottom : juce::RectanglePlacement::yTop);
    g.addTransform(juce::AffineTransform::identity);

}
