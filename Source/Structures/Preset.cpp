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
	theModeNode = ValueTree(IDs::modeSlotsNode);
	theKeyboardNode = ValueTree(IDs::pianoNode);
    theMidiSettingsNode = ValueTree(IDs::midiSettingsNode);
	
	theModeNode.setProperty(IDs::mode1SlotNumber, mode1SlotNumber, nullptr);
	theModeNode.setProperty(IDs::mode2SlotNumber, mode2SlotNumber, nullptr);

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
	if (!presetNodeIn.isValid())
		return;
	
	theModeNode = ValueTree(IDs::modeSlotsNode);
	Array<ValueTree> modeSlots = extractNodes(presetNodeIn, IDs::modePresetNode);
	addModes(modeSlots);

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

	parentNode.addChild(theModeNode, -1, nullptr);
	parentNode.addChild(theKeyboardNode, -1, nullptr);
	parentNode.addChild(theMidiSettingsNode, -1, nullptr);
}

void SvkPreset::commitPreset()
{
	theModeNode.removeAllChildren(nullptr);

	for (int i = 0; i < modeSlots.size(); i++)
	{
		theModeNode.addChild(modeSlots[i].createCopy(), i, nullptr);
	}
}

ValueTree SvkPreset::getMode(int slotNum)
{
	return modeSlots[slotNum];
}

int SvkPreset::getMode1SlotNumber()
{
	return mode1SlotNumber;
}
int SvkPreset::getMode2SlotNumber()
{
	return mode2SlotNumber;
}

void SvkPreset::setMode1SlotNumber(int slotNumIn)
{
	if (slotNumIn >= 0)
	{
		mode1SlotNumber = slotNumIn % modeSlots.size();
		theModeNode.setProperty(IDs::mode1SlotNumber, mode1SlotNumber, nullptr);
	}
}

void SvkPreset::setMode2SlotNumber(int slotNumIn)
{
	if (slotNumIn >= 0)
	{
		mode2SlotNumber = slotNumIn % modeSlots.size();
		theModeNode.setProperty(IDs::mode2SlotNumber, mode2SlotNumber, nullptr);
	}
}

int SvkPreset::setModeSlot(ValueTree modeNodeIn, int slotNumber = 0)
{
	if (Mode::isValidMode(modeNodeIn))
	{
		if (modeSlots.size() < slotNumber + 1)
			modeSlots.resize(slotNumber + 1);

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

String SvkPreset::toString()
{
	return parentNode.toXmlString();
}
