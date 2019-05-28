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

class SvkPresetManager
{
	std::unique_ptr<SvkPreset> presetLoaded;
	std::unique_ptr<Mode> modeLoaded;

	Array<ValueTree> loadedFactoryPresets;
	Array<ValueTree> loadedUserPresets;

	Array<Array<int>> presetsSorted;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvkPresetManager)

public:

	ValueTree presetLibraryNode;
	ValueTree pluginSettingsNode;
	ValueTree presetLoadedNode;
	ValueTree modeLoadedNode;

	SvkPresetManager(ValueTree pluginSettingsNodeIn);
	~SvkPresetManager();

	SvkPreset* getPresetLoaded();
	Mode* getModeLoaded();
	
	// Can load either full preset or just Mode
	bool loadPreset(ValueTree presetNodeIn);
	bool loadPreset(int libraryIndexIn);
	bool loadPreset(SvkPreset* presetIn);
	bool loadPreset();

	bool savePreset(String absolutePath="");

	void createFactoryPresets();

	static SvkPreset* presetFromFile(String absoluteFilePath = "");


};