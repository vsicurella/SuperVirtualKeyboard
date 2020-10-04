/*
  ==============================================================================

  ModeInfoDialog.h
  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../CommonFunctions.h"
#include "../../Structures/Mode.h"

class ModeInfoDialog  : public Component,
                        public ChangeBroadcaster,
                        public TextEditor::Listener,
                        public Button::Listener,
                        public Slider::Listener
{
public:
    //==============================================================================
    ModeInfoDialog (Mode* modeIn);
    ~ModeInfoDialog();

    //==============================================================================

    void commitMode();

    void textEditorTextChanged(TextEditor& textEditor) override;
    void textEditorEscapeKeyPressed(TextEditor& textEditor) override {};
    void textEditorReturnKeyPressed(TextEditor& textEditor) override {};
    void textEditorFocusLost(TextEditor& textEditor) override {};

    void paint (Graphics& g) override;
    void resized() override {};
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;


private:
    Mode* modeOriginal;
    Mode modeWorking;
    ValueTree modeNode;

    int lastRotateAmt = 0;

    //==============================================================================
    std::unique_ptr<TextEditor> familyBox;
    std::unique_ptr<TextEditor> stepsBox;
    std::unique_ptr<TextEditor> infoBox;
    std::unique_ptr<Label> familyNameLbl;
    std::unique_ptr<Label> stepsLbl;
    std::unique_ptr<Label> intervalSizesLbl;
    std::unique_ptr<Label> nameLabel;
    std::unique_ptr<Label> modeSizeLbl;
    std::unique_ptr<Label> scaleSizeLbl;
    std::unique_ptr<ToggleButton> defaultNameBtn;
    std::unique_ptr<Label> infoLbl;
    std::unique_ptr<TextEditor> nameBox;
    std::unique_ptr<TextButton> saveButton;
    std::unique_ptr<Slider> rotateSld;
    std::unique_ptr<Label> modeRotateLbl;
    std::unique_ptr<Label> scaleSizeReadout;
    std::unique_ptr<Label> modeSizeReadout;
    std::unique_ptr<Label> intervalSizeReadout;
    std::unique_ptr<TextButton> closeButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModeInfoDialog)
};
