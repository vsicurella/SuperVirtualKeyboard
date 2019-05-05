/*
 ==============================================================================
 
 FractionalGrid.cpp
 Created: 22 Apr 2019 12:05:22pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#include "Preset.h"

SvkPreset::SvkPreset(ValueTree modeNodeIn, ValueTree pianoNodeIn)
{
	parentNode = ValueTree(IDs::presetNode);
	theModeNode.copyPropertiesAndChildrenFrom(modeNodeIn, nullptr);
	theKeyPlaceNode = parentNode.getChildWithName(IDs::keyPlacePresetNode);
	theKeyRatiosNode = parentNode.getChildWithName(IDs::keyRatioPresetNode);
	theKeyColorsNode = parentNode.getChildWithName(IDs::keyColorPresetNode);

	parentNode.addChild(theModeNode, 0, nullptr);
	parentNode.addChild(theKeyPlaceNode, 1, nullptr);
	parentNode.addChild(theKeyRatiosNode, 2, nullptr);
	parentNode.addChild(theKeyColorsNode, 3, nullptr);
}

SvkPreset::SvkPreset(Mode* modeIn, VirtualKeyboard::Keyboard* pianoIn)
{
	SvkPreset(modeIn->modeNode, pianoIn->getNode());
}

SvkPreset::SvkPreset(ValueTree presetNodeIn)
{
	parentNode.copyPropertiesAndChildrenFrom(presetNodeIn, nullptr);
	theModeNode = parentNode.getChildWithName(IDs::modePresetNode);
	theKeyPlaceNode = parentNode.getChildWithName(IDs::keyPlacePresetNode);
	theKeyRatiosNode = parentNode.getChildWithName(IDs::keyRatioPresetNode);
	theKeyColorsNode = parentNode.getChildWithName(IDs::keyColorPresetNode);
}

SvkPreset::SvkPreset(SvkPreset& presetToCopy)
{
	SvkPreset(presetToCopy.parentNode);
}

SvkPreset::~SvkPreset() {}

bool SvkPreset::writeToFile(String absoluteFilePath = "")
{
	File fileOut = File(absoluteFilePath);

	if (!fileOut.getParentDirectory().exists())
	{
		FileChooser chooser = FileChooser("Save your preset",
			File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory),
			"*.svk");
		
		chooser.browseForFileToSave(true);
		fileOut = chooser.getResult();
	}

	std::unique_ptr<XmlElement> xml(parentNode.createXml());
	return xml->writeToFile(fileOut, "");
}

SvkPreset SvkPreset::loadFromFile(String absoluteFilePath = "")
{
	ValueTree nodeIn;
	File fileIn = File(absoluteFilePath);

	if (!fileIn.exists())
	{
		FileChooser chooser = FileChooser("Open preset",
			File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory),
			"*.svk");

		chooser.browseForFileToOpen();
		fileIn = chooser.getResult();
	}

	std::unique_ptr<XmlElement> xml = parseXML(fileIn);
	nodeIn = ValueTree::fromXml(*(xml.get()));

	if (nodeIn.hasType(IDs::presetNode))
	{
		return SvkPreset(nodeIn);
	}
	
	return SvkPreset(ValueTree());
}

String SvkPreset::toString()
{
	return parentNode.toXmlString();
}