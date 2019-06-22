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
ModeInfoDialog::ModeInfoDialog (Mode* modeIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    familyBox.reset (new TextEditor ("Family Box"));
    addAndMakeVisible (familyBox.get());
    familyBox->setMultiLine (false);
    familyBox->setReturnKeyStartsNewLine (false);
    familyBox->setReadOnly (false);
    familyBox->setScrollbarsShown (true);
    familyBox->setCaretVisible (true);
    familyBox->setPopupMenuEnabled (true);
    familyBox->setText (TRANS("Meantone"));

    familyBox->setBounds (8, 168, 192, 24);

    stepsBox.reset (new TextEditor ("Steps Box"));
    addAndMakeVisible (stepsBox.get());
    stepsBox->setMultiLine (false);
    stepsBox->setReturnKeyStartsNewLine (false);
    stepsBox->setReadOnly (true);
    stepsBox->setScrollbarsShown (true);
    stepsBox->setCaretVisible (false);
    stepsBox->setPopupMenuEnabled (true);
    stepsBox->setText (TRANS("2 2 1 2 2 2 1"));

    stepsBox->setBounds (8, 40, 192, 24);

    infoBox.reset (new TextEditor ("Info Box"));
    addAndMakeVisible (infoBox.get());
    infoBox->setMultiLine (true);
    infoBox->setReturnKeyStartsNewLine (false);
    infoBox->setReadOnly (false);
    infoBox->setScrollbarsShown (true);
    infoBox->setCaretVisible (true);
    infoBox->setPopupMenuEnabled (true);
    infoBox->setText (TRANS("Ever hear of it?"));

    infoBox->setBounds (8, 336, 320, 128);

    familyNameLbl.reset (new Label ("Family Name Label",
                                    TRANS("Temperament Family:\n")));
    addAndMakeVisible (familyNameLbl.get());
    familyNameLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    familyNameLbl->setJustificationType (Justification::centredLeft);
    familyNameLbl->setEditable (false, false, false);
    familyNameLbl->setColour (TextEditor::textColourId, Colours::black);
    familyNameLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    familyNameLbl->setBounds (8, 136, 150, 24);

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

    nameLabel->setBounds (9, 73, 96, 24);

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
    defaultNameBtn->addListener (this);
    defaultNameBtn->setToggleState (true, dontSendNotification);

    defaultNameBtn->setBounds (217, 105, 111, 24);

    infoLbl.reset (new Label ("Info Label",
                              TRANS("Info:")));
    addAndMakeVisible (infoLbl.get());
    infoLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    infoLbl->setJustificationType (Justification::centredLeft);
    infoLbl->setEditable (false, false, false);
    infoLbl->setColour (TextEditor::textColourId, Colours::black);
    infoLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    infoLbl->setBounds (8, 304, 40, 24);

    nameBox.reset (new TextEditor ("Name Box"));
    addAndMakeVisible (nameBox.get());
    nameBox->setMultiLine (false);
    nameBox->setReturnKeyStartsNewLine (false);
    nameBox->setReadOnly (false);
    nameBox->setScrollbarsShown (true);
    nameBox->setCaretVisible (true);
    nameBox->setPopupMenuEnabled (true);
    nameBox->setText (TRANS("Meantone[7] 12"));

    nameBox->setBounds (9, 105, 192, 24);

    saveButton.reset (new TextButton ("Save Button"));
    addAndMakeVisible (saveButton.get());
    saveButton->setButtonText (TRANS("Save"));
    saveButton->addListener (this);

    saveButton->setBounds (32, 488, 104, 24);

    rotateSld.reset (new Slider ("Rotate Slider"));
    addAndMakeVisible (rotateSld.get());
    rotateSld->setRange (-3, 4, 1);
    rotateSld->setSliderStyle (Slider::IncDecButtons);
    rotateSld->setTextBoxStyle (Slider::TextBoxLeft, false, 48, 20);
    rotateSld->addListener (this);

    rotateSld->setBounds (216, 40, 96, 24);

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

    closeButton.reset (new TextButton ("closeButton"));
    addAndMakeVisible (closeButton.get());
    closeButton->setButtonText (TRANS("Close"));
    closeButton->addListener (this);

    closeButton->setBounds (192, 488, 104, 24);


    //[UserPreSize]
    mode = modeIn;
    modeNode = ValueTree(mode->modeNode);

    stepsBox->setText(mode->getStepsString());
    familyBox->setText(mode->getFamily());

    nameBox->setText(mode->getName());
	nameBox->setEnabled(false);

	defaultNameBtn->setClickingTogglesState(true);
    if (mode->getName() != mode->getDescription())
        defaultNameBtn->setToggleState(false, sendNotification);

	scaleSizeReadout->setText(String(mode->getScaleSize()), dontSendNotification);
    modeSizeReadout->setText(String(mode->getModeSize()), dontSendNotification);

    intervalSizeReadout->setText(arrayToString(mode->getIntervalSizeCount()), dontSendNotification);

    infoBox->setText(mode->getInfo());

	rotateSld->setRange(-(mode->getModeSize() - 1), mode->getModeSize() - 1, 1);

    //[/UserPreSize]

    setSize (340, 525);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ModeInfoDialog::~ModeInfoDialog()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	removeAllChangeListeners();
    //[/Destructor_pre]

    familyBox = nullptr;
    stepsBox = nullptr;
    infoBox = nullptr;
    familyNameLbl = nullptr;
    stepsLbl = nullptr;
    intervalSizesLbl = nullptr;
    nameLabel = nullptr;
    modeSizeLbl = nullptr;
    scaleSizeLbl = nullptr;
    defaultNameBtn = nullptr;
    infoLbl = nullptr;
    nameBox = nullptr;
    saveButton = nullptr;
    rotateSld = nullptr;
    modeRotateLbl = nullptr;
    scaleSizeReadout = nullptr;
    modeSizeReadout = nullptr;
    intervalSizeReadout = nullptr;
    closeButton = nullptr;


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

		if (defaultNameBtn->getToggleState())
		{
			nameBox->setText(mode->getDescription());
			nameBox->setEnabled(false);
		}
		else
		{
			nameBox->setEnabled(true);
		}
        //[/UserButtonCode_defaultNameBtn]
    }
    else if (buttonThatWasClicked == saveButton.get())
    {
        //[UserButtonCode_saveButton] -- add your button handler code here..
		commitMode();
		sendChangeMessage();
		getParentComponent()->exitModalState(0);
        //[/UserButtonCode_saveButton]
    }
    else if (buttonThatWasClicked == closeButton.get())
    {
        //[UserButtonCode_closeButton] -- add your button handler code here..
		getParentComponent()->exitModalState(0);
        //[/UserButtonCode_closeButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void ModeInfoDialog::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == rotateSld.get())
    {
        //[UserSliderCode_rotateSld] -- add your slider handling code here..
		String modeRotatedSteps = mode->getStepsString(sliderThatWasMoved->getValue());
		modeNode.setProperty(IDs::stepString, modeRotatedSteps, nullptr);
		stepsBox->setText(modeRotatedSteps);
        //[/UserSliderCode_rotateSld]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ModeInfoDialog::commitMode()
{
	modeNode.setProperty(IDs::modeName, nameBox->getText(), nullptr);
	modeNode.setProperty(IDs::family, familyBox->getText(), nullptr);
	modeNode.setProperty(IDs::modeInfo, infoBox->getText(), nullptr);

	mode->restoreNode(modeNode);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ModeInfoDialog" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster" constructorParams="Mode* modeIn"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="340" initialHeight="525">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="Family Box" id="23f020c7a9a4bed2" memberName="familyBox"
              virtualName="" explicitFocusOrder="0" pos="8 168 192 24" initialText="Meantone"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="Steps Box" id="5f84971388b9cded" memberName="stepsBox"
              virtualName="" explicitFocusOrder="0" pos="8 40 192 24" initialText="2 2 1 2 2 2 1"
              multiline="0" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <TEXTEDITOR name="Info Box" id="9460b63ee92bad3f" memberName="infoBox" virtualName=""
              explicitFocusOrder="0" pos="8 336 320 128" initialText="Ever hear of it?"
              multiline="1" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="Family Name Label" id="dc9e44984d2c580c" memberName="familyNameLbl"
         virtualName="" explicitFocusOrder="0" pos="8 136 150 24" edTextCol="ff000000"
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
         virtualName="" explicitFocusOrder="0" pos="9 73 96 24" edTextCol="ff000000"
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
                virtualName="" explicitFocusOrder="0" pos="217 105 111 24" buttonText="Use Default"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <LABEL name="Info Label" id="4618a6a917e67a95" memberName="infoLbl"
         virtualName="" explicitFocusOrder="0" pos="8 304 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Info:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="Name Box" id="fd47b34a29e0b3c1" memberName="nameBox" virtualName=""
              explicitFocusOrder="0" pos="9 105 192 24" initialText="Meantone[7] 12"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="Save Button" id="819ae7d5095491a2" memberName="saveButton"
              virtualName="" explicitFocusOrder="0" pos="32 488 104 24" buttonText="Save"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="Rotate Slider" id="58e50eefb260dd55" memberName="rotateSld"
          virtualName="" explicitFocusOrder="0" pos="216 40 96 24" min="-3.0"
          max="4.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="48" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
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
  <TEXTBUTTON name="closeButton" id="7fdae5390323aa2b" memberName="closeButton"
              virtualName="" explicitFocusOrder="0" pos="192 488 104 24" buttonText="Close"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

