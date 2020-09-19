/*
  ==============================================================================

    ColourSettingsPanel.h
    Created: 13 Sep 2020 2:41:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"
#include "../ColourLibraryComponent.h"

class FocussedColourSelector : public FocussedComponent, public ColourSelector, private ChangeListener
{
public:

	FocussedColourSelector(Component* componentToFocusOn = nullptr)
		: FocussedComponent(componentToFocusOn), ColourSelector(
			ColourSelector::ColourSelectorOptions::showColourAtTop
			+ ColourSelector::ColourSelectorOptions::editableColour
			+ ColourSelector::ColourSelectorOptions::showColourspace
			, 4, 0)
	{
		setName("FocussedColourSelector");

		// Called when colour changes
		addChangeListener(this);
	};

	~FocussedColourSelector() {};

	void setComponentToFocusOn(Component* componentPtr) override
	{
		FocussedComponent::setComponentToFocusOn(componentPtr);

		PaintSwatch* swatch = dynamic_cast<PaintSwatch*>(componentPtr);
		if (swatch)
			setCurrentColour(swatch->getCurrentColour(), dontSendNotification);
		else
		{
			VirtualKeyboard::Keyboard* svk = dynamic_cast<VirtualKeyboard::Keyboard*>(componentPtr);
			if (svk)
			{
				svk->getProperties().set(IDs::colorSelected, getCurrentColour().toString());
			}
		}
	}


private:

	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		FocusableComponent* focussed = dynamic_cast<FocusableComponent*>(currentFocus);

		if (focussed)
		{
			focussed->performFocusFunction(getCurrentColour().toString());
		}
		else if (currentFocus)
		{
			currentFocus->getProperties().set(IDs::colorSelected, getCurrentColour().toString());
		}
	}

};

class ColourSettingsPanel : public SvkSettingsPanel,
							public ChangeListener
{
public:

	ColourSettingsPanel(SvkPluginState*);

	~ColourSettingsPanel() override;

	void resized() override;

	void buttonClicked(Button*) override;

	void modifierKeysChanged(const ModifierKeys&) override;

	void mouseMove(const MouseEvent& event) override;

	void mouseDown(const MouseEvent& event) override;

	void mouseUp(const MouseEvent& event) override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	void setKeyboardPointer(VirtualKeyboard::Keyboard* keyboardPointer) override;

	ColourSelector* getColourSelector();

private:

	void checkModifiers(const ModifierKeys& keys);

private:

	FocussedColourSelector* colourSelector;
	ColourLibraryComponent* colourLibrary;

	const int paintTypeRadioGroup = 100;

	TextButton* degreePaintButton;
	TextButton* layerPaintButton;
	TextButton* keyPaintButton;

	TextButton* resetColourToggle;
	bool userClickedReset = false;

	bool ctrlHeld = false;
	bool shiftHeld = false;
	bool altOptionHeld = false;

};

