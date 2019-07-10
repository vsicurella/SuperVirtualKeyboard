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

    presetBox1->setBounds (88, 32, 150, 24);

    presetBox2.reset (new ReferencedComboBox());
    addAndMakeVisible (presetBox2.get());
    presetBox2->setName ("Preset Box 2");

    presetBox2->setBounds (88, 64, 150, 24);

    mapModesBtn.reset (new TextButton ("Map Modes Button"));
    addAndMakeVisible (mapModesBtn.get());
    mapModesBtn->setButtonText (TRANS("Map!"));
    mapModesBtn->addListener (this);

    mapModesBtn->setBounds (88, 101, 152, 24);

    mapFullBtn.reset (new ToggleButton ("Map Full Button"));
    addAndMakeVisible (mapFullBtn.get());
    mapFullBtn->setTooltip (TRANS("Maps each new modal note to original modal notes, and approximates the notes in between."));
    mapFullBtn->setButtonText (TRANS("Mode to Mode"));
    mapFullBtn->setRadioGroupId (10);
    mapFullBtn->addListener (this);
    mapFullBtn->setToggleState (true, dontSendNotification);

    mapFullBtn->setBounds (432, 29, 120, 24);

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

    rootFromSld.reset (new Slider ("Root To Slidrer"));
    addAndMakeVisible (rootFromSld.get());
    rootFromSld->setRange (0, 127, 1);
    rootFromSld->setSliderStyle (Slider::IncDecButtons);
    rootFromSld->setTextBoxStyle (Slider::TextBoxRight, false, 60, 20);
    rootFromSld->addListener (this);

    rootFromSld->setBounds (247, 32, 136, 24);

    rootToSld.reset (new Slider ("Root To Slidrer"));
    addAndMakeVisible (rootToSld.get());
    rootToSld->setRange (0, 127, 1);
    rootToSld->setSliderStyle (Slider::IncDecButtons);
    rootToSld->setTextBoxStyle (Slider::TextBoxRight, false, 60, 20);
    rootToSld->addListener (this);

    rootToSld->setBounds (247, 64, 136, 24);

    rootNoteFromLbl.reset (new Label ("new label",
                                      TRANS("C4")));
    addAndMakeVisible (rootNoteFromLbl.get());
    rootNoteFromLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    rootNoteFromLbl->setJustificationType (Justification::centredLeft);
    rootNoteFromLbl->setEditable (false, false, false);
    rootNoteFromLbl->setColour (TextEditor::textColourId, Colours::black);
    rootNoteFromLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    rootNoteFromLbl->setBounds (384, 30, 48, 24);

    rootNoteToLbl.reset (new Label ("new label",
                                    TRANS("C4")));
    addAndMakeVisible (rootNoteToLbl.get());
    rootNoteToLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    rootNoteToLbl->setJustificationType (Justification::centredLeft);
    rootNoteToLbl->setEditable (false, false, false);
    rootNoteToLbl->setColour (TextEditor::textColourId, Colours::black);
    rootNoteToLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    rootNoteToLbl->setBounds (384, 64, 48, 24);

    ModeMapTypeLbl.reset (new Label ("Mode Map Type Label",
                                     TRANS("Mapping Type:")));
    addAndMakeVisible (ModeMapTypeLbl.get());
    ModeMapTypeLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    ModeMapTypeLbl->setJustificationType (Justification::centredLeft);
    ModeMapTypeLbl->setEditable (false, false, false);
    ModeMapTypeLbl->setColour (TextEditor::textColourId, Colours::black);
    ModeMapTypeLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    ModeMapTypeLbl->setBounds (432, 5, 103, 24);

    mode1OrderBox.reset (new ComboBox ("Mode 1 Order Box"));
    addAndMakeVisible (mode1OrderBox.get());
    mode1OrderBox->setEditableText (false);
    mode1OrderBox->setJustificationType (Justification::centredLeft);
    mode1OrderBox->setTextWhenNothingSelected (String());
    mode1OrderBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode1OrderBox->addListener (this);

    mode1OrderBox->setBounds (618, 7, 63, 24);

    mode2OrderBox.reset (new ComboBox ("Mode 2 Order Box"));
    addAndMakeVisible (mode2OrderBox.get());
    mode2OrderBox->setEditableText (false);
    mode2OrderBox->setJustificationType (Justification::centredLeft);
    mode2OrderBox->setTextWhenNothingSelected (String());
    mode2OrderBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode2OrderBox->addListener (this);

    mode2OrderBox->setBounds (698, 7, 63, 24);

    orderSelectionLbl.reset (new Label ("Order Label",
                                        TRANS("Order:")));
    addAndMakeVisible (orderSelectionLbl.get());
    orderSelectionLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    orderSelectionLbl->setJustificationType (Justification::centredLeft);
    orderSelectionLbl->setEditable (false, false, false);
    orderSelectionLbl->setColour (TextEditor::textColourId, Colours::black);
    orderSelectionLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    orderSelectionLbl->setBounds (562, 6, 48, 24);

    orderOffsetSld1.reset (new Slider ("Order Offset Slider 1"));
    addAndMakeVisible (orderOffsetSld1.get());
    orderOffsetSld1->setRange (0, 10, 1);
    orderOffsetSld1->setSliderStyle (Slider::IncDecButtons);
    orderOffsetSld1->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 30);
    orderOffsetSld1->addListener (this);

    orderOffsetSld1->setBounds (618, 48, 64, 56);

    orderOffsetSld2.reset (new Slider ("Order Offset Slider 2"));
    addAndMakeVisible (orderOffsetSld2.get());
    orderOffsetSld2->setRange (0, 10, 1);
    orderOffsetSld2->setSliderStyle (Slider::IncDecButtons);
    orderOffsetSld2->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 30);
    orderOffsetSld2->addListener (this);

    orderOffsetSld2->setBounds (698, 48, 64, 56);


    //[UserPreSize]
    midiSettingsNode = pluginState->midiSettingsNode;
    
    presetBox1->setMenu(*pluginState->presetManager->getMode2Menu());
	presetBox1->addListener(this);
    presetBox2->setMenu(*pluginState->presetManager->getMode2Menu());
	presetBox2->addListener(this);

	midiInputFilter = pluginState->getMidiProcessor()->getMidiInputFilter();
	midiOutputFilter = pluginState->getMidiProcessor()->getMidiOutputFilter();

    modeMapper = std::make_unique<ModeMapper>();

    setOrderMappingVisibility(false);
    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..

    //[/Constructor]
}

