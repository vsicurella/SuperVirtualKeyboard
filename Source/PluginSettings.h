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

	bool createPresetFolder = false;
	bool saveFactoryPresets = false;

public:

	SvkPluginSettings();
	SvkPluginSettings(SvkPluginSettings& settingsToCopy);
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
