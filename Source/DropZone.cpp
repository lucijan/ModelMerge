#include "DropZone.h"

DropZone::DropZone()
{
}

juce::File DropZone::getFile() const
{
    return m_file;
}

void DropZone::paint(juce::Graphics &g)
{
    auto borderColour = findColour(juce::TextEditor::outlineColourId);
    g.setColour(borderColour);

    const auto rect = getLocalBounds().reduced(2);

    auto thickness = m_inDrag ? 2.0f : 1.0f;

    juce::Path outline;
    outline.addRectangle(rect.toFloat());

    if(m_inDrag)
    {
        g.setFillType(juce::FillType(borderColour.withAlpha(0.3f)));
        g.fillPath(outline);
    }

    g.strokePath(outline, juce::PathStrokeType(thickness));

    if(!m_file.exists())
    {
        g.drawText(juce::translate("Drop MTR folder here"), rect,
                   juce::Justification::centred);
    }
    else
    {
        g.setColour(findColour(juce::TextEditor::textColourId));
        g.drawText(juce::translate(m_file.getFileName()), rect,
                   juce::Justification::centred);
    }
}

void DropZone::setFile(const juce::File &file)
{
    m_file = file;
    repaint();

    if(onFileChanged)
    {
        onFileChanged();
    }
}

bool DropZone::isInterestedInFileDrag(const juce::StringArray &files)
{
    if(!files.isEmpty() && juce::File(files[0]).isDirectory())
    {
        return true;
    }

    return false;
}

void DropZone::filesDropped(const juce::StringArray &files, int x, int y)
{
    m_inDrag = false;

    if(files.size() > 0)
    {
        juce::File file(files[0]);
        if(!file.isDirectory())
        {
            setFile(juce::File());
            return;
        }

        auto songFile = file.getChildFile("song.sys");
        if(!songFile.exists())
        {
            setFile(juce::File());
            return;
        }

        juce::FileInputStream stream(songFile);
        char magic[6];
        stream.read(magic, 6);
        juce::String magicString(magic, 6);
        if(magicString != "TASCAM")
        {
            setFile(juce::File());
            return;
        }

        setFile(file);
    }

    repaint();
}

void DropZone::fileDragEnter(const juce::StringArray &files, int x, int y)
{
    m_inDrag = isInterestedInFileDrag(files);
    repaint();
}

void DropZone::fileDragExit(const juce::StringArray &files)
{
    m_inDrag = false;
    repaint();
}
