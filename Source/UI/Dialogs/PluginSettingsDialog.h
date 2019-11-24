/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../PluginState.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginSettingsDialog  : public Component,
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
    //[UserMethods]     -- You can add your own custom methods in this section.
    File findDirectory(const String prompt);
	ComboBox* getMidiOutputBox();

    void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SvkPluginState* pluginState;
    Array<MidiDeviceInfo> availableOuts;
    //[/UserVariables]

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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginSettingsDialog)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

