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
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginState.h"
#include "Structures/ModeLayout.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ScaleEditPopup  : public Component,
                        public ChangeBroadcaster,
                        public Button::Listener,
                        public ComboBox::Listener
{
public:
    //==============================================================================
    ScaleEditPopup (Array<ModeLayout>* presetsArrayIn,  ValueTree& presetsSortedIn);
    ~ScaleEditPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	ModeLayout* presetSelected;
	String get_preset_name();
	void populate_preset_menu();
	String get_input();
	String build_scale_string(std::vector<int> scaleIn); // pretty dumb
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    bool keyPressed (const KeyPress& key) override;
    bool keyStateChanged (bool isKeyDown) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	bool enterDown = false;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextEditor> textEditor;
    std::unique_ptr<Label> instructions;
    std::unique_ptr<TextButton> sendScale;

    std::unique_ptr<ComboBox> scalePresets;
	std::unique_ptr<PopupMenu> sortByScale;
	std::unique_ptr<PopupMenu> sortByMode;
	std::unique_ptr<PopupMenu> sortByFamily;

	Array<ModeLayout>* presets;
	ValueTree presetsSorted;

	StringPairArray scaleToMode;
	StringPairArray modeToMode;
	StringPairArray familyToMode;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScaleEditPopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

