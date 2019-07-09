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

#include "KeyboardEditorBar.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
KeyboardEditorBar::KeyboardEditorBar (SvkPluginState* pluginStateIn, ApplicationCommandManager* managerIn)
    : pluginState(pluginStateIn), appCmdMgr(managerIn)
{
    //[Constructor_pre] You can add your own custom stuff here..

    //[/Constructor_pre]

    setName ("Keyboard Editor Bar");
    modeTextEditor.reset (new TextEditor ("Custom Mode Entry"));
    addAndMakeVisible (modeTextEditor.get());
    modeTextEditor->setMultiLine (false);
    modeTextEditor->setReturnKeyStartsNewLine (false);
    modeTextEditor->setReadOnly (false);
    modeTextEditor->setScrollbarsShown (true);
    modeTextEditor->setCaretVisible (true);
    modeTextEditor->setPopupMenuEnabled (true);
    modeTextEditor->setText (TRANS("2 2 1 2 2 2 1"));

    sendScaleBtn.reset (new TextButton ("Send Scale Button"));
    addAndMakeVisible (sendScaleBtn.get());
    sendScaleBtn->setButtonText (TRANS("Send Scale"));
    sendScaleBtn->addListener (this);

    offsetSld.reset (new Slider ("Offset Slider"));
    addAndMakeVisible (offsetSld.get());
    offsetSld->setRange (0, 127, 1);
    offsetSld->setSliderStyle (Slider::IncDecButtons);
    offsetSld->setTextBoxStyle (Slider::TextBoxLeft, false, 60, 20);
    offsetSld->addListener (this);

    offsetLabel.reset (new Label ("Offset Label",
                                  TRANS("Root:")));
    addAndMakeVisible (offsetLabel.get());
    offsetLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    offsetLabel->setJustificationType (Justification::centredLeft);
    offsetLabel->setEditable (false, false, false);
    offsetLabel->setColour (TextEditor::textColourId, Colours::black);
    offsetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mapButton.reset (new TextButton ("Map Notes Button"));
    addAndMakeVisible (mapButton.get());
    mapButton->setButtonText (TRANS("Edit Mapping"));
    mapButton->setConnectedEdges (Button::ConnectedOnBottom);
    mapButton->addListener (this);
    mapButton->setColour (TextButton::buttonColourId, Colour (0xff5c6ea4));
    mapButton->setColour (TextButton::buttonOnColourId, Colour (0xffa7b438));

    presetLibraryBox.reset (new ReferencedComboBox ("Preset Box Main"));
    addAndMakeVisible (presetLibraryBox.get());
    presetLibraryBox->setName ("PresetLibraryBox");

    periodTransposeSld.reset (new Slider ("Transpose Period Slider"));
    addAndMakeVisible (periodTransposeSld.get());
    periodTransposeSld->setRange (-10, 10, 1);
    periodTransposeSld->setSliderStyle (Slider::IncDecButtons);
    periodTransposeSld->setTextBoxStyle (Slider::TextBoxLeft, false, 60, 20);
    periodTransposeSld->addListener (this);

    shiftLabel.reset (new Label ("Shift Label",
                                 TRANS("Shift:")));
    addAndMakeVisible (shiftLabel.get());
    shiftLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    shiftLabel->setJustificationType (Justification::centredLeft);
    shiftLabel->setEditable (false, false, false);
    shiftLabel->setColour (TextEditor::textColourId, Colours::black);
    shiftLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
	mapButton->setVisible(false);

    presetLibraryBox->setMenu(*pluginState->presetManager->getPresetMenu());
    presetLibraryBox->setEditableText(false);
	presetLibraryBox->addListener(this);

	pianoMenu.reset(new KeyboardMenu(appCmdMgr));
	addAndMakeVisible(pianoMenu.get());
    pianoMenu->toBack();

	modeTextEditor->setInputFilter(pluginState->textFilterIntOrSpace.get(), false);
    modeTextEditor->setMouseClickGrabsKeyboardFocus(true);
	modeTextEditor->addListener(this);

    offsetSld->setValue(pluginState->getMidiProcessor()->getRootNote(), dontSendNotification);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    mapButton->setClickingTogglesState(true);
    //[/Constructor]
}

