#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <regex>

//==============================================================================
NeuAmpNetDemoEditor::NeuAmpNetDemoEditor(NeuAmpNetProcessor& p)
    : AudioProcessorEditor(&p), processor(p),
    inputSlider(processor.m_apvts.getParameter(param_names[Parameters::k_input])),
    outputSlider(processor.m_apvts.getParameter(param_names[Parameters::k_output])),
    bypassButton(juce::Colour(0xff707070), juce::Colour(0xff408BB5)),
    powerButton(),
    modelButton(),
    gainParameter(),
    toneParameter(),
    volumeParameter(),

    inputAttachment(processor.m_apvts, param_names[Parameters::k_input], inputSlider),
    outputAttachment(processor.m_apvts, param_names[Parameters::k_output], outputSlider),
    bypassButtonAttachment(processor.m_apvts, param_names[Parameters::k_bypass], bypassButton),
    powerButtonAttachment(processor.m_apvts, param_names[Parameters::k_bypass], powerButton)
{
    addAndMakeVisible(inputSlider);
    addAndMakeVisible(outputSlider);
    addAndMakeVisible(modelButton);
    addAndMakeVisible(bypassButton);
    addAndMakeVisible(powerButton);

    addAndMakeVisible(gainParameter);
    addAndMakeVisible(toneParameter);
    addAndMakeVisible(volumeParameter);

    modelButton.onClick = [this] { this->ButtonClicked(); };

    this->startTimerHz(60);

    setSize(1055, 544);
}

NeuAmpNetDemoEditor::~NeuAmpNetDemoEditor()
{
}

inline std::pair<std::string, std::tuple<float, float, float>> getModelName(std::string filename)
{
    // Format is : {gain}-{tone}-{volume}-{extra info}.json
    std::regex reg ("(.*)0\\.(\\d+)-0\\.(\\d+)-0\\.(\\d+)-(\.*)\.json");

    std::cmatch cm;
    std::regex_match(filename.c_str(), cm, reg, std::regex_constants::match_default);

    std::vector<std::string> model_vars;
    for (int i = 2; i < cm.size(); ++i) {
        model_vars.push_back(cm[i]);
    }

    // Shortcut out in case we use models that don't use the normal name convention
    if (model_vars.size() < 4)
    {
        return std::make_pair(std::string("Unknown"), std::tuple(0.5f, 0.5f, 0.5f));
    }


    std::for_each(model_vars[3].begin(), model_vars[3].end(), [](char& c) {
        c = ::toupper(c);
    });
    std::string model_name = model_vars[3];

    float gain = std::stof("0." + model_vars[0]);
    float tone = std::stof("0." + model_vars[1]);
    float volume = std::stof("0." + model_vars[2]);

    return std::make_pair(model_name, std::tuple(gain, tone, volume));
}

//==============================================================================
void NeuAmpNetDemoEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff3e434f));

    juce::Image background = juce::ImageCache::getFromMemory(assets::UIBlank_png, assets::UIBlank_pngSize);

    auto bounds = getLocalBounds();
    g.drawImage(background, bounds.toFloat(), juce::RectanglePlacement::fillDestination);

    modelButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0x00ffffff));
    modelButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(0x00ffffff));
    modelButton.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colour(0xffffffff));

    auto settings = processor.getSettings();
    std::string modelName = settings.pathToModel.toStdString();
    if (modelName == "")
    {
        modelName = "0.5-0.5-0.5-model-LSTM-1.json";
    }

    modelButton.m_font = juce::Font(25.f, juce::Font::underlined);
    modelButton.m_colour = juce::Colours::white;

    std::pair<std::string, std::tuple<float, float, float>> model = getModelName(modelName);
    modelButton.setButtonText("Model: " + model.first);

    auto [gain, tone, volume] = model.second;

    gainParameter.m_normalisedValue = gain;
    toneParameter.m_normalisedValue = tone;
    volumeParameter.m_normalisedValue = volume;

    juce::Rectangle<int> clippingLED(915, 451, 12, 12);
    if (processor.isClipping.value)
    {
        g.setColour(juce::Colours::orangered);
    }
    else
    {
        g.setColour(juce::Colour(0xff42c1a3));
    }
    g.fillEllipse(clippingLED.toFloat());
}

void NeuAmpNetDemoEditor::resized()
{
    auto bounds = getLocalBounds();

    // Input button
    juce::Rectangle<float> inputArea(975.94f, 345.f, 23.11f, 23.11f);
    inputSlider.setBounds(inputArea.toNearestInt());

    // Output button
    juce::Rectangle<float> outputArea(63.94f, 347.f, 23.11f, 23.11f);
    outputSlider.setBounds(outputArea.toNearestInt());

    // Power button
    juce::Rectangle<int> powerButtonArea(107, 357, 60, 176);
    powerButton.setBounds(powerButtonArea);

    // Bypass button
    juce::Rectangle<int> bypassButtonArea(187, 469, 22, 22);
    bypassButton.setBounds(bypassButtonArea);

    // Model button
    juce::Rectangle<int> modelButtonArea(107, 270, 270, 30);
    modelButton.setBounds(modelButtonArea);

    // Gain
    juce::Rectangle<int> gainButtonArea(323, 366, 82, 82);
    gainParameter.setBounds(gainButtonArea);
    
    // Tone
    juce::Rectangle<int> toneButtonArea(532, 366, 82, 82);
    toneParameter.setBounds(toneButtonArea);
    
    // Volume
    juce::Rectangle<int> volumeButtonArea(741, 366, 82, 82);
    volumeParameter.setBounds(volumeButtonArea);
}

void NeuAmpNetDemoEditor::ButtonClicked()
{
    juce::FileChooser fileChooser("Choose a model", juce::File(), "*.json");
    
    if (fileChooser.browseForFileToOpen())
    {
        juce::File file = fileChooser.getResult();
        if (file.existsAsFile())
        {
            processor.SetModelPath(file.getFullPathName());
            //this->repaint();
        }
    }
}

void NeuAmpNetDemoEditor::timerCallback()
{
    this->repaint();
}
