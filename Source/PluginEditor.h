#pragma once

#include "Parameters.h"
#include "PluginProcessor.h"
#include "RotarySlider.h"
#include "BypassButton.h"

//==============================================================================
class NeuAmpNetDemoEditor : public juce::AudioProcessorEditor
{
public:
    explicit NeuAmpNetDemoEditor(NeuAmpNetProcessor&);
    ~NeuAmpNetDemoEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NeuAmpNetProcessor& processor;

    using APVTS = juce::AudioProcessorValueTreeState;
    using SliderAttachment = APVTS::SliderAttachment;
    using ButtonAttachment = APVTS::ButtonAttachment;

    RotarySlider inputSlider, outputSlider;
    BypassButton bypassButton;
    TextButton modelButton;
    SliderAttachment inputAttachment, outputAttachment;
    ButtonAttachment  bypassButtonAttachment;

    void ButtonClicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuAmpNetDemoEditor)
};