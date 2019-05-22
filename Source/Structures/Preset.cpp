/*
 ==============================================================================
 
 FractionalGrid.cpp
 Created: 22 Apr 2019 12:05:22pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#include "Preset.h"

SvkPreset::SvkPreset()
{
	parentNode = ValueTree(IDs::presetNode);
}

SvkPreset::SvkPreset(ValueTree presetNodeIn)
{
	parentNode = ValueTree(IDs::presetNode);
	thePluginSettingsNode = ValueTree(IDs::pluginSettingsNode);
	theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);
	theModeNode = ValueTree(IDs::modePresetNode);
	theKeyboardNode = ValueTree(IDs::pianoNode);

	thePluginSettingsNode.copyPropertiesAndChildrenFrom(
		presetNodeIn.getChildWithName(IDs::pluginSettingsNode), nullptr);
	theMidiSettingsNode.copyPropertiesAndChildrenFrom(
		presetNodeIn.getChildWithName(IDs::midiSettingsNode), nullptr);
	theModeNode.copyPropertiesAndChildrenFrom(
		presetNodeIn.getChildWithName(IDs::modePresetNode), nullptr);
	theKeyboardNode.copyPropertiesAndChildrenFrom(
		presetNodeIn.getChildWithName(IDs::pianoNode), nullptr);

	parentNode.addChild(thePluginSettingsNode, 0, nullptr);
	parentNode.addChild(theMidiSettingsNode, 1, nullptr);
	parentNode.addChild(theModeNode, 2, nullptr);
	parentNode.addChild(theKeyboardNode, 3, nullptr);
}

SvkPreset::SvkPreset(SvkPreset& presetToCopy)
{
	SvkPreset(presetToCopy.parentNode);
}

bool SvkPreset::updateParentNode(ValueTree pluginStateNodeIn)
{
	updatePluginSettingsNode(pluginStateNodeIn.getChildWithName(IDs::pluginSettingsNode));
	updateMidiSettingsNode(pluginStateNodeIn.getChildWithName(IDs::midiSettingsNode));
	updateModeNode(pluginStateNodeIn.getChildWithName(IDs::modePresetNode));
	updateKeyboardNode(pluginStateNodeIn.getChildWithName(IDs::pianoNode));

	return pluginStateNodeIn.isValid();
}

bool SvkPreset::updatePluginSettingsNode(ValueTree pluginSettingsIn)
{
	if (!thePluginSettingsNode.isValid())
	{
		thePluginSettingsNode = ValueTree(IDs::pluginSettingsNode);
		parentNode.addChild(thePluginSettingsNode, 0, nullptr);
	}

	if (pluginSettingsIn.hasType(IDs::pluginSettingsNode))
	{
		thePluginSettingsNode.copyPropertiesAndChildrenFrom(pluginSettingsIn, nullptr);
	}

	return pluginSettingsIn.isValid();
}

bool SvkPreset::updateMidiSettingsNode(ValueTree midiSettingsIn)
{
	if (!theMidiSettingsNode.isValid())
	{
		theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);
		parentNode.addChild(theMidiSettingsNode, 1, nullptr);
	}

	if (midiSettingsIn.hasType(IDs::midiSettingsNode))
	{
		theMidiSettingsNode.copyPropertiesAndChildrenFrom(midiSettingsIn, nullptr);
	}

	return midiSettingsIn.isValid();
}

bool SvkPreset::updateModeNode(ValueTree modeNodeIn)
{
	if (!theModeNode.isValid())
    {
        theModeNode = ValueTree(IDs::modePresetNode);
        parentNode.addChild(theModeNode, 2, nullptr);
    }
    
	if (modeNodeIn.hasType(IDs::modePresetNode))
		theModeNode.copyPropertiesAndChildrenFrom(modeNodeIn, nullptr);
    
	return modeNodeIn.isValid();
}

bool SvkPreset::updateKeyboardNode(ValueTree keyboardNodeIn)
{
	if (!theKeyboardNode.isValid())
	{
		theKeyboardNode = ValueTree(IDs::pianoNode);
		parentNode.addChild(theKeyboardNode, 3, nullptr);
	}

	if (keyboardNodeIn.hasType(IDs::pianoNode))
		theKeyboardNode.copyPropertiesAndChildrenFrom(keyboardNodeIn, nullptr);
	
	return keyboardNodeIn.isValid();
}

bool SvkPreset::writeToFile(String absoluteFilePath)
{
	File fileOut = File(absoluteFilePath);

	if (!fileOut.getParentDirectory().exists())
	{		
		FileChooser chooser("Save your preset", 
			File::getSpecialLocation(File::userDocumentsDirectory),
			"*.svk");

		chooser.browseForFileToSave(true);
		fileOut = chooser.getResult();
	}

	DBG(parentNode.toXmlString());

	if (fileOut.getParentDirectory().exists())
	{
		std::unique_ptr<XmlElement> xml(parentNode.createXml());
		return xml->writeToFile(fileOut, "");
	}

	return false;
}

SvkPreset* SvkPreset::loadFromFile(String absoluteFilePath)
{
	ValueTree nodeIn;
	File fileIn = File(absoluteFilePath);
    SvkPreset* presetOut = nullptr;

	if (!fileIn.exists())
	{
		FileChooser chooser("Open preset",
			File::getSpecialLocation(File::userDocumentsDirectory),
			"*.svk");

		chooser.browseForFileToOpen();
		fileIn = chooser.getResult();
	}

	if (fileIn.exists())
	{
		std::unique_ptr<XmlElement> xml = parseXML(fileIn);
		nodeIn = ValueTree::fromXml(*(xml.get()));

		if (nodeIn.hasType(IDs::presetNode))
		{
			presetOut = new SvkPreset(nodeIn);
		}
	}
    
    return presetOut;
}

String SvkPreset::toString()
{
	return parentNode.toXmlString();
}
