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
    thePropertiesNode = ValueTree(IDs::presetProperties);
	theModeNode = ValueTree(IDs::modeSlotsNode);
	theKeyboardNode = ValueTree(IDs::pianoNode);
    theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);
	
	thePropertiesNode.setProperty(IDs::mode1SlotNumber, mode1SlotNumber, nullptr);
	thePropertiesNode.setProperty(IDs::mode2SlotNumber, mode2SlotNumber, nullptr);

    parentNode.addChild(thePropertiesNode, -1, nullptr);
	parentNode.addChild(theModeNode, -1, nullptr);
    parentNode.addChild(theKeyboardNode, -1, nullptr);
    parentNode.addChild(theMidiSettingsNode, -1, nullptr);
}

SvkPreset::SvkPreset(ValueTree presetNodeIn)
	: SvkPreset()
{
	restoreFromNode(presetNodeIn);
}

SvkPreset::SvkPreset(SvkPreset& presetToCopy)
:     SvkPreset(presetToCopy.parentNode)
{
}


SvkPreset::~SvkPreset() {}

bool SvkPreset::restoreFromNode(ValueTree presetNodeIn)
{
    bool success = false;
    
	if (!presetNodeIn.isValid())
		return success;
	
	theModeNode = ValueTree(IDs::modeSlotsNode);
	Array<ValueTree> modeSlots;
	
	if (isValidPresetNode(presetNodeIn, modeSlots))
	{
		addModes(modeSlots);
        
        ValueTree propertiesTry = presetNodeIn.getChildWithName(IDs::presetProperties);
        if (propertiesTry.isValid())
        {
            mode1SlotNumber = propertiesTry[IDs::mode1SlotNumber];
            mode2SlotNumber = propertiesTry[IDs::mode2SlotNumber];
        }
        
		ValueTree keyboardNodeTry = presetNodeIn.getChildWithName(IDs::pianoNode);
		if (keyboardNodeTry.isValid())
			theKeyboardNode = keyboardNodeTry.createCopy();
		else
			theKeyboardNode = ValueTree(IDs::pianoNode);

		ValueTree mapNodeTry = presetNodeIn.getChildWithName(IDs::midiSettingsNode);
		if (mapNodeTry.isValid())
			theMidiSettingsNode = mapNodeTry.createCopy();
		else
			theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);

		parentNode = ValueTree(IDs::presetNode);
        
        success = true;
	}
	else
	{
		theKeyboardNode = ValueTree(IDs::pianoNode);
		theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);
	}

	parentNode.addChild(theModeNode, -1, nullptr);
	parentNode.addChild(theKeyboardNode, -1, nullptr);
	parentNode.addChild(theMidiSettingsNode, -1, nullptr);
    
    return success;
}

void SvkPreset::commitPreset()
{
	theModeNode.removeAllChildren(nullptr);

	for (int i = 0; i < modeSlots.size(); i++)
	{
		theModeNode.addChild(modeSlots[i].createCopy(), i, nullptr);
	}
}

ValueTree SvkPreset::getModeInSlot(int slotNum)
{
	return modeSlots[slotNum];
}

ValueTree SvkPreset::getMode1()
{
	return modeSlots[mode1SlotNumber];
}

ValueTree SvkPreset::getMode2()
{
	return modeSlots[mode2SlotNumber];
}

int SvkPreset::getMode1SlotNumber()
{
	return mode1SlotNumber;
}
int SvkPreset::getMode2SlotNumber()
{
	return mode2SlotNumber;
}

int SvkPreset::getNumModes()
{
	return modeSlots.size();
}

void SvkPreset::setMode1SlotNumber(int slotNumIn)
{
	if (slotNumIn >= 0)
	{
		mode1SlotNumber = jlimit(0, modeSlots.size(), slotNumIn);
		theModeNode.setProperty(IDs::mode1SlotNumber, mode1SlotNumber, nullptr);
	}
}

void SvkPreset::setMode2SlotNumber(int slotNumIn)
{
	if (slotNumIn >= 0)
	{
		mode2SlotNumber = jlimit(0, modeSlots.size(), slotNumIn);
		theModeNode.setProperty(IDs::mode2SlotNumber, mode2SlotNumber, nullptr);
	}
}

int SvkPreset::setModeSlot(ValueTree modeNodeIn, int slotNumber)
{
	if (Mode::isValidMode(modeNodeIn))
	{
		modeSlots.ensureStorageAllocated(slotNumber + 1);

		modeSlots.set(slotNumber, modeNodeIn);
		return slotNumber;
	}

	return -1;
}

int SvkPreset::addMode(ValueTree modeNodeIn)
{
	if (Mode::isValidMode(modeNodeIn))
	{
		modeSlots.add(modeNodeIn);
		return modeSlots.size() - 1;
	}

	return -1;
}

bool SvkPreset::addModes(Array<ValueTree> modeSlotsIn)
{
	int numAdded = 0;

	for (auto mode : modeSlotsIn)
	{
		if (Mode::isValidMode(mode))
		{
			modeSlots.add(mode);
			numAdded++;
		}
	}

	return numAdded > 0;
}

bool SvkPreset::isValidPresetNode(ValueTree presetNodeIn)
{
	bool isValid = presetNodeIn.hasType(IDs::presetNode);
	
	if (isValid)
	{
		if (extractNodes(presetNodeIn, IDs::modePresetNode).size() > 0)
			return true;
	}

	return false;
}

bool SvkPreset::isValidPresetNode(ValueTree presetNodeIn, Array<ValueTree>& modesContained)
{
	bool isValid = presetNodeIn.hasType(IDs::presetNode);

	if (isValid)
	{
		modesContained = extractNodes(presetNodeIn, IDs::modePresetNode);

		if (modesContained.size() > 0)
			return true;
	}

	return false;
}

String SvkPreset::toString()
{
	return parentNode.toXmlString();
}
