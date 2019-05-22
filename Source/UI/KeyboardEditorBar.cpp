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
KeyboardEditorBar::KeyboardEditorBar (SuperVirtualKeyboardPluginState* pluginStateIn, ApplicationCommandManager* managerIn)
    : pluginState(pluginStateIn), appCmdMgr(managerIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
	presets = pluginState->get_presets();
	presetsSorted = pluginState->get_presets_sorted();
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

    modeLibraryBox.reset (new ComboBox ("Mode Library Box"));
    addAndMakeVisible (modeLibraryBox.get());
    modeLibraryBox->setEditableText (true);
    modeLibraryBox->setJustificationType (Justification::centredLeft);
    modeLibraryBox->setTextWhenNothingSelected (TRANS("Pick a mode..."));
    modeLibraryBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    modeLibraryBox->addListener (this);

    offsetSld.reset (new Slider ("Offset Slider"));
    addAndMakeVisible (offsetSld.get());
    offsetSld->setRange (0, 127, 1);
    offsetSld->setSliderStyle (Slider::IncDecButtons);
    offsetSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
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
    mapButton->setButtonText (TRANS("Map Notes"));
    mapButton->setConnectedEdges (Button::ConnectedOnBottom);
    mapButton->addListener (this);
    mapButton->setColour (TextButton::buttonColourId, Colour (0xff5c6ea4));
    mapButton->setColour (TextButton::buttonOnColourId, Colour (0xffa7b438));


    //[UserPreSize]
	pianoMenu.reset(new KeyboardMenu(appCmdMgr));
	addAndMakeVisible(pianoMenu.get());
	pianoMenu->toBack();
    
    modeTextEditor->setMouseClickGrabsKeyboardFocus(true);
	modeTextEditor->addListener(this);
	
    offsetSld->setValue(pluginState->getMidiProcessor()->getRootNote(), dontSendNotification);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	populate_preset_menu();
    mapButton->setClickingTogglesState(true);
    //[/Constructor]
}

KeyboardEditorBar::~KeyboardEditorBar()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    modeTextEditor = nullptr;
    sendScaleBtn = nullptr;
    modeLibraryBox = nullptr;
    offsetSld = nullptr;
    offsetLabel = nullptr;
    mapButton = nullptr;


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
	pianoMenu->setBounds(1, 5, proportionOfWidth(1.0f), 24);
    //[/UserPreResize]

    modeTextEditor->setBounds (getWidth() - 381 - 150, 6, 150, 24);
    sendScaleBtn->setBounds (getWidth() - 283 - 88, 6, 88, 24);
    modeLibraryBox->setBounds (getWidth() - 4 - 150, 6, 150, 24);
    offsetSld->setBounds (getWidth() - 235, 2, 69, 32);
    offsetLabel->setBounds (getWidth() - 280, 6, 47, 24);
    mapButton->setBounds (getWidth() - 542 - 79, 6, 79, 24);
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

void KeyboardEditorBar::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == modeLibraryBox.get())
    {
        //[UserComboBoxCode_modeLibraryBox] -- add your combo box handling code here..
		if (comboBoxThatHasChanged->getSelectedId() > 0)
			pluginState->setCurrentMode(menuToPresetIndex[modeLibraryBox->getText()]);
		else
		{
			String newModeName;
			// Preset is a user preset
			if ((int)pluginState->getCurrentPreset()->parentNode.getProperty(IDs::libraryIndexOfMode) == 0)
			{
				pluginState->getCurrentMode()->setFamily(comboBoxThatHasChanged->getText());
			}

			// If it's a default preset, it reverts to original name
			newModeName = pluginState->getCurrentMode()->getDescription();
			comboBoxThatHasChanged->setText(newModeName, dontSendNotification);
		}
        //[/UserComboBoxCode_modeLibraryBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void KeyboardEditorBar::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == offsetSld.get())
    {
        //[UserSliderCode_offsetSld] -- add your slider handling code here..
		pluginState->getMidiProcessor()->setRootNote((int)offsetSld->getValue());
		sendChangeMessage();
        //[/UserSliderCode_offsetSld]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void KeyboardEditorBar::mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel)
{
    //[UserCode_mouseWheelMove] -- Add your code here...
	if (modeLibraryBox->isMouseOver())
	{
		modeLibraryBox->setSelectedId(modeLibraryBox->getSelectedId() + (wheel.isReversed * 2 - 1));
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
    return modeLibraryBox->getSelectedId();
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

void KeyboardEditorBar::setModeReadoutText(String steps)
{
	modeTextEditor->setText(steps, false);
}

void KeyboardEditorBar::setModeLibraryText(String presetName)
{
	modeLibraryBox->setText(presetName, dontSendNotification);
}

void KeyboardEditorBar::createAndSendMode()
{
	String steps = modeTextEditor->getText();
	int index = pluginState->is_mode_in_presets(steps);

	if (index)
		pluginState->setCurrentMode(index);
	else
		pluginState->setCurrentMode(new Mode(steps, "Custom"));
}

void KeyboardEditorBar::allowUserInput(bool isAllowed)
{
 	modeLibraryBox->setEnabled(isAllowed);
	offsetSld->setEnabled(isAllowed);
	sendScaleBtn->setEnabled(isAllowed);
	modeTextEditor->setEnabled(isAllowed);
	pianoMenu->setEnabled(isAllowed);
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
	menuParent->setBounds(0, 0, 350, LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
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
        menu.addCommandItem(appCmdMgr, IDs::CommandIDs::saveCustomLayout, "Save layout");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::loadCustomLayout, "Load layout");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::saveReaperMap, "Export to Reaper MIDI note names file");
	}
	else if (topLevelMenuIndex == 1)
	{
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setKeyColor, "Set default Key Colors");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setMidiNoteOffset, "Set layout offset");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::pianoPlayMode, "Play Mode");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::pianoEditMode, "Edit Mode");
	}
	else if (topLevelMenuIndex == 2)
	{
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setPianoHorizontal, "Horizontal Keyboard");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setPianoVerticalL, "Vertical Keyboard Left");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setPianoVerticalR, "Vertical Keyboard Right");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setKeysNestedRight, "Cascade Keys");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setKeysNestedCenter, "Flat Keys");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::setKeysAdjacent, "Adjacent Keys");
	}

	return menu;
}

