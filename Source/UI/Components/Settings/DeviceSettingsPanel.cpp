/*
  ==============================================================================

    DeviceSettingsPanel.cpp
    Created: 13 Nov 2019 7:09:59pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "DeviceSettingsPanel.h"

DeviceSettingsPanel::DeviceSettingsPanel(SvkPluginState* pluginStateIn)
	: SvkSettingsPanel(pluginState,
		{ 
			IDs::midiInputName,
			IDs::midiOutputName 
		},
		{ 
			SvkControlProperties(ControlTypeNames::MenuControl, "Midi Input Device"),
			SvkControlProperties(ControlTypeNames::MenuControl, "Midi Output Device")
		}
	)
{
	refreshDevices();
	startTimer(1000);
}

DeviceSettingsPanel::~DeviceSettingsPanel()
{
}

void DeviceSettingsPanel::paint(Graphics& g)
{
	SvkSettingsPanel::paint(g);
}

void DeviceSettingsPanel::resized()
{
	SvkSettingsPanel::resized();
}

void DeviceSettingsPanel::comboBoxChanged(ComboBox* comboBoxThatChanged)
{
	// Midi Input Changed
	if (idToControl[IDs::midiInputName] == comboBoxThatChanged)
	{
		DBG("Midi Input Selected");
	}

	// Midi Output Changed
	else if (idToControl[IDs::midiOutputName] == comboBoxThatChanged)
	{
		DBG("Midi Output Selected");
	}
}

void DeviceSettingsPanel::timerCallback()
{
	refreshDevices();
}

void DeviceSettingsPanel::refreshDevices()
{
	Array<MidiDeviceInfo> inputDevices = MidiInput::getAvailableDevices();

	ComboBox* midiInputMenu = dynamic_cast<ComboBox*>(controls[1]);
	//midiInputMenu->setText(pluginState->getMidiProcessor()->getOutputName(), dontSendNotification);

	if (availableIns != inputDevices && !midiInputMenu->isPopupActive())
	{
		availableOuts = inputDevices;
		midiInputMenu->clear();

		int i = 0;
		for (auto device : availableOuts)
		{
			midiInputMenu->addItem(device.name, ++i);
		}
	}

	Array<MidiDeviceInfo> outputDevices = MidiOutput::getAvailableDevices();

	ComboBox* midiOutputMenu = dynamic_cast<ComboBox*>(controls[1]);
	//midiOutputMenu->setText(pluginState->getMidiProcessor()->getOutputName(), dontSendNotification);

	if (availableOuts != outputDevices && !midiOutputMenu->isPopupActive())
	{
		availableOuts = outputDevices;
		midiOutputMenu->clear();

		int i = 0;
		for (auto device : availableOuts)
		{
			midiOutputMenu->addItem(device.name, ++i);
		}
	}
}