/*
  ==============================================================================

    ViewSettingsPanel.h
    Created: 13 Nov 2019 7:09:31pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../SvkUiPanel.h"
#include "../../../PluginState.h"

class ViewSettingsPanel : public SvkUiPanel
{
    
public:
    
    ViewSettingsPanel(SvkPluginState* pluginStateIn);
    
    ~ViewSettingsPanel();
    
    void connectToProcessor(AudioProcessorValueTreeState& processorTree) override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    
    SvkPluginState* pluginState;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViewSettingsPanel)
};
