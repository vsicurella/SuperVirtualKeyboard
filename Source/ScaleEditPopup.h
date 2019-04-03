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
                        public Label::Listener,
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
    void labelTextChanged (Label* labelThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    bool keyPressed (const KeyPress& key) override;
    bool keyStateChanged (bool isKeyDown) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	std::unique_ptr < HashMap<String, std::vector<int>>> presets;
	bool enterDown = false;

	/*
	Sensi[5] 8 Mavila[7] 9
		Future 10edo
		Orgone[7] 11	Diatonic[7] 12
		Father[8] 13
		Orwell[9] 13 Titanium[9] 14	  Mavila[9] 16 Diatonic[7] 17
		Maqamic[7] 17 	Maqamic[10] 17 Machine[11] 17
		Diatonic[7] 19
		Orwell[9] 22 Diatonic[7] 24
		Diatonic[7] 31", std::vector<int>({ 5, 5, 3, 5, 5, 5, 3 }));
		presets.get()->set("BP Lambda", std::vector<int>({ , 2, 1, 2, 2, 2, 1 }));
	presets.get()->set("BP Dur II", std::vector<int>({ 2, 2, 1, 2, 2, 2, 1 }));
	*/

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