MidiSettingsComponent::~MidiSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	presetBox1->removeListener(this);
	presetBox2->removeListener(this);
    //[/Destructor_pre]

    modeFromLbl = nullptr;
    modeToLbl = nullptr;
    rootNoteLabel = nullptr;
    presetBox1 = nullptr;
    presetBox2 = nullptr;
    mapModesBtn = nullptr;
    mapFullBtn = nullptr;
    mapModeToScaleBtn = nullptr;
    mapModeOrdersBtn = nullptr;
    rootFromSld = nullptr;
    rootToSld = nullptr;
    rootNoteFromLbl = nullptr;
    rootNoteToLbl = nullptr;
    ModeMapTypeLbl = nullptr;
    mode1OrderBox = nullptr;
    mode2OrderBox = nullptr;
    orderSelectionLbl = nullptr;
    orderOffsetSld1 = nullptr;
    orderOffsetSld2 = nullptr;


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

    modeFromLbl->setBounds (0, 42 - (21 / 2), 88, 21);
    modeToLbl->setBounds (16, 76 - (24 / 2), 72, 24);
    rootNoteLabel->setBounds (88 + 160, 5, 80, 24);
    mapModeToScaleBtn->setBounds (432 + 0, 29 + 24, 128, 24);
    mapModeOrdersBtn->setBounds (432 + 0, 29 + 48, 128, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
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
			NoteMap* prevMap = pluginState->getMidiProcessor()->getInputNoteMap();
			NoteMap mapToUse = modeMapper->map(*modeSelected1.get(), *modeSelected2.get(),
				mode1OrderBox->getSelectedId() - 1, mode2OrderBox->getSelectedId() - 1,
				orderOffsetSld1->getValue(), orderOffsetSld2->getValue(),
				*prevMap);

            pluginState->setMidiInputMap(mapToUse);
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

void MidiSettingsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == rootFromSld.get())
    {
        //[UserSliderCode_rootFromSld] -- add your slider handling code here..
        rootNoteFromLbl->setText(MidiMessage::getMidiNoteName(rootFromSld->getValue(), true, true, 4), dontSendNotification);
        modeSelected1->setRootNote(rootFromSld->getValue());
        //[/UserSliderCode_rootFromSld]
    }
    else if (sliderThatWasMoved == rootToSld.get())
    {
        //[UserSliderCode_rootToSld] -- add your slider handling code here..
        rootNoteToLbl->setText(MidiMessage::getMidiNoteName(rootToSld->getValue(), true, true, 4), dontSendNotification);
        modeSelected2->setRootNote(rootToSld->getValue());
        //[/UserSliderCode_rootToSld]
    }
    else if (sliderThatWasMoved == orderOffsetSld1.get())
    {
        //[UserSliderCode_orderOffsetSld1] -- add your slider handling code here..

        //[/UserSliderCode_orderOffsetSld1]
    }
    else if (sliderThatWasMoved == orderOffsetSld2.get())
    {
        //[UserSliderCode_orderOffsetSld2] -- add your slider handling code here..
        //[/UserSliderCode_orderOffsetSld2]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MidiSettingsComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    if (comboBoxThatHasChanged == presetBox1.get())
    {
        if (presetBox1->getSelectedId() > 0)
        {
            ValueTree modeToLoad = pluginState->presetManager->getModeInLibrary(presetBox1->getSelectedId()-1);
            modeSelected1 = std::make_unique<Mode>(modeToLoad);
            modeSelected1->setRootNote(rootFromSld->getValue());

            mode1OrderBox->getRootMenu()->clear();

            for (int i = 0; i < modeSelected1->getMaxStep(); i++)
                mode1OrderBox->addItem(String(i), i+1);
        }
        
        
    }

    else if (comboBoxThatHasChanged == presetBox2.get())
    {
        if (presetBox2->getSelectedId() > 0)
        {
            ValueTree modeToLoad = pluginState->presetManager->getModeInLibrary(presetBox2->getSelectedId()-1);
            modeSelected2 = std::make_unique<Mode>(modeToLoad);
            modeSelected2->setRootNote(rootToSld->getValue());
        }
        else
        {
            ValueTree modeToLoad = pluginState->getModeLoaded()->modeNode;
            modeSelected2 = std::make_unique<Mode>(modeToLoad);
            presetBox2->setText(modeSelected2->getDescription());
        }

        mode2OrderBox->getRootMenu()->clear();

        for (int i = 0; i < modeSelected2->getMaxStep(); i++)
            mode2OrderBox->addItem(String(i), i+1);
    }
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == mode1OrderBox.get())
    {
        //[UserComboBoxCode_mode1OrderBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_mode1OrderBox]
    }
    else if (comboBoxThatHasChanged == mode2OrderBox.get())
    {
        //[UserComboBoxCode_mode2OrderBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_mode2OrderBox]
    }

    //[UsercomboBoxChanged_Post]
    
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

