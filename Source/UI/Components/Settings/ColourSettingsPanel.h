/*
  ==============================================================================

    ColourSettingsPanel.h
    Created: 13 Sep 2020 2:41:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"

class ColourSettingsPanel : public SvkSettingsPanel
{
public:

	ColourSettingsPanel(SvkPluginState*);

	~ColourSettingsPanel() override;

	void resized() override;

	void buttonClicked(Button*) override;

	void modifierKeysChanged(const ModifierKeys&) override;

	void mouseDown(const MouseEvent& event) override;

	void mouseUp(const MouseEvent& event) override;

	ColourSelector* getColourSelector();

private:

	std::unique_ptr<ColourSelector> colourSelector;
	FlexItem* colourSelectorFlex;

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
