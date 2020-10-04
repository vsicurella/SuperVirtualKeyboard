/*
  ==============================================================================

  ModeInfoDialog.cpp
  Vincenzo Sicurella

  ==============================================================================
*/

#include "ModeInfoDialog.h"

//==============================================================================
ModeInfoDialog::ModeInfoDialog (Mode* modeIn)
    : modeOriginal(modeIn), modeWorking(Mode(modeIn->modeNode)), modeNode(modeWorking.modeNode)
{
    familyBox.reset (new TextEditor ("Family Box"));
    addAndMakeVisible (familyBox.get());
    familyBox->setMultiLine (false);
    familyBox->setReturnKeyStartsNewLine (false);
    familyBox->setReadOnly (false);
    familyBox->setScrollbarsShown (true);
    familyBox->setCaretVisible (true);
    familyBox->setPopupMenuEnabled (true);
    familyBox->setText (TRANS("Meantone"));

    familyBox->setBounds (8, 168, 192, 24);

    stepsBox.reset (new TextEditor ("Steps Box"));
    addAndMakeVisible (stepsBox.get());
    stepsBox->setMultiLine (false);
    stepsBox->setReturnKeyStartsNewLine (false);
    stepsBox->setReadOnly (true);
    stepsBox->setScrollbarsShown (true);
    stepsBox->setCaretVisible (false);
    stepsBox->setPopupMenuEnabled (true);
    stepsBox->setText (TRANS("2 2 1 2 2 2 1"));

    stepsBox->setBounds (8, 40, 192, 24);

    infoBox.reset (new TextEditor ("Info Box"));
    addAndMakeVisible (infoBox.get());
    infoBox->setMultiLine (true);
    infoBox->setReturnKeyStartsNewLine (false);
    infoBox->setReadOnly (false);
    infoBox->setScrollbarsShown (true);
    infoBox->setCaretVisible (true);
    infoBox->setPopupMenuEnabled (true);
    infoBox->setText (TRANS("Ever hear of it?"));

    infoBox->setBounds (8, 336, 320, 128);

    familyNameLbl.reset (new Label ("Family Name Label",
                                    TRANS("Temperament Family:\n")));
    addAndMakeVisible (familyNameLbl.get());
    familyNameLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    familyNameLbl->setJustificationType (Justification::centredLeft);
    familyNameLbl->setEditable (false, false, false);
    familyNameLbl->setColour (TextEditor::textColourId, Colours::black);
    familyNameLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    familyNameLbl->setBounds (8, 136, 150, 24);

    stepsLbl.reset (new Label ("Steps Label",
                               TRANS("Steps:\n")));
    addAndMakeVisible (stepsLbl.get());
    stepsLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    stepsLbl->setJustificationType (Justification::centredLeft);
    stepsLbl->setEditable (false, false, false);
    stepsLbl->setColour (TextEditor::textColourId, Colours::black);
    stepsLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    stepsLbl->setBounds (8, 8, 56, 24);

    intervalSizesLbl.reset (new Label ("Interval Sizes Label",
                                       TRANS("Interval Sizes:")));
    addAndMakeVisible (intervalSizesLbl.get());
    intervalSizesLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    intervalSizesLbl->setJustificationType (Justification::centredLeft);
    intervalSizesLbl->setEditable (false, false, false);
    intervalSizesLbl->setColour (TextEditor::textColourId, Colours::black);
    intervalSizesLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    intervalSizesLbl->setBounds (8, 272, 95, 24);

    nameLabel.reset (new Label ("Name Label",
                                TRANS("Mode Name:")));
    addAndMakeVisible (nameLabel.get());
    nameLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    nameLabel->setJustificationType (Justification::centredLeft);
    nameLabel->setEditable (false, false, false);
    nameLabel->setColour (TextEditor::textColourId, Colours::black);
    nameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    nameLabel->setBounds (9, 73, 96, 24);

    modeSizeLbl.reset (new Label ("Mode Size Label",
                                  TRANS("Mode Size:")));
    addAndMakeVisible (modeSizeLbl.get());
    modeSizeLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeSizeLbl->setJustificationType (Justification::centredLeft);
    modeSizeLbl->setEditable (false, false, false);
    modeSizeLbl->setColour (TextEditor::textColourId, Colours::black);
    modeSizeLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeSizeLbl->setBounds (8, 240, 80, 24);

    scaleSizeLbl.reset (new Label ("Scale Size Label",
                                   TRANS("Scale Size:")));
    addAndMakeVisible (scaleSizeLbl.get());
    scaleSizeLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    scaleSizeLbl->setJustificationType (Justification::centredLeft);
    scaleSizeLbl->setEditable (false, false, false);
    scaleSizeLbl->setColour (TextEditor::textColourId, Colours::black);
    scaleSizeLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    scaleSizeLbl->setBounds (8, 208, 71, 24);

    defaultNameBtn.reset (new ToggleButton ("Default Name Button"));
    addAndMakeVisible (defaultNameBtn.get());
    defaultNameBtn->setButtonText (TRANS("Use Default"));
    defaultNameBtn->addListener (this);
    defaultNameBtn->setToggleState (true, dontSendNotification);

    defaultNameBtn->setBounds (217, 105, 111, 24);

    infoLbl.reset (new Label ("Info Label",
                              TRANS("Info:")));
    addAndMakeVisible (infoLbl.get());
    infoLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    infoLbl->setJustificationType (Justification::centredLeft);
    infoLbl->setEditable (false, false, false);
    infoLbl->setColour (TextEditor::textColourId, Colours::black);
    infoLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    infoLbl->setBounds (8, 304, 40, 24);

    nameBox.reset (new TextEditor ("Name Box"));
    addAndMakeVisible (nameBox.get());
    nameBox->setMultiLine (false);
    nameBox->setReturnKeyStartsNewLine (false);
    nameBox->setReadOnly (false);
    nameBox->setScrollbarsShown (true);
    nameBox->setCaretVisible (true);
    nameBox->setPopupMenuEnabled (true);
    nameBox->setText (TRANS("Meantone[7] 12"));

    nameBox->setBounds (9, 105, 192, 24);

    saveButton.reset (new TextButton ("Save Button"));
    addAndMakeVisible (saveButton.get());
    saveButton->setButtonText (TRANS("Save"));
    saveButton->addListener (this);

    saveButton->setBounds (32, 488, 104, 24);

    rotateSld.reset (new Slider ("Rotate Slider"));
    addAndMakeVisible (rotateSld.get());
    rotateSld->setRange (-3, 4, 1);
    rotateSld->setSliderStyle (Slider::IncDecButtons);
    rotateSld->setTextBoxStyle (Slider::TextBoxLeft, false, 48, 20);
    rotateSld->addListener (this);

    rotateSld->setBounds (216, 40, 96, 24);

    modeRotateLbl.reset (new Label ("Mode Rotate Label",
                                    TRANS("Shift Mode:")));
    addAndMakeVisible (modeRotateLbl.get());
    modeRotateLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeRotateLbl->setJustificationType (Justification::centredLeft);
    modeRotateLbl->setEditable (false, false, false);
    modeRotateLbl->setColour (TextEditor::textColourId, Colours::black);
    modeRotateLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeRotateLbl->setBounds (216, 8, 103, 24);

    scaleSizeReadout.reset (new Label ("Scale Size Readout",
                                       TRANS("12\n")));
    addAndMakeVisible (scaleSizeReadout.get());
    scaleSizeReadout->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    scaleSizeReadout->setJustificationType (Justification::centredLeft);
    scaleSizeReadout->setEditable (false, false, false);
    scaleSizeReadout->setColour (TextEditor::textColourId, Colours::black);
    scaleSizeReadout->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    scaleSizeReadout->setBounds (88, 208, 31, 24);

    modeSizeReadout.reset (new Label ("Mode Size Readout",
                                      TRANS("7")));
    addAndMakeVisible (modeSizeReadout.get());
    modeSizeReadout->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeSizeReadout->setJustificationType (Justification::centredLeft);
    modeSizeReadout->setEditable (false, false, false);
    modeSizeReadout->setColour (TextEditor::textColourId, Colours::black);
    modeSizeReadout->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    modeSizeReadout->setBounds (88, 240, 103, 24);

    intervalSizeReadout.reset (new Label ("Interval Size Readout",
                                          TRANS("7, 5")));
    addAndMakeVisible (intervalSizeReadout.get());
    intervalSizeReadout->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    intervalSizeReadout->setJustificationType (Justification::centredLeft);
    intervalSizeReadout->setEditable (false, false, false);
    intervalSizeReadout->setColour (TextEditor::textColourId, Colours::black);
    intervalSizeReadout->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    intervalSizeReadout->setBounds (104, 272, 143, 24);

    closeButton.reset (new TextButton ("closeButton"));
    addAndMakeVisible (closeButton.get());
    closeButton->setButtonText (TRANS("Close"));
    closeButton->addListener (this);

    closeButton->setBounds (192, 488, 104, 24);

    rotateSld->setIncDecButtonsMode(Slider::IncDecButtonMode::incDecButtonsDraggable_Horizontal);

    familyBox->addListener(this);

    stepsBox->setText(modeWorking.getStepsString());
    familyBox->setText(modeWorking.getFamily());

    nameBox->setText(modeWorking.getName());
    nameBox->setEnabled(false);

    defaultNameBtn->setClickingTogglesState(true);
    if (modeWorking.getName() != modeWorking.getDescription())
        defaultNameBtn->setToggleState(false, sendNotification);

    scaleSizeReadout->setText(String(modeWorking.getScaleSize()), dontSendNotification);
    modeSizeReadout->setText(String(modeWorking.getModeSize()), dontSendNotification);

    intervalSizeReadout->setText(arrayToString(modeWorking.getIntervalSizeCount()), dontSendNotification);

    infoBox->setText(modeWorking.getInfo());

    if (modeWorking.getModeSize() > 1)
        rotateSld->setRange(-(modeWorking.getModeSize() - 1), modeWorking.getModeSize() - 1, 1);
    else
        rotateSld->setEnabled(false);

    setSize (340, 525);
}

