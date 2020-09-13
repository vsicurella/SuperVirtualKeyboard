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

	degreePaintButton = static_cast<TextButton*>(flexBox.items[0].associatedComponent);
	degreePaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
	degreePaintButton->setToggleState(true, dontSendNotification);

	layerPaintButton = static_cast<TextButton*>(flexBox.items[1].associatedComponent);
	layerPaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);

	keyPaintButton = static_cast<TextButton*>(flexBox.items[2].associatedComponent);
	keyPaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);


	resetColourToggle = static_cast<TextButton*>(flexBox.items[3].associatedComponent);

	FlexItem selectorItem = FlexItem(*colourSelector.get());
	selectorItem = selectorItem.withFlex(0);

	flexBox.items.insert(0, selectorItem);
	colourSelectorFlex = &flexBox.items.getReference(0);
	
	setSize(100, 100);
}

ColourSettingsPanel::~ColourSettingsPanel()
{
	colourSelector = nullptr;
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
	//if (buttonThatWasClicked == degreePaintButton)
	//{

	//}
	//else if (buttonThatWasClicked == layerPaintButton)
	//{

	//}
	//else if (buttonThatWasClicked == keyPaintButton)
	//{

	//}

	if (buttonThatWasClicked == resetColourToggle)
	{
		userClickedReset = true;
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
		keyPaintButton->setToggleState(true, dontSendNotification);

	else if (shiftHeld)
		layerPaintButton->setToggleState(true, dontSendNotification);

	else
		degreePaintButton->setToggleState(true, dontSendNotification);
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

ColourSelector* ColourSettingsPanel::getColourSelector()
{
	return colourSelector.get();
}

