/*
  ==============================================================================

    DebugSettingsPanel.h
    Created: 14 Nov 2019 11:46:31pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../../../Structures/OwnedHashMap.h"

#include "../../../PluginIDs.h"
#include "../../../PluginState.h"

class DebugSettingsPanel : public Component, public Slider::Listener
{
    
public:
    
    DebugSettingsPanel(SvkPluginState* pluginStateIn);
    
    ~DebugSettingsPanel();
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void sliderValueChanged(Slider *slider) override;
    
private:
    
    SvkPluginState* pluginState;
    SvkParameters* svkParameters;
    
    OwnedArray<Component> controls;
    OwnedArray<Label> labels;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugSettingsPanel)
};
