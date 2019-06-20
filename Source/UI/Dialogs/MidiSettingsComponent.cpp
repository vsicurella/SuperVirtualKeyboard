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
    modeFromLbl.reset (new Label ("Mode From Label",
                                  TRANS("From Mode:")));
    addAndMakeVisible (modeFromLbl.get());
    modeFromLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeFromLbl->setJustificationType (Justification::centredRight);
    modeFromLbl->setEditable (false, false, false);
    modeFromLbl->setColour (TextEditor::textColourId, Colours::black);
    modeFromLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeToLbl.reset (new Label ("Mode To Label",
                                TRANS("To Mode:")));
    addAndMakeVisible (modeToLbl.get());
    modeToLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeToLbl->setJustificationType (Justification::centredRight);
    modeToLbl->setEditable (false, false, false);
    modeToLbl->setColour (TextEditor::textColourId, Colours::black);
    modeToLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    rootFromSld.reset (new Slider ("Root From Slider"));
    addAndMakeVisible (rootFromSld.get());
    rootFromSld->setRange (0, 127, 1);
    rootFromSld->setSliderStyle (Slider::IncDecButtons);
    rootFromSld->setTextBoxStyle (Slider::TextBoxRight, false, 60, 20);
    rootFromSld->addListener (this);

    rootToSld.reset (new Slider ("Root To Slider"));
    addAndMakeVisible (rootToSld.get());
    rootToSld->setRange (0, 127, 1);
    rootToSld->setSliderStyle (Slider::IncDecButtons);
    rootToSld->setTextBoxStyle (Slider::TextBoxRight, false, 60, 20);
    rootToSld->addListener (this);

    rootNoteLabel.reset (new Label ("Root Note Label",
                                    TRANS("Root Note:")));
    addAndMakeVisible (rootNoteLabel.get());
    rootNoteLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    rootNoteLabel->setJustificationType (Justification::centredLeft);
    rootNoteLabel->setEditable (false, false, false);
    rootNoteLabel->setColour (TextEditor::textColourId, Colours::black);
    rootNoteLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    presetBox1.reset (new ReferencedComboBox());
    addAndMakeVisible (presetBox1.get());
    presetBox1->setName ("Preset Box 1");

    presetBox1->setBounds (640, 664, 150, 24);

    presetBox2.reset (new ReferencedComboBox());
    addAndMakeVisible (presetBox2.get());
    presetBox2->setName ("Preset Box 2");

    presetBox2->setBounds (640, 696, 150, 24);

    mapModesBtn.reset (new TextButton ("Map Modes Button"));
    addAndMakeVisible (mapModesBtn.get());
    mapModesBtn->setButtonText (TRANS("Map!"));
    mapModesBtn->addListener (this);

    midiStdNoteLabel.reset (new Label ("Standard Midi Note Label",
                                       TRANS("C4")));
    addAndMakeVisible (midiStdNoteLabel.get());
    midiStdNoteLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    midiStdNoteLabel->setJustificationType (Justification::centredLeft);
    midiStdNoteLabel->setEditable (false, false, false);
    midiStdNoteLabel->setColour (TextEditor::textColourId, Colours::black);
    midiStdNoteLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    midiStdNoteLabel2.reset (new Label ("Standard Midi Note Label",
                                        TRANS("C4")));
    addAndMakeVisible (midiStdNoteLabel2.get());
    midiStdNoteLabel2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    midiStdNoteLabel2->setJustificationType (Justification::centredLeft);
    midiStdNoteLabel2->setEditable (false, false, false);
    midiStdNoteLabel2->setColour (TextEditor::textColourId, Colours::black);
    midiStdNoteLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mapFullBtn.reset (new ToggleButton ("Map Full Button"));
    addAndMakeVisible (mapFullBtn.get());
    mapFullBtn->setTooltip (TRANS("Maps each new modal note to original modal notes, and approximates the notes in between."));
    mapFullBtn->setButtonText (TRANS("Mode to Mode"));
    mapFullBtn->setRadioGroupId (10);
    mapFullBtn->addListener (this);
    mapFullBtn->setToggleState (true, dontSendNotification);

    mapFullBtn->setBounds (1096, 608, 120, 24);

    mapModeToScaleBtn.reset (new ToggleButton ("Mode To Scale Button"));
    addAndMakeVisible (mapModeToScaleBtn.get());
    mapModeToScaleBtn->setTooltip (TRANS("Maps the full new mode to the full original scale. This is good when the new mode size is equal to the original scale size."));
    mapModeToScaleBtn->setButtonText (TRANS("Mode to Scale"));
    mapModeToScaleBtn->setRadioGroupId (10);
    mapModeToScaleBtn->addListener (this);

    mapModeOrdersBtn.reset (new ToggleButton ("Map Full Button"));
    addAndMakeVisible (mapModeOrdersBtn.get());
    mapModeOrdersBtn->setTooltip (TRANS("Maps the notes belonging to one modal order to another order."));
    mapModeOrdersBtn->setButtonText (TRANS("Mode by Order"));
    mapModeOrdersBtn->setRadioGroupId (10);
    mapModeOrdersBtn->addListener (this);


    //[UserPreSize]
    presetBox1->setMenu(*pluginState->presetManager->getPresetMenu());
	presetBox1->addListener(this);
    presetBox2->setMenu(*pluginState->presetManager->getPresetMenu());
	presetBox2->addListener(this);

	midiInputFilter = pluginState->getMidiProcessor()->getMidiInputFilter();
	midiOutputFilter = pluginState->getMidiProcessor()->getMidiOutputFilter();

    modeMapper = std::make_unique<ModeMapper>();

    /*
	remapTable.reset(new TableListBox("Midi Remap Table"));
	remapTableModel.reset(new MidiRemapTableModel(remapTable.get(), midiInputFilter, midiOutputFilter, pluginState->textFilterInt.get()));
	remapTable->setModel(remapTableModel.get());
	remapTable->setBounds(8, 184, 352, 264);

	remapTable->setHeader(new TableHeaderComponent());
	remapTable->getHeader().addColumn("Key", 1, remapTable->getWidth() * 0.2);
	remapTable->getHeader().addColumn("Input Note", 2, remapTable->getWidth() * 0.35);
	remapTable->getHeader().addColumn("Output Note", 3, remapTable->getWidth() * 0.35);

	addAndMakeVisible(remapTable.get());
     */

    //[/UserPreSize]

    setSize (560, 150);


    //[Constructor] You can add your own custom stuff here..

    //[/Constructor]
}

