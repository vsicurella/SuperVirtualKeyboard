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
#include "Structures/ModeMapper.h"
#include "Structures/OwnedHashMap.h"

#include "UI/Components/VirtualKeyboard/KeyboardComponent.h"

typedef OwnedHashMap<Identifier, RangedAudioParameter, IDasStringHash> SvkParameters;

struct SvkPluginState : public ChangeBroadcaster,
						public ChangeListener,
                        public AudioProcessorParameter::Listener
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
    SvkParameters* getParameters();
    
    VirtualKeyboard::Keyboard* getKeyboard();
    ModeMapper* getModeMapper();
    
    NoteMap* getMidiInputMap();
    NoteMap* getMidiOutputMap();
	int getMappingMode();
	int getMappingStyle();
    bool isAutoMapping();
    
    SvkPreset* getPresetinSlot(int slotNumIn=0);
    SvkPreset* getPresetViewed();
	int getPresetSlotNumViewed();
	int getNumModesInPresetViewed();
    
    Mode* getModeInSlot(int slotNumIn);
    Mode* getModeViewed();
	int getModeViewedNum();

    Mode* getMode1();
    Mode* getMode2();
    Mode* getModeCustom();

	int getMode1Root();
	int getMode2Root();
    
    int getMapOrder1();
    int getMapOrder2();
    int getMapOrderOffset1();
    int getMapOrderOffset2();

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
    
    void setMapOrder1(int orderIn);
    void setMapOrder2(int orderIn);
    void setMapOrderOffset1(int offsetIn);
    void setMapOrderOffset2(int offsetIn);
    
    void setPeriodShift(int shiftIn);
    void setMidiChannel(int midiChannelIn);
    
	void setMidiInputMap(NoteMap noteMapIn);
    void setMidiOutputMap(NoteMap noteMapIn);
    
    void setMapMode(int mapModeSelectionIn);
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

	void updateModeViewed(bool sendChange=true);
    void updateToPreset(bool sendChange = true);
    
    void updateFromParameter(Identifier paramId);

	void commitModeInfo();
    void commitPresetChanges();
	bool savePresetViewedToFile();
	bool loadPresetFromFile(bool replaceViewed);

	bool saveModeViewedToFile();
	bool loadModeFromFile();
    
    //==============================================================================

    void parameterValueChanged(int parameterIndex, float newValue);
     
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting);

	void changeListenerCallback(ChangeBroadcaster* source) override;
    
	//==============================================================================

	std::unique_ptr<TextFilterIntOrSpace> textFilterIntOrSpace;
	std::unique_ptr<TextFilterInt> textFilterInt;

private:
    
    void initializeParameters();

	std::unique_ptr<SvkPresetManager> presetManager;
	std::unique_ptr<SvkMidiProcessor> midiProcessor;
	std::unique_ptr<SvkPluginSettings> pluginSettings;

	std::unique_ptr<ApplicationCommandManager> appCmdMgr;
	std::unique_ptr<UndoManager> undoManager;

	std::unique_ptr<VirtualKeyboard::Keyboard> virtualKeyboard;
	std::unique_ptr<ModeMapper> modeMapper;

    SvkParameters svkParameters;

    bool presetEdited = false;

    SvkPreset* presetViewed;
	int presetSlotNumViewed = 0;
    
    Mode* modeViewed; // What is currently on screen
    int modeViewedNum = 1; // The mode box view selection
	int modePresetSlotNum = 0; // The slot number of the mode in the current preset
    
	// Mapping parameters

    int mapModeSelected = 0;
    int mapStyleSelected = 1;
    
    int mapOrder1 = 0;
    int mapOrder2 = 0;
    int mapOrderOffset1 = 0;
    int mapOrderOffset2 = 0;
};
