/*
  ==============================================================================

    PluginState.h
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#include "CommonFunctions.h"
#include "PluginIDs.h"
#include "PluginSettings.h"
#include "Structures/Preset.h"
#include "MidiProcessor.h"

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

	OwnedArray<Mode>* getPresets();
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

	void setMidiRootNote(int rootNoteIn);
    
	void updateKeyboardSettingsPreset();
	void updateMidiSettingsNode();

	bool savePreset();
	bool loadPreset();
    
	//==============================================================================
	
	std::unique_ptr<ApplicationCommandManager> appCmdMgr;
	std::unique_ptr<SvkPluginSettings> pluginSettings;

	std::unique_ptr<TextFilterIntOrSpace> textFilterIntOrSpace;
	std::unique_ptr<TextFilterInt> textFilterInt;

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
