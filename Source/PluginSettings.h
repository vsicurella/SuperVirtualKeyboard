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
    const String presetSubDirectory = "Presets";
    const String modeSubDirectory = "Modes";

	File factoryDefaultSettingsLocation = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile(appFolderName);
	File factoryDefaultPresetLocation = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(appFolderName).getChildFile(presetSubDirectory);
    File factoryDefaultModeLocation = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(appFolderName).getChildFile(modeSubDirectory);

	File currentSettingsLocation;
	File currentPresetLocation;
    File currentModeLocation;

	bool createPresetFolder = true;
	bool saveFactoryModes = false;
    
    bool resetDirectories = false;

	int midiIndexSelected = -1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvkPluginSettings)

public:

	SvkPluginSettings();
	~SvkPluginSettings() {}

	ValueTree pluginSettingsNode;

	bool updateNode(bool writeSettings=false);
	bool restoreNode(ValueTree pluginSettingsNodeIn);

	String getSettingsPath();
	String getPresetPath();
    String getModePath();

	bool getCreatePresetFolder();
	bool getSaveFactoryModes();

    int getMidiIndexSelected();

    void setPresetDirectory(File presetDirectoryIn);
    void setModeDirectory(File modeDirectoryIn);
    void setSettingsDirectory(File settingsDirectoryIn);

	void setCreatePresetFolder(bool shouldCreateFolder);
	void setSaveFactoryPresets(bool shouldSavePresets);

	void setMidiIndexSelected(int indexIn);
	

};
