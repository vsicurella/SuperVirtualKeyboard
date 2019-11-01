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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PluginControlComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PluginControlComponent::PluginControlComponent (SvkPluginState* pluginStateIn)
    : pluginState(pluginStateIn), appCmdMgr(pluginState->getAppCmdMgr())
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    mode1Box.reset (new ReferencedComboBox ("Mode1 Box"));
    addAndMakeVisible (mode1Box.get());
    mode1Box->setEditableText (false);
    mode1Box->setJustificationType (Justification::centredLeft);
    mode1Box->setTextWhenNothingSelected (String());
    mode1Box->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode1Box->addListener (this);

    mode2Box.reset (new ReferencedComboBox ("Mode2 Box"));
    addAndMakeVisible (mode2Box.get());
    mode2Box->setEditableText (false);
    mode2Box->setJustificationType (Justification::centredLeft);
    mode2Box->setTextWhenNothingSelected (String());
    mode2Box->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode2Box->addListener (this);

    mode1RootSld.reset (new Slider ("Mode1 Root Slider"));
    addAndMakeVisible (mode1RootSld.get());
    mode1RootSld->setRange (0, 127, 1);
    mode1RootSld->setSliderStyle (Slider::IncDecButtons);
    mode1RootSld->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    mode1RootSld->addListener (this);

    mode2RootSld.reset (new Slider ("mode2RootSld"));
    addAndMakeVisible (mode2RootSld.get());
    mode2RootSld->setRange (0, 127, 1);
    mode2RootSld->setSliderStyle (Slider::IncDecButtons);
    mode2RootSld->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    mode2RootSld->addListener (this);

    scaleEntryBtn.reset (new TextButton ("Scale Entry Button"));
    addAndMakeVisible (scaleEntryBtn.get());
    scaleEntryBtn->setButtonText (TRANS("OK"));
    scaleEntryBtn->addListener (this);

    modeInfoButton.reset (new TextButton ("Mode Info Button"));
    addAndMakeVisible (modeInfoButton.get());
    modeInfoButton->setButtonText (TRANS("i"));
    modeInfoButton->addListener (this);

    periodShiftSld.reset (new Slider ("Period Shift Slider"));
    addAndMakeVisible (periodShiftSld.get());
    periodShiftSld->setRange (-10, 10, 1);
    periodShiftSld->setSliderStyle (Slider::IncDecButtons);
    periodShiftSld->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    periodShiftSld->addListener (this);

    mode1ViewBtn.reset (new ToggleButton ("Mode1 View Button"));
    addAndMakeVisible (mode1ViewBtn.get());
    mode1ViewBtn->setButtonText (String());
    mode1ViewBtn->setRadioGroupId (10);
    mode1ViewBtn->addListener (this);
    mode1ViewBtn->setToggleState (true, dontSendNotification);

    mode2ViewBtn.reset (new ToggleButton ("Mode2 View Button"));
    addAndMakeVisible (mode2ViewBtn.get());
    mode2ViewBtn->setButtonText (String());
    mode2ViewBtn->setRadioGroupId (10);
    mode2ViewBtn->addListener (this);

    mode1RootLbl.reset (new Label ("Mode1 Root Label",
                                   TRANS("C4")));
    addAndMakeVisible (mode1RootLbl.get());
    mode1RootLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    mode1RootLbl->setJustificationType (Justification::centredLeft);
    mode1RootLbl->setEditable (false, false, false);
    mode1RootLbl->setColour (TextEditor::textColourId, Colours::black);
    mode1RootLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mode2RootLbl.reset (new Label ("Mode 2 Root Label",
                                   TRANS("F5")));
    addAndMakeVisible (mode2RootLbl.get());
    mode2RootLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    mode2RootLbl->setJustificationType (Justification::centredLeft);
    mode2RootLbl->setEditable (false, false, false);
    mode2RootLbl->setColour (TextEditor::textColourId, Colours::black);
    mode2RootLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mapStyleBox.reset (new ComboBox ("Mapping Style Box"));
    addAndMakeVisible (mapStyleBox.get());
    mapStyleBox->setEditableText (false);
    mapStyleBox->setJustificationType (Justification::centredLeft);
    mapStyleBox->setTextWhenNothingSelected (TRANS("Mode To Mode"));
    mapStyleBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mapStyleBox->addItem (TRANS("Mode To Mode"), 1);
    mapStyleBox->addItem (TRANS("Scale To Mode"), 2);
    mapStyleBox->addItem (TRANS("By Orders"), 3);
    mapStyleBox->addListener (this);

    midiChannelSld.reset (new Slider ("Midi Channel Slider"));
    addAndMakeVisible (midiChannelSld.get());
    midiChannelSld->setRange (1, 16, 1);
    midiChannelSld->setSliderStyle (Slider::IncDecButtons);
    midiChannelSld->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    midiChannelSld->addListener (this);

    midiChannelLbl.reset (new Label ("Midi Channel Label",
                                     TRANS("MIDI Channel:")));
    addAndMakeVisible (midiChannelLbl.get());
    midiChannelLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    midiChannelLbl->setJustificationType (Justification::centredLeft);
    midiChannelLbl->setEditable (false, false, false);
    midiChannelLbl->setColour (TextEditor::textColourId, Colours::black);
    midiChannelLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    noteNumsBtn.reset (new TextButton ("Note Numbers Button"));
    addAndMakeVisible (noteNumsBtn.get());
    noteNumsBtn->setButtonText (TRANS("#"));
    noteNumsBtn->addListener (this);

    periodShiftLbl.reset (new Label ("Period Shift Label",
                                     TRANS("Period Shift:")));
    addAndMakeVisible (periodShiftLbl.get());
    periodShiftLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    periodShiftLbl->setJustificationType (Justification::centredLeft);
    periodShiftLbl->setEditable (false, false, false);
    periodShiftLbl->setColour (TextEditor::textColourId, Colours::black);
    periodShiftLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    editColorsBtn.reset (new TextButton ("Edit Colors Button"));
    addAndMakeVisible (editColorsBtn.get());
    editColorsBtn->setButtonText (TRANS("Edit Colors"));
    editColorsBtn->addListener (this);

    keyStyleBox.reset (new ComboBox ("Key Style Box"));
    addAndMakeVisible (keyStyleBox.get());
    keyStyleBox->setEditableText (false);
    keyStyleBox->setJustificationType (Justification::centredLeft);
    keyStyleBox->setTextWhenNothingSelected (String());
    keyStyleBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    keyStyleBox->addItem (TRANS("Nested Right"), 1);
    keyStyleBox->addItem (TRANS("Nested Center"), 2);
    keyStyleBox->addItem (TRANS("Flat"), 3);
    keyStyleBox->addItem (TRANS("Adjacent"), 4);
    keyStyleBox->addListener (this);

    saveBtn.reset (new TextButton ("Save Button"));
    addAndMakeVisible (saveBtn.get());
    saveBtn->setButtonText (TRANS("Save"));
    saveBtn->addListener (this);

    loadBtn.reset (new TextButton ("Load Button"));
    addAndMakeVisible (loadBtn.get());
    loadBtn->setButtonText (TRANS("Load"));
    loadBtn->addListener (this);

    mapStyleLbl.reset (new Label ("Mapping Style Label",
                                  TRANS("Mapping Style:")));
    addAndMakeVisible (mapStyleLbl.get());
    mapStyleLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    mapStyleLbl->setJustificationType (Justification::centredLeft);
    mapStyleLbl->setEditable (false, false, false);
    mapStyleLbl->setColour (TextEditor::textColourId, Colours::black);
    mapStyleLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    highlightStyleBox.reset (new ComboBox ("Highlight Style Box"));
    addAndMakeVisible (highlightStyleBox.get());
    highlightStyleBox->setEditableText (false);
    highlightStyleBox->setJustificationType (Justification::centredLeft);
    highlightStyleBox->setTextWhenNothingSelected (String());
    highlightStyleBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    highlightStyleBox->addItem (TRANS("Full Key"), 1);
    highlightStyleBox->addItem (TRANS("Inside"), 2);
    highlightStyleBox->addItem (TRANS("Border"), 3);
    highlightStyleBox->addItem (TRANS("Circles"), 4);
    highlightStyleBox->addItem (TRANS("Squares"), 5);
    highlightStyleBox->addListener (this);

    keyboardViewport.reset (new Viewport ("Keyboard Viewport"));
    addAndMakeVisible (keyboardViewport.get());
    keyboardViewport->setScrollBarsShown (false, true);
    keyboardViewport->setScrollBarThickness (12);

    settingsButton.reset (new TextButton ("Settings Button"));
    addAndMakeVisible (settingsButton.get());
    settingsButton->setButtonText (TRANS("Settings"));
    settingsButton->addListener (this);

    mapOrderEditBtn.reset (new TextButton ("Map Order Edit Button"));
    addAndMakeVisible (mapOrderEditBtn.get());
    mapOrderEditBtn->setButtonText (TRANS("Edit Mapping"));
    mapOrderEditBtn->addListener (this);

    mapModeBox.reset (new ComboBox ("Mapping Node Box"));
    addAndMakeVisible (mapModeBox.get());
    mapModeBox->setEditableText (false);
    mapModeBox->setJustificationType (Justification::centredLeft);
    mapModeBox->setTextWhenNothingSelected (TRANS("Mapping Off"));
    mapModeBox->setTextWhenNoChoicesAvailable (TRANS("Mapping Off"));
    mapModeBox->addItem (TRANS("Mapping Off"), 1);
    mapModeBox->addItem (TRANS("Auto Map"), 2);
    mapModeBox->addItem (TRANS("Manual Map"), 3);
    mapModeBox->addListener (this);

    mapApplyBtn.reset (new TextButton ("new button"));
    addAndMakeVisible (mapApplyBtn.get());
    mapApplyBtn->setButtonText (TRANS("Apply"));
    mapApplyBtn->addListener (this);

    mapApplyBtn->setBounds (391, -40, 55, 24);

    scaleTextBox.reset (new TextEditor ("Scale Text Box"));
    addAndMakeVisible (scaleTextBox.get());
    scaleTextBox->setMultiLine (false);
    scaleTextBox->setReturnKeyStartsNewLine (false);
    scaleTextBox->setReadOnly (false);
    scaleTextBox->setScrollbarsShown (true);
    scaleTextBox->setCaretVisible (true);
    scaleTextBox->setPopupMenuEnabled (true);
    scaleTextBox->setText (String());

    sizeToggleBtn.reset (new Component());
    addAndMakeVisible (sizeToggleBtn.get());
    sizeToggleBtn->setName ("SizeToggleButton");

    transposeSld.reset (new Slider ("TransposeSlider"));
    addAndMakeVisible (transposeSld.get());
    transposeSld->setRange (-128, 127, 1);
    transposeSld->setSliderStyle (Slider::IncDecButtons);
    transposeSld->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    transposeSld->addListener (this);


    //[UserPreSize]
    mapOrderEditBtn->setVisible(false);

	scaleTextBox->addListener(this);

	noteNumsBtn->setClickingTogglesState(true);

    scaleTextBox->setInputFilter(pluginState->textFilterIntOrSpace.get(), false);

	// allows for implementing mouseDown() to update the menus
	mode1Box->setInterceptsMouseClicks(false, false);
	mode2Box->setInterceptsMouseClicks(false, false);

    saveMenu.reset(new PopupMenu());
    saveMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::saveMode);
    saveMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::savePresetToFile);
    saveMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::exportReaperMap);
    saveMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::exportAbletonMap);

    loadMenu.reset(new PopupMenu());
    loadMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::loadMode);
    loadMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::loadPreset);

    //[/UserPreSize]

    setSize (1000, 250);


    //[Constructor] You can add your own custom stuff here..
	keyboardViewport->setBounds(7, 40, proportionOfWidth(0.9830f), getHeight() - 83);

	// DISABLED BECAUSE OF MOBILE
	mapStyleBox->setVisible(false);
	mapApplyBtn->setVisible(false);
	mapStyleLbl->setVisible(false);
	mapModeBox->setVisible(false);
	mode1Box->setVisible(false);
	mode1RootLbl->setVisible(false);
	mode1RootSld->setVisible(false);
	mode2RootLbl->setVisible(false);
	mode2RootSld->setVisible(false);
	mode1ViewBtn->setVisible(false);
	mode2ViewBtn->setVisible(false);

	// DISABLED BECAUSE NOT IMPLEMENTED
    editColorsBtn->setEnabled(false);
    mapModeBox->setItemEnabled(3, false);
	//keyStyleBox->setItemEnabled(2, false);
	//keyStyleBox->setItemEnabled(4, false);
	highlightStyleBox->setItemEnabled(2, false);
	highlightStyleBox->setItemEnabled(3, false);
	highlightStyleBox->setItemEnabled(4, false);
	highlightStyleBox->setItemEnabled(5, false);

    //[/Constructor]
}

