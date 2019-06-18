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
    
	parentNode.addChild(theModeNode, 0, nullptr);
}

SvkPreset::SvkPreset(ValueTree presetNodeIn)
{
	parentNode = ValueTree(IDs::presetNode);

    theModeNode = ValueTree(IDs::modePresetNode);
    theKeyboardNode = ValueTree(IDs::pianoNode);
    
    ValueTree nodeToCommit;
    
    nodeToCommit = extractNode(presetNodeIn, IDs::modePresetNode);

    if (nodeToCommit.isValid())
        theModeNode.copyPropertiesAndChildrenFrom(nodeToCommit, nullptr);
    
    nodeToCommit = extractNode(presetNodeIn, IDs::pianoNode);

    if (nodeToCommit.isValid())
        theKeyboardNode.copyPropertiesAndChildrenFrom(nodeToCommit, nullptr);

	parentNode.addChild(theModeNode, -1, nullptr);
	parentNode.addChild(theKeyboardNode, -1, nullptr);
}

SvkPreset::SvkPreset(SvkPreset& presetToCopy)
{
	SvkPreset(presetToCopy.parentNode);
}

SvkPreset::~SvkPreset() {}

bool SvkPreset::updateModeNode(ValueTree modeNodeIn)
{
	if (!theModeNode.isValid())
    {
        theModeNode = ValueTree(IDs::modePresetNode);
        parentNode.addChild(theModeNode, 0, nullptr);
    }
    
	theModeNode = ValueTree(modeNodeIn);
    
	return theModeNode.isValid();
}

bool SvkPreset::updateKeyboardNode(ValueTree keyboardNodeIn)
{
	if (!theKeyboardNode.isValid())
	{
		theKeyboardNode = ValueTree(IDs::pianoNode);
		parentNode.addChild(theKeyboardNode, -1, nullptr);
	}

	theKeyboardNode = ValueTree(keyboardNodeIn);
	
	return theKeyboardNode.isValid();
}

bool SvkPreset::updateMapNode(ValueTree mapNodeIn)
{
    if (!mapNodeIn.isValid())
    {
        theMapNode = ValueTree(IDs::midiMapNode);
        parentNode.addChild(theMapNode, -1, nullptr);
    }
    
    theMapNode = ValueTree(mapNodeIn);
    
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
