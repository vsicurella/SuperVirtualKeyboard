/*
  ==============================================================================

    GeneralSettingsPanel.h
    Created: 3 Sep 2020 9:11:33pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../SvkUiPanel.h"
#include "../../../PluginState.h"

class GeneralSettingsPanel : public Component,
							public Timer,
							public ChangeBroadcaster,
							private Button::Listener,
							private ComboBox::Listener
							
{
    
public:
    
    GeneralSettingsPanel(SvkPluginState*);
    ~GeneralSettingsPanel();
    
	File findDirectory(const String prompt);
	ComboBox* getMidiOutputBox();

	void buttonClicked(Button* buttonThatWasClicked);
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged);

	void timerCallback() override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    
    SvkPluginState* pluginState;
    
    //OwnedArray<ButtonAttachment> buttonAttachments;
    //OwnedArray<ComboBoxAttachment> comboBoxAttachments;
    //OwnedArray<SliderAttachment> sliderAttachments;

	Array<MidiDeviceInfo> availableOuts;

	std::unique_ptr<TextEditor> presetDirectoryText;
	std::unique_ptr<Label> presetDirectoryLbl;
	std::unique_ptr<TextButton> presetDirectoryBtn;
	std::unique_ptr<TextEditor> modeDirectoryText;
	std::unique_ptr<Label> modeDirectoryLbl;
	std::unique_ptr<TextButton> modeDirectoryBtn;
	std::unique_ptr<TextEditor> settingsDirectoryText;
	std::unique_ptr<Label> settingsDirectoryLbl;
	std::unique_ptr<TextButton> settingsDirectoryBtn;
	std::unique_ptr<ToggleButton> localDirectoryBtn;
	std::unique_ptr<Label> headerLbl;
	std::unique_ptr<ComboBox> midiDeviceBox;
	std::unique_ptr<Label> midiOutputLbl;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneralSettingsPanel)
};
