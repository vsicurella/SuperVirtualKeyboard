/*
  ==============================================================================

    PluginSettings.h
    Created: 27 May 2019 1:46:06pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginIDs.h"
#include "CommonFunctions.h"

class SvkPluginSettings
{
	const String appFolderName = "SuperVirtualKeyboard";
	const String settingsFileName = "SuperVirtualKeyboard.settings";

	File factoryDefaultSettingsLocation = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile(appFolderName);
	File factoryDefaultPresetLocations = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(appFolderName);

	File currentSettingsLocation;
	File currentPresetLocation;

	bool createPresetFolder = true;
	bool saveFactoryPresets = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvkPluginSettings)

public:

	SvkPluginSettings();
	~SvkPluginSettings() {}

	ValueTree pluginSettingsNode;

	bool updateNode(bool writeSettings=false);
	bool restoreNode(ValueTree pluginSettingsNodeIn);

	String getSettingsPath();
	String getPresetPath();

	bool getCreatePresetFolder();
	bool getSaveFactoryPresets();

	void setCreatePresetFolder(bool shouldCreateFolder);
	void setSaveFactoryPresets(bool shouldSavePresets);

};
