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
#include "../../Structures/PresetManager.h"


class PresetLibraryBox : public ComboBox
{
	SvkPresetManager* presetManager;
	ValueTree presetsLibraryNode;

	std::unique_ptr<OwnedArray<PopupMenu>> menus;

	Array<Array<ValueTree>>* presetsSorted;
	Array<Array<int>>* sortedIndicies;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetLibraryBox)

public:

	PresetLibraryBox(SvkPresetManager* presetManagerIn, String nameIn=String());
	~PresetLibraryBox();

	void populateMenu();
	void showPopup() override;
};