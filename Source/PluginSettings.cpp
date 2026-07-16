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
        auto settingsFile = settingsDirectory.getChildFile(settingsFileName);
        auto xml = parseXML(settingsFile);
       
        if (xml.get())
        {
            settingsLoad = ValueTree::fromXml(*(xml.get()));
            restoreNode(settingsLoad);
            DBG("Loaded these settings\n " + settingsLoad.toXmlString());
        }
        
        // ??
        // resetDirectories = true;
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
}

SvkPluginSettings::~SvkPluginSettings()
{
    getSettingsNode(true);
}

ValueTree SvkPluginSettings::getSettingsNode(bool writeSettings)
{
    ValueTree pluginSettingsNode(SvkProperty::globalSettingsNode);
    pluginSettingsNode.setProperty(SvkProperty::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);
    pluginSettingsNode.setProperty(SvkProperty::settingsDirectory, currentSettingsLocation.getFullPathName(), nullptr);
    pluginSettingsNode.setProperty(SvkProperty::presetDirectory, currentPresetLocation.getFullPathName(), nullptr);
    pluginSettingsNode.setProperty(SvkProperty::modeDirectory, currentModeLocation.getFullPathName(), nullptr);
    pluginSettingsNode.setProperty(SvkProperty::createPresetFolder, createPresetFolder, nullptr);
    pluginSettingsNode.setProperty(SvkProperty::saveFactoryModes, saveFactoryModes, nullptr);
    pluginSettingsNode.setProperty(SvkProperty::minimalViewSetting, minimalView, nullptr);

    pluginSettingsNode.setProperty(SvkProperty::reaperUseScaleDegrees, reaperOptions.useScaleDegrees, nullptr);
    pluginSettingsNode.setProperty(SvkProperty::reaperAccidentalStyle, (int)reaperOptions.accidentalStyle, nullptr);
    pluginSettingsNode.setProperty(SvkProperty::reaperIncludeOctaves, reaperOptions.includeOctaves, nullptr);
    pluginSettingsNode.setProperty(SvkProperty::reaperOctaveDelimiter, reaperOptions.octaveDelimiter, nullptr);
    pluginSettingsNode.setProperty(SvkProperty::reaperKeyCenterNote, reaperOptions.keyCenterNote, nullptr);
    pluginSettingsNode.setProperty(SvkProperty::reaperKeyCenterOctave, reaperOptions.keyCenterOctave, nullptr);

    if (writeSettings)
        writeSettingsToFile(pluginSettingsNode);
    
    return pluginSettingsNode;
}

bool SvkPluginSettings::restoreNode(ValueTree pluginSettingsNodeIn)
{
    if (!pluginSettingsNodeIn.hasType(SvkProperty::globalSettingsNode))
        return false;

    currentSettingsLocation = File(pluginSettingsNodeIn[SvkProperty::settingsDirectory].toString());
    currentPresetLocation = File(pluginSettingsNodeIn[SvkProperty::presetDirectory].toString());
    currentModeLocation = File(pluginSettingsNodeIn[SvkProperty::modeDirectory].toString());

    createPresetFolder = (bool)pluginSettingsNodeIn[SvkProperty::createPresetFolder];
    saveFactoryModes = (bool)pluginSettingsNodeIn[SvkProperty::saveFactoryModes];
    minimalView = (bool)pluginSettingsNodeIn[SvkProperty::minimalViewSetting];

    if (pluginSettingsNodeIn.hasProperty(SvkProperty::reaperUseScaleDegrees))
        reaperOptions.useScaleDegrees = (bool)pluginSettingsNodeIn[SvkProperty::reaperUseScaleDegrees];
    if (pluginSettingsNodeIn.hasProperty(SvkProperty::reaperAccidentalStyle))
        reaperOptions.accidentalStyle = (ReaperWriter::Options::AccidentalStyle)(int)pluginSettingsNodeIn[SvkProperty::reaperAccidentalStyle];
    if (pluginSettingsNodeIn.hasProperty(SvkProperty::reaperIncludeOctaves))
        reaperOptions.includeOctaves = (bool)pluginSettingsNodeIn[SvkProperty::reaperIncludeOctaves];
    if (pluginSettingsNodeIn.hasProperty(SvkProperty::reaperOctaveDelimiter))
        reaperOptions.octaveDelimiter = pluginSettingsNodeIn[SvkProperty::reaperOctaveDelimiter].toString();
    if (pluginSettingsNodeIn.hasProperty(SvkProperty::reaperKeyCenterNote))
        reaperOptions.keyCenterNote = (int)pluginSettingsNodeIn[SvkProperty::reaperKeyCenterNote];
    if (pluginSettingsNodeIn.hasProperty(SvkProperty::reaperKeyCenterOctave))
        reaperOptions.keyCenterOctave = (int)pluginSettingsNodeIn[SvkProperty::reaperKeyCenterOctave];

    return true;
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

bool SvkPluginSettings::getMinimalView()
{
    return minimalView;
}

ReaperWriter::Options SvkPluginSettings::getReaperOptions()
{
    return reaperOptions;
}

void SvkPluginSettings::setReaperOptions(ReaperWriter::Options optionsIn)
{
    reaperOptions = optionsIn;

    // Persist immediately so the choices survive an unclean teardown.
    getSettingsNode(true);
}

void SvkPluginSettings::setPresetDirectory(File presetDirectoryIn)
{
    currentPresetLocation = presetDirectoryIn;
}

void SvkPluginSettings::setModeDirectory(File modeDirectoryIn)
{
    currentModeLocation = modeDirectoryIn;
}

void SvkPluginSettings::setSettingsDirectory(File settingsDirectoryIn)
{
    currentSettingsLocation = settingsDirectoryIn;
}

void SvkPluginSettings::setCreatePresetFolder(bool shouldCreateFolder)
{
    createPresetFolder = shouldCreateFolder;
}

void SvkPluginSettings::setSaveFactoryPresets(bool shouldSavePresets)
{
    saveFactoryModes = shouldSavePresets;
}

void SvkPluginSettings::setMinimalView(bool shouldBeMinimal)
{
    if (minimalView == shouldBeMinimal)
        return;

    minimalView = shouldBeMinimal;

    // Persist immediately so the preference survives even if the plugin isn't
    // torn down cleanly (the destructor is the only other write point).
    getSettingsNode(true);
}

bool SvkPluginSettings::writeSettingsToFile(ValueTree settingsNode)
{
    DBG("Writing settings to: " + currentSettingsLocation.getFullPathName());
    auto xml = settingsNode.createXml();
    return xml->writeTo(currentSettingsLocation.getChildFile(settingsFileName));
}
