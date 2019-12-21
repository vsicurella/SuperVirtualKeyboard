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
    
    bool parametersInitialized = false;

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

    float getParameterValue(int paramInd);
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

    bool isAutoRetuning();

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
    void setPresetViewed(int presetViewedIn, bool updateParameter=false);
	void setModeViewed(int modeViewedIn, bool updateParameter=false);
	void handleModeSelection(int modeBoxNum, int idIn, bool updateParameter=false);

	void setMapMode(int mapModeSelectionIn, bool updateParameter=false);
	void setMapStyle(int mapStyleIn, bool updateParameter=false);
    
    void setMPEOn(bool mpeOnIn, bool updateParameter=false);
    void setMPELegacy(bool mpeLegacyIn, bool updateParameter=false);
    void setGlobalPitchBendMax(int globalPitchBendMax, bool updateParameter=false);
    void setNotePitchBendMax(int notePitchBendMax, bool updateParameter=false);
    void setMaxPolyphony(int maxVoicesIn, bool updateParameter=false);
    
    void setRetuneOn(bool toRetuneIn, bool updateParameter=false);
    void setRetuneAuto(bool toRetuneAutoIn, bool updateParameter=false);
    void setRetuneMidiNotePreserved(bool preseveMidiNoteRetune, bool updateParameter=false);

	void setPeriodShift(int shiftIn, bool updateParameter=false);
    void setTransposeAmt(int stepsIn, bool updateParameter=false);
	void setMidiChannel(int midiChannelIn, bool updateParameter=false);
	void setShowNoteNums(bool showNoteNumsIn, bool updateParameter=false);
	void setShowFilteredNoteNums(bool showFilteredNoteNumsIn, bool updateParameter=false);
	void setShowNoteLabels(bool showNoteLabelsIn, bool updateParameter=false);
	void setScrollingMode(int modeIn, bool updateParameter=false);
	void setScrollingStyle(int styleIn, bool updateParameter=false);
	void setNumKeysInWidth(int numKeysIn, bool updateParameter=false);
	void setNumKeyboardRows(int rowsIn, bool updateParameter=false);
	void setKeyboardOrientation(int orientationIn, bool updateParameter=false);
	void setKeyStyle(int keyStyleIn, bool updateParameter=false);
	void setHighlightStyle(int highlightStyleIn, bool updateParameter=false);

	void setVelocityBehavior(int velocityIdIn, bool updateParameter=false);
	void setVelocityScalar(int velocityScalar, bool updateParameter=false);
	void setVelocityFixed(int velocityFixed, bool updateParameter=false);

	void setModeSlotRoot(int modeSlotIn, int rootNoteIn, bool updateParameter=false);
	void setMode1Root(int rootNoteIn, bool updateParameter=false);
    void setMode2Root(int rootNoteIn, bool updateParameter=false);
    
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
    void setTuningToModeET(double period);

	//==============================================================================
	// Internal Functionality

	void sendMappingToKeyboard();
	void sendMappingToKeyboard(ValueTree mapNodeIn);

	void updateModeViewed(bool sendChange=true);
    void updateToPreset(bool sendChange = true);
    
	void updateParameter(Identifier paramId);
	void updateParameters();
    void updateFromParameter(int paramInd);

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
    Array<Identifier> svkParameterIDs;

    SvkPreset* presetViewed;
    Mode* modeViewed; // What is currently on screen
    
	bool presetEdited = false;

	// Mapping parameters
    
    int mapOrder1 = 0;
    int mapOrder2 = 0;
    int mapOrderOffset1 = 0;
    int mapOrderOffset2 = 0;
    
    bool autoRetuneOn = false;
};
