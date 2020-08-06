/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MapByOrderDialog.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
MapByOrderDialog::MapByOrderDialog (ModeMapper* modeMapperIn, Mode* mode1In, Mode* mode2In)
    : modeMapper(modeMapperIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    mode1OrderBox.reset (new ComboBox ("Mode 1 Order Box"));
    addAndMakeVisible (mode1OrderBox.get());
    mode1OrderBox->setEditableText (false);
    mode1OrderBox->setJustificationType (Justification::centredLeft);
    mode1OrderBox->setTextWhenNothingSelected (String());
    mode1OrderBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode1OrderBox->addListener (this);

    mode1OrderBox->setBounds (76, 15, 63, 24);

    mode2OrderBox.reset (new ComboBox ("Mode 2 Order Box"));
    addAndMakeVisible (mode2OrderBox.get());
    mode2OrderBox->setEditableText (false);
    mode2OrderBox->setJustificationType (Justification::centredLeft);
    mode2OrderBox->setTextWhenNothingSelected (String());
    mode2OrderBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode2OrderBox->addListener (this);

    mode2OrderBox->setBounds (156, 15, 63, 24);

    orderSelectionLbl.reset (new Label ("Order Label",
                                        TRANS("Order:")));
    addAndMakeVisible (orderSelectionLbl.get());
    orderSelectionLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    orderSelectionLbl->setJustificationType (Justification::centredLeft);
    orderSelectionLbl->setEditable (false, false, false);
    orderSelectionLbl->setColour (TextEditor::textColourId, Colours::black);
    orderSelectionLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    orderSelectionLbl->setBounds (20, 14, 48, 24);

    orderOffsetSld1.reset (new Slider ("Order Offset Slider 1"));
    addAndMakeVisible (orderOffsetSld1.get());
    orderOffsetSld1->setRange (0, 10, 1);
    orderOffsetSld1->setSliderStyle (Slider::IncDecButtons);
    orderOffsetSld1->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 30);
    orderOffsetSld1->addListener (this);

    orderOffsetSld1->setBounds (76, 56, 64, 56);

    orderOffsetSld2.reset (new Slider ("Order Offset Slider 2"));
    addAndMakeVisible (orderOffsetSld2.get());
    orderOffsetSld2->setRange (0, 10, 1);
    orderOffsetSld2->setSliderStyle (Slider::IncDecButtons);
    orderOffsetSld2->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 30);
    orderOffsetSld2->addListener (this);

    orderOffsetSld2->setBounds (156, 56, 64, 56);

    label.reset (new Label ("new label",
                            TRANS("Degree\n"
                            "Offsets:")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (12, 48, 64, 40);


    //[UserPreSize]

	mode1 = mode1In;
	mode2 = mode2In;

    mode1OrderBox->clear();
    mode2OrderBox->clear();

    for (int i = 0; i < mode1->getMaxStep(); i++)
        mode1OrderBox->addItem(String(i), i+1);

    for (int i = 0; i < mode2->getMaxStep(); i++)
        mode2OrderBox->addItem(String(i), i+1);

    mode1OrderBox->setSelectedId(modeMapper->getMode1OrderNum() + 1, dontSendNotification);
    mode2OrderBox->setSelectedId(modeMapper->getMode2OrderNum() + 1, dontSendNotification);
    orderOffsetSld1->setValue(modeMapper->getMode1OrderOffset(), dontSendNotification);
    orderOffsetSld2->setValue(modeMapper->getMode2OrderOffset(), dontSendNotification);

    //[/UserPreSize]

    setSize (250, 130);


    //[Constructor] You can add your own custom stuff here..

    //[/Constructor]
}

MapByOrderDialog::~MapByOrderDialog()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    mode1OrderBox = nullptr;
    mode2OrderBox = nullptr;
    orderSelectionLbl = nullptr;
    orderOffsetSld1 = nullptr;
    orderOffsetSld2 = nullptr;
    label = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MapByOrderDialog::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MapByOrderDialog::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MapByOrderDialog::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == mode1OrderBox.get())
    {
        //[UserComboBoxCode_mode1OrderBox] -- add your combo box handling code here..
		modeMapper->setMode1OrderNum(mode1OrderBox->getSelectedId()-1);
        //[/UserComboBoxCode_mode1OrderBox]
    }
    else if (comboBoxThatHasChanged == mode2OrderBox.get())
    {
        //[UserComboBoxCode_mode2OrderBox] -- add your combo box handling code here..
		modeMapper->setMode2OrderNum(mode2OrderBox->getSelectedId()-1);
        //[/UserComboBoxCode_mode2OrderBox]
    }

    //[UsercomboBoxChanged_Post]

    //[/UsercomboBoxChanged_Post]
}

void MapByOrderDialog::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == orderOffsetSld1.get())
    {
        //[UserSliderCode_orderOffsetSld1] -- add your slider handling code here..
		modeMapper->setMode1OrderOffset(orderOffsetSld1->getValue());
        //[/UserSliderCode_orderOffsetSld1]
    }
    else if (sliderThatWasMoved == orderOffsetSld2.get())
    {
        //[UserSliderCode_orderOffsetSld2] -- add your slider handling code here..
		modeMapper->setMode2OrderOffset(orderOffsetSld2->getValue());
        //[/UserSliderCode_orderOffsetSld2]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MapByOrderDialog" componentName=""
                 parentClasses="public Component" constructorParams="SvkPluginState* pluginStateIn"
                 variableInitialisers="pluginState(pluginStateIn)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="250" initialHeight="130">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="Mode 1 Order Box" id="a6f2553e2e1863be" memberName="mode1OrderBox"
            virtualName="" explicitFocusOrder="0" pos="76 15 63 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="Mode 2 Order Box" id="7532edff0093d46e" memberName="mode2OrderBox"
            virtualName="" explicitFocusOrder="0" pos="156 15 63 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="Order Label" id="29afd7379cf490f4" memberName="orderSelectionLbl"
         virtualName="" explicitFocusOrder="0" pos="20 14 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Order:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="Order Offset Slider 1" id="e9b30506aa92202b" memberName="orderOffsetSld1"
          virtualName="" explicitFocusOrder="0" pos="76 56 64 56" min="0.0"
          max="10.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="30" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="Order Offset Slider 2" id="4e17d241f1693224" memberName="orderOffsetSld2"
          virtualName="" explicitFocusOrder="0" pos="156 56 64 56" min="0.0"
          max="10.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="30" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="6391dd5cf3f8590f" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="12 48 64 40" edTextCol="ff000000"
         edBkgCol="0" labelText="Degree&#10;Offsets:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...

//[/EndFile]

