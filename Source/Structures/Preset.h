/*
  ==============================================================================

    FractionalGrid.h
    Created: 24 Mar 2019 1:58:51pm
    Author:  Vincenzo

	Wrapper class for setting preset data in a ValueTree.

	The general idea is you can load up to 128 modes into a preset, and this will allow you to
	maniuplate the mode slots and keeps track of which mode slots are loaded,
	with mode selectors 0 and 1 being the main ones (0 = input, 1 = output).

	A custom mode has it's own slot and there can only be one at a time. To take advantage of 
	mode slot features, the custom mode must be saved to file and loaded into a slot.

	Slot numbers and slot indicies do not need to match, and the slot indicies are only
	used internally

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginIDs.h"
#include "../CommonFunctions.h"
#include "Mode.h"

class SvkPreset
{
	ValueTree parentNode;
    ValueTree thePropertiesNode;
	ValueTree theModeSlots;
	ValueTree theModeSelectors;
	ValueTree theKeyboardNode;
    ValueTree theMidiSettingsNode;

public:

	SvkPreset();
	SvkPreset(const ValueTree presetNodeIn);
	SvkPreset(const SvkPreset& presetToCopy);
	~SvkPreset();

	/*
		Returns the parent node of the preset
		Will sort mode slots by slot number if called with true.
	*/
	ValueTree getPresetNode(bool sortModeSlots = false);

	/*
		Recreate a preset from a ValueTree
	*/
	bool restoreFromNode(ValueTree presetNodeIn, bool createCopy = false); // TODO: Revise default parameter

	/*
		Returns the amount of modes loaded into slots
	*/
    int getModeSlotsSize();

	/*
		Returns the slot number of the mode loaded by the given selector
	*/
	int getSlotNumberBySelector(int selectorNumIn);

	/*
		Returns the slot number of the mode loaded by selector 0 (input keyboard)
	*/
	int getMode1SlotNumber();

	/*
		Returns the slot number of the mode loaded by selector 1 (output keyboard)
	*/
	int getMode2SlotNumber();

	/*
		Returns the mode with the given slot number (which may be different from the index it's in)
	*/
	ValueTree getModeInSlot(int slotNum);

	/*
		Returns the mode loaded in the given mode selector
	*/
	ValueTree getModeBySelector(int selectorNumIn);

	/*
		Returns the mode loaded in selector 0 (input keyboard)
	*/
    ValueTree getMode1();

	/*
		Returns the mode loaded in selector 1 (output keyboard)
	*/
    ValueTree getMode2();

	/*
		Sets the slot number used by the given selector, and returns the slot index
		If slot number doesn't exist, nothing will happen and this will return -1
	*/
	int setModeSelectorSlotNum(int selectorNumIn, int slotNumIn);
    void setMode1SlotNumber(int slotNumIn);
    void setMode2SlotNumber(int slotNumIn);
    
	/*
		Loads the given mode into the given slot number, and returns the slot index
	*/
	int setModeSlot(ValueTree modeNodeIn, int slotNumber);

	/*
		Puts the given mode in the smallest available free slot and returns the slot number
	*/
	int addModeSlot(ValueTree modeNodeIn);

	/*
		Empties the given mode slot
	*/
	void removeModeSlot(int slotNumberIn);
    
	/*
		Returns a readable version of the parent node
	*/
	String toString();

	static bool isValidPresetNode(ValueTree presetNodeIn);
	static bool extractModesIfValid(ValueTree presetNodeIn, Array<ValueTree>& modesContained);

private:

	/*
		Finds the slot index of a given slot number, or returns -1 if not found
	*/
	int getSlotNumberIndex(int slotNumIn);

	int getNextFreeSlotNumber();
	
private:

	Array<int> slotNumbersInUse;
};
