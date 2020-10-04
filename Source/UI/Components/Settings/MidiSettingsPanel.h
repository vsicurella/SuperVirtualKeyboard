/*
  ==============================================================================

    MidiSettingsPanel.h
    Created: 13 Nov 2019 7:09:59pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"

class MidiSettingsPanel : public SvkSettingsPanel, public Timer
{
    
public:
    
    MidiSettingsPanel(SvkPluginState*);
    ~MidiSettingsPanel();

    void visibilityChanged() override;

    void sliderValueChanged(Slider* sliderThatChanged) override;

    void comboBoxChanged(ComboBox* comboBoxThatChanged) override;

    void timerCallback() override;

    void refreshDevices();
    
private:

    HashMap<Identifier, LabelledComponent*, IDasStringHash> idToLabelledControl;
    
    LabelledComponent* inputBoxLabelled;
    LabelledComponent* outputBoxLabelled;
    ComboBox* inputBox;
    ComboBox* outputBox;

    Slider* periodShiftSlider;
    Slider* transposeSlider;
    Slider* midiChannelSlider;

    Array<MidiDeviceInfo> availableIns;
    Array<MidiDeviceInfo> availableOuts;

    SvkMidiProcessor* midiProcessor;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiSettingsPanel)
};
