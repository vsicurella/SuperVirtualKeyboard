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
    
    std::unique_ptr<PopupMenu> modeMenu;
    OwnedArray<PopupMenu> modeSubMenu;

	// Methods
	void createFactoryModes();
	void resortModeLibrary();

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
    
    PopupMenu* getModeMenu();

	bool loadMode(int presetSlotNum, int modeSlotNum, ValueTree presetNodeIn, bool sendChangeSignal = true);
	bool loadMode(int presetSlotNum, int modeSlotNum, Mode* presetIn, bool sendChangeSignal = true);
	bool loadMode(int presetSlotNum, int modeSlotNum, bool sendChangeSignal = true);
	
	bool loadPreset(int slotNumber, ValueTree presetNodeIn, bool sendChangeSignal=true);
	bool loadPreset(int slotNumber, int indexIn, bool sendChangeSignal=true);
	bool loadPreset(int slotNumber, SvkPreset* presetIn, bool sendChangeSignal=true);
	bool loadPreset(int slotNumber, bool sendChangeSignal=true);

	bool savePreset(int slotNumber=0, String absolutePath="");
    
    bool commitPresetNode(int slotNumber, ValueTree nodeIn);
    bool commitModeNode(int slotNumber, ValueTree modeNodeIn);
    bool commitKeyboardNode(int slotNumber, ValueTree keyboardNodeIn);
    bool commitMapNode(int slotNumber, ValueTree mapNodeIn);

	void initializeModePresets();
	void loadPresetDirectory();

	int addModeToLibrary(ValueTree presetNodeIn);
	void addModeToSort(ValueTree presetNodeIn);
	int addAndSortMode(ValueTree presetNodeIn);
    
    void buildPresetMenu();

	static ValueTree presetFromFile(String absoluteFilePath = "");
};
