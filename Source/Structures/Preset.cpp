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
	theModeSelectors = parentNode.getOrCreateChildWithName(IDs::modeSelectorsNode, nullptr);
	theKeyboardNode = parentNode.getOrCreateChildWithName(IDs::pianoNode, nullptr);
    theMidiSettingsNode = parentNode.getOrCreateChildWithName(IDs::midiSettingsNode, nullptr);
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

ValueTree SvkPreset::getPresetNode(bool sortModeSlots)
{
	if (sortModeSlots)
	{
		HashMap<int, ValueTree> slotMap;
		for (auto mode : theModeSlots)
			slotMap.set(mode[IDs::modeSlotNumber], mode);

		theModeSlots.removeAllChildren(nullptr);

		for (auto slotNum : slotNumbersInUse)
			theModeSlots.appendChild(slotMap[slotNum], nullptr);
	}

	return parentNode;
}

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
		theModeSelectors = parentNode.getOrCreateChildWithName(IDs::modeSelectorsNode, nullptr);
		theKeyboardNode = parentNode.getOrCreateChildWithName(IDs::pianoNode, nullptr);
		theMidiSettingsNode = parentNode.getOrCreateChildWithName(IDs::midiSettingsNode, nullptr);

		slotNumbersInUse.clear();
		for (int i = 0; i < theModeSlots.getNumChildren(); i++)
		{
			ValueTree modeNode = theModeSlots.getChild(i);

			int slotNum = jlimit(0, MAX_MODE_SLOTS_INDEX, (int)modeNode[IDs::modeSlotNumber]);
			slotNumbersInUse.addUsingDefaultSort(slotNum);
		}

		// Ensure this has at least 2 mode selectors
		while (theModeSelectors.getNumChildren() < 2)
		{
			ValueTree selectorNode(IDs::modeSelectorsNode);
			selectorNode.setProperty(IDs::modeSlotNumber, theModeSelectors.getNumChildren(), nullptr);
			selectorNode.setProperty(IDs::modeSelectorRootNote, 60, nullptr); // TODO: default midi root note
			theModeSelectors.appendChild(selectorNode, nullptr);
		}
	}
    
    return true;
}

int SvkPreset::getModeSlotsSize()
{
	return theModeSlots.getNumChildren();
}

int SvkPreset::getSlotNumberBySelector(int selectorNumIn)
{
	return theModeSelectors.getChild(selectorNumIn)[IDs::modeSlotNumber];
}

int SvkPreset::getMode1SlotNumber()
{
	return getSlotNumberBySelector(0);
}
int SvkPreset::getMode2SlotNumber()
{
	return getSlotNumberBySelector(1);
}

int SvkPreset::getSlotNumberIndex(int slotNumIn)
{
	for (int i = 0; i < theModeSlots.getNumChildren(); i++)
	{
		if (slotNumIn == (int)(theModeSlots.getChild(i)[IDs::modeSlotNumber]))
			return i;
	}

	return -1;
}

int SvkPreset::getNextFreeSlotNumber()
{
	for (int i = 1; i < slotNumbersInUse.size(); i++)
	{
		if (slotNumbersInUse[i] - slotNumbersInUse[i - 1] > 1)
			return slotNumbersInUse[i - 1] + 1;
	}

	return -1;
}

ValueTree SvkPreset::getModeInSlot(int slotNum)
{
	return theModeSlots.getChild(getSlotNumberIndex(slotNum));
}

ValueTree SvkPreset::getModeBySelector(int selectorNumIn)
{
	return theModeSlots.getChild(getSlotNumberIndex(getSlotNumberBySelector(selectorNumIn)));
}

ValueTree SvkPreset::getMode1()
{
	return getModeBySelector(0);
}

ValueTree SvkPreset::getMode2()
{
	return getModeBySelector(1);
}

int SvkPreset::setModeSelectorSlotNum(int selectorNumIn, int slotNumIn)
{
	slotNumIn = jlimit(0, MAX_MODE_SLOTS_INDEX, slotNumIn);

	theModeSelectors.getChild(selectorNumIn).setProperty(IDs::modeSlotNumber, slotNumIn, nullptr);

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

int SvkPreset::setModeSlot(ValueTree modeNodeIn, int slotNumberIn)
{
	if (Mode::isValidMode(modeNodeIn))
	{
		slotNumberIn = jlimit(0, MAX_MODE_SLOTS_INDEX, slotNumberIn);

		ValueTree slotNode;

		int slotIndex = getSlotNumberIndex(slotNumberIn);
		if (slotIndex >= 0)
		{
			slotNode = theModeSlots.getChild(slotIndex);
		}
		else
		{
			slotIndex = theModeSlots.getNumChildren() - 1;
			slotNode = ValueTree(IDs::modeSlotsNode);
			theModeSlots.appendChild(slotNode, nullptr);
		}

		slotNode.setProperty(IDs::modeSlotNumber, slotNumberIn, nullptr);
		slotNode.appendChild(modeNodeIn.createCopy(), nullptr);

		slotNumbersInUse.addUsingDefaultSort(slotNumberIn);

		return slotIndex;
	}

	return -1;
}

int SvkPreset::addModeSlot(ValueTree modeNodeIn)
{
	int slotIndex = getNextFreeSlotNumber();
	return setModeSlot(modeNodeIn, slotIndex);
}

void SvkPreset::removeModeSlot(int slotNumberIn)
{
	int slotIndex = getSlotNumberIndex(slotNumberIn);

	if (slotIndex >= 0)
	{
		theModeSlots.removeChild(slotIndex, nullptr);
		slotNumbersInUse.removeFirstMatchingValue(slotNumberIn);
	}
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

bool SvkPreset::extractModesIfValid(ValueTree presetNodeIn, Array<ValueTree>& modesContained)
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