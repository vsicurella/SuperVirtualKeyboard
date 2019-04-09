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

#include "ScaleEditPopup.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ScaleEditPopup::ScaleEditPopup ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	presets.reset(new HashMap<String, std::vector<int>>());

	presets.get()->set("Mavila[5] 7", std::vector<int>({ 1, 2, 1, 2, 1 }));
	presets.get()->set("Father[5] 8", std::vector<int>({ 2, 1, 2, 2, 1 }));
	presets.get()->set("Mavila[7] 9", std::vector<int>({ 1, 1, 2, 1, 1, 1, 2 }));
	presets.get()->set("Dicot[7] 10", std::vector<int>({ 1, 2, 1, 2, 1, 2, 1}));
	presets.get()->set("Machine[6] 11", std::vector<int>({ 2, 2, 2, 2, 2, 1 }));
	presets.get()->set("Orgone[7] 11", std::vector<int>({ 2, 1, 2, 1, 2, 1, 2}));
	presets.get()->set("Audmented[6] 12", std::vector<int>({ 3, 1, 3, 1, 3, 1 }));
	presets.get()->set("Meantone[7] 12", std::vector<int>({ 2, 2, 1, 2, 2, 2, 1 }));
	presets.get()->set("Diminished[8] 12", std::vector<int>({ 2, 1, 2, 1, 2, 1, 2, 1 }));
	presets.get()->set("Father[8] 13", std::vector<int>({ 2, 2, 1, 2, 2, 1, 2, 1 }));
	presets.get()->set("Orwell[9] 13", std::vector<int>({ 2, 1, 2, 1, 2, 1, 2, 1, 1 }));
	presets.get()->set("Hedgehog[8] 14", std::vector<int>({ 2, 2, 2, 2, 1, 2, 2, 1 }));
	presets.get()->set("Titanium[9] 14", std::vector<int>({ 2, 2, 1, 2, 1, 2, 1, 2, 1 }));
	presets.get()->set("Blackwood[5] 15", std::vector<int>({ 3, 3, 3, 3, 3 }));
	presets.get()->set("Orgone[7] 15", std::vector<int>({ 3, 1, 3, 1, 3, 1, 3 }));
	presets.get()->set("Porcupine[8] 15", std::vector<int>({ 2, 2, 2, 2, 2, 2, 2, 1 }));
	presets.get()->set("Orgone[11] 15", std::vector<int>({ 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1}));
	presets.get()->set("Gorgo[6] 16", std::vector<int>({ 3, 3, 1, 3, 3, 3 }));
	presets.get()->set("Mavila[9] 16", std::vector<int>({ 2, 2, 2, 1, 2, 2, 2, 2, 1 }));
	presets.get()->set("Lemba[10] 16", std::vector<int>({ 2, 1, 2, 1, 2, 2, 1, 2, 1, 2 }));
	presets.get()->set("Meantone[7] 17", std::vector<int>({ 3, 3, 1, 3, 3, 3, 1 }));
	presets.get()->set("Maqamic[7] 17", std::vector<int>({ 3, 2, 3, 2, 3, 2, 2 }));
	presets.get()->set("Maqamic[10] 17", std::vector<int>({ 2, 2, 1, 2, 2, 1, 2, 2, 2, 1 }));
	presets.get()->set("Machine[11] 17", std::vector<int>({ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2 }));
	presets.get()->set("Bicycle[6] 18", std::vector<int>({ 4, 4, 1, 4, 4, 1 }));
	presets.get()->set("Father[8] 18", std::vector<int>({ 3, 1, 3, 3, 1, 3, 3, 1 }));
	presets.get()->set("Meantone[7] 19", std::vector<int>({ 3, 3, 2, 3, 3, 3, 2 }));
	presets.get()->set("Keemun[8] 19", std::vector<int>({ 2, 3, 2, 2, 3, 2, 3, 2 }));
	presets.get()->set("Negri[9] 19", std::vector<int>({ 2, 2, 2, 2, 3, 2, 2, 2, 2 }));
	presets.get()->set("Mavila[7] 20", std::vector<int>({ 5, 2, 2, 5, 2, 2, 2 }));
	presets.get()->set("Blackwood[10] 20", std::vector<int>({ 3, 1, 3, 1, 3, 1, 3, 1, 3, 1 }));
	presets.get()->set("Miracle[9] 21", std::vector<int>({ 2, 3, 2, 2, 3, 2, 2, 3, 2 }));
	presets.get()->set("Superpyth[7] 22", std::vector<int>({ 4, 4, 1, 4, 4, 4, 1}));
	presets.get()->set("Hedgehog[8] 22", std::vector<int>({ 3, 3, 3, 3, 2, 3, 3, 2 }));
	presets.get()->set("Orwell[9] 22", std::vector<int>({ 3, 2, 3, 2, 3, 2, 3, 2, 2 }));
	presets.get()->set("Mavila[9] 23", std::vector<int>({ 3, 3, 3, 1, 3, 3, 3, 3, 1 }));
	presets.get()->set("Sephiroth[10] 23", std::vector<int>({ 2, 2, 2, 3, 2, 2, 3, 2, 2, 3 }));
	presets.get()->set("Godzilla[5] 24", std::vector<int>({ 5, 5, 4, 5, 5 }));
	presets.get()->set("Triforce[6] 24", std::vector<int>({ 5, 3, 5, 3, 5, 3  }));
	presets.get()->set("Dastgah-e Sehgah / Maqam Nairuz [7] 24", std::vector<int>({ 4, 3, 3, 4, 3, 3, 4 }));
	presets.get()->set("Meantone[7] 24", std::vector<int>({ 4, 4, 2, 4, 4, 4, 2 }));
	presets.get()->set("Mohajira[7] 24", std::vector<int>({ 4, 3, 4, 3, 4, 3, 3 }));
	presets.get()->set("Meantone[7] 26", std::vector<int>({ 4, 4, 3, 4, 4, 4, 3 }));
	presets.get()->set("Orgone[11] 26", std::vector<int>({ 2, 3, 2, 2, 3, 2, 3, 2, 2, 3, 2 }));
	presets.get()->set("Injera[12] 26", std::vector<int>({ 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 3}));
	presets.get()->set("Meantone[7] 31", std::vector<int>({ 5, 5, 3, 5, 5, 5, 3 }));
	presets.get()->set("Orwell[9] 31", std::vector<int>({ 4, 3, 4, 3, 4, 3, 4, 3, 3, }));
	presets.get()->set("BP Lambda", std::vector<int>({ 2, 1, 1, 2, 1, 2, 1, 2, 1 }));
	presets.get()->set("BP Dur II", std::vector<int>({ 2, 1, 1, 2, 1, 2, 1, 1, 2 }));
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

    instructions.reset (new Label ("Instructions",
                                   TRANS("Enter your scale like this:\n"
                                   "2 2 1 2 2 2 1")));
    addAndMakeVisible (instructions.get());
    instructions->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    instructions->setJustificationType (Justification::centred);
    instructions->setEditable (false, false, false);
    instructions->setColour (TextEditor::textColourId, Colours::black);
    instructions->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

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


    //[UserPreSize]
	StringArray presetNames;
	for (HashMap<String, std::vector<int>>::Iterator key (*presets.get()); key.next();)
	{
		presetNames.add(key.getKey());
	}
	presetNames.sortNatural();

	int i = 0;
	for (auto str : presetNames)
	{
		scalePresets.get()->addItem(str, ++i);
	}
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ScaleEditPopup::~ScaleEditPopup()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    textEditor = nullptr;
    instructions = nullptr;
    sendScale = nullptr;
    scalePresets = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ScaleEditPopup::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ScaleEditPopup::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    textEditor->setBounds (proportionOfWidth (0.2830f), 8, 150, 24);
    instructions->setBounds (proportionOfWidth (0.0172f), 0, 175, 40);
    sendScale->setBounds (proportionOfWidth (0.5402f), 8, 88, 24);
    scalePresets->setBounds (proportionOfWidth (0.7203f), 8, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ScaleEditPopup::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == sendScale.get())
    {
        //[UserButtonCode_sendScale] -- add your button handler code here..
		sendChangeMessage();
        //[/UserButtonCode_sendScale]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void ScaleEditPopup::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == scalePresets.get())
    {
        //[UserComboBoxCode_scalePresets] -- add your combo box handling code here..
		std::vector<int> scale = get_scale(scalePresets.get()->getText());
		textEditor.get()->setText(build_scale_string(scale));
		sendChangeMessage();
        //[/UserComboBoxCode_scalePresets]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

bool ScaleEditPopup::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
	if (!enterDown && KeyPress::isKeyCurrentlyDown(KeyPress::returnKey))
	{
		enterDown = true;
		sendChangeMessage();
	}
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}

bool ScaleEditPopup::keyStateChanged (bool isKeyDown)
{
    //[UserCode_keyStateChanged] -- Add your code here...
	if (enterDown && KeyPress::isKeyCurrentlyDown(KeyPress::returnKey))
	{
		enterDown = false;
	}
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyStateChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

String ScaleEditPopup::get_preset_name()
{
	return scalePresets.get()->getText();
}
std::vector<int> ScaleEditPopup::get_scale(String hashKeyIn)
{
	return presets.get()->operator[](hashKeyIn);
}
String ScaleEditPopup::get_input()
{
	return textEditor->getText();
}

String ScaleEditPopup::build_scale_string(std::vector<int> scaleIn)
{
	String out;

	for (int i = 0; i < scaleIn.size(); i++)
	{
		out += String(scaleIn.at(i));

		if (i < scaleIn.size() - 1)
			out += " ";
	}
	return out;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ScaleEditPopup" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
    <METHOD name="keyStateChanged (bool isKeyDown)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="new text editor" id="8c559f3dc17dcbb0" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="28.296% 8 150 24"
              initialText="2 2 1 2 2 2 1" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="Instructions" id="4c079e580647d111" memberName="instructions"
         virtualName="" explicitFocusOrder="0" pos="1.715% 0 175 40" edTextCol="ff000000"
         edBkgCol="0" labelText="Enter your scale like this:&#10;2 2 1 2 2 2 1"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="36"/>
  <TEXTBUTTON name="Send Scale" id="3a2872f3357f900b" memberName="sendScale"
              virtualName="" explicitFocusOrder="0" pos="54.019% 8 88 24" buttonText="Send Scale"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="Preset Box" id="91d2066d9e23de1c" memberName="scalePresets"
            virtualName="" explicitFocusOrder="0" pos="72.026% 8 150 24"
            editable="0" layout="33" items="" textWhenNonSelected="Pick a mode..."
            textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

