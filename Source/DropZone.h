#pragma once

#include <JuceHeader.h>
#include <functional>

class DropZone : public juce::Component,
                 public juce::FileDragAndDropTarget
{
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DropZone)

public:
    DropZone();

    juce::File getFile() const;
    std::function<void()> onFileChanged;

private:
    void setFile(const juce::File &file);

    void paint(juce::Graphics &g) override;

    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    void fileDragEnter(const juce::StringArray &files, int x, int y) override;
    void fileDragExit(const juce::StringArray &files) override;

    juce::File m_file;
    bool m_inDrag = false;
};
