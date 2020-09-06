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
	//presetDirectoryText->setBounds(128, 32, 320, 24);
	//presetDirectoryLbl->setBounds(16, 32, 112, 24);
	//modeDirectoryText->setBounds(128, 72, 320, 24);
	//modeDirectoryLbl->setBounds(20, 72, 112, 24);
	//modeDirectoryBtn->setBounds(456, 72, 23, 24);
	//settingsDirectoryText->setBounds(128, 112, 320, 24);
	//settingsDirectoryLbl->setBounds(16, 112, 111, 24);
	//settingsDirectoryBtn->setBounds(456, 112, 23, 24);
	//localDirectoryBtn->setBounds(24, 153, 184, 24);

	for (auto item : flexBox.items)
	{
		item.minHeight = 24;
		item.maxHeight = 24;
	}

	for (auto control : controls)
	{
		DirectoryBrowserComponent* controlPtr = (DirectoryBrowserComponent*)control;
		String controlName = control->getName();

		if (controlName == "Preset Directory")
		{
			controlPtr->setText(pluginState->getPluginSettings()->getPresetPath());
		}
		else if (controlName == "Mode Directory")
		{
			controlPtr->setText(pluginState->getPluginSettings()->getModePath());
		}
		else if (controlName == "Settings Directory")
		{
			controlPtr->setText(pluginState->getPluginSettings()->getSettingsPath());
		}
	}

	setSize(100, 100);
}

GeneralSettingsPanel::~GeneralSettingsPanel()
{

}

void GeneralSettingsPanel::buttonClicked(Button* buttonThatWasClicked)
{


}

void GeneralSettingsPanel::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	//if (comboBoxThatHasChanged == midiDeviceBox.get())
	//{
	//	int deviceNum = comboBoxThatHasChanged->getSelectedId() - 1;

	//	if (deviceNum >= 0)
	//		pluginState->getMidiProcessor()->setMidiOutput(availableOuts.getUnchecked(deviceNum).identifier);
	//}
}