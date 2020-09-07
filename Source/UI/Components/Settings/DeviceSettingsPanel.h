/*
  ==============================================================================

    DeviceSettingsPanel.h
    Created: 13 Nov 2019 7:09:59pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"

class DeviceSettingsPanel : public SvkSettingsPanel, public Timer
{
    
public:
    
    DeviceSettingsPanel(SvkPluginState*);
    
    ~DeviceSettingsPanel();
    
    
    void paint(Graphics& g) override;
    void resized() override;

	void comboBoxChanged(ComboBox* comboBoxThatChanged) override;

	void timerCallback() override;

	void refreshDevices();
    
private:
    
    SvkPluginState* pluginState;

	LabelledComponent<ComboBox>* inputBoxLabelled;
	LabelledComponent<ComboBox>* outputBoxLabelled;
	ComboBox* inputBox;
	ComboBox* outputBox;

	Array<MidiDeviceInfo> availableIns;
	Array<MidiDeviceInfo> availableOuts;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeviceSettingsPanel)
};
