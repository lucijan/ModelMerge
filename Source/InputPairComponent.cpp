#include "InputPairComponent.h"

InputPairComponent::InputPairComponent(juce::PropertiesFile *defaults, int firstChan) :
    m_firstChan(firstChan),
    m_stereoToggle("link", juce::DrawableButton::ButtonStyle::ImageFitted)
{
    addAndMakeVisible(m_label);
    addAndMakeVisible(m_stereoToggle);
    addAndMakeVisible(m_firstName);
    addAndMakeVisible(m_secondName);

    m_label.setJustificationType(juce::Justification::centredRight);
    m_label.setText(juce::String(firstChan) + "/" + juce::String(firstChan + 1),
                    juce::dontSendNotification);

    m_stereoToggle.setClickingTogglesState(true);

    const auto fg = findColour(juce::TextButton::textColourOnId);

    m_monoIcon = getIcon("mono_svg");
    m_monoIcon->replaceColour(juce::Colours::black, fg);

    m_stereoIcon = getIcon("stereo_svg");
    m_stereoIcon->replaceColour(juce::Colours::black, fg);

    m_stereoToggle.setImages(m_monoIcon.get(), nullptr, nullptr, nullptr,
                             m_stereoIcon.get());

    m_stereoToggle.onStateChange = [this]() {
        m_secondName.setEnabled(!m_stereoToggle.getToggleState());
    };

    auto prefix = juce::String("Pair") + juce::String(firstChan);

    m_stereoToggle.setToggleState(defaults->getBoolValue(prefix + "StereoLink",
                                                         firstChan >= 13),
                                  juce::sendNotification);

    m_firstName.setText(defaults->getValue(prefix + "FirstLabel"));
    m_secondName.setText(defaults->getValue(prefix + "SecondLabel"));
}

void InputPairComponent::savePreferences(juce::PropertiesFile *defaults)
{
    auto prefix = juce::String("Pair") + juce::String(m_firstChan);

    defaults->setValue(prefix + "StereoLink", m_stereoToggle.getToggleState());
    defaults->setValue(prefix + "FirstLabel", m_firstName.getTextValue());
    defaults->setValue(prefix + "SecondLabel", m_secondName.getTextValue());
}

int InputPairComponent::firstChannel() const
{
    return m_firstChan;
}

bool InputPairComponent::isLinked() const
{
    return m_stereoToggle.getToggleState();
}

juce::String InputPairComponent::firstName() const
{
    return m_firstName.getText();
}

juce::String InputPairComponent::secondName() const
{
    return m_secondName.getText();
}

void InputPairComponent::resized()
{
    juce::Grid grid;
    grid.setGap(juce::Grid::Px(5));

    grid.templateRows.add(juce::Grid::Fr(1));
    grid.templateColumns.add(juce::Grid::Px(60));
    grid.templateColumns.add(juce::Grid::Px(30));
    grid.templateColumns.add(juce::Grid::Fr(1));
    grid.templateColumns.add(juce::Grid::Fr(1));

    grid.items.add(m_label);
    grid.items.add(m_stereoToggle);
    grid.items.add(m_firstName);
    grid.items.add(m_secondName);

    grid.performLayout(getLocalBounds());
}

std::unique_ptr<juce::Drawable> InputPairComponent::getIcon(const char *name)
{
    int size = 0;

    auto data = BinaryData::getNamedResource(name, size);
    if(data)
    {
        return juce::Drawable::createFromImageData(data, size);
    }

    return nullptr;
}