//==============================================================================

void KeyboardEditorBar::populate_preset_menu()
{
	modeLibraryBox->clear();
	menuToPresetIndex.clear();

	menuSortByScale.reset(new PopupMenu());
	menuSortByMode.reset(new PopupMenu());
	menuSortByFamily.reset(new PopupMenu());

	Mode* mode;
	String name;
	String p_name;
	int presetIndex;
	int size = 0;
	int p_size = 0;
	int c = 1;

	for (int i = 0; i < presets->size() - 1; i++)
	{
		mode = presetsSorted->getUnchecked(SortType::scaleSizeSort).getUnchecked(i);
		name = mode->getScaleDescription();
		presetIndex = presets->indexOf(mode);

		size = mode->getScaleSize();
		if (i > 0 && p_size != size)
		{
			menuSortByScale.get()->addSeparator();
			p_size = size;
		}

		menuSortByScale.get()->addItem(c++, name);
		menuToPresetIndex.set(name, presetIndex);
	}

	for (int i = 0; i < presets->size() - 1; i++)
	{
		mode = presetsSorted->getUnchecked(SortType::modeSizeSort).getUnchecked(i);
		name = mode->getModeDescription();
		presetIndex = presets->indexOf(mode);

		size = mode->getModeSize();
		if (i > 0 && p_size != size)
		{
			menuSortByMode.get()->addSeparator();
			p_size = size;
		}

		menuSortByMode.get()->addItem(c++, name);
		menuToPresetIndex.set(name, presetIndex);
	}

	for (int i = 0; i < presets->size() - 1; i++)
	{
		mode = presetsSorted->getUnchecked(SortType::familySort)[i];
		name = mode->getDescription();
		presetIndex = presets->indexOf(mode);

		if (i > 0 && p_name != mode->getFamily())
		{
			menuSortByFamily.get()->addSeparator();
			p_name = mode->getFamily();
		}

		menuSortByFamily.get()->addItem(c++, name);
		menuToPresetIndex.set(name, presetIndex);
	}

	modeLibraryBox->getRootMenu()->addSubMenu("by Scale Size", *menuSortByScale.get());
	modeLibraryBox->getRootMenu()->addSubMenu("by Mode Size", *menuSortByMode.get());
	modeLibraryBox->getRootMenu()->addSubMenu("by Family", *menuSortByFamily.get());
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="KeyboardEditorBar" componentName="Keyboard Editor Bar"
                 parentClasses="public Component, private TextEditor::Listener, public ChangeBroadcaster"
                 constructorParams="SuperVirtualKeyboardPluginState* pluginStateIn, ApplicationCommandManager* managerIn"
                 variableInitialisers="pluginState(pluginStateIn), appCmdMgr(managerIn)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseWheelMove (const MouseEvent&amp; e, const MouseWheelDetails&amp; wheel)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="Custom Mode Entry" id="8c559f3dc17dcbb0" memberName="modeTextEditor"
              virtualName="" explicitFocusOrder="0" pos="381Rr 6 150 24" initialText="2 2 1 2 2 2 1"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="Send Scale Button" id="3a2872f3357f900b" memberName="sendScaleBtn"
              virtualName="" explicitFocusOrder="0" pos="283Rr 6 88 24" buttonText="Send Scale"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="Mode Library Box" id="91d2066d9e23de1c" memberName="modeLibraryBox"
            virtualName="" explicitFocusOrder="0" pos="4Rr 6 150 24" editable="1"
            layout="33" items="" textWhenNonSelected="Pick a mode..." textWhenNoItems="(no choices)"/>
  <SLIDER name="Offset Slider" id="c1c294edca92ea2f" memberName="offsetSld"
          virtualName="" explicitFocusOrder="0" pos="235R 2 69 32" min="0.0"
          max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Offset Label" id="1389380960314b49" memberName="offsetLabel"
         virtualName="" explicitFocusOrder="0" pos="280R 6 47 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Root:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Map Notes Button" id="bd06ada115b52b19" memberName="mapButton"
              virtualName="" explicitFocusOrder="0" pos="542Rr 6 79 24" bgColOff="ff5c6ea4"
              bgColOn="ffa7b438" buttonText="Map Notes" connectedEdges="8"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

