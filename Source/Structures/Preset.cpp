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
	theKeyboardNode = ValueTree(IDs::pianoNode);
    theMapNode = ValueTree(IDs::midiMapNode);
    
	parentNode.addChild(theModeNode, -1, nullptr);
}

SvkPreset::SvkPreset(ValueTree presetNodeIn)
	: SvkPreset()
{
	parentNode = presetNodeIn.createCopy();
	if (!presetNodeIn.isValid() || !presetNodeIn.hasType(IDs::presetNode))
		return;
    
	ValueTree modeNodeTry = parentNode.getChildWithName(IDs::modePresetNode);
	if (modeNodeTry.isValid())
		theModeNode = modeNodeTry;

    ValueTree keyboardNodeTry = parentNode.getChildWithName(IDs::pianoNode);
	if (keyboardNodeTry.isValid())
		theKeyboardNode = keyboardNodeTry;
    
	ValueTree mapNodeTry = parentNode.getChildWithName(IDs::midiMapNode);
	if (mapNodeTry.isValid())
		theMapNode = mapNodeTry;
}

SvkPreset::SvkPreset(SvkPreset& presetToCopy)
:     SvkPreset(presetToCopy.parentNode)
{
}


SvkPreset::~SvkPreset() {}

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
		parentNode.addChild(theKeyboardNode, -1, nullptr);
	}

    theKeyboardNode.copyPropertiesAndChildrenFrom(keyboardNodeIn, nullptr);

	return theKeyboardNode.isValid();
}

bool SvkPreset::updateMapNode(ValueTree mapNodeIn)
{
    if (!mapNodeIn.isValid())
    {
        theMapNode = ValueTree(IDs::midiMapNode);
        parentNode.addChild(theMapNode, -1, nullptr);
    }
    
    theMapNode.copyPropertiesAndChildrenFrom(mapNodeIn, nullptr);

    return theMapNode.isValid();
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
