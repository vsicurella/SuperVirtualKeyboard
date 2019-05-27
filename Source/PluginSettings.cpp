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
		settingsLoad = ValueTree::fromXml(*(xml.get()));

		restoreNode(settingsLoad);
	}

	if (currentSettingsLocation == File())
	{
		currentSettingsLocation = factoryDefaultSettingsLocation;
		currentSettingsLocation = currentSettingsLocation.getChildFile(appFolderName);
		currentSettingsLocation.createDirectory();

		pluginSettingsNode = ValueTree(IDs::pluginSettingsNode);
		updateNode();
	}

	if (currentPresetLocation == File())
	{
		currentPresetLocation = factoryDefaultPresetLocations;
		currentPresetLocation = currentSettingsLocation.getChildFile(appFolderName);

		if (createPresetFolder)
			currentPresetLocation.createDirectory();
	}
}

SvkPluginSettings::SvkPluginSettings(SvkPluginSettings& settingsToCopy)
{
	if (!restoreNode(settingsToCopy.pluginSettingsNode))
		SvkPluginSettings();
}

void SvkPluginSettings::updateNode()
{
	pluginSettingsNode.setProperty(IDs::settingsDirectory, currentPresetLocation.getFullPathName(), nullptr);
	pluginSettingsNode.setProperty(IDs::presetDirectory, currentSettingsLocation.getFullPathName(), nullptr);
	pluginSettingsNode.setProperty(IDs::createPresetFolder, createPresetFolder, nullptr);
	pluginSettingsNode.setProperty(IDs::saveFactoryPresets, saveFactoryPresets, nullptr);
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