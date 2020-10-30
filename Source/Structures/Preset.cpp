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
    parentNode.setProperty(IDs::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);

    thePropertiesNode = parentNode.getOrCreateChildWithName(IDs::presetProperties, nullptr);
    theModeSlots = parentNode.getOrCreateChildWithName(IDs::modeSlotsNode, nullptr);
    theModeSelectors = parentNode.getOrCreateChildWithName(IDs::modeSelectorsNode, nullptr);
    theCustomMode = parentNode.getOrCreateChildWithName(IDs::modeCustomNode, nullptr);
    theKeyboardNode = parentNode.getOrCreateChildWithName(IDs::pianoNode, nullptr);
    theMidiSettingsNode = parentNode.getOrCreateChildWithName(IDs::midiSettingsNode, nullptr);
    theMappingsNode = parentNode.getOrCreateChildWithName(IDs::midiMapNode, nullptr);
}

SvkPreset::SvkPreset(const ValueTree presetNodeIn)
    : SvkPreset()
{
    restoreFromNode(presetNodeIn, true);
}

SvkPreset::SvkPreset(const SvkPreset& presetToCopy)
:     SvkPreset(presetToCopy.parentNode.createCopy())
{
}

SvkPreset::~SvkPreset() {}

ValueTree SvkPreset::getPresetNode(bool sortModeSlots)
{
    if (sortModeSlots)
    {
        // TODO: Maybe rethink the context of this...
        // If sorted, PresetManager's ModeSlots will need to be resynched

        HashMap<int, ValueTree> slotMap;
        for (auto mode : theModeSlots)
            slotMap.set(mode[IDs::modeSlotNumber], mode);

        theModeSlots.removeAllChildren(nullptr);

        for (auto slotNum : slotNumbersInUse)
            theModeSlots.appendChild(slotMap[slotNum], nullptr);
    }

    return parentNode;
}

ValueTree SvkPreset::getKeyboardNode()
{
    return theKeyboardNode;
}

ValueTree SvkPreset::getMidiSettingsNode()
{
    return theMidiSettingsNode;
}

ValueTree SvkPreset::getMappingsNode()
{
    return theMappingsNode;
}

bool SvkPreset::restoreFromNode(ValueTree presetNodeIn, bool createCopy)
{
    if (isValidPresetNode(presetNodeIn))
    {
        if (createCopy)
            presetNodeIn = presetNodeIn.createCopy();

        parentNode = presetNodeIn;
        parentNode.setProperty(IDs::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);
        
        thePropertiesNode = parentNode.getOrCreateChildWithName(IDs::presetProperties, nullptr);
        theModeSlots = parentNode.getOrCreateChildWithName(IDs::modeSlotsNode, nullptr);
        theModeSelectors = parentNode.getOrCreateChildWithName(IDs::modeSelectorsNode, nullptr);
        theCustomMode = parentNode.getOrCreateChildWithName(IDs::modeCustomNode, nullptr);
        theKeyboardNode = parentNode.getOrCreateChildWithName(IDs::pianoNode, nullptr);
        theMidiSettingsNode = parentNode.getOrCreateChildWithName(IDs::midiSettingsNode, nullptr);
        theMappingsNode = parentNode.getOrCreateChildWithName(IDs::midiMapNode, nullptr);

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

        // Make sure mode selectors have a midi root note
        for (auto selector : theModeSelectors)
        {
            if (!selector.hasProperty(IDs::modeSelectorRootNote))
                selector.setProperty(IDs::modeSelectorRootNote, 60, nullptr);
        }

        return true;
    }
    
    return false;
}

Array<int> SvkPreset::getSlotNumbersInUse() const
{
    return slotNumbersInUse;
}

int SvkPreset::getModeSelectorViewed() const
{
    return thePropertiesNode[IDs::modeSelectorViewed];
}

bool SvkPreset::isSlotNumberInUse(int modeSlotNumberIn) const
{
    return slotNumbersInUse.contains(modeSlotNumberIn);
}

int SvkPreset::getModeSlotsSize() const
{
    return theModeSlots.getNumChildren();
}

int SvkPreset::getSlotNumberBySelector(int selectorNumIn) const
{
    return theModeSelectors.getChild(selectorNumIn)[IDs::modeSlotNumber];
}

int SvkPreset::getMode1SlotNumber() const
{
    return getSlotNumberBySelector(0);
}
int SvkPreset::getMode2SlotNumber() const
{
    return getSlotNumberBySelector(1);
}

int SvkPreset::getModeViewedSlotNumber() const
{
    return getSlotNumberBySelector(getModeSelectorViewed());
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

    if (slotNumbersInUse.size() < MAX_MODE_SLOTS_INDEX + 1)
        return slotNumbersInUse.size();

    return -1;
}

int SvkPreset::getModeSelectorRootNote(int modeSelectorNum) const
{
    return theModeSelectors.getChild(modeSelectorNum)[IDs::modeSelectorRootNote];
}

ValueTree SvkPreset::getModeSlots()
{
    return theModeSlots;
}

ValueTree SvkPreset::getModeInSlot(int slotNum)
{
    return theModeSlots.getChild(getSlotNumberIndex(slotNum)).getChild(0);
}

ValueTree SvkPreset::getModeBySelector(int selectorNumIn)
{
    return theModeSlots.getChild(getSlotNumberIndex(getSlotNumberBySelector(selectorNumIn))).getChild(0);
}

ValueTree SvkPreset::getMode1()
{
    return getModeBySelector(0);
}

ValueTree SvkPreset::getMode2()
{
    return getModeBySelector(1);
}

