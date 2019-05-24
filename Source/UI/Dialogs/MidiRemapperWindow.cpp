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

#include "MidiRemapperWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiRemapperWindow::MidiRemapperWindow (SuperVirtualKeyboardPluginState* pluginStateIn)
    : DialogWindow("Midi Remapper Dialog", Colours::slategrey, true),
      pluginState(pluginStateIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("Midi Remapper Dialog");
    comboBox.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (comboBox.get());
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (String());
    comboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox->addListener (this);

    comboBox->setBounds (112, 16, 240, 24);

    midiInuptLable.reset (new Label ("Midi Input Label",
                                     TRANS("Input Device:")));
    addAndMakeVisible (midiInuptLable.get());
    midiInuptLable->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    midiInuptLable->setJustificationType (Justification::centredLeft);
    midiInuptLable->setEditable (false, false, false);
    midiInuptLable->setColour (TextEditor::textColourId, Colours::black);
    midiInuptLable->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    midiInuptLable->setBounds (8, 16, 95, 24);

    remapGroup.reset (new GroupComponent ("Remap Settings Group",
                                          TRANS("notes")));
    addAndMakeVisible (remapGroup.get());

    remapGroup->setBounds (8, 176, 352, 528);

    slider.reset (new Slider ("new slider"));
    addAndMakeVisible (slider.get());
    slider->setRange (0, 127, 1);
    slider->setSliderStyle (Slider::TwoValueHorizontal);
    slider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider->addListener (this);

    slider->setBounds (112, 64, 248, 24);

    noteRangeLabel.reset (new Label ("Note Range Label",
                                     TRANS("Note Range:")));
    addAndMakeVisible (noteRangeLabel.get());
    noteRangeLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    noteRangeLabel->setJustificationType (Justification::centredLeft);
    noteRangeLabel->setEditable (false, false, false);
    noteRangeLabel->setColour (TextEditor::textColourId, Colours::black);
    noteRangeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    noteRangeLabel->setBounds (8, 64, 150, 24);

    manualMapToggle.reset (new ToggleButton ("ManualMapToggle"));
    addAndMakeVisible (manualMapToggle.get());
    manualMapToggle->setButtonText (TRANS("Manual Mapping"));
    manualMapToggle->addListener (this);
    manualMapToggle->setToggleState (true, dontSendNotification);

    manualMapToggle->setBounds (16, 128, 150, 24);

    modeMapToggle.reset (new ToggleButton ("Mode Map Toggle"));
    addAndMakeVisible (modeMapToggle.get());
    modeMapToggle->setButtonText (TRANS("Mode Remapping"));
    modeMapToggle->addListener (this);

    modeMapToggle->setBounds (160, 128, 150, 24);

    noteViewport.reset (new Viewport ("Notes Viewport"));
    addAndMakeVisible (noteViewport.get());

    noteViewport->setBounds (8, 184, 352, 520);

    modeLibraryBoxOG.reset (new ComboBox ("Mode Library Original"));
    addAndMakeVisible (modeLibraryBoxOG.get());
    modeLibraryBoxOG->setEditableText (false);
    modeLibraryBoxOG->setJustificationType (Justification::centredLeft);
    modeLibraryBoxOG->setTextWhenNothingSelected (TRANS("Meantone[7] 12"));
    modeLibraryBoxOG->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    modeLibraryBoxOG->addListener (this);

    modeLibraryBoxOG->setBounds (472, 72, 150, 24);

    modeBoxOriginal.reset (new Label ("Mode Box Original",
                                      TRANS("From Mode:")));
    addAndMakeVisible (modeBoxOriginal.get());
    modeBoxOriginal->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeBoxOriginal->setJustificationType (Justification::centredRight);
    modeBoxOriginal->setEditable (false, false, false);
    modeBoxOriginal->setColour (TextEditor::textColourId, Colours::black);
    modeBoxOriginal->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeBoxOriginal->setBounds (376, 72, 88, 24);

    modeLibraryBoxOG2.reset (new ComboBox ("Mode Library Original"));
    addAndMakeVisible (modeLibraryBoxOG2.get());
    modeLibraryBoxOG2->setEditableText (false);
    modeLibraryBoxOG2->setJustificationType (Justification::centredLeft);
    modeLibraryBoxOG2->setTextWhenNothingSelected (TRANS("Meantone[7] 12"));
    modeLibraryBoxOG2->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    modeLibraryBoxOG2->addListener (this);

    modeLibraryBoxOG2->setBounds (472, 118, 150, 24);

    modeBoxRemap.reset (new Label ("Mode Box Remap",
                                   TRANS("To Mode:")));
    addAndMakeVisible (modeBoxRemap.get());
    modeBoxRemap->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeBoxRemap->setJustificationType (Justification::centredRight);
    modeBoxRemap->setEditable (false, false, false);
    modeBoxRemap->setColour (TextEditor::textColourId, Colours::black);
    modeBoxRemap->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeBoxRemap->setBounds (376, 118, 88, 24);

    modeOriginalRootSld.reset (new Slider ("Mode Original Root Slider"));
    addAndMakeVisible (modeOriginalRootSld.get());
    modeOriginalRootSld->setRange (0, 127, 1);
    modeOriginalRootSld->setSliderStyle (Slider::IncDecButtons);
    modeOriginalRootSld->setTextBoxStyle (Slider::TextBoxRight, false, 80, 20);
    modeOriginalRootSld->addListener (this);

    modeOriginalRootSld->setBounds (640, 72, 80, 24);

    modeRemapRootSld.reset (new Slider ("Mode Remap Root Slider"));
    addAndMakeVisible (modeRemapRootSld.get());
    modeRemapRootSld->setRange (0, 127, 1);
    modeRemapRootSld->setSliderStyle (Slider::IncDecButtons);
    modeRemapRootSld->setTextBoxStyle (Slider::TextBoxRight, false, 80, 20);
    modeRemapRootSld->addListener (this);

    modeRemapRootSld->setBounds (640, 112, 80, 24);

    rootNoteLabel.reset (new Label ("Root Note Label",
                                    TRANS("Root Note:")));
    addAndMakeVisible (rootNoteLabel.get());
    rootNoteLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    rootNoteLabel->setJustificationType (Justification::centredLeft);
    rootNoteLabel->setEditable (false, false, false);
    rootNoteLabel->setColour (TextEditor::textColourId, Colours::black);
    rootNoteLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    rootNoteLabel->setBounds (640, 40, 80, 24);

    degreeGroup.reset (new GroupComponent ("Degree Group",
                                           TRANS("degrees")));
    addAndMakeVisible (degreeGroup.get());

    degreeGroup->setBounds (368, 176, 384, 528);

    degreesViewport.reset (new Viewport ("Degrees Viewport"));
    addAndMakeVisible (degreesViewport.get());

    degreesViewport->setBounds (376, 184, 376, 520);


    //[UserPreSize]

	noteViewport->setViewedComponent(remapGroup.get());
	degreesViewport->setViewedComponent(degreeGroup.get());

	//noteEditorTextBoxes.resize(128);

	TextEditor* textBox;

	for (int i = 0; i < 128; i++)
	{
		noteEditorTextBoxes.add(Array<std::unique_ptr<TextEditor>>());
	
		for (int j = 0; j < 6; j++)
		{
			noteEditorTextBoxes.getUnchecked(i).add(std::unique_ptr<TextEditor>());

			switch (j)
			{
			case(0):
				textBox = new TextEditor("Key/Midi Note " + String(i));
				textBox->setReadOnly(true);
				break;
			case(1):
				textBox = new TextEditor("Note In");
				textBox->setReadOnly(false);
				break;
			case(2):
				textBox = new TextEditor("Note Out");
				textBox->setReadOnly(false);
				break;
			case(3):
				textBox = new TextEditor("Label");
				textBox->setReadOnly(false);
				break;
			case(4):
				textBox = new TextEditor("Color");
				textBox->setReadOnly(true);
				break;
			case(5):
				textBox = new TextEditor("Reaper Character");
				textBox->setReadOnly(true);
				break;
			default:
				break;
			}

			noteEditorTextBoxes.getUnchecked(i).getUnchecked(j).reset(textBox);
			textBox->setSize(boxW, boxH);
			addAndMakeVisible(textBox);
			textBox->addListener(this);
		}
	}

    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MidiRemapperWindow::~MidiRemapperWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    comboBox = nullptr;
    midiInuptLable = nullptr;
    remapGroup = nullptr;
    slider = nullptr;
    noteRangeLabel = nullptr;
    manualMapToggle = nullptr;
    modeMapToggle = nullptr;
    noteViewport = nullptr;
    modeLibraryBoxOG = nullptr;
    modeBoxOriginal = nullptr;
    modeLibraryBoxOG2 = nullptr;
    modeBoxRemap = nullptr;
    modeOriginalRootSld = nullptr;
    modeRemapRootSld = nullptr;
    rootNoteLabel = nullptr;
    degreeGroup = nullptr;
    degreesViewport = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiRemapperWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiRemapperWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
	positionTextBoxes();
    //[/UserResized]
}

