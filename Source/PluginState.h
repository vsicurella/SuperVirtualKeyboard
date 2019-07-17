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
	bool isAutoMapOn();
	int getMappingStyle();
    
    SvkPreset* getPresetinSlot(int slotNumIn=0);
    SvkPreset* getPresetViewed();
	int getNumModesInPresetViewed();
    
    Mode* getModeInSlot(int slotNumIn);
    Mode* getModeViewed();
	int getModeViewedNum();

    Mode* getMode1();
    Mode* getMode2();
    Mode* getModeCustom();

	int getMode1Root();
	int getMode2Root();

	int getPeriodShift();
	int getMidiChannelOut();

	bool isShowingNoteNums();
	int getKeyStyle();
	int getHighlightStyle();

    //==============================================================================
    
    bool isPresetEdited();

    void setPresetViewed(int presetViewedIn);
	void setModeViewed(int modeViewedIn);

    void handleModeSelection(int modeBoxNum, int idIn);
    void setModeCustom(String stepsIn);

	void setMode1Root(int rootNoteIn);
    void setMode2Root(int rootNoteIn);
    
    void setPeriodShift(int shiftIn);
    void setMidiChannel(int midiChannelIn);
    
	void setMidiInputMap(NoteMap noteMapIn);
    void setMidiOutputMap(NoteMap noteMapIn);
    void setAutoMapping(bool isAutoMappingIn);
    
    void setMapStyle(int mapStyleIn);
    void doMapping(const Mode* mode1, const Mode* mode2, int mappingType=-1,
                   int mode1OrderIn=0, int mode2OrderIn=0, int mode1OrderOffsetIn=0, int mode2OrderOffsetIn=0);
    void doMapping();

	void sendMappingToKeyboard();
	void sendMappingToKeyboard(ValueTree mapNodeIn);
    
	void setNoteNumsShowing(bool showNoteNumsIn);
    void setKeyStyle(int keyStyleIn);
    void setHighlightStyle(int highlightStyleIn);
    
	//==============================================================================

	void updateModeViewed();
    void updateToPreset();

    void commitPresetChanges();
	bool savePresetViewedToFile();
	bool loadPresetFromFile(bool replaceViewed);

	bool saveModeViewedToFile();
	bool loadModeIntoSlot();

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
	int modePresetSlotNum = 0; // The slot number of the mode in the current preset
    
	// Mapping parameters

    bool isAutoMapping = true;
    int mapStyleSelected = 0;
    
    int mapOrder1 = 0;
    int mapOrder2 = 0;
    int mapOrderOffset1 = 0;
    int mapOrderOffset2 = 0;
};
