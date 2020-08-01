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
    thePropertiesNode = parentNode.getOrCreateChildWithName(IDs::presetProperties, nullptr);
	theModeSlots = parentNode.getOrCreateChildWithName(IDs::modeSlotsNode, nullptr);
	theKeyboardNode = parentNode.getOrCreateChildWithName(IDs::pianoNode, nullptr);
    theMidiSettingsNode = parentNode.getOrCreateChildWithName(IDs::midiSettingsNode, nullptr);

	modeSelectorSlotNumbers.set(0, 0);
	modeSelectorSlotNumbers.set(1, 1);
}

SvkPreset::SvkPreset(const ValueTree presetNodeIn)
	: SvkPreset()
{
	restoreFromNode(presetNodeIn, true);
}

SvkPreset::SvkPreset(const SvkPreset& presetToCopy)
:     SvkPreset(presetToCopy.parentNode)
{
}

SvkPreset::~SvkPreset() {}

bool SvkPreset::restoreFromNode(ValueTree presetNodeIn, bool createCopy)
{
	if (!presetNodeIn.isValid())
		return false;

	if (isValidPresetNode(presetNodeIn))
	{
		if (createCopy)
		{
			DBG("copying this preset: " + presetNodeIn.toXmlString());
			parentNode.copyPropertiesAndChildrenFrom(presetNodeIn, nullptr);
		}
		else
		{
			DBG("copying this preset: " + presetNodeIn.toXmlString());
			parentNode = presetNodeIn;
		}

		thePropertiesNode = parentNode.getOrCreateChildWithName(IDs::presetProperties, nullptr);
		theModeSlots = parentNode.getOrCreateChildWithName(IDs::modeSlotsNode, nullptr);
		theKeyboardNode = parentNode.getOrCreateChildWithName(IDs::pianoNode, nullptr);
		theMidiSettingsNode = parentNode.getOrCreateChildWithName(IDs::midiSettingsNode, nullptr);

		modeSlots.clear();
		modeSelectorSlotNumbers.clear();

		for (int i = 0; i < theModeSlots.getNumChildren(); i++)
		{
			ValueTree modeNode = theModeSlots.getChild(i);
			modeSlots.add(modeNode);

			int slotNum = jlimit(0, (int)MAX_MODE_SLOTS_INDEX, (int)modeNode[IDs::modeSlotNumber]);
			modeSelectorSlotNumbers.add(slotNum);
		}

		DBGArray(modeSelectorSlotNumbers, "Mode Selector Slot Numbers");
	}
    
    return true;
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
	return modeSelectorSlotNumbers[0];
}
int SvkPreset::getMode2SlotNumber()
{
	return modeSelectorSlotNumbers[1];
}

int SvkPreset::getSlotNumberBySelector(int modeNumIn)
{
    return modeSelectorSlotNumbers[modeNumIn];
}

int SvkPreset::getSlotNumberIndex(int slotNumIn)
{
	for (int i = 0; i < modeSlots.size(); i++)
	{
		if (slotNumIn == (int)(modeSlots.getReference(i)[IDs::modeSlotNumber]))
		{
			return i;
		}
	}

	return -1;
}

int SvkPreset::setModeSelectorSlotNum(int selectorNumIn, int slotNumIn)
{
	slotNumIn = jlimit(0, (int)MAX_MODE_SLOTS_INDEX, slotNumIn);
	
	int slotIndex = getSlotNumberIndex(slotNumIn);

	if (slotIndex >= 0)
	{
		modeSelectorSlotNumbers.set(selectorNumIn, slotIndex);

		Identifier modeSelectorID = selectorNumIn == 0
			? IDs::mode1SlotNum
			: IDs::mode2SlotNum;

		thePropertiesNode.setProperty(modeSelectorID, slotNumIn, nullptr);
	}
	
    return slotNumIn;
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
		ValueTree modeNodeAdded = modeNodeIn.createCopy();
		modeNodeAdded.setProperty(IDs::modeSlotNumber, slotNumber, nullptr);
		
		int slotIndex = getSlotNumberIndex(slotNumber);
		if (slotIndex >= 0)
		{
			modeSlots.set(slotIndex, modeNodeAdded);
			theModeSlots.getChild(slotNumber).copyPropertiesAndChildrenFrom(modeNodeAdded, nullptr);
		}
		else
		{
			modeSlots.add(modeNodeAdded);
			theModeSlots.appendChild(modeNodeAdded, nullptr);
			slotIndex = theModeSlots.getNumChildren() - 1;
		}

		return slotIndex;
	}

	return -1;
}

int SvkPreset::addMode(ValueTree modeNodeIn)
{
	if (Mode::isValidMode(modeNodeIn))
	{
		ValueTree modeNodeAdded = modeNodeIn.createCopy();
		modeSlots.add(modeNodeIn);
		theModeSlots.addChild(modeNodeAdded, -1, nullptr);

		return theModeSlots.getNumChildren() - 1;
	}

	return -1;
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