ModeInfoDialog::~ModeInfoDialog()
{
    removeAllChangeListeners();

    familyBox = nullptr;
    stepsBox = nullptr;
    infoBox = nullptr;
    familyNameLbl = nullptr;
    stepsLbl = nullptr;
    intervalSizesLbl = nullptr;
    nameLabel = nullptr;
    modeSizeLbl = nullptr;
    scaleSizeLbl = nullptr;
    defaultNameBtn = nullptr;
    infoLbl = nullptr;
    nameBox = nullptr;
    saveButton = nullptr;
    rotateSld = nullptr;
    modeRotateLbl = nullptr;
    scaleSizeReadout = nullptr;
    modeSizeReadout = nullptr;
    intervalSizeReadout = nullptr;
    closeButton = nullptr;
}

//==============================================================================
void ModeInfoDialog::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
}

void ModeInfoDialog::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == defaultNameBtn.get())
    {
        if (defaultNameBtn->getToggleState())
        {
            nameBox->setEnabled(false);
            modeWorking.setFamily(familyBox->getText());
            modeNode.setProperty(IDs::family, familyBox->getText(), nullptr);
            nameBox->setText(modeWorking.getDescription());
        }
        else
        {
            nameBox->setEnabled(true);
        }
    }
    else if (buttonThatWasClicked == saveButton.get())
    {
        commitMode();
        sendChangeMessage();
        getParentComponent()->exitModalState(0);
    }
    else if (buttonThatWasClicked == closeButton.get())
    {
        getParentComponent()->exitModalState(0);
    }
}

void ModeInfoDialog::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == rotateSld.get())
    {
        modeWorking.rotate(lastRotateAmt - sliderThatWasMoved->getValue());
        lastRotateAmt = sliderThatWasMoved->getValue();

        modeNode.setProperty(IDs::stepString, modeWorking.getStepsString(), nullptr);
        stepsBox->setText(modeWorking.getStepsString());
    }
}

void ModeInfoDialog::textEditorTextChanged(TextEditor& textEditor)
{
    if (textEditor.getName() == familyBox->getName())
    {
        if (defaultNameBtn->getToggleState())
        {
            modeWorking.setFamily(familyBox->getText());
            nameBox->setText(modeWorking.getDescription());
        }
    }

}

void ModeInfoDialog::commitMode()
{
    modeNode.setProperty(IDs::modeName, nameBox->getText(), nullptr);
    modeNode.setProperty(IDs::family, familyBox->getText(), nullptr);
    modeNode.setProperty(IDs::modeInfo, infoBox->getText(), nullptr);

    modeOriginal->restoreNode(modeNode);
}
