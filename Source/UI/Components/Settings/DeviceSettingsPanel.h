/*
  ==============================================================================

    DeviceSettingsPanel.h
    Created: 13 Nov 2019 7:09:59pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../SvkUiPanel.h"
#include "../../../PluginState.h"

class DeviceSettingsPanel : public SvkUiPanel
{
    
public:
    
    DeviceSettingsPanel(SvkPluginState* pluginStateIn);
    
    ~DeviceSettingsPanel();
    
    void connectToProcessor(AudioProcessorValueTreeState& processorTree) override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    
    SvkPluginState* pluginState;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeviceSettingsPanel)
};
