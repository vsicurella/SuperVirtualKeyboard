/*
  ==============================================================================

    KeyboardSettingsPanel.h
    Created: 13 Nov 2019 7:09:31pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"

class KeyboardSettingsPanel : public SvkSettingsPanel
{
    
public:
    
    KeyboardSettingsPanel(SvkPluginState*);

    void comboBoxChanged(ComboBox*) override;
    void sliderValueChanged(Slider*) override;
    void buttonClicked(Button*) override;

    void setKeyboardPointer(VirtualKeyboard::Keyboard*) override;
      
private:

    LabelledComponent* layoutLabel;
    LabelledComponent* highlightLabel;
    LabelledComponent* ratioLabel;

    ComboBox* keyLayoutBox;
    ComboBox* keyHighlightBox;
    Slider* keyRatioSlider;

    TextButton* showNoteNumbers;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardSettingsPanel)
};
