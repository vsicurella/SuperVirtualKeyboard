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
    ScaleEditPopup (OwnedArray<ModeLayout>* presetsArrayIn, Array<Array<ModeLayout*>>* presetsSortedIn);
    ~ScaleEditPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	ModeLayout* presetSelected;
    bool isUserPreset = false;
    
	String get_preset_name();
	ModeLayout* get_preset(String anyNameIn);
    
    int get_preset_id(String anyNameIn);
    
    int get_selected_preset_id();
    void set_selected_preset(ModeLayout* presetIn);
    void set_selected_preset(int comboBoxIdIn);
    void set_text_boxes(String presetName, String steps);

	void populate_preset_menu();
	String get_input();
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
    
    OwnedArray<ModeLayout>* presets;
    Array<Array<ModeLayout*>>* presetsSorted;
    
    HashMap<String, int> menuToPresetIndex;
    std::unique_ptr<PopupMenu> menuSortByScale;
    std::unique_ptr<PopupMenu> menuSortByMode;
    std::unique_ptr<PopupMenu> menuSortByFamily;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextEditor> textEditor;
    std::unique_ptr<Label> instructions;
    std::unique_ptr<TextButton> sendScale;
    std::unique_ptr<ComboBox> scalePresets;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScaleEditPopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

