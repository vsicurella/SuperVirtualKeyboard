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
			SvkControlProperties(ControlTypeNames::DirectoryControl, "Preset Directory", true),
			SvkControlProperties(ControlTypeNames::DirectoryControl, "Mode Directory", true),
			SvkControlProperties(ControlTypeNames::DirectoryControl, "Settings Directory", true)
		}
	)
{
	for (int i = 0; i < flexBox.items.size(); i++)
	{
		FlexItem& item = flexBox.items.getReference(i);
		item.maxHeight = 24;
		item.minWidth = 500;
	}

	presetLabel = static_cast<LabelledComponent*>(controls[0]);
	presetLabel->setComponentSize(320, 24);

	presetDirectoryBrowser = LabelledComponent::getComponentPointer<DirectoryBrowserComponent>(presetLabel);
	presetDirectoryBrowser->setText(pluginState->getPluginSettings()->getPresetPath());
	presetDirectoryBrowser->addListener(this);

	modeLabel = static_cast<LabelledComponent*>(controls[1]);
	modeLabel->setComponentSize(320, 24);

	modeDirectoryBrowser = LabelledComponent::getComponentPointer<DirectoryBrowserComponent>(modeLabel);
	modeDirectoryBrowser->setText(pluginState->getPluginSettings()->getModePath());
	modeDirectoryBrowser->addListener(this);

	settingsLabel = static_cast<LabelledComponent*>(controls[2]);
	settingsLabel->setComponentSize(320, 24);

	settingsDirectoryBrowser = LabelledComponent::getComponentPointer<DirectoryBrowserComponent>(settingsLabel);
	settingsDirectoryBrowser->setText(pluginState->getPluginSettings()->getSettingsPath());
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
		pluginState->getPluginSettings()->setPresetDirectory(directorySelected);
	}

	else if (browser == modeLabel->get())
	{
		pluginState->getPluginSettings()->setModeDirectory(directorySelected);
	}

	else if (browser == settingsLabel->get())
	{
		pluginState->getPluginSettings()->setSettingsDirectory(directorySelected);
	}
}