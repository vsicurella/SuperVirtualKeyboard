/*
  ==============================================================================

    KeyboardSettingsPanel.cpp
    Created: 13 Nov 2019 7:09:31pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "KeyboardSettingsPanel.h"

KeyboardSettingsPanel::KeyboardSettingsPanel(SvkPluginState* pluginStateIn)
    : SvkSettingsPanel("KeyboardSettingsPanel", pluginStateIn,
        {
            "Keyboard settings"
        },
        {
            IDs::keyboardKeysStyle,
            IDs::keyboardHighlightStyle,
            IDs::pianoWHRatio,
            IDs::pianoKeyShowNumber
        }, 
        {
            SvkControlProperties(ControlTypeNames::MenuControl, "Key Layout Style", true),
            SvkControlProperties(ControlTypeNames::MenuControl, "Key Highlight Style", true),
            SvkControlProperties(ControlTypeNames::SliderControl, "Key Proportion", true),
            SvkControlProperties(ControlTypeNames::ToggleControl, "Show Key Numbers")
        }
    )
{
    layoutLabel = static_cast<LabelledComponent*>(controls[0]);
    layoutLabel->setComponentSize(136, controlMinHeight);

    keyLayoutBox = LabelledComponent::getComponentPointer<ComboBox>(layoutLabel);
    keyLayoutBox->addListener(this);
    keyLayoutBox->setEditableText(false);
    keyLayoutBox->setJustificationType(Justification::centredLeft);
    keyLayoutBox->setTextWhenNothingSelected(String());
    keyLayoutBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    keyLayoutBox->addItem(TRANS("Nested Right"), 1);
    keyLayoutBox->addItem(TRANS("Nested Center"), 2);
    keyLayoutBox->addItem(TRANS("Flat"), 3);
    keyLayoutBox->addItem(TRANS("Adjacent"), 4);

    highlightLabel = static_cast<LabelledComponent*>(controls[1]);
    highlightLabel->setComponentSize(96, controlMinHeight);

    keyHighlightBox = LabelledComponent::getComponentPointer<ComboBox>(highlightLabel);
    keyHighlightBox->addListener(this);
    keyHighlightBox->setEditableText(false);
    keyHighlightBox->setJustificationType(Justification::centredLeft);
    keyHighlightBox->setTextWhenNothingSelected(String());
    keyHighlightBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    keyHighlightBox->addItem(TRANS("Full Key"), 1);
    keyHighlightBox->addItem(TRANS("Inside"), 2);
    keyHighlightBox->addItem(TRANS("Border"), 3);
    keyHighlightBox->addItem(TRANS("Circles"), 4);
    keyHighlightBox->addItem(TRANS("Squares"), 5);
    keyHighlightBox->addListener(this);

    ratioLabel = static_cast<LabelledComponent*>(controls[2]);
    ratioLabel->setComponentSize(225, controlMinHeight);
    flexSections.getReference(0).items.getReference(2).minWidth = ratioLabel->getWidth();

    keyRatioSlider = LabelledComponent::getComponentPointer<Slider>(ratioLabel);
    keyRatioSlider->setRange(0.01, 1, 0.01);
    keyRatioSlider->addListener(this);

    showNoteNumbers = static_cast<TextButton*>(controls[3]);
    showNoteNumbers->setSize(Font().getStringWidth(showNoteNumbers->getButtonText()) + 5, controlMinHeight);
    showNoteNumbers->addListener(this);
}

void KeyboardSettingsPanel::comboBoxChanged(ComboBox* boxThatChanged)
{
    if (virtualKeyboard)
    {
        if (boxThatChanged == keyLayoutBox)
        {
            virtualKeyboard->setKeyStyle(boxThatChanged->getSelectedId());
            virtualKeyboard->resized();
        }

        else if (boxThatChanged == keyHighlightBox)
        {
            virtualKeyboard->setHighlightStyle(boxThatChanged->getSelectedId());
        }
    }
}

void KeyboardSettingsPanel::sliderValueChanged(Slider* sliderChanged)
{
    if (virtualKeyboard)
    {
        if (sliderChanged == keyRatioSlider)
        {
            virtualKeyboard->setKeySizeRatio(sliderChanged->getValue(), false);
            getParentComponent()->getParentComponent()->resized();
        }
    }
}

void KeyboardSettingsPanel::buttonClicked(Button* clickedButton)
{
    if (virtualKeyboard)
    {
        if (clickedButton == showNoteNumbers)
        {
            virtualKeyboard->setShowNoteNumbers(clickedButton->getToggleState());
        }
    }
}

void KeyboardSettingsPanel::setKeyboardPointer(VirtualKeyboard::Keyboard* keyboardPointer)
{
    virtualKeyboard = keyboardPointer;

    keyLayoutBox->setSelectedId(virtualKeyboard->getKeyPlacementStyle(), dontSendNotification);
    keyHighlightBox->setSelectedId(virtualKeyboard->getHighlightStyle(), dontSendNotification);
    keyRatioSlider->setValue(virtualKeyboard->getKeySizeRatio(), dontSendNotification);
    showNoteNumbers->setToggleState(virtualKeyboard->isShowingNoteNumbers(), dontSendNotification);
}