/*
  ==============================================================================

    GeneralSettingsPanel.cpp
    Created: 13 Nov 2019 7:11:33pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "GeneralSettingsPanel.h"

GeneralSettingsPanel::GeneralSettingsPanel(SvkPluginSettings& globalSettingsIn, SvkPreset& presetIn)
    : SvkSettingsPanel("GeneralSettingsPanel", presetIn, { "File Directories" },
        {
            IDs::presetDirectory,
            IDs::modeDirectory,
            IDs::settingsDirectory
        },
        {
            SvkControlProperties(ControlTypeNames::DirectoryControl, "Preset Directory", true),
            SvkControlProperties(ControlTypeNames::DirectoryControl, "Mode Directory", true),
            SvkControlProperties(ControlTypeNames::DirectoryControl, "Settings Directory", true)
        },
        FlexBox(), {}
    ),
    globalSettings(globalSettingsIn)
{
    presetLabel = static_cast<LabelledComponent*>(idToControl[IDs::presetDirectory]);
    presetLabel->setComponentSize(320, 24);

    presetDirectoryBrowser = LabelledComponent::getComponentPointer<DirectoryBrowserComponent>(presetLabel);
    presetDirectoryBrowser->setText(globalSettings.getPresetPath());
    presetDirectoryBrowser->addListener(this);

    modeLabel = static_cast<LabelledComponent*>(idToControl[IDs::modeDirectory]);
    modeLabel->setComponentSize(320, 24);

    modeDirectoryBrowser = LabelledComponent::getComponentPointer<DirectoryBrowserComponent>(modeLabel);
    modeDirectoryBrowser->setText(globalSettings.getModePath());
    modeDirectoryBrowser->addListener(this);

    settingsLabel = static_cast<LabelledComponent*>(idToControl[IDs::settingsDirectory]);
    settingsLabel->setComponentSize(320, 24);

    settingsDirectoryBrowser = LabelledComponent::getComponentPointer<DirectoryBrowserComponent>(settingsLabel);
    settingsDirectoryBrowser->setText(globalSettings.getSettingsPath());
    settingsDirectoryBrowser->addListener(this);

    setSize(100, 100);
}

GeneralSettingsPanel::~GeneralSettingsPanel()
{

}

void GeneralSettingsPanel::directoryChanged(DirectoryBrowserComponent* browser, File directorySelected)
{
    if (browser == presetLabel->get())
    {
        globalSettings.setPresetDirectory(directorySelected);
    }

    else if (browser == modeLabel->get())
    {
        globalSettings.setModeDirectory(directorySelected);
    }

    else if (browser == settingsLabel->get())
    {
        globalSettings.setSettingsDirectory(directorySelected);
    }
}