void MidiRemapperWindow::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox.get())
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox]
    }
    else if (comboBoxThatHasChanged == modeLibraryBoxOG.get())
    {
        //[UserComboBoxCode_modeLibraryBoxOG] -- add your combo box handling code here..
        //[/UserComboBoxCode_modeLibraryBoxOG]
    }
    else if (comboBoxThatHasChanged == modeLibraryBoxOG2.get())
    {
        //[UserComboBoxCode_modeLibraryBoxOG2] -- add your combo box handling code here..
        //[/UserComboBoxCode_modeLibraryBoxOG2]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MidiRemapperWindow::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider.get())
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        //[/UserSliderCode_slider]
    }
    else if (sliderThatWasMoved == modeOriginalRootSld.get())
    {
        //[UserSliderCode_modeOriginalRootSld] -- add your slider handling code here..
        //[/UserSliderCode_modeOriginalRootSld]
    }
    else if (sliderThatWasMoved == modeRemapRootSld.get())
    {
        //[UserSliderCode_modeRemapRootSld] -- add your slider handling code here..
        //[/UserSliderCode_modeRemapRootSld]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MidiRemapperWindow::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == manualMapToggle.get())
    {
        //[UserButtonCode_manualMapToggle] -- add your button handler code here..
        //[/UserButtonCode_manualMapToggle]
    }
    else if (buttonThatWasClicked == modeMapToggle.get())
    {
        //[UserButtonCode_modeMapToggle] -- add your button handler code here..
        //[/UserButtonCode_modeMapToggle]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiRemapperWindow::textEditorTextChanged(TextEditor& textEditorThatChanged)
{
	String name = textEditorThatChanged.getName();

	if (name.startsWith("Key"))
	{

	}
	else if (name.startsWith("Note In"))
	{
		inputRemapper->setNote(name.fromLastOccurrenceOf("Note ", false, false).getIntValue(), textEditorThatChanged.getText().getIntValue());
	}
	else if (name.startsWith("Note Out"))
	{
		inputRemapper->setNote(name.fromLastOccurrenceOf("Note ", false, false).getIntValue(), textEditorThatChanged.getText().getIntValue());
	}
	else if (name.startsWith("Label"))
	{

	}
	else if (name.startsWith("Colour"))
	{

	}
	else if (name.startsWith("Reaper"))
	{

	}
}

void MidiRemapperWindow::positionTextBoxes()
{
	TextEditor* textBox;

	int x = remapGroup->getX();
	int y = remapGroup->getY();

	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			switch (j)
			{
			default:
				//noteEditorTextBoxes.getUnchecked(i).add(textBox);
				break;

			case(1):
				//noteEditorTextBoxes.getUnchecked(i).add(textBox);
				break;
			case(2):
				//noteEditorTextBoxes.getUnchecked(i).add(textBox);
				break;
			case(3):
				//noteEditorTextBoxes.getUnchecked(i).add(textBox);
				break;
			case(4):
				//noteEditorTextBoxes.getUnchecked(i).add(textBox);
				break;
			case(5):
				//noteEditorTextBoxes.getUnchecked(i).add(textBox);
				break;

				textBox->setTopLeftPosition(x + i * boxH, y + j*boxW);
			}
		}
	}
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiRemapperWindow" componentName="Midi Remapper Dialog"
                 parentClasses="public DialogWindow, public TextEditor::Listener"
                 constructorParams="SuperVirtualKeyboardPluginState* pluginStateIn"
                 variableInitialisers="DialogWindow(&quot;Midi Remapper Dialog&quot;, Colours::slategrey, true),&#10;pluginState(pluginStateIn)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="new combo box" id="fdc675fc3d4fef33" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="112 16 240 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="Midi Input Label" id="87de2dc7bc47e162" memberName="midiInuptLable"
         virtualName="" explicitFocusOrder="0" pos="8 16 95 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Input Device:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="Remap Settings Group" id="12b438308e8b3765" memberName="remapGroup"
                  virtualName="" explicitFocusOrder="0" pos="8 176 352 528" title="notes"/>
  <SLIDER name="new slider" id="9cd98eb1a345671e" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="112 64 248 24" min="0" max="1.27e2"
          int="1" style="TwoValueHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="Note Range Label" id="42e0042c75044473" memberName="noteRangeLabel"
         virtualName="" explicitFocusOrder="0" pos="8 64 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Note Range:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="ManualMapToggle" id="3c967f0af78d5468" memberName="manualMapToggle"
                virtualName="" explicitFocusOrder="0" pos="16 128 150 24" buttonText="Manual Mapping"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="Mode Map Toggle" id="bffb6c931fccea1c" memberName="modeMapToggle"
                virtualName="" explicitFocusOrder="0" pos="160 128 150 24" buttonText="Mode Remapping"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <VIEWPORT name="Notes Viewport" id="ae7882599456b6e8" memberName="noteViewport"
            virtualName="" explicitFocusOrder="0" pos="8 184 352 520" vscroll="1"
            hscroll="1" scrollbarThickness="8" contentType="0" jucerFile=""
            contentClass="" constructorParams=""/>
  <COMBOBOX name="Mode Library Original" id="5c0ffc83cbe274c1" memberName="modeLibraryBoxOG"
            virtualName="" explicitFocusOrder="0" pos="472 72 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="Meantone[7] 12" textWhenNoItems="(no choices)"/>
  <LABEL name="Mode Box Original" id="5b2fda69d594937a" memberName="modeBoxOriginal"
         virtualName="" explicitFocusOrder="0" pos="376 72 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="From Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.5e1"
         kerning="0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="Mode Library Original" id="d97fde7b8560b769" memberName="modeLibraryBoxOG2"
            virtualName="" explicitFocusOrder="0" pos="472 118 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="Meantone[7] 12" textWhenNoItems="(no choices)"/>
  <LABEL name="Mode Box Remap" id="44d358cbb3982e4" memberName="modeBoxRemap"
         virtualName="" explicitFocusOrder="0" pos="376 118 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="To Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.5e1"
         kerning="0" bold="0" italic="0" justification="34"/>
  <SLIDER name="Mode Original Root Slider" id="31eb79d952d2e1aa" memberName="modeOriginalRootSld"
          virtualName="" explicitFocusOrder="0" pos="640 72 80 24" min="0"
          max="1.27e2" int="1" style="IncDecButtons" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Mode Remap Root Slider" id="c98578a060cce67f" memberName="modeRemapRootSld"
          virtualName="" explicitFocusOrder="0" pos="640 112 80 24" min="0"
          max="1.27e2" int="1" style="IncDecButtons" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Root Note Label" id="effe36cc4475b201" memberName="rootNoteLabel"
         virtualName="" explicitFocusOrder="0" pos="640 40 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Root Note:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.5e1"
         kerning="0" bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="Degree Group" id="a67b607a4e18fcc5" memberName="degreeGroup"
                  virtualName="" explicitFocusOrder="0" pos="368 176 384 528" title="degrees"/>
  <VIEWPORT name="Degrees Viewport" id="5b50b4addf240fe6" memberName="degreesViewport"
            virtualName="" explicitFocusOrder="0" pos="376 184 376 520" vscroll="1"
            hscroll="1" scrollbarThickness="8" contentType="0" jucerFile=""
            contentClass="" constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

