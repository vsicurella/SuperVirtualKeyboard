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
	const String settingsFileName = "SvkPluginSettings";

	File factoryDefaultSettingsLocation = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile(appFolderName);
	File factoryDefaultPresetLocations = File::getSpecialLocation(File::userDesktopDirectory).getChildFile(appFolderName);

	File currentSettingsLocation;
	File currentPresetLocation;

	bool createPresetFolder = true;
	bool saveFactoryPresets = false;

public:

	SvkPluginSettings();
	SvkPluginSettings(SvkPluginSettings& settingsToCopy);
	~SvkPluginSettings() {}

	ValueTree pluginSettingsNode;

	void updateNode();
	bool restoreNode(ValueTree pluginSettingsNodeIn);

	String getSettingsPath();
	String getPresetPath();

};