/*
void MidiSettingsComponent::visibilityChanged(bool isVisible)
{
    midiSettingsNode = pluginState->midiSettingsNode;
}
*/
void MidiSettingsComponent::setMode1(int selectedIdIn)
{
    modeSelected1.reset(new Mode(pluginState->presetManager->getModeInLibrary(selectedIdIn)));
    presetBox1->setSelectedId(selectedIdIn);
}

void MidiSettingsComponent::setMode1(ValueTree modeNodeIn)
{
    modeSelected1.reset(new Mode(modeNodeIn));
    presetBox1->setText(modeSelected1->getName());
}

void MidiSettingsComponent::setMode1(Mode* modeIn)
{
    setMode1(modeIn->modeNode);
}

void MidiSettingsComponent::setMode2(int selectedIdIn)
{
    modeSelected1.reset(new Mode(pluginState->presetManager->getModeInLibrary(selectedIdIn)));
    presetBox2->setSelectedId(selectedIdIn);
}

void MidiSettingsComponent::setMode2(ValueTree modeNodeIn)
{
    modeSelected2.reset(new Mode(modeNodeIn));
    presetBox1->setText(modeSelected2->getName());
}

void MidiSettingsComponent::setMode2(Mode* modeIn)
{
    setMode2(modeIn->modeNode);
}

void MidiSettingsComponent::updateModeMapper()
{
    if (mapFullBtn->getToggleState())
    {
        modeMapper->setMapType(ModeMapper::ModeToMode);
    }

    else if (mapModeToScaleBtn->getToggleState())
    {
        modeMapper->setMapType(ModeMapper::ModeToScale);
    }

    else if (mapModeOrdersBtn->getToggleState())
    {
        modeMapper->setMapType(ModeMapper::ModeByOrder);
    }

    setOrderMappingVisibility(mapModeOrdersBtn->getToggleState());
}

void MidiSettingsComponent::setMode1RootNote(int rootNoteIn)
{
    rootFromSld->setValue(rootNoteIn);
}

void MidiSettingsComponent::setMode2RootNote(int rootNoteIn)
{
    rootToSld->setValue(rootNoteIn);
}