MidiSettingsComponent::~MidiSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //remapTable.release();
	//remapTableModel.release();

	presetBox1->removeListener(this);
	presetBox2->removeListener(this);
    //[/Destructor_pre]

    modeFromLbl = nullptr;
    modeToLbl = nullptr;
    rootFromSld = nullptr;
    rootToSld = nullptr;
    rootNoteLabel = nullptr;
    presetBox1 = nullptr;
    presetBox2 = nullptr;
    mapModesBtn = nullptr;
    midiStdNoteLabel = nullptr;
    midiStdNoteLabel2 = nullptr;
    mapFullBtn = nullptr;
    mapModeToScaleBtn = nullptr;
    mapModeOrdersBtn = nullptr;


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

    modeFromLbl->setBounds (640 + -3550, 664 + roundToInt (24 * 18452.7000f) - (21 / 2), 88, 21);
    modeToLbl->setBounds (640 + -3535, 696 + roundToInt (24 * -12217.8000f) - (24 / 2), 72, 24);
    rootFromSld->setBounds (640 + 150 - -15821, 664 + roundToInt (24 * -16508.8000f) - (24 / 2), 104, 24);
    rootToSld->setBounds (640 + 150 - -15925 - 104, 696 + roundToInt (24 * -19063.4000f) - (24 / 2), 104, 24);
    rootNoteLabel->setBounds (640 + -2992, 664 + -3531, 80, 24);
    mapModesBtn->setBounds (640 + 0, 736, roundToInt (150 * 1.0000f), 24);
    midiStdNoteLabel->setBounds (640 + 150 - -130 - 32, 664 + roundToInt (24 * -1.8333f) - (24 / 2), 32, 24);
    midiStdNoteLabel2->setBounds (640 + 150 - -35 - 32, (696 + roundToInt (24 * -19063.4000f) - (24 / 2)) + roundToInt (24 * 1.5000f) - (24 / 2), 32, 24);
    mapModeToScaleBtn->setBounds (1096, 608 + 24, 128, 24);
    mapModeOrdersBtn->setBounds (1096 + 0, (608 + 24) + 24, 128, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiSettingsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == rootFromSld.get())
    {
        //[UserSliderCode_rootFromSld] -- add your slider handling code here..
        midiStdNoteLabel->setText(MidiMessage::getMidiNoteName(rootFromSld->getValue(), true, true, 4), dontSendNotification);
        modeSelected1->setRootNote(rootFromSld->getValue());
        //[/UserSliderCode_rootFromSld]
    }
    else if (sliderThatWasMoved == rootToSld.get())
    {
        //[UserSliderCode_rootToSld] -- add your slider handling code here..
        midiStdNoteLabel2->setText(MidiMessage::getMidiNoteName(rootToSld->getValue(), true, true, 4), dontSendNotification);
        modeSelected2->setRootNote(rootToSld->getValue());
        //[/UserSliderCode_rootToSld]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MidiSettingsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == mapModesBtn.get())
    {
        //[UserButtonCode_mapModesBtn] -- add your button handler code here..
		if (modeSelected1.get() && modeSelected2.get())
		{
            NoteMap mapToUse = modeMapper->map(*modeSelected1.get(), *modeSelected2.get());
            pluginState->getMidiProcessor()->setMidiInputMap(mapToUse);
		}
        //[/UserButtonCode_mapModesBtn]
    }
    else if (buttonThatWasClicked == mapFullBtn.get())
    {
        //[UserButtonCode_mapFullBtn] -- add your button handler code here..
        updateModeMapper();
        //[/UserButtonCode_mapFullBtn]
    }
    else if (buttonThatWasClicked == mapModeToScaleBtn.get())
    {
        //[UserButtonCode_mapModeToScaleBtn] -- add your button handler code here..
        updateModeMapper();
        //[/UserButtonCode_mapModeToScaleBtn]
    }
    else if (buttonThatWasClicked == mapModeOrdersBtn.get())
    {
        //[UserButtonCode_mapModeOrdersBtn] -- add your button handler code here..
        updateModeMapper();
        //[/UserButtonCode_mapModeOrdersBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiSettingsComponent::updateModeMapper()
{
    if (mapFullBtn->getToggleState())
        modeMapper->setMapType(ModeMapper::ModeToMode);
    else if (mapModeToScaleBtn->getToggleState())
        modeMapper->setMapType(ModeMapper::ModeToScale);
    else if (mapModeOrdersBtn->getToggleState())
        modeMapper->setMapType(ModeMapper::ModeByOrder);
}

void MidiSettingsComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{

    if (comboBoxThatHasChanged == presetBox1.get())
    {
        if (presetBox1->getSelectedId() > 0)
        {
            ValueTree modeToLoad = pluginState->presetManager->getMode(presetBox2->getSelectedId()-1);
            modeSelected1 = std::make_unique<Mode>(modeToLoad);
            modeSelected1->setRootNote(rootFromSld->getValue());

        }
    }

    else if (comboBoxThatHasChanged == presetBox2.get())
    {
        if (presetBox2->getSelectedId() > 0)
        {
            ValueTree modeToLoad = pluginState->presetManager->getMode(presetBox2->getSelectedId()-1);
            modeSelected2 = std::make_unique<Mode>(modeToLoad);
            modeSelected2->setRootNote(rootToSld->getValue());
        }
        else
        {
            ValueTree modeToLoad = pluginState->getModeLoaded()->modeNode;
            modeSelected2 = std::make_unique<Mode>(modeToLoad);
            presetBox2->setText(modeSelected2->getDescription());
        }
    }
}

void MidiSettingsComponent::setMode1SelectedId(int selectedIdIn)
{
    presetBox1->setSelectedId(selectedIdIn);
}

void MidiSettingsComponent::setMode2SelectedId(int selectedIdIn)
{
	presetBox2->setSelectedId(selectedIdIn);
}

void MidiSettingsComponent::setMode1RootNote(int rootNoteIn)
{
    rootFromSld->setValue(rootNoteIn);
}

void MidiSettingsComponent::setMode2RootNote(int rootNoteIn)
{
    rootToSld->setValue(rootNoteIn);
}

void MidiSettingsComponent::refreshTables()
{
    //remapTable->updateContent();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiSettingsComponent" componentName="Midi Remapper Dialog"
                 parentClasses="public Component, public ComboBox::Listener" constructorParams="SvkPluginState* pluginStateIn"
                 variableInitialisers="pluginState(pluginStateIn)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="560" initialHeight="150">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="Mode From Label" id="5b2fda69d594937a" memberName="modeFromLbl"
         virtualName="" explicitFocusOrder="0" pos="-4190 1.57087e+06%c 88 21"
         posRelativeX="ee5b2def32e4fdbc" posRelativeY="ee5b2def32e4fdbc"
         edTextCol="ff000000" edBkgCol="0" labelText="From Mode:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="Mode To Label" id="44d358cbb3982e4" memberName="modeToLbl"
         virtualName="" explicitFocusOrder="0" pos="-4175 875615%c 72 24"
         posRelativeX="eb2e366c9ad8965c" posRelativeY="eb2e366c9ad8965c"
         edTextCol="ff000000" edBkgCol="0" labelText="To Mode:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="Root From Slider" id="c07be411c5642943" memberName="rootFromSld"
          virtualName="" explicitFocusOrder="0" pos="-17591R 1.31077e+06%c 104 24"
          posRelativeX="ee5b2def32e4fdbc" posRelativeY="ee5b2def32e4fdbc"
          min="0.0" max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="Root To Slider" id="c98578a060cce67f" memberName="rootToSld"
          virtualName="" explicitFocusOrder="0" pos="-17695Rr -1.06625e+06%c 104 24"
          posRelativeX="ee5b2def32e4fdbc" posRelativeY="eb2e366c9ad8965c"
          min="0.0" max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Root Note Label" id="effe36cc4475b201" memberName="rootNoteLabel"
         virtualName="" explicitFocusOrder="0" pos="-3632 -4195 80 24"
         posRelativeX="ee5b2def32e4fdbc" posRelativeY="ee5b2def32e4fdbc"
         edTextCol="ff000000" edBkgCol="0" labelText="Root Note:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="Preset Box 1" id="ee5b2def32e4fdbc" memberName="presetBox1"
                    virtualName="ReferencedComboBox" explicitFocusOrder="0" pos="640 664 150 24"
                    class="ComboBox" params=""/>
  <GENERICCOMPONENT name="Preset Box 2" id="eb2e366c9ad8965c" memberName="presetBox2"
                    virtualName="ReferencedComboBox" explicitFocusOrder="0" pos="640 696 150 24"
                    class="ComboBox" params=""/>
  <TEXTBUTTON name="Map Modes Button" id="7a76936bfde482aa" memberName="mapModesBtn"
              virtualName="" explicitFocusOrder="0" pos="0 736 100% 24" posRelativeX="eb2e366c9ad8965c"
              posRelativeW="eb2e366c9ad8965c" buttonText="Map!" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="Standard Midi Note Label" id="11131762b34ef489" memberName="midiStdNoteLabel"
         virtualName="" explicitFocusOrder="0" pos="-130Rr -183.333%c 32 24"
         posRelativeX="ee5b2def32e4fdbc" posRelativeY="ee5b2def32e4fdbc"
         edTextCol="ff000000" edBkgCol="0" labelText="C4" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Standard Midi Note Label" id="ae7f26feafef53e9" memberName="midiStdNoteLabel2"
         virtualName="" explicitFocusOrder="0" pos="-35Rr 150%c 32 24"
         posRelativeX="ee5b2def32e4fdbc" posRelativeY="c98578a060cce67f"
         edTextCol="ff000000" edBkgCol="0" labelText="C4" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="Map Full Button" id="13073042620ecb96" memberName="mapFullBtn"
                virtualName="" explicitFocusOrder="0" pos="1096 608 120 24" tooltip="Maps each new modal note to original modal notes, and approximates the notes in between."
                buttonText="Mode to Mode" connectedEdges="0" needsCallback="1"
                radioGroupId="10" state="1"/>
  <TOGGLEBUTTON name="Mode To Scale Button" id="9ae3cb9cbc095450" memberName="mapModeToScaleBtn"
                virtualName="" explicitFocusOrder="0" pos="1096 24 128 24" posRelativeY="13073042620ecb96"
                posRelativeW="13073042620ecb96" tooltip="Maps the full new mode to the full original scale. This is good when the new mode size is equal to the original scale size."
                buttonText="Mode to Scale" connectedEdges="0" needsCallback="1"
                radioGroupId="10" state="0"/>
  <TOGGLEBUTTON name="Map Full Button" id="29c32248e9bd2479" memberName="mapModeOrdersBtn"
                virtualName="" explicitFocusOrder="0" pos="0 24 128 24" posRelativeX="9ae3cb9cbc095450"
                posRelativeY="9ae3cb9cbc095450" tooltip="Maps the notes belonging to one modal order to another order."
                buttonText="Mode by Order" connectedEdges="0" needsCallback="1"
                radioGroupId="10" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...

//[/EndFile]