ValueTree SvkPreset::getModeViewed()
{
    return getModeBySelector(thePropertiesNode[IDs::modeSelectorViewed]);
}

ValueTree SvkPreset::getCustomMode()
{
    return theCustomMode.getChild(0);
}

int SvkPreset::setModeSelectorSlotNum(int selectorNumIn, int slotNumIn)
{
    if (selectorNumIn >= 0 && slotNumIn >= 0)
    {
        ValueTree modeSelectorNode = ValueTree(IDs::modeSelectorsNode);

        if (selectorNumIn < theModeSelectors.getNumChildren())
            modeSelectorNode = theModeSelectors.getChild(selectorNumIn);
    
        else
            theModeSelectors.appendChild(modeSelectorNode, nullptr);
        
        modeSelectorNode.setProperty(IDs::modeSlotNumber, slotNumIn, nullptr);
        return slotNumIn;
    }

    return -1;
}

void SvkPreset::setMode1SlotNumber(int slotNumIn)
{
    setModeSelectorSlotNum(0, slotNumIn);
}

void SvkPreset::setMode2SlotNumber(int slotNumIn)
{
    setModeSelectorSlotNum(1, slotNumIn);
}

void SvkPreset::setModeSelectorRootNote(int modeSelectorNumIn, int rootNoteIn)
{
    ValueTree node = theModeSelectors.getChild(modeSelectorNumIn);

    if (node.isValid() && rootNoteIn >= 0 && rootNoteIn < 128)
        node.setProperty(IDs::modeSelectorRootNote, rootNoteIn, nullptr);
}

int SvkPreset::setModeSlot(ValueTree modeNodeIn, int slotNumberIn)
{
    if (Mode::isValidMode(modeNodeIn) && slotNumberIn >= 0 && slotNumberIn <= MAX_MODE_SLOTS_INDEX)
    {
        ValueTree slotNode = ValueTree(IDs::modeSlotsNode);
        int slotIndex = getSlotNumberIndex(slotNumberIn);
        if (slotIndex >= 0)
        {
            slotNode = theModeSlots.getChild(slotIndex);
            slotNode.removeAllChildren(nullptr);
        }
        else
        {
            slotIndex = theModeSlots.getNumChildren();
            theModeSlots.appendChild(slotNode, nullptr);
        }

        slotNode.setProperty(IDs::modeSlotNumber, slotNumberIn, nullptr);
        slotNode.appendChild(modeNodeIn.createCopy(), nullptr);

        if (slotNumbersInUse.indexOf(slotNumberIn) < 0)
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

ValueTree SvkPreset::setCustomMode(ValueTree customModeNodeIn, bool createCopy)
{
    if (Mode::isValidMode(customModeNodeIn))
    {
        ValueTree nodeToAdd = customModeNodeIn;

        if (createCopy)
            nodeToAdd = customModeNodeIn.createCopy();

        theCustomMode.removeAllChildren(nullptr);
        theCustomMode.appendChild(nodeToAdd, nullptr);

        return nodeToAdd;
    }

    return ValueTree();
}

int SvkPreset::removeModeSlot(int slotNumberIn)
{
    int slotIndex = getSlotNumberIndex(slotNumberIn);

    if (slotIndex >= 0)
    {
        theModeSlots.removeChild(slotIndex, nullptr);
        slotNumbersInUse.removeFirstMatchingValue(slotNumberIn);
    }

    return slotIndex;
}

void SvkPreset::resetModeSlots()
{
    theModeSelectors.removeAllChildren(nullptr);
    slotNumbersInUse.clear();

    addModeSlot(STD_TUNING_MODE_NODE);
    addModeSlot(STD_TUNING_MODE_NODE);

    setMode1SlotNumber(0);
    setMode2SlotNumber(1);
}

bool SvkPreset::isValidPresetNode(ValueTree presetNodeIn)
{
    bool isValid = presetNodeIn.hasType(IDs::presetNode);

    if (isValid)
    {
        isValid *= presetNodeIn.getChildWithName(IDs::modeSlotsNode).getNumChildren() > 0;
            
        // TODO: more checks?
    }

    return isValid;
}

SvkPreset SvkPreset::getDefaultPreset()
{
    SvkPreset defaultPreset;

    ValueTree propertiesNode = defaultPreset.parentNode.getOrCreateChildWithName(IDs::presetProperties, nullptr);
    propertiesNode.setProperty(IDs::modeSelectorViewed, 1, nullptr);

    defaultPreset.resetModeSlots();

    ValueTree pianoNode = defaultPreset.parentNode.getOrCreateChildWithName(IDs::pianoNode, nullptr);
    pianoNode.setProperty(IDs::keyboardKeysStyle, 1, nullptr);
    pianoNode.setProperty(IDs::pianoKeysShowNoteNumbers, 0, nullptr);
    pianoNode.setProperty(IDs::keyboardHighlightStyle, 1, nullptr);
    
    ValueTree mappingMode = defaultPreset.parentNode.getOrCreateChildWithName(IDs::midiMapNode, nullptr);
    mappingMode.setProperty(IDs::mappingMode, 1, nullptr);
    mappingMode.setProperty(IDs::autoMappingStyle, 1, nullptr);

    ValueTree customMode = defaultPreset.parentNode.getOrCreateChildWithName(IDs::modeCustomNode, nullptr);
    customMode.appendChild(Mode::createNode("1"), nullptr);

    return defaultPreset;
}

String SvkPreset::toString()
{
    return parentNode.toXmlString();
}