void MidiSettingsComponent::setOrderMappingVisibility(bool isVisible)
{
    mode1OrderBox->setVisible(isVisible);
    mode2OrderBox->setVisible(isVisible);
    orderOffsetSld1->setVisible(isVisible);
    orderOffsetSld2->setVisible(isVisible);

    if (isVisible)
        setSize(785, getHeight());
    else
        setSize(560, getHeight());
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
  <LABEL name="Mode From Label" id="5b2fda69d594937a" memberName="modeFromLbl"
         virtualName="" explicitFocusOrder="0" pos="0 42.5c 88 21" edTextCol="ff000000"
         edBkgCol="0" labelText="From Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="Mode To Label" id="44d358cbb3982e4" memberName="modeToLbl"
         virtualName="" explicitFocusOrder="0" pos="16 76c 72 24" edTextCol="ff000000"
         edBkgCol="0" labelText="To Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="Root Note Label" id="effe36cc4475b201" memberName="rootNoteLabel"
         virtualName="" explicitFocusOrder="0" pos="160 5 80 24" posRelativeX="ee5b2def32e4fdbc"
         edTextCol="ff000000" edBkgCol="0" labelText="Root Note:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="Preset Box 1" id="ee5b2def32e4fdbc" memberName="presetBox1"
                    virtualName="ReferencedComboBox" explicitFocusOrder="0" pos="88 32 150 24"
                    class="ComboBox" params=""/>
  <GENERICCOMPONENT name="Preset Box 2" id="eb2e366c9ad8965c" memberName="presetBox2"
                    virtualName="ReferencedComboBox" explicitFocusOrder="0" pos="88 64 150 24"
                    class="ComboBox" params=""/>
  <TEXTBUTTON name="Map Modes Button" id="7a76936bfde482aa" memberName="mapModesBtn"
              virtualName="" explicitFocusOrder="0" pos="88 101 152 24" buttonText="Map!"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="Map Full Button" id="13073042620ecb96" memberName="mapFullBtn"
                virtualName="" explicitFocusOrder="0" pos="432 29 120 24" tooltip="Maps each new modal note to original modal notes, and approximates the notes in between."
                buttonText="Mode to Mode" connectedEdges="0" needsCallback="1"
                radioGroupId="10" state="1"/>
  <TOGGLEBUTTON name="Mode To Scale Button" id="9ae3cb9cbc095450" memberName="mapModeToScaleBtn"
                virtualName="" explicitFocusOrder="0" pos="0 24 128 24" posRelativeX="13073042620ecb96"
                posRelativeY="13073042620ecb96" posRelativeW="13073042620ecb96"
                tooltip="Maps the full new mode to the full original scale. This is good when the new mode size is equal to the original scale size."
                buttonText="Mode to Scale" connectedEdges="0" needsCallback="1"
                radioGroupId="10" state="0"/>
  <TOGGLEBUTTON name="Map Full Button" id="29c32248e9bd2479" memberName="mapModeOrdersBtn"
                virtualName="" explicitFocusOrder="0" pos="0 48 128 24" posRelativeX="13073042620ecb96"
                posRelativeY="13073042620ecb96" posRelativeH="13073042620ecb96"
                tooltip="Maps the notes belonging to one modal order to another order."
                buttonText="Mode by Order" connectedEdges="0" needsCallback="1"
                radioGroupId="10" state="0"/>
  <SLIDER name="Root To Slidrer" id="e054f1f854595a3f" memberName="rootFromSld"
          virtualName="" explicitFocusOrder="0" pos="247 32 136 24" min="0.0"
          max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="Root To Slidrer" id="c149a995958ccfb0" memberName="rootToSld"
          virtualName="" explicitFocusOrder="0" pos="247 64 136 24" min="0.0"
          max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="527ed968b95a71d8" memberName="rootNoteFromLbl"
         virtualName="" explicitFocusOrder="0" pos="384 30 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="C4" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="d317d95427cdd2e3" memberName="rootNoteToLbl"
         virtualName="" explicitFocusOrder="0" pos="384 64 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="C4" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Mode Map Type Label" id="a1790ac734607ce8" memberName="ModeMapTypeLbl"
         virtualName="" explicitFocusOrder="0" pos="432 5 103 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mapping Type:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="Mode 1 Order Box" id="a6f2553e2e1863be" memberName="mode1OrderBox"
            virtualName="" explicitFocusOrder="0" pos="618 7 63 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="Mode 2 Order Box" id="7532edff0093d46e" memberName="mode2OrderBox"
            virtualName="" explicitFocusOrder="0" pos="698 7 63 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="Order Label" id="29afd7379cf490f4" memberName="orderSelectionLbl"
         virtualName="" explicitFocusOrder="0" pos="562 6 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Order:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="Order Offset Slider 1" id="e9b30506aa92202b" memberName="orderOffsetSld1"
          virtualName="" explicitFocusOrder="0" pos="618 48 64 56" min="0.0"
          max="10.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="30" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="Order Offset Slider 2" id="4e17d241f1693224" memberName="orderOffsetSld2"
          virtualName="" explicitFocusOrder="0" pos="698 48 64 56" min="0.0"
          max="10.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="30" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...

//[/EndFile]

