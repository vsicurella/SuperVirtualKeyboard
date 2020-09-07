	/*
  ==============================================================================

    DeviceSettingsPanel.cpp
    Created: 13 Nov 2019 7:09:59pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "DeviceSettingsPanel.h"

DeviceSettingsPanel::DeviceSettingsPanel(SvkPluginState* pluginStateIn)
	: SvkSettingsPanel(pluginStateIn,
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
	inputBox->addListener(this);

	outputBoxLabelled = (LabelledComponent<ComboBox>*) flexBox.items[1].associatedComponent;
	outputBoxLabelled->setComponentSize(320, 24);
	outputBox = outputBoxLabelled->get();
	outputBox->addListener(this);

	FlexItem& itemIn = flexBox.items.getReference(0);
	itemIn = itemIn.withMaxHeight(48).withMinWidth(inputBoxLabelled->getWidth());

	FlexItem& itemOut = flexBox.items.getReference(1);
	itemOut = itemOut.withMaxHeight(48).withMinWidth(outputBoxLabelled->getWidth());
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

void DeviceSettingsPanel::visibilityChanged()
{
	if (isVisible())
	{
		refreshDevices();
		startTimer(1000);
	}
	else
	{
		stopTimer();
	}
}

void DeviceSettingsPanel::comboBoxChanged(ComboBox* comboBoxThatChanged)
{
	// Midi Input Changed
	if (inputBox == comboBoxThatChanged)
	{
		MidiDeviceInfo& device = availableIns.getReference(inputBox->getSelectedId() - 1);
		DBG("Midi Input Selected: " + device.name);
		pluginState->getMidiProcessor()->setMidiInput(device.identifier);
	}

	// Midi Output Changed
	else if (outputBox == comboBoxThatChanged)
	{
		MidiDeviceInfo& device = availableOuts.getReference(outputBox->getSelectedId() - 1);
		DBG("Midi Output Selected: " + device.name);
		pluginState->getMidiProcessor()->setMidiOutput(device.identifier);
	}
}

void DeviceSettingsPanel::timerCallback()
{
	refreshDevices();
}

void DeviceSettingsPanel::refreshDevices()
{
	Array<MidiDeviceInfo> inputDevices = MidiInput::getAvailableDevices();

	if (availableIns != inputDevices && !inputBox->isPopupActive())
	{
		availableIns = inputDevices;
		inputBox->clear(dontSendNotification);
		inputBox->setText(pluginState->getMidiProcessor()->getInputName(), dontSendNotification);

		int i = 1;
		for (auto device : availableIns)
		{
			inputBox->addItem(device.name, i++);
		}
	}

	Array<MidiDeviceInfo> outputDevices = MidiOutput::getAvailableDevices();

	if (availableOuts != outputDevices && !outputBox->isPopupActive())
	{
		availableOuts = outputDevices;
		outputBox->clear(dontSendNotification);
		outputBox->setText(pluginState->getMidiProcessor()->getOutputName(), dontSendNotification);

		int i = 1;
		for (auto device : availableOuts)
		{
			outputBox->addItem(device.name, i++);
		}
	}
}