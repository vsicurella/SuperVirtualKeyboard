/*
  ==============================================================================

    PresetManager.h
    Created: 27 May 2019 10:57:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "../CommonFunctions.h"
#include "../PluginIDs.h"
#include "Preset.h"
#include "Mode.h"

class SvkPresetManager : public ComboBox::Listener,
							public ChangeBroadcaster
{
	std::unique_ptr<SvkPreset> presetLoaded;

	Array<ValueTree> loadedFactoryPresets;
	Array<ValueTree> loadedUserPresets;

	Array<Array<ValueTree>> presetsSorted;
	int numSortTypes = 4;

	ScaleSizeSorter scaleSizeSort;
	ModeSizeSorter modeSizeSort;
	FamilyNameSorter familyNameSort;

	// Methods
	void createFactoryPresets();
	void resortPresetLibrary();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvkPresetManager)

public:

	ValueTree presetLibraryNode;
	ValueTree pluginSettingsNode;
	ValueTree presetNode;

	SvkPresetManager(ValueTree pluginSettingsNodeIn);
	~SvkPresetManager();

	SvkPreset* getPresetLoaded();

	Array<Array<ValueTree>>* getPresetsSorted();
	
	// Can load either full preset or just Mode
	bool loadPreset(ValueTree presetNodeIn);
	bool loadPreset(int libraryIndexIn);
	bool loadPreset(SvkPreset* presetIn);
	bool loadPreset();

	bool savePreset(String absolutePath="");

	void intializePresets();
	void loadPresetDirectory();

	int addPresetToLibrary(ValueTree presetNodeIn);
	void addPresetToSort(ValueTree presetNodeIn);
	int addAndSortPreset(ValueTree presetNodeIn);

	void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;

	static SvkPreset* presetFromFile(String absoluteFilePath = "");


};