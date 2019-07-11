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
#include "Structures/ModeMapper.h"

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
	ModeMapper* getModeMapper();
    
    NoteMap* getMidiInputMap();
    NoteMap* getMidiOutputMap();
    
    bool isPresetEdited();

	void setModeViewed(int modeViewedIn);
	
	void changeModeInCurrentSlot(int modeLibraryIndexIn);
	void addModeToNewSlot(int modeLibraryIndexIn);
	void addModeToNewSlot(ValueTree modePresetNodeIn);

	void setModeViewedRoot(int rootNoteIn);

	void setMidiInputMap(NoteMap noteMapIn);
	void setMidiOutputMap(NoteMap noteMapIn);
    
    void updatePluginToPresetLoaded();

    void commitPresetChanges();
	bool savePresetViewedToFile();
	bool loadPresetFromFile(bool replaceViewed=true);

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
	std::unique_ptr<ModeMapper> modeMapper;

    bool presetEdited = false;

	int presetSlotNumViewed = 0;
	int modeViewed = 1;
};
