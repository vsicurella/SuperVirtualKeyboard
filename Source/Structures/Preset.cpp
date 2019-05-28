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
	theModeNode = ValueTree(IDs::modePresetNode);
	parentNode.addChild(theModeNode, 0, nullptr);
}

SvkPreset::SvkPreset(ValueTree presetNodeIn)
{
	parentNode = ValueTree(IDs::presetNode);
	theModeNode = ValueTree(IDs::modePresetNode);
	theKeyboardNode = ValueTree(IDs::pianoNode);

	theModeNode.copyPropertiesAndChildrenFrom(presetNodeIn.getChild(0), nullptr);
	theKeyboardNode.copyPropertiesAndChildrenFrom(presetNodeIn.getChild(1), nullptr);

	parentNode.addChild(theModeNode, 0, nullptr);
	parentNode.addChild(theKeyboardNode, 1, nullptr);

	if (presetNodeIn.hasProperty(IDs::rootMidiNote))
		parentNode.setProperty(IDs::rootMidiNote,(int)presetNodeIn[IDs::rootMidiNote], nullptr);
}

SvkPreset::SvkPreset(SvkPreset& presetToCopy)
{
	SvkPreset(presetToCopy.parentNode);
}

SvkPreset::~SvkPreset() {}

bool SvkPreset::updateParentNode(ValueTree pluginStateIn)
{
    if (pluginStateIn.hasType(IDs::pluginStateNode) || pluginStateIn.hasType((IDs::presetNode)))
    {
        theKeyboardNode.copyPropertiesFrom(pluginStateIn.getChildWithName(IDs::pianoNode), nullptr);
        
        theModeNode.copyPropertiesFrom(pluginStateIn.getChildWithName(IDs::modePresetNode), nullptr);
        
        thePluginSettingsNode.copyPropertiesFrom(pluginStateIn.getChildWithName(IDs::pluginSettingsNode), nullptr);
        
        theMidiSettingsNode.copyPropertiesFrom(pluginStateIn.getChildWithName(IDs::midiSettingsNode), nullptr);
        
    }
    
    return pluginStateIn.isValid();
}

bool SvkPreset::updateModeNode(ValueTree modeNodeIn)
{
	if (!theModeNode.isValid())
    {
        theModeNode = ValueTree(IDs::modePresetNode);
        parentNode.addChild(theModeNode, 0, nullptr);
    }
    
	theModeNode.copyPropertiesAndChildrenFrom(modeNodeIn, nullptr);
    
	return theModeNode.isValid();
}

bool SvkPreset::updateKeyboardNode(ValueTree keyboardNodeIn)
{
	if (!theKeyboardNode.isValid())
	{
		theKeyboardNode = ValueTree(IDs::pianoNode);
		parentNode.addChild(theKeyboardNode, 1, nullptr);
	}

	theKeyboardNode.copyPropertiesAndChildrenFrom(keyboardNodeIn, nullptr);
	
	return theKeyboardNode.isValid();
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

	if (fileOut.getParentDirectory().exists())
	{
		std::unique_ptr<XmlElement> xml(parentNode.createXml());
		return xml->writeToFile(fileOut, "");
	}

	return false;
}

String SvkPreset::toString()
{
	return parentNode.toXmlString();
}
