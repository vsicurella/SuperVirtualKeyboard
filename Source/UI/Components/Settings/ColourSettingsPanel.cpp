/*
  ==============================================================================

    ColourSettingsPanel.cpp
    Created: 13 Sep 2020 2:41:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ColourSettingsPanel.h"

ColourSettingsPanel::ColourSettingsPanel(SvkPluginState* stateIn)
	: SvkSettingsPanel("ColourSettingsPanel", stateIn, 3,
		{
			IDs::pianoKeyColor,
			IDs::pianoKeyColorsDegree,
			IDs::pianoKeyColorsLayer,
			IDs::pianoKeyColorsIndividual,
			IDs::pianoKeyColorReset
		},
		{
			SvkControlProperties(ControlTypeNames::GenericControl, "Color Selector"),
			SvkControlProperties(ControlTypeNames::ToggleControl, "Paint by Scale Degree", false, 1),
			SvkControlProperties(ControlTypeNames::ToggleControl, "Paint by Layer (hold Shift)", false, 1),
			SvkControlProperties(ControlTypeNames::ToggleControl, "Paint by Individual Key (hold Ctrl)", false, 1),
			SvkControlProperties(ControlTypeNames::ToggleControl, "Reset Color to Default (Right-click)", false, 1)
		},
		FlexBox(), {}, { FlexItem() }
	)
{
	// Set up Color Selector and replace generic control
	colourSelector = new ColourSelector(
		  ColourSelector::ColourSelectorOptions::showColourAtTop
		+ ColourSelector::ColourSelectorOptions::editableColour
		+ ColourSelector::ColourSelectorOptions::showColourspace
	, 4, 0);
	addAndMakeVisible(colourSelector);
	colourSelector->addChangeListener(this);
	controls.set(0, colourSelector, true);
	getSectionFlexBox(0)->items.getReference(0).associatedComponent = colourSelector;

	degreePaintButton = static_cast<TextButton*>(controls[1]);
	degreePaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
	degreePaintButton->setToggleState(true, dontSendNotification);
	degreePaintButton->addListener(this);

	layerPaintButton = static_cast<TextButton*>(controls[2]);
	layerPaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
	layerPaintButton->addListener(this);

	keyPaintButton = static_cast<TextButton*>(controls[3]);
	keyPaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
	keyPaintButton->addListener(this);

	resetColourToggle = static_cast<TextButton*>(controls[4]);
	resetColourToggle->addListener(this);

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
	getSectionFlexBox(0)->items.getReference(0).height = getHeight();
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
	checkModifiers(modifiers);
}

void ColourSettingsPanel::mouseMove(const MouseEvent& event)
{
	if (!virtualKeyboard->isMouseOver())
		checkModifiers(event.mods);
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
	if (source == colourSelector)
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

	Colour init = Colour::fromString(virtualKeyboard->getProperties()[IDs::colorSelected].toString());
	if (init.isTransparent())
		init = virtualKeyboard->getKeyOrderColor(0);
	colourSelector->setCurrentColour(init, dontSendNotification);

	// TODO fill swatches

	virtualKeyboard->addMouseListener(this, true);
}

ColourSelector* ColourSettingsPanel::getColourSelector()
{
	return colourSelector;
}

void ColourSettingsPanel::checkModifiers(const ModifierKeys& modifiers)
{
	bool valueChanged = true;

	if (modifiers.isCtrlDown() && !ctrlHeld)
		ctrlHeld = true;

	else if (ctrlHeld && !modifiers.isCtrlDown())
		ctrlHeld = false;

	else if (modifiers.isShiftDown() && !shiftHeld)
		shiftHeld = true;

	else if (shiftHeld && !modifiers.isShiftDown())
		shiftHeld = false;

	else
		valueChanged = false;

	if (valueChanged)
	{
		if (ctrlHeld)
			keyPaintButton->setToggleState(true, sendNotification);

		else if (shiftHeld)
			layerPaintButton->setToggleState(true, sendNotification);

		else
			degreePaintButton->setToggleState(true, sendNotification);
	}
}