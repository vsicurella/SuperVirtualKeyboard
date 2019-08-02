/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../PluginSettings.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginSettingsDialog  : public Component,
                              public Button::Listener
{
public:
    //==============================================================================
    PluginSettingsDialog (SvkPluginSettings* pluginSettingsIn);
    ~PluginSettingsDialog();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    File findDirectory(const String prompt);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SvkPluginSettings* pluginSettings;
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


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginSettingsDialog)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

