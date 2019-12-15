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
#include "Midi/MidiProcessor.h"
#include "PresetManager.h"
#include "Structures/Preset.h"
#include "Structures/ModeMapper.h"
#include "Structures/OwnedHashMap.h"

#include "UI/Components/VirtualKeyboard/KeyboardComponent.h"

typedef OwnedHashMap<Identifier, RangedAudioParameter, IDasStringHash> SvkParameters;

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
    // Object getters

    ApplicationCommandManager* getAppCmdMgr();
    UndoManager* getUndoManager();
    
	SvkPresetManager* getPresetManager();
	SvkMidiProcessor* getMidiProcessor();
	SvkPluginSettings* getPluginSettings();
    SvkParameters* getParameters();
    
    VirtualKeyboard::Keyboard* getKeyboard();
    ModeMapper* getModeMapper();
    
    NoteMap* getMidiInputFilterMap();
    NoteMap* getMidiOutputFilterMap();

	SvkPreset* getPresetinSlot(int slotNumIn = 0);
	SvkPreset* getPresetViewed();

	Mode* getModeInSlot(int slotNumIn);
	Mode* getModeViewed();
	Mode* getMode1();
	Mode* getMode2();
	Mode* getModeCustom();

	Tuning* getTuning();

	//==============================================================================
	// Parameter Getters

	float getParameterValue(Identifier paramId);
	float getParameterMin(Identifier paramId);
	float getParameterMax(Identifier paramId);
	float getParameterDefaultValue(Identifier paramId);

	int getPresetSlotNumViewed();
	int getNumModesInPresetViewed();
	int getModeViewedNum();

	int getMappingMode();
	int getMappingStyle();
    bool isAutoMapping();

	int getModeSlotRoot(int slotNum);
	int getMode1Root();
	int getMode2Root();
    
    int getMapOrder1();
    int getMapOrder2();
    int getMapOrderOffset1();
    int getMapOrderOffset2();

	bool isPresetEdited();

    //==============================================================================
    // Parameter setters

	void setParameterValue(Identifier paramIdIn, float valueIn);

	// Should either add arg "bool updateParm=false" to all of these, or implement updateFromParameter()
    void setPresetViewed(int presetViewedIn);
	void setModeViewed(int modeViewedIn);
	void handleModeSelection(int modeBoxNum, int idIn);

	void setMapMode(int mapModeSelectionIn);
	void setMapStyle(int mapStyleIn);

	void setPeriodShift(int shiftIn);
	void setMidiChannel(int midiChannelIn);
	void setShowNoteNums(bool showNoteNumsIn);
	void setShowFilteredNoteNums(bool showFilteredNoteNumsIn);
	void setShowNoteLabels(bool showNoteLabelsIn);
	void setScrollingMode(int modeIn);
	void setScrollingStyle(int styleIn);
	void setNumKeysInWidth(int numKeysIn);
	void setNumKeyboardRows(int rowsIn);
	void setKeyboardOrientation(int orientationIn);
	void setKeyStyle(int keyStyleIn);
	void setHighlightStyle(int highlightStyleIn);

	void setVelocityBehavior(int velocityIdIn);
	void setVelocityScalar(int velocityScalar);
	void setVelocityFixed(int velocityFixed);

	void setModeSlotRoot(int modeSlotIn, int rootNoteIn);
	void setMode1Root(int rootNoteIn);
    void setMode2Root(int rootNoteIn);
    
	// Move these to VirtualKeyboard?
/*	void setKeyWidthMod(int keyNumIn, float widthMod);
	void setKeyHeightMod(int keyNumIn, float heightMod);
	void setKeyXOffset(int keyNumIn, int xOffset);
	void setKeyYOffset(int keyNumIn, int yOffset); */   

	//==============================================================================
	// User Functionality

	void setMidiInputMap(NoteMap noteMapIn);
    void setMidiOutputMap(NoteMap noteMapIn);
    
	void setModeCustom(String stepsIn);

	void setMapOrder1(int orderIn);
	void setMapOrder2(int orderIn);
	void setMapOrderOffset1(int offsetIn);
	void setMapOrderOffset2(int offsetIn);

    void doMapping(const Mode* mode1, const Mode* mode2, int mappingType=-1,
                   int mode1OrderIn=0, int mode2OrderIn=0, int mode1OrderOffsetIn=0, int mode2OrderOffsetIn=0);
    void doMapping();

	void setTuning(const Tuning* tuningToCopy);
	void setTuningToET(double period, double divisions);

	//==============================================================================
	// Internal Functionality

	void sendMappingToKeyboard();
	void sendMappingToKeyboard(ValueTree mapNodeIn);

	void updateModeViewed(bool sendChange=true);
    void updateToPreset(bool sendChange = true);
    
	void updateParameter(Identifier paramId);
	void updateParameters();
    void updateFromParameter(Identifier paramId);

	void commitModeInfo();
    void commitPresetChanges();
	bool savePresetViewedToFile();
	bool loadPresetFromFile(bool replaceViewed);

	bool saveModeViewedToFile();
	bool loadModeFromFile();
    
    //==============================================================================

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
	std::unique_ptr<Tuning> tuning;

    SvkParameters svkParameters;

    SvkPreset* presetViewed;
    Mode* modeViewed; // What is currently on screen
    
	bool presetEdited = false;

	// Mapping parameters
    
    int mapOrder1 = 0;
    int mapOrder2 = 0;
    int mapOrderOffset1 = 0;
    int mapOrderOffset2 = 0;
};
