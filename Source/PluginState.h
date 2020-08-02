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
#include "PluginModes.h"
#include "PluginSettings.h"
#include "Midi/MidiProcessor.h"
#include "PresetManager.h"
#include "Structures/Preset.h"
#include "Structures/ModeMapper.h"

#include "UI/Components/VirtualKeyboard/KeyboardComponent.h"

struct SvkPluginState : public ChangeBroadcaster,
						public ChangeListener,
                        private AudioProcessorValueTreeState::Listener
{
    AudioProcessorValueTreeState& svkTree;
	ValueTree pluginStateNode;
	ValueTree pluginSettingsNode;
	ValueTree midiSettingsNode;
	ValueTree modeLibraryNode;
	ValueTree pluginEditorNode;
	ValueTree pianoNode;
    
    SvkPluginState(AudioProcessorValueTreeState& svkValueTreeIn);
	~SvkPluginState() {}
    
    void recallState(ValueTree nodeIn, bool fallbackToDefaultSettings = false);
    
	//==============================================================================
    // Object getters
    
	SvkPresetManager* getPresetManager();
	SvkMidiProcessor* getMidiProcessor();
	SvkPluginSettings* getPluginSettings();
    
    VirtualKeyboard::Keyboard* getKeyboard();
    ModeMapper* getModeMapper();
    
    NoteMap* getMidiInputFilterMap();
    NoteMap* getMidiOutputFilterMap();

	ValueTree getPreset();

	Mode* getModeInSlot(int slotNumIn);
	Mode* getModeViewed();
	Mode* getMode1();
	Mode* getMode2();
	Mode* getModeCustom();

	//==============================================================================
	// Parameter Getters

	float getParameterValue(Identifier paramId);
	float getParameterMin(Identifier paramId);
	float getParameterMax(Identifier paramId);
	float getParameterDefaultValue(Identifier paramId);

	int getNumModesLoaded();
	int getModeSelectorViewed();

	int getMappingMode();
	int getMappingStyle();
    bool isAutoMapping();

    //bool isAutoRetuning();

	int getModeSlotRoot(int slotNum);
	int getMode1Root();
	int getMode2Root();
    
    int getMapOrder1();
    int getMapOrder2();
    int getMapOrderOffset1();
    int getMapOrderOffset2();

	bool isPresetEdited();

    //==============================================================================

	void setParameterValue(Identifier paramIdIn, float valueIn);

	void setMapMode(int mapModeSelectionIn);
	void setMapStyle(int mapStyleIn);

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

	void setModeSelectorRoot(int modeSlotIn, int rootNoteIn, bool updateParameter=false);
    
	// Move these to VirtualKeyboard?
/*	void setKeyWidthMod(int keyNumIn, float widthMod);
	void setKeyHeightMod(int keyNumIn, float heightMod);
	void setKeyXOffset(int keyNumIn, int xOffset);
	void setKeyYOffset(int keyNumIn, int yOffset); */   

	//==============================================================================
	// User Functionality
    
    void setModeSelectorViewed(int modeViewedIn);
    void handleModeSelection(int modeBoxNum, int idIn);

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

	void revertToSavedPreset(bool fallbackToDefaultSettings = false, bool sendChange = true);

	bool savePresetToFile();
	bool loadPresetFromFile();

	bool saveModeViewedToFile();
	bool loadModeFromFile();

	void commitModeInfo();

	// TODO: make it so it's not necessary to call this before saving
	void commitStateNode();

	//==============================================================================

	void parameterChanged(const String& paramID, float newValue) override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	//==============================================================================

	std::unique_ptr<TextFilterIntOrSpace> textFilterIntOrSpace;
	std::unique_ptr<TextFilterInt> textFilterInt;

private:
	//==============================================================================
	// Internal Functionality

	void loadPropertiesOfNode(ValueTree pluginStateNodeIn, bool fallbackToDefault = false);

	void sendMappingToKeyboard();
	void sendMappingToKeyboard(ValueTree mapNodeIn);

	void updateModeViewed(bool sendChange=true);

private:

	std::unique_ptr<SvkPresetManager> presetManager;
	std::unique_ptr<SvkMidiProcessor> midiProcessor;
	std::unique_ptr<SvkPluginSettings> pluginSettings;

	std::unique_ptr<VirtualKeyboard::Keyboard> virtualKeyboard;
	std::unique_ptr<ModeMapper> modeMapper;

	ValueTree factoryDefaultPluginStateNode;
	ValueTree defaultPluginStateNode;
    
	ValueTree svkPreset;
    Mode* modeViewed; // What is currently on screen
    int modeSelectorViewedNum = 1;
    
	bool presetEdited = false;
    

	// Mapping parameters
    
    int mapOrder1 = 0;
    int mapOrder2 = 0;
    int mapOrderOffset1 = 0;
    int mapOrderOffset2 = 0;
    
    bool autoRetuneOn = false;
};
