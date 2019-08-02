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

    scaleTextBox.reset (new TextEditor ("Scale Text Box"));
    addAndMakeVisible (scaleTextBox.get());
    scaleTextBox->setMultiLine (false);
    scaleTextBox->setReturnKeyStartsNewLine (false);
    scaleTextBox->setReadOnly (false);
    scaleTextBox->setScrollbarsShown (true);
    scaleTextBox->setCaretVisible (true);
    scaleTextBox->setPopupMenuEnabled (true);
    scaleTextBox->setText (String());

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

    mode2ViewBtn.reset (new ToggleButton ("Mode2 View Button"));
    addAndMakeVisible (mode2ViewBtn.get());
    mode2ViewBtn->setButtonText (String());
    mode2ViewBtn->setRadioGroupId (10);
    mode2ViewBtn->addListener (this);
    mode2ViewBtn->setToggleState (true, dontSendNotification);

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
    mapStyleBox->setTextWhenNothingSelected (String());
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
                                     TRANS("MIDI Channel")));
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
                                     TRANS("Period Shift")));
    addAndMakeVisible (periodShiftLbl.get());
    periodShiftLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    periodShiftLbl->setJustificationType (Justification::centredLeft);
    periodShiftLbl->setEditable (false, false, false);
    periodShiftLbl->setColour (TextEditor::textColourId, Colours::black);
    periodShiftLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mapButton.reset (new TextButton ("Map Button"));
    addAndMakeVisible (mapButton.get());
    mapButton->setButtonText (TRANS("GO"));
    mapButton->addListener (this);

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

    saveBtn->setBounds (24, 16, 48, 24);

    loadBtn.reset (new TextButton ("Load Button"));
    addAndMakeVisible (loadBtn.get());
    loadBtn->setButtonText (TRANS("Load"));
    loadBtn->addListener (this);

    loadBtn->setBounds (80, 16, 48, 24);

    exportBtn.reset (new TextButton ("exportButton"));
    addAndMakeVisible (exportBtn.get());
    exportBtn->setButtonText (TRANS("Export"));
    exportBtn->addListener (this);

    exportBtn->setBounds (136, 16, 88, 24);

    autoMapBtn.reset (new TextButton ("AutoMap Button"));
    addAndMakeVisible (autoMapBtn.get());
    autoMapBtn->setButtonText (TRANS("Auto Map"));
    autoMapBtn->addListener (this);

    autoMapBtn->setBounds (24, 48, 104, 24);

    manualMapBtn.reset (new TextButton ("Manual Map Button"));
    addAndMakeVisible (manualMapBtn.get());
    manualMapBtn->setButtonText (TRANS("Manual Map"));
    manualMapBtn->addListener (this);

    manualMapBtn->setBounds (136, 48, 88, 24);

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

    settingsButton.reset (new TextButton ("Settings Button"));
    addAndMakeVisible (settingsButton.get());
    settingsButton->setButtonText (TRANS("Settings"));
    settingsButton->addListener (this);


    //[UserPreSize]
	scaleTextBox->addListener(this);

	autoMapBtn->setClickingTogglesState(true);
	noteNumsBtn->setClickingTogglesState(true);

	// allows for implementing mouseDown() to update the menus
	mode1Box->setInterceptsMouseClicks(false, false);
	mode2Box->setInterceptsMouseClicks(false, false);

    saveMenu.reset(new PopupMenu());
    saveMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::saveMode);
    saveMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::savePresetToFile);

    loadMenu.reset(new PopupMenu());
    loadMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::loadMode);
    loadMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::loadPreset);

    exportMenu.reset(new PopupMenu());
    exportMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::exportReaperMap);
    exportMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::exportAbletonMap);

    //[/UserPreSize]

    setSize (1000, 250);


    //[Constructor] You can add your own custom stuff here..

	// DISABLED BECAUSE NOT IMPLEMENTED
	manualMapBtn->setEnabled(false);
	keyStyleBox->setItemEnabled(2, false);
	keyStyleBox->setItemEnabled(4, false);
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
    scaleTextBox = nullptr;
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
    mapButton = nullptr;
    editColorsBtn = nullptr;
    keyStyleBox = nullptr;
    saveBtn = nullptr;
    loadBtn = nullptr;
    exportBtn = nullptr;
    autoMapBtn = nullptr;
    manualMapBtn = nullptr;
    mapStyleLbl = nullptr;
    highlightStyleBox = nullptr;
    keyboardViewport = nullptr;
    settingsButton = nullptr;


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

    mode1Box->setBounds (getWidth() - 209, 16, 150, 24);
    mode2Box->setBounds (getWidth() - 59 - 150, 48, 150, 24);
    mode1RootSld->setBounds (getWidth() - 301, 16, 79, 24);
    mode2RootSld->setBounds (getWidth() - 301, 48, 79, 24);
    scaleTextBox->setBounds ((getWidth() / 2) + -56 - (207 / 2), 16, 207, 24);
    scaleEntryBtn->setBounds ((getWidth() / 2) + 72 - (31 / 2), 16, 31, 24);
    modeInfoButton->setBounds ((getWidth() / 2) + -181 - (24 / 2), 16, 24, 24);
    periodShiftSld->setBounds (108, getHeight() - 40, 86, 24);
    mode1ViewBtn->setBounds (getWidth() - 57, 16, 31, 24);
    mode2ViewBtn->setBounds (getWidth() - 57, 48, 31, 24);
    mode1RootLbl->setBounds (getWidth() - 333, 16, 32, 24);
    mode2RootLbl->setBounds (getWidth() - 333, 48, 32, 24);
    mapStyleBox->setBounds ((getWidth() / 2) + -29 - (150 / 2), 48, 150, 24);
    midiChannelSld->setBounds (292, getHeight() - 40, 86, 24);
    midiChannelLbl->setBounds (197, getHeight() - 40, 96, 24);
    noteNumsBtn->setBounds (392, getHeight() - 40, 24, 24);
    periodShiftLbl->setBounds (24, getHeight() - 40, 88, 24);
    mapButton->setBounds ((getWidth() / 2) + 72 - (32 / 2), 48, 32, 24);
    editColorsBtn->setBounds (696, getHeight() - 40, 79, 24);
    keyStyleBox->setBounds (432, getHeight() - 40, 136, 24);
    mapStyleLbl->setBounds ((getWidth() / 2) + -152 - (96 / 2), 48, 96, 24);
    highlightStyleBox->setBounds (584, getHeight() - 40, 96, 24);
    keyboardViewport->setBounds (24, 80, proportionOfWidth (0.9380f), getHeight() - 132);
    settingsButton->setBounds (792, getHeight() - 40, 88, 24);
    //[UserResized] Add your own custom resize handling here..

	Component* svk = keyboardViewport->getViewedComponent();
	if (svk)
		svk->setBounds(0, 0, keyboardViewport->getMaximumVisibleWidth(), keyboardViewport->getMaximumVisibleHeight());

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
			appCmdMgr->invokeDirectly(IDs::CommandIDs::viewMode1, true);
        //[/UserButtonCode_mode1ViewBtn]
    }
    else if (buttonThatWasClicked == mode2ViewBtn.get())
    {
        //[UserButtonCode_mode2ViewBtn] -- add your button handler code here..
		if (mode2ViewBtn->getToggleState())
			appCmdMgr->invokeDirectly(IDs::CommandIDs::viewMode2, true);
        //[/UserButtonCode_mode2ViewBtn]
    }
    else if (buttonThatWasClicked == noteNumsBtn.get())
    {
        //[UserButtonCode_noteNumsBtn] -- add your button handler code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::showMidiNoteNumbers, true);
        //[/UserButtonCode_noteNumsBtn]
    }
    else if (buttonThatWasClicked == mapButton.get())
    {
        //[UserButtonCode_mapButton] -- add your button handler code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::applyMapping, true);
        //[/UserButtonCode_mapButton]
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
        saveMenu->showAt(saveBtn.get());
        //[/UserButtonCode_saveBtn]
    }
    else if (buttonThatWasClicked == loadBtn.get())
    {
        //[UserButtonCode_loadBtn] -- add your button handler code here..
		//appCmdMgr->invokeDirectly(IDs::CommandIDs::loadPreset, true);
        loadMenu->showAt(loadBtn.get());
        //[/UserButtonCode_loadBtn]
    }
    else if (buttonThatWasClicked == exportBtn.get())
    {
        //[UserButtonCode_exportBtn] -- add your button handler code here..
		//appCmdMgr->invokeDirectly(IDs::CommandIDs::exportPreset, true);
        exportMenu->showAt(exportBtn.get());
        //[/UserButtonCode_exportBtn]
    }
    else if (buttonThatWasClicked == autoMapBtn.get())
    {
        //[UserButtonCode_autoMapBtn] -- add your button handler code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setAutoMap, true);
        //[/UserButtonCode_autoMapBtn]
    }
    else if (buttonThatWasClicked == manualMapBtn.get())
    {
        //[UserButtonCode_manualMapBtn] -- add your button handler code here..
		appCmdMgr->invokeDirectly(IDs::CommandIDs::beginMapEditing, true);
        //[/UserButtonCode_manualMapBtn]
    }
    else if (buttonThatWasClicked == settingsButton.get())
    {
        //[UserButtonCode_settingsButton] -- add your button handler code here..
        appCmdMgr->invokeDirectly(IDs::CommandIDs::showSettingsDialog, true);
        //[/UserButtonCode_settingsButton]
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

bool PluginControlComponent::getAutoMapState()
{
	return autoMapBtn->getToggleState();
}

void PluginControlComponent::setAutoMapState(bool isAutoMapping, NotificationType notify)
{
	autoMapBtn->setToggleState(isAutoMapping, notify);
}

int PluginControlComponent::getMappingStyle()
{
	return mapStyleBox->getSelectedId();
}

void PluginControlComponent::setMappingStyleId(int idIn, NotificationType notify)
{
	mapStyleBox->setSelectedId(idIn, notify);
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
            virtualName="ReferencedComboBox" explicitFocusOrder="0" pos="209R 16 150 24"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="Mode2 Box" id="e43718c6ce3f175" memberName="mode2Box" virtualName="ReferencedComboBox"
            explicitFocusOrder="0" pos="59Rr 48 150 24" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="Mode1 Root Slider" id="e732568ad188d067" memberName="mode1RootSld"
          virtualName="" explicitFocusOrder="0" pos="301R 16 79 24" min="0.0"
          max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="mode2RootSld" id="ae29d1ebf57b7bd7" memberName="mode2RootSld"
          virtualName="" explicitFocusOrder="0" pos="301R 48 79 24" min="0.0"
          max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTEDITOR name="Scale Text Box" id="39f9f4bff4e94802" memberName="scaleTextBox"
              virtualName="" explicitFocusOrder="0" pos="-56.5Cc 16 207 24"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="Scale Entry Button" id="bbb112b96c51ecf7" memberName="scaleEntryBtn"
              virtualName="" explicitFocusOrder="0" pos="72.5Cc 16 31 24" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Mode Info Button" id="c6025abb0c68f4" memberName="modeInfoButton"
              virtualName="" explicitFocusOrder="0" pos="-181Cc 16 24 24" buttonText="i"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="Period Shift Slider" id="89742a2afd3a3325" memberName="periodShiftSld"
          virtualName="" explicitFocusOrder="0" pos="108 40R 86 24" min="-10.0"
          max="10.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="Mode1 View Button" id="bb818484022dae8c" memberName="mode1ViewBtn"
                virtualName="" explicitFocusOrder="0" pos="57R 16 31 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="10" state="0"/>
  <TOGGLEBUTTON name="Mode2 View Button" id="a8183216fad07ec6" memberName="mode2ViewBtn"
                virtualName="" explicitFocusOrder="0" pos="57R 48 31 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="10" state="1"/>
  <LABEL name="Mode1 Root Label" id="2df91047cc98faf4" memberName="mode1RootLbl"
         virtualName="" explicitFocusOrder="0" pos="333R 16 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="C4" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Mode 2 Root Label" id="c21534178bfcff74" memberName="mode2RootLbl"
         virtualName="" explicitFocusOrder="0" pos="333R 48 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="F5" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="Mapping Style Box" id="217ff7f7b6934ebe" memberName="mapStyleBox"
            virtualName="" explicitFocusOrder="0" pos="-29Cc 48 150 24" editable="0"
            layout="33" items="Mode To Mode&#10;Scale To Mode&#10;By Orders"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="Midi Channel Slider" id="4f2d4d8b9051247f" memberName="midiChannelSld"
          virtualName="" explicitFocusOrder="0" pos="292 40R 86 24" min="1.0"
          max="16.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Midi Channel Label" id="d2587185be475d9c" memberName="midiChannelLbl"
         virtualName="" explicitFocusOrder="0" pos="197 40R 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI Channel" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Note Numbers Button" id="405a79d645f5f8ac" memberName="noteNumsBtn"
              virtualName="" explicitFocusOrder="0" pos="392 40R 24 24" buttonText="#"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Period Shift Label" id="140764aae2291735" memberName="periodShiftLbl"
         virtualName="" explicitFocusOrder="0" pos="24 40R 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Period Shift" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Map Button" id="22ef34e38fed1212" memberName="mapButton"
              virtualName="" explicitFocusOrder="0" pos="72Cc 48 32 24" buttonText="GO"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Edit Colors Button" id="bc86137b1b6a5176" memberName="editColorsBtn"
              virtualName="" explicitFocusOrder="0" pos="696 40R 79 24" buttonText="Edit Colors"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="Key Style Box" id="292b32e0c6cd0b80" memberName="keyStyleBox"
            virtualName="" explicitFocusOrder="0" pos="432 40R 136 24" editable="0"
            layout="33" items="Nested Right&#10;Nested Center&#10;Flat&#10;Adjacent"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="Save Button" id="b3d301956f9f6d50" memberName="saveBtn"
              virtualName="" explicitFocusOrder="0" pos="24 16 48 24" buttonText="Save"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Load Button" id="7de794bf7f813f4c" memberName="loadBtn"
              virtualName="" explicitFocusOrder="0" pos="80 16 48 24" buttonText="Load"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="exportButton" id="7db05d91d674ecfe" memberName="exportBtn"
              virtualName="" explicitFocusOrder="0" pos="136 16 88 24" buttonText="Export"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="AutoMap Button" id="d0adb9765a43b7e2" memberName="autoMapBtn"
              virtualName="" explicitFocusOrder="0" pos="24 48 104 24" buttonText="Auto Map"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Manual Map Button" id="6ead157214bd613f" memberName="manualMapBtn"
              virtualName="" explicitFocusOrder="0" pos="136 48 88 24" buttonText="Manual Map"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Mapping Style Label" id="27d88f1ce2d645c1" memberName="mapStyleLbl"
         virtualName="" explicitFocusOrder="0" pos="-152Cc 48 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mapping Style:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="Highlight Style Box" id="ddae7c4ef2fca8a5" memberName="highlightStyleBox"
            virtualName="" explicitFocusOrder="0" pos="584 40R 96 24" editable="0"
            layout="33" items="Full Key&#10;Inside&#10;Border&#10;Circles&#10;Squares"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <VIEWPORT name="Keyboard Viewport" id="1f2717bdf6633c2" memberName="keyboardViewport"
            virtualName="" explicitFocusOrder="0" pos="24 80 93.812% 132M"
            vscroll="0" hscroll="1" scrollbarThickness="8" contentType="0"
            jucerFile="" contentClass="" constructorParams=""/>
  <TEXTBUTTON name="Settings Button" id="70f30d2c8f0f81a0" memberName="settingsButton"
              virtualName="" explicitFocusOrder="0" pos="792 40R 88 24" buttonText="Settings"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

