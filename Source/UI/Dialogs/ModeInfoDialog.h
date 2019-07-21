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
#include "../../CommonFunctions.h"
#include "../../Structures/Mode.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ModeInfoDialog  : public Component,
                        public ChangeBroadcaster,
                        TextEditor::Listener,
                        public Button::Listener,
                        public Slider::Listener
{
public:
    //==============================================================================
    ModeInfoDialog (Mode* modeIn);
    ~ModeInfoDialog();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void commitMode();


	void textEditorTextChanged(TextEditor& textEditor);
	void textEditorEscapeKeyPressed(TextEditor& textEditor);
	void textEditorReturnKeyPressed(TextEditor& textEditor);
	void textEditorFocusLost(TextEditor& textEditor);

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Mode modeWorking;
    Mode* modeOriginal;
    ValueTree modeNode;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextEditor> familyBox;
    std::unique_ptr<TextEditor> stepsBox;
    std::unique_ptr<TextEditor> infoBox;
    std::unique_ptr<Label> familyNameLbl;
    std::unique_ptr<Label> stepsLbl;
    std::unique_ptr<Label> intervalSizesLbl;
    std::unique_ptr<Label> nameLabel;
    std::unique_ptr<Label> modeSizeLbl;
    std::unique_ptr<Label> scaleSizeLbl;
    std::unique_ptr<ToggleButton> defaultNameBtn;
    std::unique_ptr<Label> infoLbl;
    std::unique_ptr<TextEditor> nameBox;
    std::unique_ptr<TextButton> saveButton;
    std::unique_ptr<Slider> rotateSld;
    std::unique_ptr<Label> modeRotateLbl;
    std::unique_ptr<Label> scaleSizeReadout;
    std::unique_ptr<Label> modeSizeReadout;
    std::unique_ptr<Label> intervalSizeReadout;
    std::unique_ptr<TextButton> closeButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModeInfoDialog)
};

//[EndFile] You can add extra defines here...

//[/EndFile]

