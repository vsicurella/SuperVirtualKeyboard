/*
  ==============================================================================

    DebugSettingsPanel.h
    Created: 14 Nov 2019 11:46:31pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../../SvkUiPanel.h"
#include "../../../Structures/OwnedHashMap.h"
#include "../../../PluginIDs.h"
#include "../../../PluginState.h"

class DebugSettingsPanel : public SvkUiPanel, public Slider::Listener
{
    
public:
    
    DebugSettingsPanel(AudioProcessorValueTreeState& processorTreeIn, Array<String>* paramIDsIn);
    ~DebugSettingsPanel();
    
    void connectToProcessor() override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void sliderValueChanged(Slider *slider) override;
    
private:

    OwnedArray<Component> controls;
    OwnedArray<Label> labels;
    
    Array<String>* paramIDs;
    
    OwnedArray<ButtonAttachment> buttonAttachments;
    OwnedArray<ComboBoxAttachment> comboBoxAttachments;
    OwnedArray<SliderAttachment> sliderAttachments;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugSettingsPanel)
};
