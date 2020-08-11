/*
  ==============================================================================


  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../PluginState.h"

class PluginSettingsDialog  : //public SvkUiPanel,
							  public Component,
                              public ChangeBroadcaster,
                              private Timer,
                              public Button::Listener,
                              public ComboBox::Listener
{
public:
    //==============================================================================
    PluginSettingsDialog (SvkPluginState* pluginStateIn);
    ~PluginSettingsDialog();

    //==============================================================================

	File findDirectory(const String prompt);
	ComboBox* getMidiOutputBox();

    void timerCallback() override;

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;

private:
    SvkPluginState* pluginState;
    Array<MidiDeviceInfo> availableOuts;

    //==============================================================================
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


    //==============================================================================
};