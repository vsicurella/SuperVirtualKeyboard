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
	ValueTree modeLibraryNode;
	ValueTree pluginEditorNode;
	ValueTree pianoNode;
    
    SvkPluginState();
	~SvkPluginState() {}

	//==============================================================================

	SvkPresetManager* getPresetManager();
	SvkMidiProcessor* getMidiProcessor();
	SvkPluginSettings* getPluginSettings();

	ApplicationCommandManager* getAppCmdMgr();
	UndoManager* getUndoManager();

	SvkPreset* getPresetLoaded(int slotNumIn=0);
	VirtualKeyboard::Keyboard* getKeyboard();
    
    NoteMap* getMidiInputMap();
    NoteMap* getMidiOutputMap();
    
    bool isPresetEdited();

	void loadMode(int presetIndexIn, int modeNumberIn=1);
	void loadMode(ValueTree modeNodeIn, int modeNumberIn=1);

	void setMidiRootNote(int rootNoteIn);
	void setMidiInputMap(NoteMap noteMapIn);
	void setMidiOutputMap(NoteMap noteMapIn);
    
    void updatePluginToPresetLoaded();

    void commitPresetChanges();
	bool savePresetToFile();
	bool loadPreset();

	void recallState(ValueTree nodeIn);

	void changeListenerCallback(ChangeBroadcaster* source) override;
    
	//==============================================================================
	


	std::unique_ptr<TextFilterIntOrSpace> textFilterIntOrSpace;
	std::unique_ptr<TextFilterInt> textFilterInt;

private:

	std::unique_ptr<SvkPresetManager> presetManager;
	std::unique_ptr<SvkMidiProcessor> midiProcessor;
	std::unique_ptr<SvkPluginSettings> pluginSettings;

	std::unique_ptr<ApplicationCommandManager> appCmdMgr;
	std::unique_ptr<UndoManager> undoManager;

	std::unique_ptr<VirtualKeyboard::Keyboard> virtualKeyboard;
	
	OwnedArray<SvkPreset> presetsWorking;
	
	Array<ValueTree> loadedFactoryModes;
	Array<ValueTree> loadedUserModes;

	Array<Array<int>> modesSorted;
    bool presetEdited = false;
};
