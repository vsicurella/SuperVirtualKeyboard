	/*
  ==============================================================================

    DeviceSettingsPanel.cpp
    Created: 13 Nov 2019 7:09:59pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "DeviceSettingsPanel.h"

DeviceSettingsPanel::DeviceSettingsPanel(SvkPluginState* pluginStateIn)
	: SvkSettingsPanel("DeviceSettingsPanel", pluginStateIn, { "MIDI Input & Output" },
		{ 
			IDs::midiInputName,
			IDs::midiOutputName 
		},
		{ 
			SvkControlProperties(ControlTypeNames::MenuControl, "Midi Input Device", true),
			SvkControlProperties(ControlTypeNames::MenuControl, "Midi Output Device", true)
		}
	)
{
	FlexBox& col1 = flexSections.getReference(0);

	inputBoxLabelled = static_cast<LabelledComponent*>(idToControl[IDs::midiInputName]);
	inputBoxLabelled->setComponentSize(320, 24);
	inputBox = LabelledComponent::getComponentPointer<ComboBox>(inputBoxLabelled);
	inputBox->addListener(this);

	outputBoxLabelled = static_cast<LabelledComponent*>(idToControl[IDs::midiOutputName]);
	outputBoxLabelled->setComponentSize(320, 24);
	outputBox = LabelledComponent::getComponentPointer<ComboBox>(outputBoxLabelled);
	outputBox->addListener(this);

	FlexItem& itemIn = col1.items.getReference(0);
	itemIn = itemIn.withMaxHeight(48).withMinWidth(inputBoxLabelled->getWidth());

	FlexItem& itemOut = col1.items.getReference(1);
	itemOut = itemOut.withMaxHeight(48).withMinWidth(outputBoxLabelled->getWidth());
}

DeviceSettingsPanel::~DeviceSettingsPanel()
{
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