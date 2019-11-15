/*
  ==============================================================================

    DebugSettingsPanel.h
    Created: 14 Nov 2019 11:46:31pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../PluginState.h"

class DebugSettingsPanel : public Component
{
    
public:
    
    DebugSettingsPanel(SvkPluginState* pluginStateIn);
    
    ~DebugSettingsPanel();
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    
    SvkPluginState* pluginState;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugSettingsPanel)
};
