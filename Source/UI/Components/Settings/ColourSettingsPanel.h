/*
  ==============================================================================

    ColourSettingsPanel.h
    Created: 13 Sep 2020 2:41:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"

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

	ColourSelector* colourSelector;

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
