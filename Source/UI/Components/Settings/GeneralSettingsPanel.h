/*
  ==============================================================================

    GeneralSettingsPanel.h
    Created: 13 Nov 2019 7:11:33pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../SvkUiPanel.h"
#include "../../../PluginState.h"

class GeneralSettingsPanel : public SvkUiPanel
{
    
public:
    
    GeneralSettingsPanel(SvkPluginState* pluginStateIn);
    
    ~GeneralSettingsPanel();
    
    void connectToProcessor(AudioProcessorValueTreeState& processorTree) override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    
    SvkPluginState* pluginState;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneralSettingsPanel)
};
