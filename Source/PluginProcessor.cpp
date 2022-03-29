#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <fstream>

//==============================================================================
NeuAmpNetProcessor::NeuAmpNetProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
{
}

NeuAmpNetProcessor::~NeuAmpNetProcessor()
{
}

//==============================================================================
const juce::String NeuAmpNetProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NeuAmpNetProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool NeuAmpNetProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool NeuAmpNetProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double NeuAmpNetProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NeuAmpNetProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NeuAmpNetProcessor::getCurrentProgram()
{
    return 0;
}

void NeuAmpNetProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String NeuAmpNetProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void NeuAmpNetProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void NeuAmpNetProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{

    juce::ignoreUnused(sampleRate, samplesPerBlock);
    auto settings = getSettings();
    if (settings.pathToModel != "") {
        juce::File f(settings.pathToModel);
        this->LoadModel(f);
    }
    else {
        this->LoadModel();
    }
    if (this->_model_ptr){
        this->_model_ptr->reset();
        // Feed the model a few samples of silence to initialize state
        constexpr size_t state_initialization_buffer_size = 1<<12;
        //float x[] = { (1.f/(float)sampleRate), 0.f};
        float x = 0.f;

        for (size_t i = 0; i < state_initialization_buffer_size; ++i)
        {
            this->_model_ptr->forward(&x);
        }
    }
}

void NeuAmpNetProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool NeuAmpNetProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void NeuAmpNetProcessor::processBlock(juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    auto settings = getSettings();
    this->_input_level.setGainDecibels(settings.input);
    this->_output_level.setGainDecibels(settings.output);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    if (this->_should_set_model || !this->_model_ptr) {
        if (settings.pathToModel != "") {
            juce::File f(settings.pathToModel);
            this->LoadModel(f);
        }
        else {
            this->LoadModel();
        }
    }

    const float* input = buffer.getReadPointer(0); // Implicitly process left channel. This is used when plugins are used in mono config.
    for (int n = 0; n < numSamples; ++n) {
        if (!settings.bypass)
        {
            float x = this->_input_level.processSample(input[n]);
            if (this->_model_ptr != nullptr) // maybe it didn't load the model correctly??
            {
                //float x_in[] = { (1.f / (float)getSampleRate()), x};
                x = this->_model_ptr->forward(&x);
            }
            float y_out = this->_output_level.processSample(x);

            for (int channel = 0; channel < totalNumInputChannels; ++channel)
            {
                auto* output = buffer.getWritePointer(channel);
                output[n] = y_out;
            }
        }
    }
}

void NeuAmpNetProcessor::LoadModel(juce::File& model_weights)
{
    auto tmp_full_path = model_weights.getFullPathName().toStdString();
    const char* path = tmp_full_path.c_str();

    std::ifstream model_file = std::ifstream(path, std::ifstream::binary);
    
    this->_model_ptr = RTNeural::json_parser::parseJson<float>(model_file);

    // if we make it this far, save the path in the state

    juce::Value model_path = m_apvts.state.getPropertyAsValue(juce::Identifier(param_names[Parameters::k_model]), nullptr, true);
    model_path.setValue(juce::var(path));
    this->_should_set_model = false;
}

void NeuAmpNetProcessor::LoadModel()
{
    juce::MemoryInputStream defaultModelStream(default_models::_0_50_50_5modellstm1_json, default_models::_0_50_50_5modellstm1_jsonSize, false);
    auto model_weights = nlohmann::json::parse(defaultModelStream.readEntireStreamAsString().toStdString());
    DBG(defaultModelStream.readEntireStreamAsString().toStdString());

    this->_model_ptr = RTNeural::json_parser::parseJson<float>(model_weights);
    this->_should_set_model = false;
}

//==============================================================================
bool NeuAmpNetProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NeuAmpNetProcessor::createEditor()
{
    return new NeuAmpNetDemoEditor(*this);
}

//==============================================================================
void NeuAmpNetProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    m_apvts.state.writeToStream(mos);
}

void NeuAmpNetProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid()) {
        m_apvts.replaceState(tree);
        Settings settings = getSettings();
        return;
    }
    // value tree from scratch. Create property for model file path
    juce::Value model_path(juce::var(""));
    m_apvts.state.setProperty(juce::Identifier(param_names[Parameters::k_model]), model_path, nullptr);
}

Settings NeuAmpNetProcessor::getSettings()
{
    Settings settings;

    // Bypass Button
    settings.bypass = m_apvts.getRawParameterValue(param_names[Parameters::k_bypass])->load();

    // Input Volume
    settings.input = m_apvts.getRawParameterValue(param_names[Parameters::k_input])->load();

    juce::Value model_path = m_apvts.state.getPropertyAsValue(juce::Identifier(param_names[Parameters::k_model]), nullptr, true);
    settings.pathToModel = model_path.toString();

    // Output level
    settings.output = m_apvts.getRawParameterValue(param_names[Parameters::k_output])->load();

    return settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout NeuAmpNetProcessor::CreateParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Bypass Button
    layout.add(std::make_unique<juce::AudioParameterBool>(param_names[Parameters::k_bypass],
        param_names[Parameters::k_bypass],
        false));

    // input
    layout.add(std::make_unique<juce::AudioParameterFloat>(param_names[Parameters::k_input],
        param_names[Parameters::k_input],
        juce::NormalisableRange<float>(-50.f, 10.f, .1f, 3.75f),
        0.f));

    // Output Volume
    layout.add(std::make_unique<juce::AudioParameterFloat>(param_names[Parameters::k_output],
        param_names[Parameters::k_output],
        juce::NormalisableRange<float>(-50.f, 10.f, 1.f, 3.75f),
        0.f));

    return layout;
}

void NeuAmpNetProcessor::SetModelPath(juce::String s)
{
    m_apvts.state.setProperty(juce::Identifier(param_names[Parameters::k_model]), juce::var(s), nullptr);
    this->_should_set_model = true;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeuAmpNetProcessor();
}