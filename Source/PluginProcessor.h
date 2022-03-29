#pragma once

#include <JuceHeader.h>
#include <RTNeural/RTNeural.h>

using Oversampler = juce::dsp::Oversampling<float>;
using Resampler = juce::ResamplingAudioSource;
using Gain = juce::dsp::Gain<float>;
using Model = RTNeural::Model<float>;

struct Settings
{
    float input{ 0 }, output{ 0 };
    bool bypass{ false };
    juce::String pathToModel { 0 };
};

class NeuAmpNetProcessor : public juce::AudioProcessor
{
public:
    NeuAmpNetProcessor();
    ~NeuAmpNetProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    Settings getSettings();

    static juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout();
    juce::AudioProcessorValueTreeState m_apvts{ *this, nullptr, "Parameters", CreateParameterLayout() };

    void SetModelPath(juce::String s);
private:

    std::unique_ptr<Model> _model_ptr;
    //Oversampler os{ 2, 2, Oversampler::FilterType::filterHalfBandPolyphaseIIR };
    Gain _input_level, _output_level;
    bool _should_set_model = false;

    void LoadModel(); // just load the default. 0.5g - 0.5t - 0.5v
    void LoadModel(juce::File& model_weights);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuAmpNetProcessor)
};