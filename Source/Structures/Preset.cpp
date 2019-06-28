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
    theMidiSettingsNode = ValueTree(IDs::midiMapNode);
    
	parentNode.addChild(theModeNode, -1, nullptr);
    parentNode.addChild(theKeyboardNode, -1, nullptr);
    parentNode.addChild(theMidiSettingsNode, -1, nullptr);
}

SvkPreset::SvkPreset(ValueTree presetNodeIn)
{
	if (!presetNodeIn.isValid())
		return;
    
	ValueTree modeNodeTry = presetNodeIn.getChildWithName(IDs::modePresetNode);
	if (modeNodeTry.isValid())
		theModeNode = modeNodeTry.createCopy();
    else if (presetNodeIn.hasType(IDs::modePresetNode))
        theModeNode = presetNodeIn.createCopy();
    else
        theModeNode = ValueTree(IDs::modePresetNode);

    ValueTree keyboardNodeTry = presetNodeIn.getChildWithName(IDs::pianoNode);
	if (keyboardNodeTry.isValid())
		theKeyboardNode = keyboardNodeTry.createCopy();
    else
        theKeyboardNode = ValueTree(IDs::pianoNode);

	ValueTree mapNodeTry = presetNodeIn.getChildWithName(IDs::midiMapNode);
	if (mapNodeTry.isValid())
		theMidiSettingsNode = mapNodeTry.createCopy();
    else
        theMidiSettingsNode = ValueTree(IDs::midiMapNode);

    parentNode = ValueTree(IDs::presetNode);
    
    parentNode.addChild(theModeNode, -1, nullptr);
    parentNode.addChild(theKeyboardNode, -1, nullptr);
    parentNode.addChild(theMidiSettingsNode, -1, nullptr);
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
    DBG("Keyboard Node Commited:\n" + theKeyboardNode.toXmlString());
	return theKeyboardNode.isValid();
}

bool SvkPreset::updateMidiNode(ValueTree midiNodeIn)
{
    if (!midiNodeIn.isValid())
    {
        theMidiSettingsNode = ValueTree(IDs::midiMapNode);
        parentNode.addChild(theMidiSettingsNode, -1, nullptr);
    }
    
    theMidiSettingsNode.copyPropertiesAndChildrenFrom(midiNodeIn, nullptr);

    return theMidiSettingsNode.isValid();
}

String SvkPreset::toString()
{
	return parentNode.toXmlString();
}
