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
    ScaleEditPopup ();
    ~ScaleEditPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	String get_preset_name();
	std::vector<int> get_scale(String hashKeyIn);
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
	std::unique_ptr < HashMap<String, std::vector<int>>> presets;
	bool enterDown = false;
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

