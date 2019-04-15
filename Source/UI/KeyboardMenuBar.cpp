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

#include "KeyboardMenuBar.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
KeyboardMenuBar::KeyboardMenuBar (SuperVirtualKeyboardPluginState* pluginStateIn, ApplicationCommandManager* managerIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
	presets = presetsArrayIn;
	presetsSorted = presetsSortedIn;
	appCmdMgr = managerIn;
	setName("Keyboard Menu Bar");
    //[/Constructor_pre]

    textEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (textEditor.get());
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setText (TRANS("2 2 1 2 2 2 1"));

    sendScale.reset (new TextButton ("Send Scale"));
    addAndMakeVisible (sendScale.get());
    sendScale->addListener (this);

    scalePresets.reset (new ComboBox ("Preset Box"));
    addAndMakeVisible (scalePresets.get());
    scalePresets->setEditableText (false);
    scalePresets->setJustificationType (Justification::centredLeft);
    scalePresets->setTextWhenNothingSelected (TRANS("Pick a mode..."));
    scalePresets->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    scalePresets->addListener (this);

    keyboardModeBtn.reset (new TextButton ("Keyboard Mode Button"));
    addAndMakeVisible (keyboardModeBtn.get());
    keyboardModeBtn->setButtonText (TRANS("Edit"));
    keyboardModeBtn->addListener (this);
    keyboardModeBtn->setColour (TextButton::buttonColourId, Colour (0xff5c7fa4));


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

KeyboardMenuBar::~KeyboardMenuBar()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    textEditor = nullptr;
    sendScale = nullptr;
    scalePresets = nullptr;
    keyboardModeBtn = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void KeyboardMenuBar::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void KeyboardMenuBar::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	pianoMenu->setBounds(1, 5, proportionOfWidth(1.0f), 24);
    //[/UserPreResize]

    textEditor->setBounds ((proportionOfWidth (0.9925f) - 40) + roundToInt (40 * -6.6500f) - 150, 6 + 0, 150, 24);
    sendScale->setBounds ((proportionOfWidth (0.9925f) - 40) + roundToInt (40 * -4.2000f) - 88, 6 + 0, 88, 24);
    scalePresets->setBounds ((proportionOfWidth (0.9925f) - 40) + roundToInt (40 * -0.2500f) - 150, 6 + 0, 150, 24);
    keyboardModeBtn->setBounds (proportionOfWidth (0.9925f) - 40, 6, 40, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void KeyboardMenuBar::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == sendScale.get())
    {
        //[UserButtonCode_sendScale] -- add your button handler code here..
		presetSelected = presets->set(0, new ModeLayout(textEditor->getText(), "Custom"), true);
		scalePresets->setText("Custom Scale");
		sendChangeMessage();
        //[/UserButtonCode_sendScale]
    }
    else if (buttonThatWasClicked == keyboardModeBtn.get())
    {
        //[UserButtonCode_keyboardModeBtn] -- add your button handler code here..

        //[/UserButtonCode_keyboardModeBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void KeyboardMenuBar::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == scalePresets.get())
    {
        //[UserComboBoxCode_scalePresets] -- add your combo box handling code here..

        int presetIndex = menuToPresetIndex[comboBoxThatHasChanged->getText()];
        ModeLayout* mode = presets->getUnchecked(presetIndex);

		if (mode)
		{
			presetSelected = mode;
			textEditor.get()->setText(presetSelected->strSteps);
		}

		sendChangeMessage();
        //[/UserComboBoxCode_scalePresets]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void KeyboardMenuBar::mouseEnter (const MouseEvent& e)
{
    //[UserCode_mouseEnter] -- Add your code here...
	setWantsKeyboardFocus(true);
    //[/UserCode_mouseEnter]
}

void KeyboardMenuBar::mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel)
{
    //[UserCode_mouseWheelMove] -- Add your code here...
	if (scalePresets->isMouseOver())
	{
		scalePresets->setSelectedId(scalePresets->getSelectedId() + (wheel.isReversed * 2 - 1));
	}
    //[/UserCode_mouseWheelMove]
}

bool KeyboardMenuBar::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
	if (!enterDown && KeyPress::isKeyCurrentlyDown(KeyPress::returnKey))
	{
		enterDown = true;
		sendChangeMessage();
	}
    return true;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}

bool KeyboardMenuBar::keyStateChanged (bool isKeyDown)
{
    //[UserCode_keyStateChanged] -- Add your code here...
	if (enterDown && KeyPress::isKeyCurrentlyDown(KeyPress::returnKey))
	{
		enterDown = false;
	}
    return true;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyStateChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

String KeyboardMenuBar::get_input()
{
	return textEditor->getText();
}

String KeyboardMenuBar::get_preset_name()
{
	return scalePresets.get()->getText();
}

ModeLayout* KeyboardMenuBar::get_preset(String anyNameIn)
{
	ModeLayout* out = nullptr;

	for (int i = 0; i < presets->size(); i++)
	{
		out = presets->getUnchecked(i);

		if (out->get_full_name() == anyNameIn ||
			out->get_name_scale_size() == anyNameIn ||
			out->get_name_mode_size() == anyNameIn )
			return out;
	}

	return out;
}

int KeyboardMenuBar::get_preset_id(String anyNameIn)
{
    return menuToPresetIndex[anyNameIn];
}

int KeyboardMenuBar::get_selected_preset_id()
{
    return scalePresets.get()->getSelectedId();
}

void KeyboardMenuBar::set_selected_preset(ModeLayout* presetIn)
{
    scalePresets.get()->setSelectedId(get_preset_id(presetIn->get_full_name()));
}

void KeyboardMenuBar::set_selected_preset(int comboBoxIdIn)
{
    scalePresets.get()->setSelectedId(comboBoxIdIn);
}

void KeyboardMenuBar::set_text_boxes(String presetName, String steps)
{
    textEditor.get()->setText(steps);
    scalePresets.get()->setText(presetName);
}

void KeyboardMenuBar::populate_preset_menu()
{
	scalePresets.get()->clear();
    menuToPresetIndex.clear();

	menuSortByScale.reset(new PopupMenu());
	menuSortByMode.reset(new PopupMenu());
	menuSortByFamily.reset(new PopupMenu());

	ModeLayout* mode;
	String name;
	String p_name;
    int presetIndex;
	int size = 0;
	int p_size = 0;
	int c = 1;

	for (int i = 0; i < presets->size() - 1; i++)
	{
		mode = presetsSorted->getUnchecked(SortType::scaleSize).getUnchecked(i);
		name = mode->get_name_scale_size();
        presetIndex = presets->indexOf(mode);

		size = mode->scaleSize;
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
		mode = presetsSorted->getUnchecked(SortType::modeSize).getUnchecked(i);
		name = mode->get_name_mode_size();
        presetIndex = presets->indexOf(mode);

		size = mode->modeSize;
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
		mode = presetsSorted->getUnchecked(SortType::family)[i];
		name = mode->get_full_name();
        presetIndex = presets->indexOf(mode);

		if (i > 0 && p_name != mode->family)
		{
			menuSortByFamily.get()->addSeparator();
			p_name = mode->family;
		}

		menuSortByFamily.get()->addItem(c++, name);
        menuToPresetIndex.set(name, presetIndex);
	}

	scalePresets->getRootMenu()->addSubMenu("by Scale Size", *menuSortByScale.get());
	scalePresets->getRootMenu()->addSubMenu("by Mode Size", *menuSortByMode.get());
	scalePresets->getRootMenu()->addSubMenu("by Family", *menuSortByFamily.get());
}

KeyboardMenuBar::KeyboardMenu::KeyboardMenu(ApplicationCommandManager* managerIn)
{
	setName("Menu Bar");

	menuParent.reset(new MenuBarComponent(this));
	addAndMakeVisible(menuParent.get());

	appCmdMgr = managerIn;
	setApplicationCommandManagerToWatch(appCmdMgr);

	setSize(500, 500);
}

KeyboardMenuBar::KeyboardMenu::~KeyboardMenu()
{
}

StringArray KeyboardMenuBar::KeyboardMenu::getMenuBarNames()
{
	return { "File", "Edit", "View" };
}

void KeyboardMenuBar::KeyboardMenu::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{

}

PopupMenu KeyboardMenuBar::KeyboardMenu::getMenuForIndex(int topLevelMenuIndex, const String &menuName)
{
	PopupMenu menu;
	if (topLevelMenuIndex == 0)
	{
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::loadCustomLayout, "Load layout");
		menu.addCommandItem(appCmdMgr, IDs::CommandIDs::saveCustomLayout, "Save layout");
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
	}

	return menu;
}

void KeyboardMenuBar::KeyboardMenu::resized()
{
	menuParent->setBounds(0, 0,	350, LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
}

MenuBarComponent* KeyboardMenuBar::KeyboardMenu::get_menu()
{
	return menuParent.get();
}



//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="KeyboardMenuBar" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster" constructorParams="SuperVirtualKeyboardPluginState* pluginStateIn, ApplicationCommandManager* managerIn"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
    <METHOD name="keyStateChanged (bool isKeyDown)"/>
    <METHOD name="mouseWheelMove (const MouseEvent&amp; e, const MouseWheelDetails&amp; wheel)"/>
    <METHOD name="mouseEnter (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="new text editor" id="8c559f3dc17dcbb0" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="-665%r 0 150 24" posRelativeX="9f75aa2c0ca39fa4"
              posRelativeY="9f75aa2c0ca39fa4" initialText="2 2 1 2 2 2 1" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="Send Scale" id="3a2872f3357f900b" memberName="sendScale"
              virtualName="" explicitFocusOrder="0" pos="-420%r 0 88 24" posRelativeX="9f75aa2c0ca39fa4"
              posRelativeY="9f75aa2c0ca39fa4" buttonText="Send Scale" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="Preset Box" id="91d2066d9e23de1c" memberName="scalePresets"
            virtualName="" explicitFocusOrder="0" pos="-25%r 0 150 24" posRelativeX="9f75aa2c0ca39fa4"
            posRelativeY="9f75aa2c0ca39fa4" editable="0" layout="33" items=""
            textWhenNonSelected="Pick a mode..." textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="Keyboard Mode Button" id="9f75aa2c0ca39fa4" memberName="keyboardModeBtn"
              virtualName="" explicitFocusOrder="0" pos="99.246%r 6 40 24"
              bgColOff="ff5c7fa4" buttonText="Edit" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

