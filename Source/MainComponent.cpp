#include "MainComponent.h"

MainComponent::MainComponent() :
    m_targetDir("Target Directory", juce::File(""), false, true, false, "", "",
                juce::translate("Choose Target Directory"))
{
    juce::PropertiesFile::Options propOptions;
    propOptions.applicationName = ProjectInfo::projectName;
    propOptions.filenameSuffix = "settings";
    propOptions.osxLibrarySubFolder = "Preferences";
    propOptions.commonToAllUsers = false;
    m_appProperties.setStorageParameters(propOptions);

    addAndMakeVisible(m_dropZone);
    m_dropZone.onFileChanged = [this]() {
        m_nameEditor.setText(m_dropZone.getFile().getFileName());
        checkExportPossible();
    };

    for(auto i = 0; i < 21; i += 2)
    {
        auto comp = std::make_unique<InputPairComponent>(m_appProperties.getUserSettings(),
                                                         i + 1);
        addAndMakeVisible(comp.get());
        m_inputPairComponents.push_back(std::move(comp));
    }

    addAndMakeVisible(m_targetFolderLabel);
    m_targetFolderLabel.setText(juce::translate("Target Folder:"),
                                juce::dontSendNotification);
    addAndMakeVisible(m_targetDir);
    auto path = m_appProperties.getUserSettings()->getValue("TargetDir",
        m_targetDir.getCurrentFile().getFullPathName());

    m_targetDir.setCurrentFile(juce::File(path), true);
    m_targetDir.addListener(this);

    addAndMakeVisible(m_nameLabel);
    m_nameLabel.setText(juce::translate("Name:"), juce::dontSendNotification);
    addAndMakeVisible(m_nameEditor);

    m_exportButton.setButtonText(juce::translate("Export"));
    m_exportButton.setEnabled(false);
    m_exportButton.onClick = [this]() {
        startExport();
    };

    addAndMakeVisible(m_exportButton);

    setSize(600, 450);
}

MainComponent::~MainComponent()
{
}

void MainComponent::checkExportPossible()
{
    DBG(m_targetDir.getCurrentFile().getFullPathName());
    auto can = m_dropZone.getFile().exists()
        && m_targetDir.getCurrentFile().exists();

    m_exportButton.setEnabled(can);
}

void MainComponent::startExport()
{
    m_appProperties.getUserSettings()->setValue("TargetDir",
        m_targetDir.getCurrentFile().getFullPathName());

    std::vector<ExporterThread::PairMapping> mapping;
    for(auto &comp : m_inputPairComponents)
    {
        comp->savePreferences(m_appProperties.getUserSettings());
        if(comp->isLinked())
        {
            ExporterThread::PairMapping m = { comp->firstChannel(),
                comp->firstChannel() + 1, comp->firstName() };

            mapping.push_back(m);
        }
        else
        {
            ExporterThread::PairMapping m1 = { comp->firstChannel(), -1,
                comp->firstName() };

            mapping.push_back(m1);

            ExporterThread::PairMapping m2 = { comp->firstChannel() + 1, -1,
                comp->secondName() };

            mapping.push_back(m2);
        }
    }

    m_appProperties.getUserSettings()->saveIfNeeded();

    if(m_targetDir.getCurrentFile().getChildFile(m_nameEditor.getText()).exists())
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon,
                                          juce::translate("File Exists"),
                                          juce::translate("File Already Exists"));

        return;
    }

    ExporterThread::PairMapping mainTrack = { 23, 24, "Main" };
    mapping.push_back(mainTrack);

    m_exporter = std::make_unique<ExporterThread>(this, m_dropZone.getFile(),
        m_targetDir.getCurrentFile().getChildFile(m_nameEditor.getText()),
        m_nameEditor.getText());
    m_exporter->setMapping(mapping);
    m_exporter->launchThread();
}

void MainComponent::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    juce::Grid grid;
    grid.setGap(juce::Grid::Px(5));

    grid.templateRows.add(juce::Grid::Fr(1));
    grid.templateColumns.add(juce::Grid::Px(95));
    grid.templateColumns.add(juce::Grid::Fr(1));

    juce::GridItem dropZoneItem(m_dropZone);
    dropZoneItem.row = {1, 2};
    dropZoneItem.column = {1, 3};

    grid.items.add(dropZoneItem);

    int row = 2;
    for(auto &comp : m_inputPairComponents)
    {
        grid.templateRows.add(juce::Grid::Fr(1));
        juce::GridItem inputRowItem(comp.get());
        inputRowItem.row = { row, row + 1 };
        inputRowItem.column = { 1, 3 };

        grid.items.add(inputRowItem);

        row++;
    }

    grid.templateRows.add(juce::Grid::Fr(1));
    grid.items.add(m_targetFolderLabel);
    grid.items.add(m_targetDir);

    grid.templateRows.add(juce::Grid::Fr(1));
    grid.items.add(m_nameLabel);
    grid.items.add(m_nameEditor);

    grid.templateRows.add(juce::Grid::Fr(1));
    grid.items.add(juce::GridItem());
    grid.items.add(m_exportButton);

    grid.performLayout(getLocalBounds().reduced(5));
}

void MainComponent::filenameComponentChanged(juce::FilenameComponent *)
{
    checkExportPossible();
    m_targetDir.addRecentlyUsedFile(m_targetDir.getCurrentFile());
}
