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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "ModeInfoDialog.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ModeInfoDialog::ModeInfoDialog ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    textEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (textEditor.get());
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setText (TRANS("Meantone"));

    textEditor->setBounds (8, 104, 192, 24);

    textEditor2.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (textEditor2.get());
    textEditor2->setMultiLine (false);
    textEditor2->setReturnKeyStartsNewLine (false);
    textEditor2->setReadOnly (false);
    textEditor2->setScrollbarsShown (true);
    textEditor2->setCaretVisible (true);
    textEditor2->setPopupMenuEnabled (true);
    textEditor2->setText (TRANS("2 2 1 2 2 2 1"));

    textEditor2->setBounds (8, 40, 192, 24);

    textEditor3.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (textEditor3.get());
    textEditor3->setMultiLine (true);
    textEditor3->setReturnKeyStartsNewLine (false);
    textEditor3->setReadOnly (false);
    textEditor3->setScrollbarsShown (true);
    textEditor3->setCaretVisible (true);
    textEditor3->setPopupMenuEnabled (true);
    textEditor3->setText (TRANS("Ever hear of it?"));

    textEditor3->setBounds (8, 336, 320, 128);

    familyNameLbl.reset (new Label ("Family Name Label",
                                    TRANS("Temperament Family:\n")));
    addAndMakeVisible (familyNameLbl.get());
    familyNameLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    familyNameLbl->setJustificationType (Justification::centredLeft);
    familyNameLbl->setEditable (false, false, false);
    familyNameLbl->setColour (TextEditor::textColourId, Colours::black);
    familyNameLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    familyNameLbl->setBounds (8, 72, 150, 24);

    stepsLbl.reset (new Label ("Steps Label",
                               TRANS("Steps:\n")));
    addAndMakeVisible (stepsLbl.get());
    stepsLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    stepsLbl->setJustificationType (Justification::centredLeft);
    stepsLbl->setEditable (false, false, false);
    stepsLbl->setColour (TextEditor::textColourId, Colours::black);
    stepsLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    stepsLbl->setBounds (8, 8, 56, 24);

    intervalSizesLbl.reset (new Label ("Interval Sizes Label",
                                       TRANS("Interval Sizes:")));
    addAndMakeVisible (intervalSizesLbl.get());
    intervalSizesLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    intervalSizesLbl->setJustificationType (Justification::centredLeft);
    intervalSizesLbl->setEditable (false, false, false);
    intervalSizesLbl->setColour (TextEditor::textColourId, Colours::black);
    intervalSizesLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    intervalSizesLbl->setBounds (8, 272, 95, 24);

    nameLabel.reset (new Label ("Name Label",
                                TRANS("Mode Name:")));
    addAndMakeVisible (nameLabel.get());
    nameLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    nameLabel->setJustificationType (Justification::centredLeft);
    nameLabel->setEditable (false, false, false);
    nameLabel->setColour (TextEditor::textColourId, Colours::black);
    nameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    nameLabel->setBounds (8, 136, 96, 24);

    modeSizeLbl.reset (new Label ("Mode Size Label",
                                  TRANS("Mode Size:")));
    addAndMakeVisible (modeSizeLbl.get());
    modeSizeLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeSizeLbl->setJustificationType (Justification::centredLeft);
    modeSizeLbl->setEditable (false, false, false);
    modeSizeLbl->setColour (TextEditor::textColourId, Colours::black);
    modeSizeLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeSizeLbl->setBounds (8, 240, 80, 24);

    scaleSizeLbl.reset (new Label ("Scale Size Label",
                                   TRANS("Scale Size:")));
    addAndMakeVisible (scaleSizeLbl.get());
    scaleSizeLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    scaleSizeLbl->setJustificationType (Justification::centredLeft);
    scaleSizeLbl->setEditable (false, false, false);
    scaleSizeLbl->setColour (TextEditor::textColourId, Colours::black);
    scaleSizeLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    scaleSizeLbl->setBounds (8, 208, 71, 24);

    defaultNameBtn.reset (new ToggleButton ("Default Name Button"));
    addAndMakeVisible (defaultNameBtn.get());
    defaultNameBtn->setButtonText (TRANS("Use Default"));
    defaultNameBtn->setRadioGroupId (1);
    defaultNameBtn->addListener (this);
    defaultNameBtn->setToggleState (true, dontSendNotification);

    defaultNameBtn->setBounds (216, 168, 111, 24);

    infoLbl.reset (new Label ("Info Label",
                              TRANS("Info:")));
    addAndMakeVisible (infoLbl.get());
    infoLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    infoLbl->setJustificationType (Justification::centredLeft);
    infoLbl->setEditable (false, false, false);
    infoLbl->setColour (TextEditor::textColourId, Colours::black);
    infoLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    infoLbl->setBounds (8, 304, 40, 24);

    textEditor4.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (textEditor4.get());
    textEditor4->setMultiLine (false);
    textEditor4->setReturnKeyStartsNewLine (false);
    textEditor4->setReadOnly (false);
    textEditor4->setScrollbarsShown (true);
    textEditor4->setCaretVisible (true);
    textEditor4->setPopupMenuEnabled (true);
    textEditor4->setText (TRANS("Meantone[7] 12"));

    textEditor4->setBounds (8, 168, 192, 24);

    applyButton.reset (new TextButton ("Apply Button"));
    addAndMakeVisible (applyButton.get());
    applyButton->setButtonText (TRANS("Apply"));
    applyButton->addListener (this);

    applyButton->setBounds (112, 488, 104, 24);

    slider.reset (new Slider ("new slider"));
    addAndMakeVisible (slider.get());
    slider->setRange (-3, 4, 1);
    slider->setSliderStyle (Slider::IncDecButtons);
    slider->setTextBoxStyle (Slider::TextBoxLeft, false, 48, 20);
    slider->addListener (this);

    slider->setBounds (216, 40, 96, 24);

    modeRotateLbl.reset (new Label ("Mode Rotate Label",
                                    TRANS("Rotate Mode:")));
    addAndMakeVisible (modeRotateLbl.get());
    modeRotateLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeRotateLbl->setJustificationType (Justification::centredLeft);
    modeRotateLbl->setEditable (false, false, false);
    modeRotateLbl->setColour (TextEditor::textColourId, Colours::black);
    modeRotateLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeRotateLbl->setBounds (216, 8, 103, 24);

    scaleSizeReadout.reset (new Label ("Scale Size Readout",
                                       TRANS("12\n")));
    addAndMakeVisible (scaleSizeReadout.get());
    scaleSizeReadout->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    scaleSizeReadout->setJustificationType (Justification::centredLeft);
    scaleSizeReadout->setEditable (false, false, false);
    scaleSizeReadout->setColour (TextEditor::textColourId, Colours::black);
    scaleSizeReadout->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    scaleSizeReadout->setBounds (88, 208, 31, 24);

    modeSizeReadout.reset (new Label ("Mode Size Readout",
                                      TRANS("7")));
    addAndMakeVisible (modeSizeReadout.get());
    modeSizeReadout->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeSizeReadout->setJustificationType (Justification::centredLeft);
    modeSizeReadout->setEditable (false, false, false);
    modeSizeReadout->setColour (TextEditor::textColourId, Colours::black);
    modeSizeReadout->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeSizeReadout->setBounds (88, 240, 103, 24);

    intervalSizeReadout.reset (new Label ("Interval Size Readout",
                                          TRANS("7, 5")));
    addAndMakeVisible (intervalSizeReadout.get());
    intervalSizeReadout->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    intervalSizeReadout->setJustificationType (Justification::centredLeft);
    intervalSizeReadout->setEditable (false, false, false);
    intervalSizeReadout->setColour (TextEditor::textColourId, Colours::black);
    intervalSizeReadout->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    intervalSizeReadout->setBounds (104, 272, 143, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (340, 525);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ModeInfoDialog::~ModeInfoDialog()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    textEditor = nullptr;
    textEditor2 = nullptr;
    textEditor3 = nullptr;
    familyNameLbl = nullptr;
    stepsLbl = nullptr;
    intervalSizesLbl = nullptr;
    nameLabel = nullptr;
    modeSizeLbl = nullptr;
    scaleSizeLbl = nullptr;
    defaultNameBtn = nullptr;
    infoLbl = nullptr;
    textEditor4 = nullptr;
    applyButton = nullptr;
    slider = nullptr;
    modeRotateLbl = nullptr;
    scaleSizeReadout = nullptr;
    modeSizeReadout = nullptr;
    intervalSizeReadout = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ModeInfoDialog::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ModeInfoDialog::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ModeInfoDialog::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == defaultNameBtn.get())
    {
        //[UserButtonCode_defaultNameBtn] -- add your button handler code here..
        //[/UserButtonCode_defaultNameBtn]
    }
    else if (buttonThatWasClicked == applyButton.get())
    {
        //[UserButtonCode_applyButton] -- add your button handler code here..
        //[/UserButtonCode_applyButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void ModeInfoDialog::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider.get())
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        //[/UserSliderCode_slider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ModeInfoDialog" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="340" initialHeight="525">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="new text editor" id="23f020c7a9a4bed2" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="8 104 192 24" initialText="Meantone"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="5f84971388b9cded" memberName="textEditor2"
              virtualName="" explicitFocusOrder="0" pos="8 40 192 24" initialText="2 2 1 2 2 2 1"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="9460b63ee92bad3f" memberName="textEditor3"
              virtualName="" explicitFocusOrder="0" pos="8 336 320 128" initialText="Ever hear of it?"
              multiline="1" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="Family Name Label" id="dc9e44984d2c580c" memberName="familyNameLbl"
         virtualName="" explicitFocusOrder="0" pos="8 72 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Temperament Family:&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Steps Label" id="a3e176f2e17e3ec6" memberName="stepsLbl"
         virtualName="" explicitFocusOrder="0" pos="8 8 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Steps:&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Interval Sizes Label" id="c08a12e462ae8cea" memberName="intervalSizesLbl"
         virtualName="" explicitFocusOrder="0" pos="8 272 95 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Interval Sizes:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Name Label" id="5c6262b1b505756a" memberName="nameLabel"
         virtualName="" explicitFocusOrder="0" pos="8 136 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mode Name:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Mode Size Label" id="bea6fdfc1faecf54" memberName="modeSizeLbl"
         virtualName="" explicitFocusOrder="0" pos="8 240 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mode Size:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Scale Size Label" id="1f77fafbb9b74b81" memberName="scaleSizeLbl"
         virtualName="" explicitFocusOrder="0" pos="8 208 71 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Scale Size:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="Default Name Button" id="9d115fe17c7425f5" memberName="defaultNameBtn"
                virtualName="" explicitFocusOrder="0" pos="216 168 111 24" buttonText="Use Default"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="1"/>
  <LABEL name="Info Label" id="4618a6a917e67a95" memberName="infoLbl"
         virtualName="" explicitFocusOrder="0" pos="8 304 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Info:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="fd47b34a29e0b3c1" memberName="textEditor4"
              virtualName="" explicitFocusOrder="0" pos="8 168 192 24" initialText="Meantone[7] 12"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="Apply Button" id="819ae7d5095491a2" memberName="applyButton"
              virtualName="" explicitFocusOrder="0" pos="112 488 104 24" buttonText="Apply"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="58e50eefb260dd55" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="216 40 96 24" min="-3.0" max="4.0"
          int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="48" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="Mode Rotate Label" id="8b18e2a5c9cac13c" memberName="modeRotateLbl"
         virtualName="" explicitFocusOrder="0" pos="216 8 103 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Rotate Mode:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Scale Size Readout" id="cb7b314f9fff4ced" memberName="scaleSizeReadout"
         virtualName="" explicitFocusOrder="0" pos="88 208 31 24" edTextCol="ff000000"
         edBkgCol="0" labelText="12&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Mode Size Readout" id="5986a0db41f5e2e7" memberName="modeSizeReadout"
         virtualName="" explicitFocusOrder="0" pos="88 240 103 24" edTextCol="ff000000"
         edBkgCol="0" labelText="7" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Interval Size Readout" id="28b3648b1d961b9" memberName="intervalSizeReadout"
         virtualName="" explicitFocusOrder="0" pos="104 272 143 24" edTextCol="ff000000"
         edBkgCol="0" labelText="7, 5" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

