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
    pluginSettingsNode = ValueTree(IDs::globalSettingsNode);
    pluginSettingsNode.setProperty(IDs::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);

    File settingsDirectory = factoryDefaultSettingsLocation;
    ValueTree settingsLoad;
         
    if (settingsDirectory.isDirectory())
    {
        std::unique_ptr<XmlElement> xml = parseXML(settingsDirectory.getChildFile(settingsFileName));
       
        if (xml.get())
        {
            settingsLoad = ValueTree::fromXml(*(xml.get()));
            restoreNode(settingsLoad);
            DBG("Loaded these settings\n " + settingsLoad.toXmlString());
        }
        
        if (!pluginSettingsNode.hasProperty(IDs::modeDirectory))
            resetDirectories = true;
    }

    if (currentSettingsLocation == File() || !currentSettingsLocation.exists())
    {
        currentSettingsLocation = factoryDefaultSettingsLocation;
        currentSettingsLocation.createDirectory();
    }

    if (currentPresetLocation == File() || !currentPresetLocation.exists() || resetDirectories)
    {
        currentPresetLocation = factoryDefaultPresetLocation;
        currentModeLocation = factoryDefaultModeLocation;
        
        DBG("CurrentPresetLocations: " + currentPresetLocation.getFullPathName());
        DBG("CurrentModeLocations: " + currentModeLocation.getFullPathName());

        if (createPresetFolder)
        {
            currentPresetLocation.createDirectory();
            currentModeLocation.createDirectory();
        }
    }
    
    updateNode();
}

SvkPluginSettings::~SvkPluginSettings()
{
    updateNode(true);
}

bool SvkPluginSettings::updateNode(bool writeSettings)
{
    pluginSettingsNode.setProperty(IDs::settingsDirectory, currentSettingsLocation.getFullPathName(), nullptr);
    pluginSettingsNode.setProperty(IDs::presetDirectory, currentPresetLocation.getFullPathName(), nullptr);
    pluginSettingsNode.setProperty(IDs::modeDirectory, currentModeLocation.getFullPathName(), nullptr);
    pluginSettingsNode.setProperty(IDs::createPresetFolder, createPresetFolder, nullptr);
    pluginSettingsNode.setProperty(IDs::saveFactoryModes, saveFactoryModes, nullptr);
    
    if (writeSettings)
    {
        DBG("Writing settings to: " + currentSettingsLocation.getFullPathName());
        std::unique_ptr<XmlElement> xml(pluginSettingsNode.createXml());
        return xml->writeTo(currentSettingsLocation.getChildFile(settingsFileName));
    }
    
    return true;
}

bool SvkPluginSettings::restoreNode(ValueTree pluginSettingsNodeIn)
{
    if (pluginSettingsNodeIn.hasType(IDs::globalSettingsNode))
    {
        pluginSettingsNode.copyPropertiesAndChildrenFrom(pluginSettingsNodeIn, nullptr);

        pluginSettingsNode.setProperty(IDs::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);

        currentSettingsLocation = File(pluginSettingsNode[IDs::settingsDirectory].toString());
        currentPresetLocation = File(pluginSettingsNode[IDs::presetDirectory].toString());
        currentModeLocation = File(pluginSettingsNode[IDs::modeDirectory].toString());

        createPresetFolder = (bool)pluginSettingsNode[IDs::createPresetFolder];
        saveFactoryModes = (bool)pluginSettingsNode[IDs::saveFactoryModes];

        return true;
    }
    return false;
}

ValueTree SvkPluginSettings::getSettingsNode()
{
    return pluginSettingsNode;
}


String SvkPluginSettings::getSettingsPath()
{
    return currentSettingsLocation.getFullPathName();
}

String SvkPluginSettings::getPresetPath()
{
    return currentPresetLocation.getFullPathName();
}

String SvkPluginSettings::getModePath()
{
    return currentModeLocation.getFullPathName();
}

bool SvkPluginSettings::getCreatePresetFolder()
{
    return createPresetFolder;
}

bool SvkPluginSettings::getSaveFactoryModes()
{
    return saveFactoryModes;
}

void SvkPluginSettings::setPresetDirectory(File presetDirectoryIn)
{
    currentPresetLocation = presetDirectoryIn;
    pluginSettingsNode.setProperty(IDs::presetDirectory, currentPresetLocation.getFullPathName(), nullptr);
}

void SvkPluginSettings::setModeDirectory(File modeDirectoryIn)
{
    currentModeLocation = modeDirectoryIn;
    pluginSettingsNode.setProperty(IDs::modeDirectory, currentModeLocation.getFullPathName(), nullptr);
}

void SvkPluginSettings::setSettingsDirectory(File settingsDirectoryIn)
{
    currentSettingsLocation = settingsDirectoryIn;
    pluginSettingsNode.setProperty(IDs::settingsDirectory, currentSettingsLocation.getFullPathName(), nullptr);
}

void SvkPluginSettings::setCreatePresetFolder(bool shouldCreateFolder)
{
    createPresetFolder = shouldCreateFolder;
    pluginSettingsNode.setProperty(IDs::createPresetFolder, createPresetFolder, nullptr);
}

void SvkPluginSettings::setSaveFactoryPresets(bool shouldSavePresets)
{
    saveFactoryModes = shouldSavePresets;
    pluginSettingsNode.setProperty(IDs::saveFactoryModes, saveFactoryModes, nullptr);
}
