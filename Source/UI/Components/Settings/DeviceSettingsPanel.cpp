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
	inputBoxLabelled = (LabelledComponent<ComboBox>*) flexBox.items[0].associatedComponent;
	inputBoxLabelled->setComponentSize(320, 24);
	inputBox = inputBoxLabelled->get();

	outputBoxLabelled = (LabelledComponent<ComboBox>*) flexBox.items[1].associatedComponent;
	outputBoxLabelled->setComponentSize(320, 24);
	outputBox = outputBoxLabelled->get();

	FlexItem& itemIn = flexBox.items.getReference(0);
	itemIn = itemIn.withMaxHeight(48).withMinWidth(inputBoxLabelled->getWidth());

	FlexItem& itemOut = flexBox.items.getReference(1);
	itemOut = itemOut.withMaxHeight(48).withMinWidth(outputBoxLabelled->getWidth());

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
	if (pluginState && pluginState->getMidiProcessor())
	{
		// Midi Input Changed
		if (idToControl[IDs::midiInputName] == comboBoxThatChanged)
		{
			MidiDeviceInfo& device = availableIns.getReference(comboBoxThatChanged->getSelectedId() - 1);
			DBG("Midi Input Selected: " + device.name);
			pluginState->getMidiProcessor()->setMidiInput(device.identifier);
		}

		// Midi Output Changed
		else if (idToControl[IDs::midiOutputName] == comboBoxThatChanged)
		{
			MidiDeviceInfo& device = availableOuts.getReference(comboBoxThatChanged->getSelectedId() - 1);
			DBG("Midi Output Selected: " + device.name);
			pluginState->getMidiProcessor()->setMidiOutput(device.identifier);
		}
	}
}

void DeviceSettingsPanel::timerCallback()
{
	refreshDevices();
}

void DeviceSettingsPanel::refreshDevices()
{
	Array<MidiDeviceInfo> inputDevices = MidiInput::getAvailableDevices();

	//if (pluginState->getMidiProcessor())
	//	inputBox->setText(pluginState->getMidiProcessor()->getOutputName(), dontSendNotification);

	if (availableIns != inputDevices && !inputBox->isPopupActive())
	{
		availableOuts = inputDevices;
		inputBox->clear();

		int i = 0;
		for (auto device : availableOuts)
		{
			inputBox->addItem(device.name, ++i);
		}
	}

	Array<MidiDeviceInfo> outputDevices = MidiOutput::getAvailableDevices();
	
	//if (pluginState->getMidiProcessor())
	//	outputBox->setText(pluginState->getMidiProcessor()->getOutputName(), dontSendNotification);

	if (availableOuts != outputDevices && !outputBox->isPopupActive())
	{
		availableOuts = outputDevices;
		outputBox->clear();

		int i = 0;
		for (auto device : availableOuts)
		{
			outputBox->addItem(device.name, ++i);
		}
	}
}