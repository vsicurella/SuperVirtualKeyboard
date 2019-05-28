/*
  ==============================================================================

    PresetLibraryBox.h
    Created: 27 May 2019 12:02:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"

#include "../../CommonFunctions.h"
#include "../../PluginState.h"


class PresetLibraryBox : public ComboBox
{
	SvkPluginState* pluginState;
	OwnedArray<Mode>* presets;
	OwnedArray<PopupMenu> menus;
	Array<Mode*> menuIdToPreset;

	bool allSorted = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetLibraryBox)

public:

	PresetLibraryBox(SvkPluginState* pluginStateIn, String nameIn=String());
	~PresetLibraryBox();

	void sortMenus();

	void showPopup() override;
};