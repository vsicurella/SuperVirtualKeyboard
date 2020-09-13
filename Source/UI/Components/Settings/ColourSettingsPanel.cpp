/*
  ==============================================================================

    ColourSettingsPanel.cpp
    Created: 13 Sep 2020 2:41:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ColourSettingsPanel.h"

ColourSettingsPanel::ColourSettingsPanel(SvkPluginState* stateIn)
	: SvkSettingsPanel(stateIn, 
		{
			IDs::pianoKeyColorsDegree,
			IDs::pianoKeyColorsLayer,
			IDs::pianoKeyColorsIndividual,
			IDs::pianoKeyColorReset
		},
		{
			SvkControlProperties(ControlTypeNames::ToggleControl, "Paint by Scale Degree"),
			SvkControlProperties(ControlTypeNames::ToggleControl, "Paint by Layer (hold Shift)"),
			SvkControlProperties(ControlTypeNames::ToggleControl, "Paint by Individual Key (hold Ctrl)"),
			SvkControlProperties(ControlTypeNames::ToggleControl, "Reset Color to Default (Right-click)")
		}
)
{
	setName("ColourSettingsPanel");

	colourSelector.reset(new ColourSelector(
		  ColourSelector::ColourSelectorOptions::showColourAtTop
		+ ColourSelector::ColourSelectorOptions::editableColour
		+ ColourSelector::ColourSelectorOptions::showColourspace
	, 4, 0));
	addAndMakeVisible(colourSelector.get());
	colourSelector->addChangeListener(this);

	degreePaintButton = static_cast<TextButton*>(controls[0]);
	degreePaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
	degreePaintButton->setToggleState(true, dontSendNotification);
	degreePaintButton->addListener(this);

	layerPaintButton = static_cast<TextButton*>(controls[1]);
	layerPaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
	layerPaintButton->addListener(this);

	keyPaintButton = static_cast<TextButton*>(controls[2]);
	keyPaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
	keyPaintButton->addListener(this);

	resetColourToggle = static_cast<TextButton*>(controls[3]);
	resetColourToggle->addListener(this);

	FlexItem selectorItem = FlexItem(*colourSelector.get());
	selectorItem = selectorItem.withFlex(0);

	flexBox.items.insert(0, selectorItem);
	colourSelectorFlex = &flexBox.items.getReference(0);
	
	setSize(100, 100);
}

ColourSettingsPanel::~ColourSettingsPanel()
{
	colourSelector = nullptr;

	if (virtualKeyboard)
		virtualKeyboard->removeMouseListener(this);
}

void ColourSettingsPanel::resized()
{
	flexBox.items.set(0, 
		colourSelectorFlex->withMinHeight(getHeight())
							.withMinWidth(getHeight() * 1.62f)
	);

	SvkSettingsPanel::resized();
}

void ColourSettingsPanel::buttonClicked(Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == degreePaintButton)
	{
		virtualKeyboard->getProperties().set(IDs::pianoKeyPaintType, 0);
	}
	else if (buttonThatWasClicked == layerPaintButton)
	{
		virtualKeyboard->getProperties().set(IDs::pianoKeyPaintType, 1);
	}
	else if (buttonThatWasClicked == keyPaintButton)
	{
		virtualKeyboard->getProperties().set(IDs::pianoKeyPaintType, 2);
	}

	if (buttonThatWasClicked == resetColourToggle)
	{
		virtualKeyboard->getProperties().set(IDs::pianoKeyColorReset, buttonThatWasClicked->getToggleState());
		userClickedReset = buttonThatWasClicked->getToggleState();
	}
}

void ColourSettingsPanel::modifierKeysChanged(const ModifierKeys& modifiers)
{
	if (modifiers.isCtrlDown() && !ctrlHeld)
		ctrlHeld = true;
	else if (ctrlHeld && !modifiers.isCtrlDown())
		ctrlHeld = false;

	else if (modifiers.isShiftDown() && !shiftHeld)
		shiftHeld = true;
	else if (shiftHeld && !modifiers.isShiftDown())
		shiftHeld = false;

	if (ctrlHeld)
		keyPaintButton->setToggleState(true, sendNotification);

	else if (shiftHeld)
		layerPaintButton->setToggleState(true, sendNotification);

	else
		degreePaintButton->setToggleState(true, sendNotification);
}

void ColourSettingsPanel::mouseDown(const MouseEvent& event)
{
	if (event.mods.isRightButtonDown())
	{
		resetColourToggle->setToggleState(true, dontSendNotification);
		userClickedReset = false;
	}
}

void ColourSettingsPanel::mouseUp(const MouseEvent& event)
{
	if (!userClickedReset && resetColourToggle->getToggleState())
		resetColourToggle->setToggleState(false, dontSendNotification);
}

void ColourSettingsPanel::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == colourSelector.get())
	{
		virtualKeyboard->getProperties().set(
			IDs::colorSelected,
			colourSelector->getCurrentColour().toString()
		);
	}
}

void ColourSettingsPanel::setKeyboardPointer(VirtualKeyboard::Keyboard* keyboardPointer)
{
	virtualKeyboard = keyboardPointer;

	// TODO fill swatches

	virtualKeyboard->addMouseListener(this, true);
}

ColourSelector* ColourSettingsPanel::getColourSelector()
{
	return colourSelector.get();
}

