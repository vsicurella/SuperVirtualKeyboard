/*
  ==============================================================================

    PluginSettings.cpp
    Created: 27 May 2019 1:46:06pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PluginSettings.h"

SvkPluginSettings::SvkPluginSettings()
{
	File settingsDirectory = factoryDefaultSettingsLocation;
	ValueTree settingsLoad;
		 
	if (settingsDirectory.isDirectory())
	{
		std::unique_ptr<XmlElement> xml = parseXML(settingsDirectory.getChildFile(settingsFileName));
       
        if (xml.get())
        {
            settingsLoad = ValueTree::fromXml(*(xml.get()));
            restoreNode(settingsLoad);
        }
	}

	if (currentSettingsLocation == File() || !currentSettingsLocation.exists())
	{
		currentSettingsLocation = factoryDefaultSettingsLocation;
        currentSettingsLocation.createDirectory();

		pluginSettingsNode = ValueTree(IDs::pluginSettingsNode);
		updateNode(true);
	}

	if (currentPresetLocation == File() || !currentPresetLocation.exists())
	{
		currentPresetLocation = factoryDefaultPresetLocations;

		if (createPresetFolder)
			currentPresetLocation.createDirectory();
	}
}

SvkPluginSettings::SvkPluginSettings(SvkPluginSettings& settingsToCopy)
{
	if (!restoreNode(settingsToCopy.pluginSettingsNode))
		SvkPluginSettings();
}

bool SvkPluginSettings::updateNode(bool writeSettings)
{
	pluginSettingsNode.setProperty(IDs::settingsDirectory, currentPresetLocation.getFullPathName(), nullptr);
	pluginSettingsNode.setProperty(IDs::presetDirectory, currentSettingsLocation.getFullPathName(), nullptr);
	pluginSettingsNode.setProperty(IDs::createPresetFolder, createPresetFolder, nullptr);
	pluginSettingsNode.setProperty(IDs::saveFactoryPresets, saveFactoryPresets, nullptr);
    
    if (writeSettings)
    {
        std::unique_ptr<XmlElement> xml(pluginSettingsNode.createXml());
        return xml->writeToFile(currentSettingsLocation.getChildFile(appFolderName), "");
    }
    
    return true;
}

bool SvkPluginSettings::restoreNode(ValueTree pluginSettingsNodeIn)
{
	if (pluginSettingsNodeIn.hasType(IDs::pluginSettingsNode))
	{
		pluginSettingsNode = pluginSettingsNodeIn;

		currentSettingsLocation = File(pluginSettingsNode.getProperty(IDs::settingsDirectory).toString());
		currentPresetLocation = File(pluginSettingsNode.getProperty(IDs::presetDirectory).toString());

		createPresetFolder = (bool)pluginSettingsNode.getProperty(IDs::createPresetFolder);
		saveFactoryPresets = (bool)pluginSettingsNode.getProperty(IDs::saveFactoryPresets);

		return true;
	}
	return false;
}


String SvkPluginSettings::getSettingsPath()
{
	return currentSettingsLocation.getFullPathName();
}

String SvkPluginSettings::getPresetPath()
{
	return currentPresetLocation.getFullPathName();
}

bool SvkPluginSettings::getCreatePresetFolder()
{
	return createPresetFolder;
}

bool SvkPluginSettings::getSaveFactoryPresets()
{
	return saveFactoryPresets;
}

void SvkPluginSettings::setCreatePresetFolder(bool shouldCreateFolder)
{
	createPresetFolder = shouldCreateFolder;
	pluginSettingsNode.setProperty(IDs::createPresetFolder, createPresetFolder, nullptr);
}

void SvkPluginSettings::setSaveFactoryPresets(bool shouldSavePresets)
{
	saveFactoryPresets = shouldSavePresets;
	pluginSettingsNode.setProperty(IDs::saveFactoryPresets, saveFactoryPresets, nullptr);
}