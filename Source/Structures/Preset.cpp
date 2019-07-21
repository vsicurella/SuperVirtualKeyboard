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

	modeSelectorSlotNumbers.add(0);
	modeSelectorSlotNumbers.add(1);
    
    thePropertiesNode.setProperty(IDs::mode1SlotNumber, 0, nullptr);
    thePropertiesNode.setProperty(IDs::mode2SlotNumber, 1, nullptr);

    parentNode.addChild(thePropertiesNode, -1, nullptr);
	parentNode.addChild(theModeNode, -1, nullptr);
    parentNode.addChild(theKeyboardNode, -1, nullptr);
    parentNode.addChild(theMidiSettingsNode, -1, nullptr);
}

SvkPreset::SvkPreset(const ValueTree presetNodeIn)
	: SvkPreset()
{
	ValueTree nodeCopy = presetNodeIn.createCopy();

	theModeNode = ValueTree(IDs::modeSlotsNode);
	Array<ValueTree> modeSlots;

	if (isValidPresetNode(nodeCopy, modeSlots))
	{
		addModes(modeSlots);

		ValueTree propertiesTry = nodeCopy.getChildWithName(IDs::presetProperties);
		if (propertiesTry.isValid())
		{
            modeSelectorSlotNumbers.set(0, propertiesTry[IDs::mode1SlotNumber]);
			modeSelectorSlotNumbers.set(1, propertiesTry[IDs::mode2SlotNumber]);
		}

		ValueTree keyboardNodeTry = nodeCopy.getChildWithName(IDs::pianoNode);
		if (keyboardNodeTry.isValid())
			theKeyboardNode = keyboardNodeTry.createCopy();
		else
			theKeyboardNode = ValueTree(IDs::pianoNode);

		ValueTree mapNodeTry = nodeCopy.getChildWithName(IDs::midiSettingsNode);
		if (mapNodeTry.isValid())
			theMidiSettingsNode = mapNodeTry.createCopy();
		else
			theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);
	}
	else
	{
		theKeyboardNode = ValueTree(IDs::pianoNode);
		theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);
	}
}

SvkPreset::SvkPreset(const SvkPreset& presetToCopy)
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
        modeSelectorSlotNumbers.ensureStorageAllocated(modeSlots.size());
        
        ValueTree propertiesTry = presetNodeIn.getChildWithName(IDs::presetProperties);
        if (propertiesTry.isValid())
        {
			modeSelectorSlotNumbers.set(0, propertiesTry[IDs::mode1SlotNumber]);
			modeSelectorSlotNumbers.set(1, propertiesTry[IDs::mode2SlotNumber]);
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
        
        success = true;
	}
	else
	{
		theKeyboardNode = ValueTree(IDs::pianoNode);
		theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);
	}
    
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


int SvkPreset::getModeSlotsSize()
{
    return modeSlots.size();
}

ValueTree SvkPreset::getModeInSlot(int slotNum)
{
	return modeSlots[slotNum];
}

ValueTree SvkPreset::getModeBySelector(int modeNumIn)
{
    return modeSlots[modeSelectorSlotNumbers[modeNumIn]];
}

ValueTree SvkPreset::getMode1()
{
	return modeSlots[modeSelectorSlotNumbers[0]];
}

ValueTree SvkPreset::getMode2()
{
	return modeSlots[modeSelectorSlotNumbers[1]];
}

int SvkPreset::getMode1SlotNumber()
{
	return modeSelectorSlotNumbers[0];;
}
int SvkPreset::getMode2SlotNumber()
{
	return modeSelectorSlotNumbers[1];;
}

int SvkPreset::getSlotNumberBySelector(int modeNumIn)
{
    return modeSelectorSlotNumbers[modeNumIn];
}

void SvkPreset::setModeSelectorSlotNum(int modeNumIn, int slotNumIn)
{
    if (slotNumIn >= 0)
    {
		int slotNumCommit = jlimit(0, modeSlots.size(), slotNumIn);
        modeSelectorSlotNumbers.set(modeNumIn, slotNumCommit);

		// add support for more mode slots
		if (modeNumIn == 0)
			thePropertiesNode.setProperty(IDs::mode1SlotNumber, slotNumCommit, nullptr);
		else if (modeNumIn == 1)
			thePropertiesNode.setProperty(IDs::mode2SlotNumber, slotNumCommit, nullptr);
    }
}

void SvkPreset::setMode1SlotNumber(int slotNumIn)
{
    setModeSelectorSlotNum(0, slotNumIn);
}

void SvkPreset::setMode2SlotNumber(int slotNumIn)
{
    setModeSelectorSlotNum(1, slotNumIn);
}

int SvkPreset::setModeSlot(ValueTree modeNodeIn, int slotNumber)
{
	if (Mode::isValidMode(modeNodeIn))
	{
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
