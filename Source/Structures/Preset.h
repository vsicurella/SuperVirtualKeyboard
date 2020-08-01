/*
  ==============================================================================

    FractionalGrid.h
    Created: 24 Mar 2019 1:58:51pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginIDs.h"
#include "../CommonFunctions.h"
#include "Mode.h"

struct SvkPreset
{
	ValueTree parentNode;
    ValueTree thePropertiesNode;
	ValueTree theModeSlots;
	ValueTree theKeyboardNode;
    ValueTree theMidiSettingsNode;

	SvkPreset();
	SvkPreset(const ValueTree presetNodeIn);
	SvkPreset(const SvkPreset& presetToCopy);
	~SvkPreset();

	bool restoreFromNode(ValueTree presetNodeIn, bool createCopy = false); // TODO: Revise default parameter

    int getModeSlotsSize();

	ValueTree getModeInSlot(int slotNum);
	ValueTree getModeBySelector(int selectorNumIn);
    ValueTree getMode1();
    ValueTree getMode2();
    
	int getSlotNumberBySelector(int modeNumIn);
    int getMode1SlotNumber();
    int getMode2SlotNumber();

	int setModeSelectorSlotNum(int selectorNumIn, int slotNumIn);
    void setMode1SlotNumber(int slotNumIn);
    void setMode2SlotNumber(int slotNumIn);
    
	int setModeSlot(ValueTree modeNodeIn, int slotNumber);
	int addMode(ValueTree modeNodeIn);
       
	String toString();	

	static bool isValidPresetNode(ValueTree presetNodeIn);
	static bool isValidPresetNode(ValueTree presetNodeIn, Array<ValueTree>& modesContained);

private:

	int getSlotNumberIndex(int slotNumIn);
	
private:

	Array<ValueTree> modeSlots;
	Array<int> modeSelectorSlotNumbers;

};
