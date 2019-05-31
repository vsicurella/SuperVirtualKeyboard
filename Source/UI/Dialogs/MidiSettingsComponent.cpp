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

#include "MidiSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
MidiSettingsComponent::MidiSettingsComponent (SvkPluginState* pluginStateIn)
    : pluginState(pluginStateIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("Midi Remapper Dialog");
    midiInputBox.reset (new ComboBox ("Input Device Box"));
    addAndMakeVisible (midiInputBox.get());
    midiInputBox->setEditableText (false);
    midiInputBox->setJustificationType (Justification::centredLeft);
    midiInputBox->setTextWhenNothingSelected (String());
    midiInputBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    midiInputBox->addListener (this);

    midiInputBox->setBounds (112, 16, 240, 24);

    midiInputLabel.reset (new Label ("Midi Input Label",
                                     TRANS("Input Device:")));
    addAndMakeVisible (midiInputLabel.get());
    midiInputLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    midiInputLabel->setJustificationType (Justification::centredLeft);
    midiInputLabel->setEditable (false, false, false);
    midiInputLabel->setColour (TextEditor::textColourId, Colours::black);
    midiInputLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    midiInputLabel->setBounds (8, 16, 95, 24);

    noteRangeSld.reset (new Slider ("Note Range Slider"));
    addAndMakeVisible (noteRangeSld.get());
    noteRangeSld->setRange (0, 127, 1);
    noteRangeSld->setSliderStyle (Slider::TwoValueHorizontal);
    noteRangeSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    noteRangeSld->addListener (this);

    noteRangeSld->setBounds (112, 64, 248, 24);

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

    modeBoxOriginal.reset (new Label ("Mode Box Original",
                                      TRANS("From Mode:")));
    addAndMakeVisible (modeBoxOriginal.get());
    modeBoxOriginal->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeBoxOriginal->setJustificationType (Justification::centredRight);
    modeBoxOriginal->setEditable (false, false, false);
    modeBoxOriginal->setColour (TextEditor::textColourId, Colours::black);
    modeBoxOriginal->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeBoxOriginal->setBounds (376, 72, 88, 24);

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
    modeOriginalRootSld->setTextBoxStyle (Slider::TextBoxRight, false, 60, 20);
    modeOriginalRootSld->addListener (this);

    modeOriginalRootSld->setBounds (632, 74, 104, 24);

    modeRemapRootSld.reset (new Slider ("Mode Remap Root Slider"));
    addAndMakeVisible (modeRemapRootSld.get());
    modeRemapRootSld->setRange (0, 127, 1);
    modeRemapRootSld->setSliderStyle (Slider::IncDecButtons);
    modeRemapRootSld->setTextBoxStyle (Slider::TextBoxRight, false, 60, 20);
    modeRemapRootSld->addListener (this);

    modeRemapRootSld->setBounds (632, 114, 104, 24);

    rootNoteLabel.reset (new Label ("Root Note Label",
                                    TRANS("Root Note:")));
    addAndMakeVisible (rootNoteLabel.get());
    rootNoteLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    rootNoteLabel->setJustificationType (Justification::centredLeft);
    rootNoteLabel->setEditable (false, false, false);
    rootNoteLabel->setColour (TextEditor::textColourId, Colours::black);
    rootNoteLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    rootNoteLabel->setBounds (630, 48, 80, 24);

    presetBox1.reset (new PresetLibraryBox (pluginState->presetManager.get()));
    addAndMakeVisible (presetBox1.get());
    presetBox1->setName ("Preset Box 1");

    presetBox1->setBounds (472, 75, 150, 24);

    presetBox2.reset (new PresetLibraryBox (pluginState->presetManager.get()));
    addAndMakeVisible (presetBox2.get());
    presetBox2->setName ("Preset Box 2");

    presetBox2->setBounds (472, 115, 150, 24);

    mapModesBtn.reset (new TextButton ("Map Modes Button"));
    addAndMakeVisible (mapModesBtn.get());
    mapModesBtn->setButtonText (TRANS("Map!"));
    mapModesBtn->addListener (this);

    mapModesBtn->setBounds (472, 168, 144, 24);


    //[UserPreSize]
	noteRangeSld->setMinValue(0);
	noteRangeSld->setMaxValue(127);

	presetBox1->addListener(this);
	presetBox2->addListener(this);

	midiInputFilter = pluginState->getMidiProcessor()->getInputRemapper();
	midiOutputFilter = pluginState->getMidiProcessor()->getOutputRemapper();

	remapTable.reset(new TableListBox("Midi Remap Table"));
	remapTableModel.reset(new MidiRemapTableModel(remapTable.get(), midiInputFilter, midiOutputFilter, pluginState->textFilterInt.get()));
	remapTable->setModel(remapTableModel.get());
	remapTable->setBounds(8, 184, 352, 264);

	remapTable->setHeader(new TableHeaderComponent());
	remapTable->getHeader().addColumn("Key", 1, remapTable->getWidth() * 0.2);
	remapTable->getHeader().addColumn("Input Note", 2, remapTable->getWidth() * 0.35);
	remapTable->getHeader().addColumn("Output Note", 3, remapTable->getWidth() * 0.35);

	addAndMakeVisible(remapTable.get());

    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..
    presetBox1->setSelectedId(8);
    presetBox2->setSelectedId(8);
    modeOriginalRootSld->setValue(60);
    modeRemapRootSld->setValue(60);
    //[/Constructor]
}

