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
    
    DebugSettingsPanel(SvkParameters* parametersIn, const Array<Identifier>* paramIDsIn);
    
    ~DebugSettingsPanel();
    
    void connectToProcessor(AudioProcessorValueTreeState& processorTree) override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void sliderValueChanged(Slider *slider) override;
    
private:
    
    SvkParameters* svkParameters;
    const Array<Identifier>* svkParamIDs;
    
    OwnedArray<Component> controls;
    OwnedArray<Label> labels;
    
    OwnedArray<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;
    OwnedArray<AudioProcessorValueTreeState::ComboBoxAttachment> comboBoxAttachments;
    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> sliderAttachments;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugSettingsPanel)
};
