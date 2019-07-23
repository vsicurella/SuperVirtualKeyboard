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
	ValueTree theModeSlotNumbers;
	ValueTree theKeyboardNode;
    ValueTree theMidiSettingsNode;

	SvkPreset();
	SvkPreset(const ValueTree presetNodeIn);
	SvkPreset(const SvkPreset& presetToCopy);
	~SvkPreset();

	bool restoreFromNode(ValueTree presetNodeIn, bool createCopy=false);

    int getModeSlotsSize();

	ValueTree getModeInSlot(int slotNum);
	ValueTree getModeBySelector(int modeNumIn);
    ValueTree getMode1();
    ValueTree getMode2();
    
	int getSlotNumberBySelector(int modeNumIn);
    int getMode1SlotNumber();
    int getMode2SlotNumber();

	void setModeSelectorSlotNum(int modeNumIn, int slotNumIn);
    void setMode1SlotNumber(int slotNumIn);
    void setMode2SlotNumber(int slotNumIn);
    
	int setModeSlot(ValueTree modeNodeIn, int slotNumber = 0);
	int addMode(ValueTree modeNodeIn);
	bool addModes(Array<ValueTree> modeSlotsIn);
       
	String toString();	

	static bool isValidPresetNode(ValueTree presetNodeIn);
	static bool isValidPresetNode(ValueTree presetNodeIn, Array<ValueTree>& modesContained);

private:

	Array<ValueTree> modeSlots;
	Array<int> modeSelectorSlotNumbers;
};