MidiSettingsComponent::~MidiSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    remapTable.release();
	remapTableModel.release();

	presetBox1->removeListener(this);
	presetBox2->removeListener(this);
    //[/Destructor_pre]

    midiInputBox = nullptr;
    midiInputLabel = nullptr;
    noteRangeSld = nullptr;
    noteRangeLabel = nullptr;
    manualMapToggle = nullptr;
    modeMapToggle = nullptr;
    modeBoxOriginal = nullptr;
    modeBoxRemap = nullptr;
    modeOriginalRootSld = nullptr;
    modeRemapRootSld = nullptr;
    rootNoteLabel = nullptr;
    presetBox1 = nullptr;
    presetBox2 = nullptr;
    mapModesBtn = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiSettingsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiSettingsComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == midiInputBox.get())
    {
        //[UserComboBoxCode_midiInputBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_midiInputBox]
    }

    //[UsercomboBoxChanged_Post]
	else if (comboBoxThatHasChanged == presetBox1.get())
	{
		modeSelected1.reset(
			new Mode(pluginState->presetManager->getMode(presetBox1->getSelectedId()),
						modeOriginalRootSld->getValue()));
	}

	else if (comboBoxThatHasChanged == presetBox2.get())
	{
		modeSelected2.reset(
			new Mode(pluginState->presetManager->getMode(presetBox1->getSelectedId()),
				modeRemapRootSld->getValue()));
	}
    //[/UsercomboBoxChanged_Post]
}

void MidiSettingsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == noteRangeSld.get())
    {
        //[UserSliderCode_noteRangeSld] -- add your slider handling code here..
        Point<int> range = Point<int>(noteRangeSld->getMinimum(), noteRangeSld->getMaximum());
        midiInputFilter->setNoteRange(range.x, range.y);
        noteRangeSld->updateText();
        //[/UserSliderCode_noteRangeSld]
    }
    else if (sliderThatWasMoved == modeOriginalRootSld.get())
    {
        //[UserSliderCode_modeOriginalRootSld] -- add your slider handling code here..
		if (modeSelected1.get())
			modeSelected1->setRootNote(modeOriginalRootSld->getValue());
        //[/UserSliderCode_modeOriginalRootSld]
    }
    else if (sliderThatWasMoved == modeRemapRootSld.get())
    {
        //[UserSliderCode_modeRemapRootSld] -- add your slider handling code here..
		if (modeSelected2.get())
			modeSelected2->setRootNote(modeRemapRootSld->getValue());
        //[/UserSliderCode_modeRemapRootSld]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MidiSettingsComponent::buttonClicked (Button* buttonThatWasClicked)
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
    else if (buttonThatWasClicked == mapModesBtn.get())
    {
        //[UserButtonCode_mapModesBtn] -- add your button handler code here..
		if (modeSelected1.get() && modeSelected2.get())
		{
			pluginState->getMidiProcessor()->setMidiInputMap(ModeMapper::map(
				modeSelected1.get(), modeSelected2.get()
			));
		}
        //[/UserButtonCode_mapModesBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MidiSettingsComponent::setModeSelected2(int selectedIdIn)
{
	presetBox2->setSelectedId(selectedIdIn);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiSettingsComponent" componentName="Midi Remapper Dialog"
                 parentClasses="public Component" constructorParams="SvkPluginState* pluginStateIn"
                 variableInitialisers="pluginState(pluginStateIn)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="Input Device Box" id="fdc675fc3d4fef33" memberName="midiInputBox"
            virtualName="" explicitFocusOrder="0" pos="112 16 240 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="Midi Input Label" id="87de2dc7bc47e162" memberName="midiInputLabel"
         virtualName="" explicitFocusOrder="0" pos="8 16 95 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Input Device:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="Note Range Slider" id="9cd98eb1a345671e" memberName="noteRangeSld"
          virtualName="" explicitFocusOrder="0" pos="112 64 248 24" min="0.0"
          max="127.0" int="1.0" style="TwoValueHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Note Range Label" id="42e0042c75044473" memberName="noteRangeLabel"
         virtualName="" explicitFocusOrder="0" pos="8 64 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Note Range:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="ManualMapToggle" id="3c967f0af78d5468" memberName="manualMapToggle"
                virtualName="" explicitFocusOrder="0" pos="16 128 150 24" buttonText="Manual Mapping"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="Mode Map Toggle" id="bffb6c931fccea1c" memberName="modeMapToggle"
                virtualName="" explicitFocusOrder="0" pos="160 128 150 24" buttonText="Mode Remapping"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="Mode Box Original" id="5b2fda69d594937a" memberName="modeBoxOriginal"
         virtualName="" explicitFocusOrder="0" pos="376 72 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="From Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="Mode Box Remap" id="44d358cbb3982e4" memberName="modeBoxRemap"
         virtualName="" explicitFocusOrder="0" pos="376 118 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="To Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="Mode Original Root Slider" id="31eb79d952d2e1aa" memberName="modeOriginalRootSld"
          virtualName="" explicitFocusOrder="0" pos="632 74 104 24" min="0.0"
          max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="Mode Remap Root Slider" id="c98578a060cce67f" memberName="modeRemapRootSld"
          virtualName="" explicitFocusOrder="0" pos="632 114 104 24" min="0.0"
          max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Root Note Label" id="effe36cc4475b201" memberName="rootNoteLabel"
         virtualName="" explicitFocusOrder="0" pos="630 48 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Root Note:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="Preset Box 1" id="ee5b2def32e4fdbc" memberName="presetBox1"
                    virtualName="PresetLibraryBox" explicitFocusOrder="0" pos="472 75 150 24"
                    class="PresetLibraryBox" params="pluginState-&gt;presetManager.get()"/>
  <GENERICCOMPONENT name="Preset Box 2" id="eb2e366c9ad8965c" memberName="presetBox2"
                    virtualName="PresetLibraryBox" explicitFocusOrder="0" pos="472 115 150 24"
                    class="PresetLibraryBox" params="pluginState-&gt;presetManager.get()"/>
  <TEXTBUTTON name="Map Modes Button" id="7a76936bfde482aa" memberName="mapModesBtn"
              virtualName="" explicitFocusOrder="0" pos="472 168 144 24" buttonText="Map!"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...

//[/EndFile]

