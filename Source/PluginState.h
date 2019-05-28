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
#include "Structures/PresetManager.h"
#include "Structures/Preset.h"
#include "MidiProcessor.h"

struct SvkPluginState : public ChangeBroadcaster
{
	ValueTree pluginStateNode;
	ValueTree presetLibraryNode;
	ValueTree keyboardWindowNode;

	ValueTree pluginSettingsNode;
	ValueTree midiSettingsNode;
	ValueTree modePresetNode;
	ValueTree pianoNode;
    
    SvkPluginState();
	~SvkPluginState() {}

	//==============================================================================

	UndoManager* getUndoManager();

    SvkMidiProcessor* getMidiProcessor();
	SvkPreset* getPresetLoaded();
	Mode* getModeLoaded();
    
    int* getMidiInputMap();
    int* getMidiOutputMap();

	void loadMode(int presetIndexIn);
	void loadMode(ValueTree modeNodeIn);

	void setMidiRootNote(int rootNoteIn);
    
	void updateKeyboardSettingsPreset();

	bool savePreset();
	bool loadPreset();
    
	//==============================================================================
	
	std::unique_ptr<ApplicationCommandManager> appCmdMgr;
	std::unique_ptr<SvkPluginSettings> pluginSettings;
	std::unique_ptr<SvkPresetManager> presetManager;

	std::unique_ptr<TextFilterIntOrSpace> textFilterIntOrSpace;
	std::unique_ptr<TextFilterInt> textFilterInt;

private:

	void createPresets();

	//==============================================================================

	std::unique_ptr<UndoManager> undoManager;
    std::unique_ptr<SvkMidiProcessor> midiProcessor;
    
	std::unique_ptr<SvkPreset> presetCurrent;
	
	Array<ValueTree> loadedFactoryPresets;
	Array<ValueTree> loadedUserPresets;

	Array<Array<int>> presetsSorted;

	Mode* modeLoaded;
};
