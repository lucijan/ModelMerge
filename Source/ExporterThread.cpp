#include "ExporterThread.h"

#if JUCE_MAC
#include <sys/stat.h>
#include <unistd.h>
#endif

static juce::int64 kBlockSize = 256;

ExporterThread::ExporterThread(juce::Component *parent, const juce::File &sourceDir,
    const juce::File &targetDir, const juce::String &name) :
    juce::ThreadWithProgressWindow(juce::translate("Exporting"),
                                   true, true, 10000, juce::translate("Cancel"),
                                   parent),
    m_parent(parent),
    m_sourceDir(sourceDir),
    m_targetDir(targetDir),
    m_name(name)
{

}

ExporterThread::~ExporterThread()
{
}

void ExporterThread::setMapping(std::vector<PairMapping> mapping)
{
    m_mapping = mapping;

}

void ExporterThread::run()
{
    setProgress(-1.0);

    std::vector<juce::File> inputFiles;

    juce::RangedDirectoryIterator it(m_sourceDir, false, "*TR*.wav");
    for(auto entry : it)
    {
        inputFiles.push_back(entry.getFile());
    }

    if(inputFiles.size() != 24)
    {
        m_status = Status::Failed;
        m_errorMessage = juce::String("Track count missmatch. Expected 24 got ")
            + juce::String(inputFiles.size());
        return;
    }

    std::sort(inputFiles.begin(), inputFiles.end(), [](const juce::File &a,
                                                       const juce::File &b) {
        return a.getFileName().compare(b.getFileName()) < 0;
    });

    m_targetDir.createDirectory();

    double numTracks = m_mapping.size();
    juce::WavAudioFormat fmt;

    auto track = 0;
    for(auto mapping : m_mapping)
    {
        if(threadShouldExit())
        {
            return;
        }

        juce::AudioSampleBuffer copyBuf(2, (int)kBlockSize);

        setProgress(track / numTracks);

        if(mapping.secondChannel == -1)
        {
            createSingleChannelTrack(inputFiles, mapping);
        }
        else
        {
            auto trackNo = juce::String(mapping.firstChannel).paddedLeft('0', 2) +
                + "_" + juce::String(mapping.firstChannel + 1).paddedLeft('0', 2);

            auto name = trackNo;
            if(mapping.name.isEmpty())
            {
                name += "_Channels_" + trackNo;
            }
            else
            {
                name += "_" + juce::File::createLegalFileName(mapping.name);
            }

            auto strCh1 = std::make_unique<juce::FileInputStream>
                (inputFiles[mapping.firstChannel - 1]);
            auto strCh2 = std::make_unique<juce::FileInputStream>
                (inputFiles[mapping.firstChannel]);

            std::unique_ptr<juce::AudioFormatReader>
                readerCh1(fmt.createReaderFor(strCh1.get(), false));
            std::unique_ptr<juce::AudioFormatReader>
                readerCh2(fmt.createReaderFor(strCh2.get(), false));

            if(readerCh1 && readerCh2)
            {
                strCh1.release();
                strCh2.release();

                if(readerCh1->lengthInSamples == 0)
                {
                    createSingleChannelTrack(inputFiles, mapping);
                    continue;
                }

                if(readerCh2->lengthInSamples == 0)
                {
                    createSingleChannelTrack(inputFiles, { mapping.firstChannel + 1,
                        -1, mapping.name });
                    continue;
                }

                auto target = m_targetDir.getChildFile(name + ".wav");
                auto stream = std::make_unique<juce::FileOutputStream>(target);
                std::unique_ptr<juce::AudioFormatWriter> writer(
                    fmt.createWriterFor(stream.get(),
                    readerCh1->sampleRate, 2, readerCh1->bitsPerSample,
                    readerCh1->metadataValues, -1));

                if(writer)
                {
                    stream.release();
                    const auto length = readerCh1->lengthInSamples;

                    for(juce::int64 i = 0; i < length; i += kBlockSize)
                    {
                        if(threadShouldExit())
                        {
                            return;
                        }

                        auto maxBlock = juce::jmin(kBlockSize, length - i);

                        juce::AudioSampleBuffer left(copyBuf.getArrayOfWritePointers(),
                                                     1, (int)kBlockSize);

                        juce::AudioSampleBuffer right(&copyBuf.getArrayOfWritePointers()[1],
                                                      1, (int)kBlockSize);

                        readerCh1->read(&left, 0, (int)maxBlock, (int)i,
                                        true, false);

                        readerCh2->read(&right, 0, (int)maxBlock, (int)i,
                                        false, true);

                        writer->writeFromAudioSampleBuffer(copyBuf, 0, (int)maxBlock);
                    }
                }
            }
        }

        track++;
    }

    m_status = Status::Success;
}

void ExporterThread::createSingleChannelTrack(std::vector<juce::File> inputFiles,
                                              const PairMapping &mapping) const
{
    juce::WavAudioFormat fmt;

    auto stream = std::make_unique<juce::FileInputStream>
        (inputFiles[mapping.firstChannel - 1]);
    std::unique_ptr<juce::AudioFormatReader>
        reader(fmt.createReaderFor(stream.get(), false));

    if(reader)
    {
        stream.release();
        if(reader->lengthInSamples == 0)
        {
            return;
        }
    }

    auto trackNo = juce::String(mapping.firstChannel).paddedLeft('0', 2);
    auto name = trackNo;
    if(mapping.name.isEmpty())
    {
        name += "_Channel_" + trackNo;
    }
    else
    {
        name += "_" + juce::File::createLegalFileName(mapping.name);
    }

    auto targetFile = m_targetDir.getChildFile(name + ".wav");
    inputFiles[mapping.firstChannel - 1].
        copyFileTo(targetFile);

#if JUCE_MAC
    chflags(targetFile.getFullPathName().toUTF8(), 0);
#endif
}

void ExporterThread::threadComplete(bool userPressedCancel)
{
    if(m_status == Status::Failed)
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon,
                                          juce::translate("Export Failed"),
                                          m_errorMessage, juce::translate("Ok"),
                                          m_parent);
    }
    else if(m_status == Status::Success)
    {
        m_targetDir.revealToUser();
    }

    if(userPressedCancel || m_status != Status::Success)
    {
        m_targetDir.deleteRecursively();
    }
}
