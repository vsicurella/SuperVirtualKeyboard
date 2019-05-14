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

SvkPreset::SvkPreset(ValueTree modeNodeIn, ValueTree keyboardNodeIn)
{
	parentNode = ValueTree(IDs::presetNode);
	theModeNode = ValueTree(IDs::modePresetNode);
	theKeyboardNode = ValueTree(IDs::pianoNode);

	theModeNode.copyPropertiesAndChildrenFrom(modeNodeIn, nullptr);
	theKeyboardNode.copyPropertiesAndChildrenFrom(keyboardNodeIn, nullptr);

	parentNode.addChild(theModeNode, 0, nullptr);
	parentNode.addChild(theKeyboardNode, 1, nullptr);
}

SvkPreset::SvkPreset(ValueTree presetNodeIn)
{
	SvkPreset(presetNodeIn.getChild(0), presetNodeIn.getChild(1));
}

SvkPreset::SvkPreset(SvkPreset& presetToCopy)
{
	SvkPreset(presetToCopy.parentNode);
}

SvkPreset::~SvkPreset() {}

bool SvkPreset::updateModeNode(ValueTree modeNodeIn)
{
	parentNode.removeChild(0, nullptr);

	theModeNode.copyPropertiesAndChildrenFrom(modeNodeIn, nullptr);

	parentNode.addChild(theModeNode, 0, nullptr);

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
    SvkPreset* presetOut;

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
			presetOut = new SvkPreset(nodeIn.getChild(0), nodeIn.getChild(1));
		}
	}
    
    return presetOut;
}

String SvkPreset::toString()
{
	return parentNode.toXmlString();
}
