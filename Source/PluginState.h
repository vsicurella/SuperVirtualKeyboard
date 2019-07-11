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
    
    void recallState(ValueTree nodeIn);

	//==============================================================================
    
    ApplicationCommandManager* getAppCmdMgr();
    UndoManager* getUndoManager();
    
	SvkPresetManager* getPresetManager();
	SvkMidiProcessor* getMidiProcessor();
	SvkPluginSettings* getPluginSettings();
    
    VirtualKeyboard::Keyboard* getKeyboard();
    ModeMapper* getModeMapper();
    
    NoteMap* getMidiInputMap();
    NoteMap* getMidiOutputMap();
    
    SvkPreset* getPresetinSlot(int slotNumIn=0);
    SvkPreset* getPresetViewed();
    
    Mode* getModeInSlot(int slotNumIn);
    Mode* getModeViewed();

    //==============================================================================
    
    bool isPresetEdited();

    void setPresetViewed(int presetViewedIn);
	void setModeViewed(int modeViewedIn);
	
	void changeModeInCurrentSlot(int modeLibraryIndexIn);
	void addModeToNewSlot(int modeLibraryIndexIn);
	void addModeToNewSlot(ValueTree modePresetNodeIn);

    void setModeViewedSlotNumber(int slotNumberIn);
	void setModeViewedRoot(int rootNoteIn);

	void setMidiInputMap(NoteMap noteMapIn);
	void setMidiOutputMap(NoteMap noteMapIn);
    
    void updatePluginToPresetLoaded();

    void commitPresetChanges();
	bool savePresetViewedToFile();
	bool loadPresetFromFile(bool replaceViewed=true);

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

    SvkPreset* presetViewed;
	int presetSlotNumViewed = 0;
    
    Mode* modeViewed; // What is currently on screen
    int modeViewedNum = 1; // The mode box view selection
	int modePresetSlotNum = 0; // The Slot number of the preset
};
