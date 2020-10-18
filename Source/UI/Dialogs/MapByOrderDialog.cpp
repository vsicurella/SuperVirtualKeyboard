/*
  ==============================================================================

  Created with Projucer version: 5.4.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#include "MapByOrderDialog.h"

//==============================================================================
MapByOrderDialog::MapByOrderDialog (ModeMapper* modeMapperIn, Mode* mode1In, Mode* mode2In)
    : modeMapper(modeMapperIn)
{

    mode1OrderBox.reset (new ComboBox ("Mode 1 Order Box"));
    addAndMakeVisible (mode1OrderBox.get());
    mode1OrderBox->setEditableText (false);
    mode1OrderBox->setJustificationType (Justification::centredLeft);
    mode1OrderBox->setTextWhenNothingSelected (String());
    mode1OrderBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode1OrderBox->addListener (this);

    mode1OrderBox->setBounds (76, 25, 63, 24);

    mode2OrderBox.reset (new ComboBox ("Mode 2 Order Box"));
    addAndMakeVisible (mode2OrderBox.get());
    mode2OrderBox->setEditableText (false);
    mode2OrderBox->setJustificationType (Justification::centredLeft);
    mode2OrderBox->setTextWhenNothingSelected (String());
    mode2OrderBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode2OrderBox->addListener (this);

    mode2OrderBox->setBounds (156, 25, 63, 24);

    orderSelectionLbl.reset (new Label ("Order Label",
                                        TRANS("Layer:")));
    addAndMakeVisible (orderSelectionLbl.get());
    orderSelectionLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    orderSelectionLbl->setJustificationType (Justification::centredLeft);
    orderSelectionLbl->setEditable (false, false, false);
    orderSelectionLbl->setColour (TextEditor::textColourId, Colours::black);
    orderSelectionLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    orderSelectionLbl->setBounds (20, 24, 48, 24);

    orderOffsetSld1.reset (new Slider ("Order Offset Slider 1"));
    addAndMakeVisible (orderOffsetSld1.get());
    orderOffsetSld1->setRange (0, 10, 1);
    orderOffsetSld1->setSliderStyle (Slider::IncDecButtons);
    orderOffsetSld1->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 30);
    orderOffsetSld1->addListener (this);

    orderOffsetSld1->setBounds (76, 66, 64, 56);

    orderOffsetSld2.reset (new Slider ("Order Offset Slider 2"));
    addAndMakeVisible (orderOffsetSld2.get());
    orderOffsetSld2->setRange (0, 10, 1);
    orderOffsetSld2->setSliderStyle (Slider::IncDecButtons);
    orderOffsetSld2->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 30);
    orderOffsetSld2->addListener (this);

    orderOffsetSld2->setBounds (156, 66, 64, 56);

    offsetLabel.reset (new Label ("new label",
                            TRANS("Degree\n"
                            "Offsets:")));
    addAndMakeVisible (offsetLabel.get());
    offsetLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    offsetLabel->setJustificationType (Justification::centredLeft);
    offsetLabel->setEditable (false, false, false);
    offsetLabel->setColour (TextEditor::textColourId, Colours::black);
    offsetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    offsetLabel->setBounds (12, 58, 64, 40);

    inputLabel.reset(new Label("Input Label", TRANS("In")));
    addAndMakeVisible(inputLabel.get());
    inputLabel->setJustificationType(Justification::centred);
    inputLabel->setBounds(mode1OrderBox->getX(), mode1OrderBox->getY() - 28, mode1OrderBox->getWidth(), 30);

    outputLabel.reset(new Label("Output Label", TRANS("Out")));
    addAndMakeVisible(outputLabel.get());
    outputLabel->setJustificationType(Justification::centred);
    outputLabel->setBounds(mode2OrderBox->getX(), mode2OrderBox->getY() - 28, mode2OrderBox->getWidth(), 30);

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

    setSize (250, 130);
}

MapByOrderDialog::~MapByOrderDialog()
{
    mode1OrderBox = nullptr;
    mode2OrderBox = nullptr;
    orderSelectionLbl = nullptr;
    orderOffsetSld1 = nullptr;
    orderOffsetSld2 = nullptr;
    offsetLabel = nullptr;
}

//==============================================================================
void MapByOrderDialog::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
}

void MapByOrderDialog::resized()
{
}

void MapByOrderDialog::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == mode1OrderBox.get())
    {
        modeMapper->setMode1OrderNum(mode1OrderBox->getSelectedId()-1);
    }
    else if (comboBoxThatHasChanged == mode2OrderBox.get())
    {
        modeMapper->setMode2OrderNum(mode2OrderBox->getSelectedId()-1);
    }
}

void MapByOrderDialog::sliderValueChanged (Slider* sliderThatWasMoved)
{

    if (sliderThatWasMoved == orderOffsetSld1.get())
    {
        modeMapper->setMode1OrderOffset(orderOffsetSld1->getValue());
    }
    else if (sliderThatWasMoved == orderOffsetSld2.get())
    {
        modeMapper->setMode2OrderOffset(orderOffsetSld2->getValue());
    }
}
