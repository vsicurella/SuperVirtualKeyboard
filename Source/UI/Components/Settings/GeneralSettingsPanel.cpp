/*
  ==============================================================================

    GeneralSettingsPanel.cpp
    Created: 13 Nov 2019 7:11:33pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "GeneralSettingsPanel.h"

GeneralSettingsPanel::GeneralSettingsPanel(SvkPluginState* pluginStateIn)
    : SvkSettingsPanel(pluginStateIn,
		{
			IDs::presetDirectory,
			IDs::modeDirectory,
			IDs::settingsDirectory
		},
		{
			SvkControlProperties(ControlTypeNames::DirectoryControl, "Preset Directory"),
			SvkControlProperties(ControlTypeNames::DirectoryControl, "Mode Directory"),
			SvkControlProperties(ControlTypeNames::DirectoryControl, "Settings Directory")
		}
	)
{
	for (int i = 0; i < flexBox.items.size(); i++)
	{
		FlexItem& item = flexBox.items.getReference(i);
		item.maxHeight = 24;
		item.minWidth = 500;
	}

	presetDirectoryBrowser = static_cast<LabelledComponent<DirectoryBrowserComponent>*>(controls[0]);
	presetDirectoryBrowser->get()->setText(pluginState->getPluginSettings()->getPresetPath());
	presetDirectoryBrowser->setComponentSize(320, 24);
	presetDirectoryBrowser->get()->addListener(this);

	modeDirectoryBrowser = static_cast<LabelledComponent<DirectoryBrowserComponent>*>(controls[1]);
	modeDirectoryBrowser->get()->setText(pluginState->getPluginSettings()->getModePath());
	modeDirectoryBrowser->setComponentSize(320, 24);
	modeDirectoryBrowser->get()->addListener(this);

	settingsDirectoryBrowser = static_cast<LabelledComponent<DirectoryBrowserComponent>*>(controls[2]);
	settingsDirectoryBrowser->get()->setText(pluginState->getPluginSettings()->getSettingsPath());
	settingsDirectoryBrowser->setComponentSize(320, 24);
	settingsDirectoryBrowser->get()->addListener(this);

	setSize(100, 100);
}

GeneralSettingsPanel::~GeneralSettingsPanel()
{

}

void GeneralSettingsPanel::directoryChanged(DirectoryBrowserComponent* browser, File directorySelected)
{
	if (browser == presetDirectoryBrowser->get())
	{
		pluginState->getPluginSettings()->setPresetDirectory(directorySelected);
	}

	else if (browser == modeDirectoryBrowser->get())
	{
		pluginState->getPluginSettings()->setModeDirectory(directorySelected);
	}

	else if (browser == settingsDirectoryBrowser->get())
	{
		pluginState->getPluginSettings()->setSettingsDirectory(directorySelected);
	}
}