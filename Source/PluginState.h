/*
  ==============================================================================

    PluginState.h
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "MidiProcessor.h"
#include "PluginIDs.h"
#include "Structures/Preset.h"
#include "Structures/MidiRemapper.h"
#include "CommonFunctions.h"

struct SuperVirtualKeyboardPluginState : public ChangeBroadcaster
{
	ValueTree pluginStateNode;
	ValueTree modeLibraryNode;
	ValueTree keyboardWindowNode;

	ValueTree pluginSettingsNode;
	ValueTree midiSettingsNode;
	ValueTree modePresetNode;
	ValueTree pianoNode;
    
    SuperVirtualKeyboardPluginState();
	~SuperVirtualKeyboardPluginState() {}

	//==============================================================================

	OwnedArray<Mode>* get_presets();
	Array<Array<Mode*>>* get_presets_sorted();
	UndoManager* getUndoManager();

    SvkMidiProcessor* getMidiProcessor();
	SvkPreset* getCurrentPreset();
	int get_current_preset_index();
	Mode* getCurrentMode();
    
    int* getMidiInputMap();
    int* getMidiOutputMap();

	int is_mode_in_presets(String stepsStringIn);

	void setCurrentMode(int presetIndexIn);
	void setCurrentMode(Mode* modeIn);
    
	void updateKeyboardSettingsPreset();
	void updateMidiSettingsNode();

	bool savePreset();
	bool loadPreset();
    
	//==============================================================================
	
	std::unique_ptr<ApplicationCommandManager> appCmdMgr;

private:

	void createPresets();

	//==============================================================================

    std::unique_ptr<SvkMidiProcessor> midiProcessor;
    std::unique_ptr<SvkPreset> presetCurrent;
	std::unique_ptr<UndoManager> undoManager;
	
    OwnedArray<Mode> presets;
	Array<Array<Mode*>> presetsSorted;

	Mode* modeCurrent;
};
