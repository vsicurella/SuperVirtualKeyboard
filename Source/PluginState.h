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
#include "MidiProcessor.h"
#include "Structures/PresetManager.h"
#include "Structures/Preset.h"
#include "UI/Components/VirtualKeyboard/Keyboard.h"

struct SvkPluginState : public ChangeBroadcaster,
						public ChangeListener
{
	ValueTree pluginStateNode;
	ValueTree presetLibraryNode;
	ValueTree pluginEditorNode;

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
	VirtualKeyboard::Keyboard* getKeyboard();
    
    int* getMidiInputMap();
    int* getMidiOutputMap();

	void loadMode(int presetIndexIn);
	void loadMode(ValueTree modeNodeIn);

	void setMidiRootNote(int rootNoteIn);
    
	void updateKeyboardSettingsPreset();

	bool savePreset();
	bool loadPreset();

	void changeListenerCallback(ChangeBroadcaster* source) override;
    
	//==============================================================================
	
	std::unique_ptr<ApplicationCommandManager> appCmdMgr;
	std::unique_ptr<SvkPluginSettings> pluginSettings;
	std::unique_ptr<SvkPresetManager> presetManager;

	std::unique_ptr<TextFilterIntOrSpace> textFilterIntOrSpace;
	std::unique_ptr<TextFilterInt> textFilterInt;

private:

	std::unique_ptr<UndoManager> undoManager;
    std::unique_ptr<SvkMidiProcessor> midiProcessor;
	std::unique_ptr<VirtualKeyboard::Keyboard> virtualKeyboard;
    
	std::unique_ptr<SvkPreset> presetCurrent;
	
	Array<ValueTree> loadedFactoryPresets;
	Array<ValueTree> loadedUserPresets;

	Array<Array<int>> presetsSorted;

	std::unique_ptr<Mode> modeLoaded;
};