KeyboardEditorBar::~KeyboardEditorBar()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	presetLibraryBox->removeListener(this);
    //[/Destructor_pre]

    modeTextEditor = nullptr;
    sendScaleBtn = nullptr;
    offsetSld = nullptr;
    offsetLabel = nullptr;
    mapButton = nullptr;
    presetLibraryBox = nullptr;
    periodTransposeSld = nullptr;
    shiftLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void KeyboardEditorBar::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void KeyboardEditorBar::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	pianoMenu->setBounds(1, 0, getWidth(), getHeight()-1);
    //[/UserPreResize]

    modeTextEditor->setBounds (getWidth() - 560 - 150, 1, 150, 24);
    sendScaleBtn->setBounds (getWidth() - 464 - 88, 1, 88, 24);
    offsetSld->setBounds (getWidth() - 272, 1, 115, 24);
    offsetLabel->setBounds ((getWidth() - 272) + -40, 1 + roundToInt (24 * 0.5000f) - (24 / 2), 47, 24);
    mapButton->setBounds (getWidth() - 717 - 106, 1, 106, 24);
    presetLibraryBox->setBounds (getWidth() - 5 - 150, 1, 150, 24);
    periodTransposeSld->setBounds ((getWidth() - 464 - 88) + 88 - -155 - 115, 1, 115, 24);
    shiftLabel->setBounds (((getWidth() - 464 - 88) + 88 - -155 - 115) + -39, 1 + roundToInt (24 * 0.5000f) - (24 / 2), 47, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void KeyboardEditorBar::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == sendScaleBtn.get())
    {
        //[UserButtonCode_sendScaleBtn] -- add your button handler code here..

		// check to see if mode matches a preset
		createAndSendMode();
        //[/UserButtonCode_sendScaleBtn]
    }
    else if (buttonThatWasClicked == mapButton.get())
    {
        //[UserButtonCode_mapButton] -- add your button handler code here..
        sendChangeMessage();
        //[/UserButtonCode_mapButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void KeyboardEditorBar::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == offsetSld.get())
    {
        //[UserSliderCode_offsetSld] -- add your slider handling code here..
		pluginState->setMidiRootNote((int)offsetSld->getValue());
		sendChangeMessage();
        //[/UserSliderCode_offsetSld]
    }
    else if (sliderThatWasMoved == periodTransposeSld.get())
    {
        //[UserSliderCode_periodTransposeSld] -- add your slider handling code here..
        pluginState->getMidiProcessor()->setPeriodShift(periodTransposeSld->getValue());
        //[/UserSliderCode_periodTransposeSld]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void KeyboardEditorBar::mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel)
{
    //[UserCode_mouseWheelMove] -- Add your code here...
	if (presetLibraryBox->isMouseOver())
	{
		presetLibraryBox->setSelectedId(presetLibraryBox->getSelectedId() + (wheel.isReversed * 2 - 1));
	}
    //[/UserCode_mouseWheelMove]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void KeyboardEditorBar::textEditorReturnKeyPressed(TextEditor& editor)
{
	createAndSendMode();
}

int KeyboardEditorBar::getModePresetIndex(String anyNameIn)
{
    return menuToPresetIndex[anyNameIn];
}

int KeyboardEditorBar::getModeLibraryIndex()
{
    return presetLibraryBox->getSelectedId();
}

bool KeyboardEditorBar::isMapButtonOn()
{
    return mapButton->getToggleState();
}

void KeyboardEditorBar::setOffsetReadout(int offIn)
{
    offsetSld->setValue(offIn, dontSendNotification);
}

int KeyboardEditorBar::getOffsetReadout()
{
	return offsetSld->getValue();
}

int KeyboardEditorBar::getPresetSelectedId()
{
	return presetLibraryBox->getSelectedId();
}


void KeyboardEditorBar::setModeReadoutText(String steps)
{
	modeTextEditor->setText(steps, false);
}

void KeyboardEditorBar::setModeLibraryText(String presetName)
{
	presetLibraryBox->setText(presetName, dontSendNotification);
}

void KeyboardEditorBar::createAndSendMode()
{
	String steps = modeTextEditor->getText();
	pluginState->loadMode(Mode::createNode(steps, "Custom"));
}

void KeyboardEditorBar::allowUserInput(bool isAllowed)
{
 	presetLibraryBox->setEnabled(isAllowed);
	offsetSld->setEnabled(isAllowed);
	sendScaleBtn->setEnabled(isAllowed);
	modeTextEditor->setEnabled(isAllowed);
	pianoMenu->setEnabled(isAllowed);
}

void KeyboardEditorBar::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{

    if (comboBoxThatHasChanged)
    {
        int id = comboBoxThatHasChanged->getSelectedId() - 1;

        if (id != pluginState->presetManager->getPresetLoadedId())
        {
            pluginState->loadMode(id);

            if (pluginState->getMidiProcessor()->isAutoRemapping())
            {
                pluginState->getMidiProcessor()->setMidiInputMap(ModeMapper::stdMidiToMode(*pluginState->getModeLoaded()));
            }
        }
    }
}


//==============================================================================



KeyboardEditorBar::KeyboardMenu::KeyboardMenu(ApplicationCommandManager* managerIn)
{
	setName("Menu Bar");

	menuParent.reset(new MenuBarComponent(this));
	addAndMakeVisible(menuParent.get());

	appCmdMgr = managerIn;
	setApplicationCommandManagerToWatch(appCmdMgr);

	setSize(500, 500);
}

KeyboardEditorBar::KeyboardMenu::~KeyboardMenu()
{
}


void KeyboardEditorBar::KeyboardMenu::resized()
{
	menuParent->setBounds(0, 0, getParentWidth(), getParentHeight()*0.8f);
}
//==============================================================================


StringArray KeyboardEditorBar::KeyboardMenu::getMenuBarNames()
{
	return { "File", "Edit", "View" };
}

void KeyboardEditorBar::KeyboardMenu::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{

}

PopupMenu KeyboardEditorBar::KeyboardMenu::getMenuForIndex(int topLevelMenuIndex, const String &menuName)
{
	PopupMenu menu;
	if (topLevelMenuIndex == 0)
	{
        menu.addCommandItem(appCmdMgr, IDs::CommandIDs::savePreset, "Save layout");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::loadPreset, "Load layout");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::saveReaperMap, "Export to Reaper MIDI note names file");
	}
	else if (topLevelMenuIndex == 1)
	{
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::beginColorEditing, "Set default Key Colors");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setMode2RootNote, "Set layout offset");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::pianoPlayMode, "Play Mode");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::pianoEditMode, "Edit Mode");
        menu.addCommandItem(appCmdMgr, IDs::CommandIDs::remapMidiNotes, "Edit Note Mapping");
        menu.addCommandItem(appCmdMgr, IDs::CommandIDs::showModeInfo, "Mode Info");
        menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setAutoRemapState, "Auto-Remap From Standard Layout");
	}
	else if (topLevelMenuIndex == 2)
	{
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setPianoHorizontal, "Horizontal Keyboard");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setPianoVerticalL, "Vertical Keyboard Left");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setPianoVerticalR, "Vertical Keyboard Right");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setKeysNestedRight, "Cascade Keys");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setKeysFlat, "Flat Keys");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setKeysAdjacent, "Adjacent Keys");
	}

	return menu;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="KeyboardEditorBar" componentName="Keyboard Editor Bar"
                 parentClasses="public Component, public ChangeBroadcaster, public TextEditor::Listener, public ComboBox::Listener"
                 constructorParams="SvkPluginState* pluginStateIn, ApplicationCommandManager* managerIn"
                 variableInitialisers="pluginState(pluginStateIn), appCmdMgr(managerIn)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseWheelMove (const MouseEvent&amp; e, const MouseWheelDetails&amp; wheel)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="Custom Mode Entry" id="8c559f3dc17dcbb0" memberName="modeTextEditor"
              virtualName="" explicitFocusOrder="0" pos="560Rr 1 150 24" initialText="2 2 1 2 2 2 1"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="Send Scale Button" id="3a2872f3357f900b" memberName="sendScaleBtn"
              virtualName="" explicitFocusOrder="0" pos="464Rr 1 88 24" buttonText="Send Scale"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="Offset Slider" id="c1c294edca92ea2f" memberName="offsetSld"
          virtualName="" explicitFocusOrder="0" pos="272R 1 115 24" min="0.0"
          max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Offset Label" id="1389380960314b49" memberName="offsetLabel"
         virtualName="" explicitFocusOrder="0" pos="-40 50%c 47 24" posRelativeX="c1c294edca92ea2f"
         posRelativeY="c1c294edca92ea2f" edTextCol="ff000000" edBkgCol="0"
         labelText="Root:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Map Notes Button" id="bd06ada115b52b19" memberName="mapButton"
              virtualName="" explicitFocusOrder="0" pos="717Rr 1 106 24" bgColOff="ff5c6ea4"
              bgColOn="ffa7b438" buttonText="Edit Mapping" connectedEdges="8"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="PresetLibraryBox" id="f920735f425400ca" memberName="presetLibraryBox"
                    virtualName="ReferencedComboBox" explicitFocusOrder="0" pos="5Rr 1 150 24"
                    class="ComboBox" params="&quot;Preset Box Main&quot;"/>
  <SLIDER name="Transpose Period Slider" id="98a42ddab038f13" memberName="periodTransposeSld"
          virtualName="" explicitFocusOrder="0" pos="-155Rr 1 115 24" posRelativeX="3a2872f3357f900b"
          min="-10.0" max="10.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Shift Label" id="80d521f3eec02525" memberName="shiftLabel"
         virtualName="" explicitFocusOrder="0" pos="-39 50%c 47 24" posRelativeX="98a42ddab038f13"
         posRelativeY="98a42ddab038f13" edTextCol="ff000000" edBkgCol="0"
         labelText="Shift:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