PluginControlComponent::~PluginControlComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    mode1Box = nullptr;
    mode2Box = nullptr;
    mode1RootSld = nullptr;
    mode2RootSld = nullptr;
    scaleEntryBtn = nullptr;
    modeInfoButton = nullptr;
    periodShiftSld = nullptr;
    mode1ViewBtn = nullptr;
    mode2ViewBtn = nullptr;
    mode1RootLbl = nullptr;
    mode2RootLbl = nullptr;
    mapStyleBox = nullptr;
    midiChannelSld = nullptr;
    midiChannelLbl = nullptr;
    noteNumsBtn = nullptr;
    periodShiftLbl = nullptr;
    editColorsBtn = nullptr;
    keyStyleBox = nullptr;
    saveBtn = nullptr;
    loadBtn = nullptr;
    mapStyleLbl = nullptr;
    highlightStyleBox = nullptr;
    keyboardViewport = nullptr;
    settingsButton = nullptr;
    mapOrderEditBtn = nullptr;
    mapModeBox = nullptr;
    mapApplyBtn = nullptr;
    scaleTextBox = nullptr;
    sizeToggleBtn = nullptr;
    transposeSld = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PluginControlComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PluginControlComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..

    //[/UserPreResize]

    mode1Box->setBounds (getWidth() - 218, -64, 150, 24);
    mode2Box->setBounds (proportionOfWidth (0.8317f) - (proportionOfWidth (0.2367f) / 2), 8, proportionOfWidth (0.2367f), 24);
    mode1RootSld->setBounds (getWidth() - 310, -64, 79, 24);
    mode2RootSld->setBounds (proportionOfWidth (0.6582f), -40, proportionOfWidth (0.0934f), 24);
    scaleEntryBtn->setBounds (proportionOfWidth (0.5870f) - proportionOfWidth (0.0510f), 8, proportionOfWidth (0.0510f), 24);
    modeInfoButton->setBounds (proportionOfWidth (0.2314f), 8, 30, 24);
    periodShiftSld->setBounds ((7 + 0) + roundToInt (proportionOfWidth (0.1104f) * 0.9808f), getHeight() - 10 - 24, proportionOfWidth (0.1231f), 24);
    mode1ViewBtn->setBounds (getWidth() - 66, -64, 31, 24);
    mode2ViewBtn->setBounds (getWidth() - 66, -32, 31, 24);
    mode1RootLbl->setBounds (getWidth() - 342, -64, 32, 24);
    mode2RootLbl->setBounds (getWidth() - 339, -32, 32, 24);
    mapStyleBox->setBounds (195 - (152 / 2), -40, 152, 24);
    midiChannelSld->setBounds ((((7 + 0) + roundToInt (proportionOfWidth (0.1104f) * 0.9808f)) + roundToInt (proportionOfWidth (0.1231f) * 1.0862f)) + roundToInt (proportionOfWidth (0.1200f) * 0.9115f), getHeight() - 10 - 24, proportionOfWidth (0.1253f), 24);
    midiChannelLbl->setBounds (((7 + 0) + roundToInt (proportionOfWidth (0.1104f) * 0.9808f)) + roundToInt (proportionOfWidth (0.1231f) * 1.0862f), getHeight() - 10 - 24, proportionOfWidth (0.1200f), 24);
    noteNumsBtn->setBounds (((((7 + 0) + roundToInt (proportionOfWidth (0.1104f) * 0.9808f)) + roundToInt (proportionOfWidth (0.1231f) * 1.0862f)) + roundToInt (proportionOfWidth (0.1200f) * 0.9115f)) + roundToInt (proportionOfWidth (0.1253f) * 1.0932f), getHeight() - 10 - 24, proportionOfWidth (0.0276f), 24);
    periodShiftLbl->setBounds (7 + 0, getHeight() - 10 - 24, proportionOfWidth (0.1104f), 24);
    editColorsBtn->setBounds (proportionOfWidth (0.9777f) - proportionOfWidth (0.1083f), getHeight() - 10 - 24, proportionOfWidth (0.1083f), 24);
    keyStyleBox->setBounds ((((((7 + 0) + roundToInt (proportionOfWidth (0.1104f) * 0.9808f)) + roundToInt (proportionOfWidth (0.1231f) * 1.0862f)) + roundToInt (proportionOfWidth (0.1200f) * 0.9115f)) + roundToInt (proportionOfWidth (0.1253f) * 1.0932f)) + roundToInt (proportionOfWidth (0.0276f) * 1.5385f), getHeight() - 10 - 24, proportionOfWidth (0.1699f), 24);
    saveBtn->setBounds (7, 8, proportionOfWidth (0.0563f), 24);
    loadBtn->setBounds (proportionOfWidth (0.0732f), 8, 48, 24);
    mapStyleLbl->setBounds (59 - (104 / 2), -40, 104, 24);
    highlightStyleBox->setBounds (((((((7 + 0) + roundToInt (proportionOfWidth (0.1104f) * 0.9808f)) + roundToInt (proportionOfWidth (0.1231f) * 1.0862f)) + roundToInt (proportionOfWidth (0.1200f) * 0.9115f)) + roundToInt (proportionOfWidth (0.1253f) * 1.0932f)) + roundToInt (proportionOfWidth (0.0276f) * 1.5385f)) + roundToInt (proportionOfWidth (0.1699f) * 1.0875f), getHeight() - 10 - 24, proportionOfWidth (0.1369f), 24);
    keyboardViewport->setBounds (7, 40, proportionOfWidth (0.9830f), getHeight() - 83);
    settingsButton->setBounds (proportionOfWidth (0.1380f), 8, proportionOfWidth (0.0860f), 24);
    mapOrderEditBtn->setBounds (383 - 96, -40, 96, 24);
    mapModeBox->setBounds (119, -32, proportionOfWidth (0.1582f), 24);
    scaleTextBox->setBounds (proportionOfWidth (0.5319f) - proportionOfWidth (0.2580f), 8, proportionOfWidth (0.2580f), 24);
    sizeToggleBtn->setBounds (proportionOfWidth (0.9830f) - proportionOfWidth (0.0244f), 8, proportionOfWidth (0.0244f), 24);
    transposeSld->setBounds (proportionOfWidth (0.7059f) - proportionOfWidth (0.1125f), 8, proportionOfWidth (0.1125f), 24);
    //[UserResized] Add your own custom resize handling here..

	VirtualKeyboard::Keyboard* svk = dynamic_cast<VirtualKeyboard::Keyboard*>(keyboardViewport->getViewedComponent());
	if (svk)
	{
		int height = keyboardViewport->getMaximumVisibleHeight();
		svk->setBounds(0, 0, svk->getPianoWidth(height), height);
	}

    //[/UserResized]
}

void PluginControlComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == mode1Box.get())
    {
        //[UserComboBoxCode_mode1Box] -- add your combo box handling code here..
        //DBG("MODE 1 SELECTED ID: " + String(mode1Box->getSelectedId()));
        appCmdMgr->invokeDirectly(IDs::CommandIDs::setMode1, true);
        //[/UserComboBoxCode_mode1Box]
    }
    else if (comboBoxThatHasChanged == mode2Box.get())
    {
        //[UserComboBoxCode_mode2Box] -- add your combo box handling code here..
        //DBG("MODE 2 SELECTED ID: " + String(mode2Box->getSelectedId()));
        appCmdMgr->invokeDirectly(IDs::CommandIDs::setMode2, true);
        //[/UserComboBoxCode_mode2Box]
    }
    else if (comboBoxThatHasChanged == mapStyleBox.get())
    {
        //[UserComboBoxCode_mapStyleBox] -- add your combo box handling code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setMappingStyle, true);

        if (inMappingMode && (mapStyleBox->getSelectedId() == 3 || mapModeBox->getSelectedId() == 3))
        {
            mapOrderEditBtn->setVisible(true);
            mapApplyBtn->setVisible(true);
        }
        else
        {
            mapOrderEditBtn->setVisible(false);
            mapApplyBtn->setVisible(false);
        }

        //[/UserComboBoxCode_mapStyleBox]
    }
    else if (comboBoxThatHasChanged == keyStyleBox.get())
    {
        //[UserComboBoxCode_keyStyleBox] -- add your combo box handling code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setKeyStyle, true);
        //[/UserComboBoxCode_keyStyleBox]
    }
    else if (comboBoxThatHasChanged == highlightStyleBox.get())
    {
        //[UserComboBoxCode_highlightStyleBox] -- add your combo box handling code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setHighlightStyle, true);
        //[/UserComboBoxCode_highlightStyleBox]
    }
    else if (comboBoxThatHasChanged == mapModeBox.get())
    {
        //[UserComboBoxCode_mapModeBox] -- add your combo box handling code here..
        appCmdMgr->invokeDirectly(IDs::CommandIDs::setMappingMode, true);
        //[/UserComboBoxCode_mapModeBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void PluginControlComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == mode1RootSld.get())
    {
        //[UserSliderCode_mode1RootSld] -- add your slider handling code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setMode1RootNote, true);
		mode1RootLbl->setText(MidiMessage::getMidiNoteName(mode1RootSld->getValue(), true, true, 4), dontSendNotification);
        //[/UserSliderCode_mode1RootSld]
    }
    else if (sliderThatWasMoved == mode2RootSld.get())
    {
        //[UserSliderCode_mode2RootSld] -- add your slider handling code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setMode2RootNote, true);
		mode2RootLbl->setText(MidiMessage::getMidiNoteName(mode2RootSld->getValue(), true, true, 4), dontSendNotification);
        //[/UserSliderCode_mode2RootSld]
    }
    else if (sliderThatWasMoved == periodShiftSld.get())
    {
        //[UserSliderCode_periodShiftSld] -- add your slider handling code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setPeriodShift, true);
        //[/UserSliderCode_periodShiftSld]
    }
    else if (sliderThatWasMoved == midiChannelSld.get())
    {
        //[UserSliderCode_midiChannelSld] -- add your slider handling code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setMidiChannelOut, true);
        //[/UserSliderCode_midiChannelSld]
    }
    else if (sliderThatWasMoved == transposeSld.get())
    {
        //[UserSliderCode_transposeSld] -- add your slider handling code here..
		//appCmdMgr->invokeDirectly(IDs::CommandIDs::setTransposeNotes, true);
        //[/UserSliderCode_transposeSld]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void PluginControlComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == scaleEntryBtn.get())
    {
        //[UserButtonCode_scaleEntryBtn] -- add your button handler code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::commitCustomScale, true);
        //[/UserButtonCode_scaleEntryBtn]
    }
    else if (buttonThatWasClicked == modeInfoButton.get())
    {
        //[UserButtonCode_modeInfoButton] -- add your button handler code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::showModeInfo, true);
        //[/UserButtonCode_modeInfoButton]
    }
    else if (buttonThatWasClicked == mode1ViewBtn.get())
    {
        //[UserButtonCode_mode1ViewBtn] -- add your button handler code here..
		if (mode1ViewBtn->getToggleState())
			appCmdMgr->invokeDirectly(IDs::CommandIDs::setModeViewed, true);
        //[/UserButtonCode_mode1ViewBtn]
    }
    else if (buttonThatWasClicked == mode2ViewBtn.get())
    {
        //[UserButtonCode_mode2ViewBtn] -- add your button handler code here..
		if (mode2ViewBtn->getToggleState())
			appCmdMgr->invokeDirectly(IDs::CommandIDs::setModeViewed, true);
        //[/UserButtonCode_mode2ViewBtn]
    }
    else if (buttonThatWasClicked == noteNumsBtn.get())
    {
        //[UserButtonCode_noteNumsBtn] -- add your button handler code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::showMidiNoteNumbers, true);
        //[/UserButtonCode_noteNumsBtn]
    }
    else if (buttonThatWasClicked == editColorsBtn.get())
    {
        //[UserButtonCode_editColorsBtn] -- add your button handler code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::beginColorEditing, true);
        //[/UserButtonCode_editColorsBtn]
    }
    else if (buttonThatWasClicked == saveBtn.get())
    {
        //[UserButtonCode_saveBtn] -- add your button handler code here..
		//appCmdMgr->invokeDirectly(IDs::CommandIDs::savePresetToFile, true);
        //saveMenu->showAt(saveBtn.get());
        //[/UserButtonCode_saveBtn]
    }
    else if (buttonThatWasClicked == loadBtn.get())
    {
        //[UserButtonCode_loadBtn] -- add your button handler code here..
		//appCmdMgr->invokeDirectly(IDs::CommandIDs::loadPreset, true);
        //loadMenu->showAt(loadBtn.get());
        //[/UserButtonCode_loadBtn]
    }
    else if (buttonThatWasClicked == settingsButton.get())
    {
        //[UserButtonCode_settingsButton] -- add your button handler code here..
        appCmdMgr->invokeDirectly(IDs::CommandIDs::showSettingsDialog, true);
        //[/UserButtonCode_settingsButton]
    }
    else if (buttonThatWasClicked == mapOrderEditBtn.get())
    {
        //[UserButtonCode_mapOrderEditBtn] -- add your button handler code here..
        appCmdMgr->invokeDirectly(IDs::CommandIDs::showMapOrderEdit, true);
        //[/UserButtonCode_mapOrderEditBtn]
    }
    else if (buttonThatWasClicked == mapApplyBtn.get())
    {
        //[UserButtonCode_mapApplyBtn] -- add your button handler code here..
        appCmdMgr->invokeDirectly(IDs::CommandIDs::applyMapping, true);
        //[/UserButtonCode_mapApplyBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void PluginControlComponent::textEditorTextChanged(TextEditor& textEditor)
{

}

void PluginControlComponent::textEditorEscapeKeyPressed(TextEditor& textEditor)
{

}

void PluginControlComponent::textEditorReturnKeyPressed(TextEditor& textEditor)
{
	appCmdMgr->invokeDirectly(IDs::CommandIDs::commitCustomScale, true);
}

void PluginControlComponent::textEditorFocusLost(TextEditor& textEditor)
{

}

void PluginControlComponent::mouseDown(const MouseEvent& e)
{
	if (mode1Box->getBounds().contains(e.getPosition()))
	{
		String display = mode1Box->getText();
		pluginState->getPresetManager()->requestModeMenu(mode1Box->getRootMenu());
		mode1Box->setText(display, dontSendNotification);
		mode1Box->showPopup();
	}

	if (mode2Box->getBounds().contains(e.getPosition()))
	{
		String display = mode2Box->getText();
		pluginState->getPresetManager()->requestModeMenu(mode2Box->getRootMenu());
		mode2Box->setText(display, dontSendNotification);
		mode2Box->showPopup();
	}
}

Viewport* PluginControlComponent::getViewport()
{
	return keyboardViewport.get();
}

TextEditor* PluginControlComponent::getScaleTextEditor()
{
	return scaleTextBox.get();
}

ComboBox* PluginControlComponent::getMappingStyleBox()
{
    return mapStyleBox.get();
}

String PluginControlComponent::getScaleEntryText()
{
	return scaleTextBox->getText();
}

void PluginControlComponent::setScaleEntryText(String textIn, NotificationType notify)
{
	scaleTextBox->setText(textIn, notify == sendNotification);
}

ReferencedComboBox* PluginControlComponent::getMode1Box()
{
	return mode1Box.get();
}

ReferencedComboBox* PluginControlComponent::getMode2Box()
{
	return mode2Box.get();
}

void PluginControlComponent::updateModeBoxMenus()
{

}


TextButton* PluginControlComponent::getModeInfoButton()
{
	return modeInfoButton.get();
}

int PluginControlComponent::getViewPosition()
{
	return keyboardViewport->getViewPositionX();
}

void PluginControlComponent::setViewPosition(int xIn)
{
	keyboardViewport->setViewPosition(Point<int>(xIn, 0));
}

void PluginControlComponent::setViewPosition(float xRatioIn)
{
	keyboardViewport->setViewPositionProportionately(xRatioIn, 1);
}


int PluginControlComponent::getMode1BoxSelection()
{
	return mode1Box->getSelectedId();
}

int PluginControlComponent::getMode2BoxSelection()
{
	return mode2Box->getSelectedId();
}

void PluginControlComponent::setMode1BoxText(String textIn, NotificationType notify)
{
	mode1Box->setText(textIn, notify);
}

void PluginControlComponent::setMode2BoxText(String textIn, NotificationType notify)
{
	mode2Box->setText(textIn, notify);
}

void PluginControlComponent::setMode1BoxId(int idIn, NotificationType notify)
{
	mode1Box->setSelectedId(idIn, notify);
}

void PluginControlComponent::setMode2BoxId(int idIn, NotificationType notify)
{
	mode2Box->setSelectedId(idIn, notify);
}

void PluginControlComponent::setMode1View(bool isViewed, NotificationType notify)
{
	mode1ViewBtn->setToggleState(isViewed, notify);
}

bool PluginControlComponent::getMode1View()
{
	return mode1ViewBtn->getToggleState();
}

void PluginControlComponent::setMode2View(bool isViewed, NotificationType notify)
{
	mode2ViewBtn->setToggleState(isViewed, notify);
}

bool PluginControlComponent::getMode2View()
{
	return mode2ViewBtn->getToggleState();
}

int PluginControlComponent::getModeViewed()
{
	return mode1ViewBtn->getToggleState() ? 0 : 1;
}

int PluginControlComponent::getMappingMode()
{
    return mapModeBox->getSelectedId();
}

void PluginControlComponent::setMappingMode(int mappingModeId, NotificationType notify)
{
	mapModeBox->setSelectedId(mappingModeId, notify);

    inMappingMode = mappingModeId > 1;

    mapStyleLbl->setVisible(inMappingMode);
    mapStyleBox->setVisible(inMappingMode);
    mapOrderEditBtn->setVisible(inMappingMode);
    mode1RootLbl->setVisible(inMappingMode);
    mode1RootSld->setVisible(inMappingMode);
    mode1Box->setVisible(inMappingMode);
    mode1ViewBtn->setVisible(inMappingMode);
    mode2ViewBtn->setVisible(inMappingMode);

    if (mappingModeId == 3 || mapStyleBox->getSelectedId() == 3)
    {
        mapOrderEditBtn->setVisible(true);
        mapApplyBtn->setVisible(true);
    }
    else
    {
        mapOrderEditBtn->setVisible(false);
        mapApplyBtn->setVisible(false);
    }

    if (inMappingMode)
        mode2ViewBtn->setToggleState(true, sendNotification);

    resized();
}

int PluginControlComponent::getMappingStyle()
{
	return mapStyleBox->getSelectedId();
}

void PluginControlComponent::setMappingStyleId(int idIn, NotificationType notify)
{
	mapStyleBox->setSelectedId(idIn, notify);

    if (inMappingMode && (idIn == 3 || mapModeBox->getSelectedId() == 3))
    {
        mapOrderEditBtn->setVisible(true);
        mapApplyBtn->setVisible(true);
    }
    else
    {
        mapOrderEditBtn->setVisible(false);
        mapApplyBtn->setVisible(false);
    }
}

int PluginControlComponent::getMode1Root()
{
	return mode1RootSld->getValue();
}

void PluginControlComponent::setMode1Root(int rootIn, NotificationType notify)
{
	mode1RootSld->setValue(rootIn, notify);
	mode1RootLbl->setText(MidiMessage::getMidiNoteName(rootIn, true, true, 4), dontSendNotification);
}

int PluginControlComponent::getMode2Root()
{
	return mode2RootSld->getValue();
}

void PluginControlComponent::setMode2Root(int rootIn, NotificationType notify)
{
	mode2RootSld->setValue(rootIn, notify);
	mode2RootLbl->setText(MidiMessage::getMidiNoteName(rootIn, true, true, 4), dontSendNotification);
}

int PluginControlComponent::getPeriodShift()
{
	return periodShiftSld->getValue();

}
void PluginControlComponent::setPeriodShift(int periodsIn, NotificationType notify)
{
	periodShiftSld->setValue(periodsIn);
}

int PluginControlComponent::getMidiChannel()
{
	return midiChannelSld->getValue();
}

void PluginControlComponent::setMidiChannel(int channelIn, NotificationType notify)
{
	midiChannelSld->setValue(channelIn, notify);
}

bool PluginControlComponent::getNoteNumsView()
{
	return noteNumsBtn->getToggleState();
}

void PluginControlComponent::setNoteNumsView(bool isViewed, NotificationType notify)
{
	noteNumsBtn->setToggleState(isViewed, notify);
}

int PluginControlComponent::getKeyStyle()
{
	return keyStyleBox->getSelectedId();
}

void PluginControlComponent::setKeyStyleId(int idIn, NotificationType notify)
{
	keyStyleBox->setSelectedId(idIn);
}

int PluginControlComponent::getHighlightStyle()
{
	return highlightStyleBox->getSelectedId();
}
void PluginControlComponent::setHighlightStyleId(int idIn, NotificationType notify)
{
	highlightStyleBox->setSelectedId(idIn, notify);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PluginControlComponent" componentName=""
                 parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="SvkPluginState* pluginStateIn" variableInitialisers="pluginState(pluginStateIn), appCmdMgr(pluginState-&gt;getAppCmdMgr())"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="1000" initialHeight="250">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="Mode1 Box" id="197cbd0054b3ea6d" memberName="mode1Box"
            virtualName="ReferencedComboBox" explicitFocusOrder="0" pos="218R -64 150 24"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="Mode2 Box" id="e43718c6ce3f175" memberName="mode2Box" virtualName="ReferencedComboBox"
            explicitFocusOrder="0" pos="83.174%c 8 23.673% 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="Mode1 Root Slider" id="e732568ad188d067" memberName="mode1RootSld"
          virtualName="" explicitFocusOrder="0" pos="310R -64 79 24" min="0.0"
          max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="mode2RootSld" id="ae29d1ebf57b7bd7" memberName="mode2RootSld"
          virtualName="" explicitFocusOrder="0" pos="65.817% -40 9.342% 24"
          posRelativeX="d5aac21b3b7762a8" min="0.0" max="127.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTBUTTON name="Scale Entry Button" id="bbb112b96c51ecf7" memberName="scaleEntryBtn"
              virtualName="" explicitFocusOrder="0" pos="58.705%r 8 5.096% 24"
              buttonText="OK" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Mode Info Button" id="c6025abb0c68f4" memberName="modeInfoButton"
              virtualName="" explicitFocusOrder="0" pos="23.142% 8 30 24" buttonText="i"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="Period Shift Slider" id="89742a2afd3a3325" memberName="periodShiftSld"
          virtualName="" explicitFocusOrder="0" pos="98.077% 10Rr 12.314% 24"
          posRelativeX="140764aae2291735" min="-10.0" max="10.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TOGGLEBUTTON name="Mode1 View Button" id="bb818484022dae8c" memberName="mode1ViewBtn"
                virtualName="" explicitFocusOrder="0" pos="66R -64 31 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="10" state="1"/>
  <TOGGLEBUTTON name="Mode2 View Button" id="a8183216fad07ec6" memberName="mode2ViewBtn"
                virtualName="" explicitFocusOrder="0" pos="66R -32 31 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="10" state="0"/>
  <LABEL name="Mode1 Root Label" id="2df91047cc98faf4" memberName="mode1RootLbl"
         virtualName="" explicitFocusOrder="0" pos="342R -64 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="C4" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Mode 2 Root Label" id="c21534178bfcff74" memberName="mode2RootLbl"
         virtualName="" explicitFocusOrder="0" pos="339R -32 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="F5" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="Mapping Style Box" id="217ff7f7b6934ebe" memberName="mapStyleBox"
            virtualName="" explicitFocusOrder="0" pos="195c -40 152 24" editable="0"
            layout="33" items="Mode To Mode&#10;Scale To Mode&#10;By Orders"
            textWhenNonSelected="Mode To Mode" textWhenNoItems="(no choices)"/>
  <SLIDER name="Midi Channel Slider" id="4f2d4d8b9051247f" memberName="midiChannelSld"
          virtualName="" explicitFocusOrder="0" pos="91.15% 10Rr 12.527% 24"
          posRelativeX="d2587185be475d9c" min="1.0" max="16.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="Midi Channel Label" id="d2587185be475d9c" memberName="midiChannelLbl"
         virtualName="" explicitFocusOrder="0" pos="108.621% 10Rr 11.996% 24"
         posRelativeX="89742a2afd3a3325" edTextCol="ff000000" edBkgCol="0"
         labelText="MIDI Channel:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Note Numbers Button" id="405a79d645f5f8ac" memberName="noteNumsBtn"
              virtualName="" explicitFocusOrder="0" pos="109.322% 10Rr 2.76% 24"
              posRelativeX="4f2d4d8b9051247f" buttonText="#" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="Period Shift Label" id="140764aae2291735" memberName="periodShiftLbl"
         virtualName="" explicitFocusOrder="0" pos="0 10Rr 11.04% 24"
         posRelativeX="1f2717bdf6633c2" edTextCol="ff000000" edBkgCol="0"
         labelText="Period Shift:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Edit Colors Button" id="bc86137b1b6a5176" memberName="editColorsBtn"
              virtualName="" explicitFocusOrder="0" pos="97.771%r 10Rr 10.828% 24"
              buttonText="Edit Colors" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <COMBOBOX name="Key Style Box" id="292b32e0c6cd0b80" memberName="keyStyleBox"
            virtualName="" explicitFocusOrder="0" pos="153.846% 10Rr 16.985% 24"
            posRelativeX="405a79d645f5f8ac" editable="0" layout="33" items="Nested Right&#10;Nested Center&#10;Flat&#10;Adjacent"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="Save Button" id="b3d301956f9f6d50" memberName="saveBtn"
              virtualName="" explicitFocusOrder="0" pos="7 8 5.626% 24" buttonText="Save"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Load Button" id="7de794bf7f813f4c" memberName="loadBtn"
              virtualName="" explicitFocusOrder="0" pos="7.325% 8 48 24" buttonText="Load"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Mapping Style Label" id="27d88f1ce2d645c1" memberName="mapStyleLbl"
         virtualName="" explicitFocusOrder="0" pos="59c -40 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mapping Style:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="Highlight Style Box" id="ddae7c4ef2fca8a5" memberName="highlightStyleBox"
            virtualName="" explicitFocusOrder="0" pos="108.75% 10Rr 13.694% 24"
            posRelativeX="292b32e0c6cd0b80" editable="0" layout="33" items="Full Key&#10;Inside&#10;Border&#10;Circles&#10;Squares"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <VIEWPORT name="Keyboard Viewport" id="1f2717bdf6633c2" memberName="keyboardViewport"
            virtualName="" explicitFocusOrder="0" pos="7 40 98.301% 83M"
            vscroll="0" hscroll="1" scrollbarThickness="12" contentType="0"
            jucerFile="" contentClass="" constructorParams=""/>
  <TEXTBUTTON name="Settings Button" id="70f30d2c8f0f81a0" memberName="settingsButton"
              virtualName="" explicitFocusOrder="0" pos="13.8% 8 8.599% 24"
              buttonText="Settings" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Map Order Edit Button" id="707c2bf20e6d5e6c" memberName="mapOrderEditBtn"
              virtualName="" explicitFocusOrder="0" pos="383r -40 96 24" buttonText="Edit Mapping"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="Mapping Node Box" id="76c96eb48f8092a" memberName="mapModeBox"
            virtualName="" explicitFocusOrder="0" pos="119 -32 15.817% 24"
            editable="0" layout="33" items="Mapping Off&#10;Auto Map&#10;Manual Map"
            textWhenNonSelected="Mapping Off" textWhenNoItems="Mapping Off"/>
  <TEXTBUTTON name="new button" id="72fd594fae3c08" memberName="mapApplyBtn"
              virtualName="" explicitFocusOrder="0" pos="391 -40 55 24" buttonText="Apply"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="Scale Text Box" id="39f9f4bff4e94802" memberName="scaleTextBox"
              virtualName="" explicitFocusOrder="0" pos="53.185%r 8 25.796% 24"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <GENERICCOMPONENT name="SizeToggleButton" id="7efc0bd141a0ba0d" memberName="sizeToggleBtn"
                    virtualName="" explicitFocusOrder="0" pos="98.301%r 8 2.442% 24"
                    class="Component" params=""/>
  <SLIDER name="TransposeSlider" id="bca2f93853f7efd8" memberName="transposeSld"
          virtualName="" explicitFocusOrder="0" pos="70.594%r 8 11.253% 24"
          min="-128.0" max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

