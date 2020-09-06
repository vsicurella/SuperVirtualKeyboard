/*
  ==============================================================================

    GeneralSettingsPanel.h
    Created: 3 Sep 2020 9:11:33pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"

class GeneralSettingsPanel : public SvkSettingsPanel
{
    
public:
    
    GeneralSettingsPanel(SvkPluginState*);
    ~GeneralSettingsPanel() override;
    
	void buttonClicked(Button* buttonThatWasClicked) override;
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

    
private:

	//std::unique_ptr<TextEditor> presetDirectoryText;
	//std::unique_ptr<Label> presetDirectoryLbl;
	//std::unique_ptr<TextButton> presetDirectoryBtn;
	//std::unique_ptr<TextEditor> modeDirectoryText;
	//std::unique_ptr<Label> modeDirectoryLbl;
	//std::unique_ptr<TextButton> modeDirectoryBtn;
	//std::unique_ptr<TextEditor> settingsDirectoryText;
	//std::unique_ptr<Label> settingsDirectoryLbl;
	//std::unique_ptr<TextButton> settingsDirectoryBtn;
	//std::unique_ptr<ToggleButton> localDirectoryBtn;
	//std::unique_ptr<ComboBox> midiDeviceBox;
	//std::unique_ptr<Label> midiOutputLbl;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneralSettingsPanel)
};
