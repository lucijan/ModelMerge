#pragma once

#include <JuceHeader.h>

class ExporterThread : public juce::ThreadWithProgressWindow
{
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExporterThread)

public:
    struct PairMapping
    {
        int firstChannel = 0;
        int secondChannel = -1;
        juce::String name;
    };

    enum class Status
    {
        Waiting,
        Failed,
        Success
    };

    ExporterThread(juce::Component *parent, const juce::File &sourceDir,
                   const juce::File &targetDir, const juce::String &name);
    ~ExporterThread() override;

    void setMapping(std::vector<PairMapping> mapping);

private:
    void run() override;
    void createSingleChannelTrack(const std::vector<juce::File> inputFiles,
                                  const PairMapping &m) const;

    void threadComplete(bool userPressedCancel) override;

    juce::Component *m_parent = nullptr;
    juce::File m_sourceDir;
    juce::File m_targetDir;
    juce::String m_name;

    Status m_status;
    juce::String m_errorMessage;

    std::vector<PairMapping> m_mapping;
};
