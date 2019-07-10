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
	OwnedArray<SvkPreset> presetsLoaded;

	Array<ValueTree> loadedFactoryPresets;
	Array<ValueTree> loadedUserPresets;

	int numberOfModes = 0;

	Array<Array<ValueTree>> modesSorted;
	int numSortTypes = 4;

	ScaleSizeSorter scaleSizeSort;
	ModeSizeSorter modeSizeSort;
	FamilyNameSorter familyNameSort;
    
	std::unique_ptr<PopupMenu> mode1Menu;
    std::unique_ptr<PopupMenu> mode2Menu;
    OwnedArray<PopupMenu> modeSubMenu;

	// Methods
	void createFactoryModes();
	void resortModeLibrary();

	void initializeModePresets();
	void loadPresetDirectory();

	int addModeToLibrary(ValueTree presetNodeIn);
	void addModeToSort(ValueTree presetNodeIn);
	int addAndSortMode(ValueTree presetNodeIn);

	void buildPresetMenu();

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

	ValueTree getMode(int indexIn);
    
	PopupMenu* getMode1Menu();
    PopupMenu* getMode2Menu();

	bool loadMode(int presetSlotNum, int modeSlotNum, ValueTree modeNodeIn, bool sendChangeSignal = true);
	bool loadMode(int presetSlotNum, int modeSlotNum, Mode* modeIn, bool sendChangeSignal = true);
	bool loadMode(int presetSlotNum, int modeSlotNum, int modeLibraryId, bool sendChangeSignal = true);
	bool loadMode(int presetSlotNum, int modeSlotNum, bool sendChangeSignal = true);
	
	bool loadPreset(int presetSlotNum, ValueTree presetNodeIn, bool sendChangeSignal=true);
	bool loadPreset(int presetSlotNum, SvkPreset* presetIn, bool sendChangeSignal = true);
	bool loadPreset(int presetSlotNum, int presetLibraryId, bool sendChangeSignal=true);
	bool loadPreset(int presetSlotNum, bool sendChangeSignal=true);

	bool savePreset(int presetSlotNum =0, String absolutePath="");
    
    bool commitPreset(int slotNumber, ValueTree presetNodeIn);


	static ValueTree nodeFromFile(String openMsg, String fileEnding, String absoluteFilePath = "");
	static ValueTree modeFromFile(String absoluteFilePath = "");
	static ValueTree presetFromFile(String absoluteFilePath = "");
};
