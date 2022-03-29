#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <regex>

//==============================================================================
NeuAmpNetDemoEditor::NeuAmpNetDemoEditor(NeuAmpNetProcessor& p)
    : AudioProcessorEditor(&p), processor(p),
    inputSlider(processor.m_apvts.getParameter(param_names[Parameters::k_input])),
    outputSlider(processor.m_apvts.getParameter(param_names[Parameters::k_output])),
    bypassButton(juce::Colours::orangered, juce::Colours::darkgrey),
    modelButton("Model", "Choose a different model"),

    inputAttachment(processor.m_apvts, param_names[Parameters::k_input], inputSlider),
    outputAttachment(processor.m_apvts, param_names[Parameters::k_output], outputSlider),
    bypassButtonAttachment(processor.m_apvts, param_names[Parameters::k_bypass], bypassButton)
{
    addAndMakeVisible(inputSlider);
    addAndMakeVisible(outputSlider);
    addAndMakeVisible(modelButton);
    addAndMakeVisible(bypassButton);

    modelButton.onClick = [this] { this->ButtonClicked(); };

    setSize(600, 500);
}

NeuAmpNetDemoEditor::~NeuAmpNetDemoEditor()
{
}

inline std::string getModelName(std::string filename)
{
    // Format is : {gain}-{tone}-{volume}-{Recurrent Cell Type}-{Iteration}
    std::regex reg ("(.*)0\\.(\\d+)-0\\.(\\d+)-0\\.(\\d+)-(\\w+)-(\\w+)-(\\d+).json");


    std::cmatch cm;
    std::regex_match(filename.c_str(), cm, reg, std::regex_constants::match_default);

    std::vector<std::string> model_vars;
    for (int i = 2; i < cm.size(); ++i) {
        model_vars.push_back(cm[i]);
    }

    // Shortcut out in case we use models that don't use the normal name convention
    if (model_vars.size() < 6)
    {
        return "Unknown";
    }

    std::string model_name = "Gain 0." + model_vars[0] + ", Tone 0." + model_vars[1] + \
                             ", Volume 0." + model_vars[2] + ". " + model_vars[4] + \
                             " cell type, Variant " + model_vars[5] + ".";

    return model_name;
}

//==============================================================================
void NeuAmpNetDemoEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff3e434f));

    auto bounds = getLocalBounds();

    juce::Rectangle<float> rect;
    rect.setSize(500.f, 300.f);
    rect.setCentre(juce::Point<float>(bounds.getCentreX(), bounds.getCentreY()));
    g.setColour(juce::Colours::antiquewhite);
    g.fillRoundedRectangle(rect, 5.f);
    
    Path outline;
    auto outlineRect = rect;
    outlineRect.expand(5.f, 5.f);
    outline.addRoundedRectangle(outlineRect, 10.f);
    g.setColour(juce::Colours::orange);
    g.strokePath(outline, juce::PathStrokeType(20.f));

    // Name label
    String name = "ML Terror 15";
    Font labelFont(50.f, Font::FontStyleFlags::bold);
    g.setFont(labelFont);

    
    auto nameArea = rect.removeFromTop(1.f/3.f * rect.getHeight());
    nameArea = nameArea.removeFromRight(nameArea.getWidth() - 25.f);
    g.setColour(juce::Colours::black);
    g.drawFittedText(name, nameArea.toNearestInt(), Justification::bottomLeft, 1);

    Settings s = this->processor.getSettings();

    // Model label
    labelFont.setSizeAndStyle(20.f, Font::FontStyleFlags::bold, labelFont.getHorizontalScale(), labelFont.getExtraKerningFactor());
    g.setFont(labelFont);

    std::string modelName = s.pathToModel.toStdString();
    if (modelName == "")
    {
        modelName = "0.5-0.5-0.5-model-LSTM-1.json";
    }

    modelName = "Model: " + getModelName(modelName);

    auto modelArea = rect.removeFromTop(1.f / 2.f * rect.getHeight());
    modelArea = modelArea.removeFromRight(modelArea.getWidth() - 25.f);
    modelArea.removeFromRight(25.f);
    g.setColour(juce::Colours::darkgrey);
    g.drawFittedText(modelName, modelArea.toNearestInt(), Justification::centred, 2);



    /*juce::Path p;
    rect.removeFromBottom(20.f);
    auto bypassButtonArea = rect.removeFromBottom(50.f);
    juce::Point<float> bypassCentre = bypassButtonArea.getCentre();

    bypassButtonArea.setSize(50.f, 50.f);
    bypassButtonArea.setCentre(bypassCentre);
    p.addEllipse(bypassButtonArea);

    g.setColour(juce::Colours::antiquewhite);
    g.fillPath(p);*/
    
}

void NeuAmpNetDemoEditor::resized()
{
    auto bounds = getLocalBounds();

    juce::Rectangle<float> rect;
    rect.setSize(500.f, 300.f);
    rect.setCentre(juce::Point<float>(bounds.getCentreX(), bounds.getCentreY()));

    auto bottomArea = rect.removeFromTop(2.f/3.f * rect.getHeight());
    bottomArea = rect;
    bottomArea.expand(-25.f, 0.f);

    auto outputArea = bottomArea.removeFromLeft(1.f/3.f * bottomArea.getWidth());
    auto middleArea = bottomArea.removeFromLeft(1.f/2.f * bottomArea.getWidth());
    auto inputArea = bottomArea;

    inputSlider.setBounds(inputArea.toNearestInt());
    outputSlider.setBounds(outputArea.toNearestInt());

    middleArea.expand(-5.f, -20.f);
    modelButton.setBounds(middleArea.toNearestInt());

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
            this->repaint();
        }
    }
}
