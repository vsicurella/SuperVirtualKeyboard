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
	theModeSlots = ValueTree(IDs::modeSlotsNode);
	theModeSlotNumbers = ValueTree(IDs::modeSlotsNumberNode);
	theKeyboardNode = ValueTree(IDs::pianoNode);
    theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);

	setModeSelectorSlotNum(0, 0);
	setModeSelectorSlotNum(1, 1);

    parentNode.addChild(thePropertiesNode, -1, nullptr);
	parentNode.addChild(theModeSlots, -1, nullptr);
	parentNode.addChild(theModeSlotNumbers, -1, nullptr);
	parentNode.addChild(theKeyboardNode, -1, nullptr);
    parentNode.addChild(theMidiSettingsNode, -1, nullptr);
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
			parentNode.copyPropertiesAndChildrenFrom(presetNodeIn, nullptr);
		}
		else
		{
			parentNode = presetNodeIn;
		}

		thePropertiesNode = parentNode.getChildWithName(IDs::presetProperties);
		theModeSlots = parentNode.getChildWithName(IDs::modeSlotsNode);
		theModeSlotNumbers = parentNode.getChildWithName(IDs::modeSlotsNumberNode);
		theKeyboardNode = parentNode.getChildWithName(IDs::pianoNode);
		theMidiSettingsNode = parentNode.getChildWithName(IDs::midiSettingsNode);
	}
	else
	{
		return false;
	}

	if (!thePropertiesNode.isValid())
	{
		thePropertiesNode = ValueTree(IDs::presetProperties);
	}

	if (!theKeyboardNode.isValid())
	{
		theKeyboardNode = ValueTree(IDs::pianoNode);
	}

	if (!theMidiSettingsNode.isValid())
	{
		theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);
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
		int numberOfModeSources = modeSelectorSlotNumbers.size();
		int slotNumCommit = jlimit(0, modeSlots.size(), slotNumIn);
		ValueTree slotNumNode;

		if (slotNumIn >= numberOfModeSources)
		{
			int filler = numberOfModeSources;

			while (filler <= slotNumIn)
			{
				modeSelectorSlotNumbers.set(filler, -1);

				slotNumNode = ValueTree(IDs::modeSlotsNumberNode);
				slotNumNode.setProperty(IDs::modeSlotNumber, -1, nullptr);
				theModeSlotNumbers.addChild(slotNumNode, filler, nullptr);

				filler++;
			}
		}

        modeSelectorSlotNumbers.set(modeNumIn, slotNumCommit);

		slotNumNode = ValueTree(IDs::modeSlotsNumberNode);
		slotNumNode.setProperty(IDs::modeSlotNumber, slotNumIn, nullptr);
		theModeSlotNumbers.addChild(slotNumNode, slotNumIn, nullptr);
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
		ValueTree modeNodeAdded = modeNodeIn.createCopy();
		modeSlots.set(slotNumber, modeNodeAdded);

		ValueTree childNode = theModeSlots.getChild(slotNumber);
		
		if (childNode.isValid())
		{
			theModeSlots.removeChild(slotNumber, nullptr);
		}

		theModeSlots.addChild(modeNodeAdded, slotNumber, nullptr);

		return slotNumber;
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

		return modeSlots.size() - 1;
	}

	return -1;
}

bool SvkPreset::addModes(Array<ValueTree> modeSlotsIn)
{
	int numAdded = 0;

	for (auto mode : modeSlotsIn)
	{
		addMode(mode);
		numAdded++;
	}

	return numAdded;
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
