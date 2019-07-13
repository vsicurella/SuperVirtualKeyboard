/*
  ==============================================================================

    PresetManager.h
    Created: 27 May 2019 10:57:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "CommonFunctions.h"
#include "PluginIDs.h"
#include "Structures/Preset.h"
#include "Structures/Mode.h"

class SvkPresetManager : public ChangeBroadcaster
{
	Array<ValueTree> loadedFactoryModes;
	Array<ValueTree> loadedUserModes;
	Array<ValueTree> favoriteModes;

	int numberOfModes = 0;

	Array<Array<ValueTree>> modesSorted;
	int numSortTypes = 4;

	ScaleSizeSorter scaleSizeSort;
	ModeSizeSorter modeSizeSort;
	FamilyNameSorter familyNameSort;
    
	std::unique_ptr<PopupMenu> modeMenu;
    OwnedArray<PopupMenu> modeSubMenu;

	OwnedArray<SvkPreset> presetsLoaded;
	OwnedArray<OwnedArray<Mode>> modeSlots;
	std::unique_ptr<Mode> modeCustom;

	// Methods
	void createFactoryModes();
	void resortModeLibrary();

	void initializeModePresets();
	void loadModeDirectory();

	int addModeToLibrary(ValueTree presetNodeIn);
	void addModeToSort(ValueTree presetNodeIn);
	int addAndSortMode(ValueTree presetNodeIn);

	void buildModeMenu();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvkPresetManager)

public:

	ValueTree presetLibraryNode;
	ValueTree modeLibraryNode;
	ValueTree pluginSettingsNode;

	SvkPresetManager(ValueTree pluginSettingsNodeIn);
	~SvkPresetManager();

	SvkPreset* getPresetLoaded(int slotNumIn=0);
    int getPresetLoadedId(int slotNumIn=0);
	ValueTree getPresetLoadedNode(int slotNumIn=0);
    
	Array<Array<ValueTree>>* getPresetsSorted();

	int getNumModesLoaded();
	int getNumModesInFavorites();

	ValueTree getModeInLibrary(int indexIn);
	Mode* getModeInSlots(int presetNumIn, int slotNumIn);
	Mode* getModeCustom();
    
	PopupMenu* getModeMenu();

	Mode* setModeCustom(ValueTree modeNodeIn);
	Mode* setModeCustom(String stepsIn, String familyIn = "undefined", int rootNoteIn = 60, String nameIn = "", String infoIn = "");

	void replaceModeInPreset(int presetSlotNum, int modeSlotNum, int modeLibraryIndexIn);
	int addModeToNewSlot(int presetSlotNum, int modeLibraryIndexIn);
	int addModeToNewSlot(int presetSlotNum, ValueTree modePresetNodeIn);

	Mode* loadModeIntoSlot(int presetSlotNum, int modeSlotNum, ValueTree modeNode);
	
	bool loadPreset(int presetSlotNum, ValueTree presetNodeIn, bool sendChangeSignal=true);
	bool loadPreset(int presetSlotNum, SvkPreset* presetIn, bool sendChangeSignal = true);
	bool loadPreset(int presetSlotNum, int presetLibraryId, bool sendChangeSignal=true);
	bool loadPreset(int presetSlotNum, bool sendChangeSignal=true);

	bool saveNodeToFile(ValueTree nodeToSave, String saveMsg, String fileEnding, String absolutePath = "");
	bool savePresetToFile(int presetSlotNum = 0, String absolutePath="");
	bool saveModeToFile(int presetSlotNum, int modeSlotNumber, String absolutePath = "");
	bool saveModeToFile(String absolutePath="");
    
    bool commitPreset(int slotNumber, ValueTree presetNodeIn);

	static ValueTree nodeFromFile(String openMsg, String fileEnding, String absoluteFilePath = "");
	static ValueTree modeFromFile(String absoluteFilePath = "");
	static ValueTree presetFromFile(String absoluteFilePath = "");
};
