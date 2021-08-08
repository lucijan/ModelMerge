#pragma once

#include <JuceHeader.h>
#include "DropZone.h"
#include "InputPairComponent.h"
#include "ExporterThread.h"

#include <vector>

class MainComponent : public juce::Component,
                      public juce::FilenameComponentListener
{
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

public:
    MainComponent();
    ~MainComponent() override;

private:
    void checkExportPossible();
    void startExport();

    void paint(juce::Graphics &g) override;
    void resized() override;

    void filenameComponentChanged(juce::FilenameComponent *) override;

    juce::ApplicationProperties m_appProperties;

    DropZone m_dropZone;
    std::vector<std::unique_ptr<InputPairComponent>> m_inputPairComponents;
    juce::Label m_targetFolderLabel;
    juce::FilenameComponent m_targetDir;
    juce::Label m_nameLabel;
    juce::TextEditor m_nameEditor;
    juce::TextButton m_exportButton;

    std::unique_ptr<ExporterThread> m_exporter;
};
