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
    modeLibraryBox->setEditableText (false);
    modeLibraryBox->setJustificationType (Justification::centredLeft);
    modeLibraryBox->setTextWhenNothingSelected (TRANS("Pick a mode..."));
    modeLibraryBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    modeLibraryBox->addListener (this);

    keyboardModeBtn.reset (new TextButton ("Keyboard Mode Button"));
    addAndMakeVisible (keyboardModeBtn.get());
    keyboardModeBtn->setButtonText (TRANS("Edit"));
    keyboardModeBtn->addListener (this);
    keyboardModeBtn->setColour (TextButton::buttonColourId, Colour (0xff5c7fa4));

    offsetSld.reset (new Slider ("Offset Slider"));
    addAndMakeVisible (offsetSld.get());
    offsetSld->setRange (-60, 67, 1);
    offsetSld->setSliderStyle (Slider::IncDecButtons);
    offsetSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    offsetSld->addListener (this);

    offsetLabel.reset (new Label ("Offset Label",
                                  TRANS("Offset:")));
    addAndMakeVisible (offsetLabel.get());
    offsetLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    offsetLabel->setJustificationType (Justification::centredLeft);
    offsetLabel->setEditable (false, false, false);
    offsetLabel->setColour (TextEditor::textColourId, Colours::black);
    offsetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
	pianoMenu.reset(new KeyboardMenu(appCmdMgr));
	addAndMakeVisible(pianoMenu.get());
	pianoMenu->toBack();
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	populate_preset_menu();
    //[/Constructor]
}

KeyboardEditorBar::~KeyboardEditorBar()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    modeTextEditor = nullptr;
    sendScaleBtn = nullptr;
    modeLibraryBox = nullptr;
    keyboardModeBtn = nullptr;
    offsetSld = nullptr;
    offsetLabel = nullptr;


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

    modeTextEditor->setBounds ((proportionOfWidth (0.9927f) - 40) + roundToInt (40 * -9.7000f) - 150, 6 + 0, 150, 24);
    sendScaleBtn->setBounds ((proportionOfWidth (0.9927f) - 40) + roundToInt (40 * -7.2000f) - 88, 6 + 0, 88, 24);
    modeLibraryBox->setBounds ((proportionOfWidth (0.9927f) - 40) + roundToInt (40 * -0.3000f) - 150, 6 + 0, 150, 24);
    keyboardModeBtn->setBounds (proportionOfWidth (0.9927f) - 40, 6, 40, 24);
    offsetSld->setBounds ((proportionOfWidth (0.9927f) - 40) + roundToInt (40 * -5.9500f), 2, 69, 32);
    offsetLabel->setBounds ((proportionOfWidth (0.9927f) - 40) + roundToInt (40 * -7.0750f), 6, 47, 24);
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
		create_and_send_mode();
        //[/UserButtonCode_sendScaleBtn]
    }
    else if (buttonThatWasClicked == keyboardModeBtn.get())
    {
        //[UserButtonCode_keyboardModeBtn] -- add your button handler code here..
		appCmdMgr->invoke(IDs::CommandIDs::setKeyColor, true);
        //[/UserButtonCode_keyboardModeBtn]
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
		pluginState->set_current_mode(menuToPresetIndex[modeLibraryBox->getText()]);
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
        pluginState->get_current_mode()->setOffset((int) offsetSld->getValue());
        //pluginState->presetCurrentNode = pluginState->get_current_mode()->modeNode;
        pluginState->presetCurrentNode.setProperty(IDs::modeOffset, (int) offsetSld->getValue(), pluginState->get_undo_mgr());
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

bool KeyboardEditorBar::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
	if (!enterDown && KeyPress::isKeyCurrentlyDown(KeyPress::returnKey))
	{
		enterDown = true;
		create_and_send_mode();
		return true;
	}
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}

bool KeyboardEditorBar::keyStateChanged (bool isKeyDown)
{
    //[UserCode_keyStateChanged] -- Add your code here...
	if (enterDown && KeyPress::isKeyCurrentlyDown(KeyPress::returnKey))
	{
		enterDown = false;
		return true;
	}
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyStateChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

int KeyboardEditorBar::get_mode_preset_index(String anyNameIn)
{
    return menuToPresetIndex[anyNameIn];
}

int KeyboardEditorBar::get_mode_library_index()
{
    return modeLibraryBox->getSelectedId();
}

void KeyboardEditorBar::set_mode_readout_text(String steps)
{
	modeTextEditor->setText(steps, false);
}

void KeyboardEditorBar::set_mode_library_text(String presetName)
{
	modeLibraryBox->setText(presetName, NotificationType::dontSendNotification);
}

void KeyboardEditorBar::create_and_send_mode()
{
	String steps = modeTextEditor->getText();
	int index = pluginState->is_mode_in_presets(steps);

	if (index)
		pluginState->set_current_mode(index);
	else
		pluginState->set_current_mode(new Mode(steps, "Custom"));
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
                 parentClasses="public Component" constructorParams="SuperVirtualKeyboardPluginState* pluginStateIn, ApplicationCommandManager* managerIn"
                 variableInitialisers="pluginState(pluginStateIn), appCmdMgr(managerIn)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
    <METHOD name="keyStateChanged (bool isKeyDown)"/>
    <METHOD name="mouseWheelMove (const MouseEvent&amp; e, const MouseWheelDetails&amp; wheel)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="Custom Mode Entry" id="8c559f3dc17dcbb0" memberName="modeTextEditor"
              virtualName="" explicitFocusOrder="0" pos="-970%r 0 150 24" posRelativeX="9f75aa2c0ca39fa4"
              posRelativeY="9f75aa2c0ca39fa4" initialText="2 2 1 2 2 2 1" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="Send Scale Button" id="3a2872f3357f900b" memberName="sendScaleBtn"
              virtualName="" explicitFocusOrder="0" pos="-720%r 0 88 24" posRelativeX="9f75aa2c0ca39fa4"
              posRelativeY="9f75aa2c0ca39fa4" buttonText="Send Scale" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="Mode Library Box" id="91d2066d9e23de1c" memberName="modeLibraryBox"
            virtualName="" explicitFocusOrder="0" pos="-30%r 0 150 24" posRelativeX="9f75aa2c0ca39fa4"
            posRelativeY="9f75aa2c0ca39fa4" editable="0" layout="33" items=""
            textWhenNonSelected="Pick a mode..." textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="Keyboard Mode Button" id="9f75aa2c0ca39fa4" memberName="keyboardModeBtn"
              virtualName="" explicitFocusOrder="0" pos="99.22%r 6 40 24" bgColOff="ff5c7fa4"
              buttonText="Edit" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="Offset Slider" id="c1c294edca92ea2f" memberName="offsetSld"
          virtualName="" explicitFocusOrder="0" pos="-595% 2 69 32" posRelativeX="9f75aa2c0ca39fa4"
          min="-6e1" max="6.7e1" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Offset Label" id="1389380960314b49" memberName="offsetLabel"
         virtualName="" explicitFocusOrder="0" pos="-707.5% 6 47 24" posRelativeX="9f75aa2c0ca39fa4"
         edTextCol="ff000000" edBkgCol="0" labelText="Offset:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

