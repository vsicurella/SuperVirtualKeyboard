/*
  ==============================================================================

    PluginState.h
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginIDs.h"
#include "Structures/Mode.h"

struct SuperVirtualKeyboardPluginState
{
	ValueTree pluginStateNode;
	ValueTree modeLibraryNode;
	ValueTree presetCurrentNode; // Always updated when some preset is selected
	ValueTree keyboardWindowNode;
	ValueTree pianoNode;

	SuperVirtualKeyboardPluginState() :
			pluginStateNode(IDs::pluginStateNode),
			modeLibraryNode(IDs::modeLibraryNode),
			presetCurrentNode(IDs::presetNode),
			undoManager(new UndoManager())

	{
		createPresets();
		pluginStateNode.addChild(modeLibraryNode, 0, nullptr);
		pluginStateNode.addChild(presetCurrentNode, 1, nullptr);
	}

	~SuperVirtualKeyboardPluginState() {}

	//==============================================================================

	OwnedArray<Mode>* get_presets();
	Array<Array<Mode*>>* get_presets_sorted();
	UndoManager* get_undo_mgr();

	ValueTree get_current_preset();
	int get_current_preset_index();
	Mode* get_current_mode();

	int is_mode_in_presets(Mode* modeIn);

	void set_current_mode(int presetIndexIn);
	void set_current_mode(Mode* modeIn);

	void set_current_key_settings(ValueTree pianoNodeIn);

	//==============================================================================


private:

	void createPresets();

	//==============================================================================

	std::unique_ptr<UndoManager> undoManager;
	OwnedArray<Mode> presets;
	Array<Array<Mode*>> presetsSorted;

	Mode* modeCurrent;
};
