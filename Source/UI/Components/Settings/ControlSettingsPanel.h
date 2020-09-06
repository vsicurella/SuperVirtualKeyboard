/*
  ==============================================================================

    ControlSettingsPanel.h
    Created: 13 Nov 2019 7:11:08pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../PluginState.h"

class ControlSettingsPanel : public Component
{
    
public:
    
    ControlSettingsPanel(AudioProcessorValueTreeState& processorTreeIn);
    
    ~ControlSettingsPanel();
    
    //void connectToProcessor() override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    
    SvkPluginState* pluginState;
    
    OwnedArray<ButtonAttachment> buttonAttachments;
    OwnedArray<ComboBoxAttachment> comboBoxAttachments;
    OwnedArray<SliderAttachment> sliderAttachments;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlSettingsPanel)
};
