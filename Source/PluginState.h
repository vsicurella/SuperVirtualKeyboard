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
#include "PresetManager.h"
#include "Structures/Preset.h"
#include "UI/Components/VirtualKeyboard/Keyboard.h"

struct SvkPluginState : public ChangeBroadcaster,
						public ChangeListener
{
	ValueTree pluginStateNode;
	ValueTree pluginSettingsNode;
	ValueTree midiSettingsNode;
	ValueTree presetLibraryNode;
	ValueTree pluginEditorNode;
	ValueTree pianoNode;

	ValueTree presetWorkingNode;
	ValueTree modePresetNode;
    
    SvkPluginState();
	~SvkPluginState() {}

	//==============================================================================

	UndoManager* getUndoManager();

    SvkMidiProcessor* getMidiProcessor();
	SvkPreset* getPresetLoaded();
	Mode* getModeLoaded();
	VirtualKeyboard::Keyboard* getKeyboard();
    
    NoteMap* getMidiInputMap();
    NoteMap* getMidiOutputMap();
    
    bool isPresetEdited();

	void loadMode(int presetIndexIn);
	void loadMode(ValueTree modeNodeIn);

	void setMidiRootNote(int rootNoteIn);
    
    void updatePluginToPresetLoaded();
    void updatePluginFromParentNode();

    void commitPresetChanges();
	bool savePreset();
	bool loadPreset();

	void recallState(ValueTree nodeIn);

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
    SvkPreset presetWorking;
    std::unique_ptr<Mode> modeLoaded;
	
	Array<ValueTree> loadedFactoryPresets;
	Array<ValueTree> loadedUserPresets;

	Array<Array<int>> presetsSorted;
    bool presetEdited = false;
    
};
