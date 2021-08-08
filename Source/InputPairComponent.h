#pragma once

#include <JuceHeader.h>

class InputPairComponent : public juce::Component
{
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputPairComponent)

public:
    InputPairComponent(juce::PropertiesFile *defaults, int firstChan);

    void savePreferences(juce::PropertiesFile *defaults);

    int firstChannel() const;
    bool isLinked() const;
    juce::String firstName() const;
    juce::String secondName() const;

private:
    void resized() override;

    static std::unique_ptr<juce::Drawable> getIcon(const char *name);

    int m_firstChan = 0;

    juce::Label m_label;
    juce::DrawableButton m_stereoToggle;
    juce::TextEditor m_firstName;
    juce::TextEditor m_secondName;

    std::unique_ptr<juce::Drawable> m_monoIcon;
    std::unique_ptr<juce::Drawable> m_stereoIcon;
};
