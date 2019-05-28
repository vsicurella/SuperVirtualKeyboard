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

class SvkPresetManager
{
	std::unique_ptr<SvkPreset> presetLoaded;

	Array<ValueTree> loadedFactoryPresets;
	Array<ValueTree> loadedUserPresets;

	Array<Array<int>> presetsSorted;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvkPresetManager)

public:

	ValueTree presetLibraryNode;
	ValueTree pluginSettingsNode;
	ValueTree presetLoadedNode;

	SvkPresetManager(ValueTree pluginSettingsNodeIn);
	~SvkPresetManager();


	SvkPreset* getPresetLoaded();
	Mode* getModeLoaded();
	
	// Can load either full preset or just Mode
	bool loadPreset();
	bool loadPreset(SvkPreset& presetIn);
	bool loadPreset(ValueTree presetNodeIn);
	bool loadPreset(int libraryIndexIn);

	bool savePreset();

	void createFactoryPresets();

	static SvkPreset* presetFromFile(String absoluteFilePath = "");


};