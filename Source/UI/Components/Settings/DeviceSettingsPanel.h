/*
  ==============================================================================

    DeviceSettingsPanel.h
    Created: 13 Nov 2019 7:09:59pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../PluginState.h"

class DeviceSettingsPanel : public Component
{
    
public:
    
    DeviceSettingsPanel(SvkPluginState* pluginStateIn);
    
    ~DeviceSettingsPanel();
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    
    SvkPluginState* pluginState;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeviceSettingsPanel)
};
