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
    parentNode = createNewParentNode();
    pullChildrenFromParentNode(parentNode);
}

ValueTree SvkPreset::createNewParentNode()
{
    ValueTree node = ValueTree(SvkProperty::presetNode);
    node.setProperty(SvkProperty::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);
    return node;
}

void SvkPreset::pullChildrenFromParentNode(ValueTree node)
{
    thePropertiesNode   = node.getOrCreateChildWithName(SvkProperty::presetProperties, nullptr);
    theModeSlots        = node.getOrCreateChildWithName(SvkProperty::modeSlotsNode, nullptr);
    theModeSelectors    = node.getOrCreateChildWithName(SvkProperty::modeSelectorsNode, nullptr);
    theCustomMode       = node.getOrCreateChildWithName(SvkProperty::modeCustomNode, nullptr);
    theKeyboardNode     = node.getOrCreateChildWithName(SvkProperty::pianoNode, nullptr);
    theMidiSettingsNode = node.getOrCreateChildWithName(SvkProperty::midiSettingsNode, nullptr);
    theMappingsNode     = node.getOrCreateChildWithName(SvkProperty::midiMapNode, nullptr);

#if SVK_VERBOSE
    DBG("Preset loaded:");
    DBG(thePropertiesNode.toXmlString());
    DBG(theModeSlots.toXmlString());
    DBG(theModeSelectors.toXmlString());
    DBG(theCustomMode.toXmlString());
    DBG(theKeyboardNode.toXmlString());
    DBG(theMidiSettingsNode.toXmlString());
    DBG(theMappingsNode.toXmlString());
#endif
}

SvkPreset::SvkPreset(const ValueTree presetNodeIn)
    : SvkPreset()
{
    restoreFromNode(presetNodeIn, true);
}

SvkPreset::SvkPreset(const SvkPreset& presetToCopy)
{
    auto presetNode = presetToCopy.parentNode;
    restoreFromNode(presetNode, true);
}

SvkPreset::~SvkPreset() {}

bool SvkPreset::operator==(const SvkPreset& preset) const
{
    return parentNode.isEquivalentTo(preset.parentNode);
}

float SvkPreset::getPresetVersion() const
{
    return (float)parentNode[SvkProperty::pluginPresetVersion];
}

ValueTree SvkPreset::getPresetNode(bool sortModeSlots)
{
    if (sortModeSlots)
    {
        // TODO: Maybe rethink the context of this...
        // If sorted, PresetManager's ModeSlots will need to be resynched

        HashMap<int, ValueTree> slotMap;
        for (auto mode : theModeSlots)
            slotMap.set(mode[SvkProperty::modeSlotNumber], mode);

        theModeSlots.removeAllChildren(nullptr);

        for (auto slotNum : slotNumbersInUse)
            theModeSlots.appendChild(slotMap[slotNum], nullptr);
    }

    return parentNode;
}

//ValueTree SvkPreset::getKeyboardNode()
//{
//    return theKeyboardNode;
//}
//
//ValueTree SvkPreset::getMidiSettingsNode()
//{
//    return theMidiSettingsNode;
//}
//
//ValueTree SvkPreset::getMappingsNode()
//{
//    return theMappingsNode;
//}

VirtualKeyboard::Orientation SvkPreset::getKeyboardOrientation() const
{
    return (VirtualKeyboard::Orientation)(int)this->theKeyboardNode[SvkProperty::keyboardOrientation];
}

VirtualKeyboard::HighlightStyle SvkPreset::getHighlightStyle() const
{
    return (VirtualKeyboard::HighlightStyle)(int)this->theKeyboardNode[SvkProperty::keyboardHighlightStyle];
}

VirtualKeyboard::ScrollingStyle SvkPreset::getScrollingStyle() const
{
    return (VirtualKeyboard::ScrollingStyle)(int)this->theKeyboardNode[SvkProperty::keyboardScrollingStyle];
}

VirtualKeyboard::KeyPlacementType SvkPreset::getKeyPlacementType() const
{
    return (VirtualKeyboard::KeyPlacementType)(int)this->theKeyboardNode[SvkProperty::keyboardKeysStyle];
}

VirtualKeyboard::VelocityStyle SvkPreset::getVelocityStyle() const
{
    return (VirtualKeyboard::VelocityStyle)(int)this->theKeyboardNode[SvkProperty::pianoVelocityBehavior];
}

MappingMode SvkPreset::getMappingMode() const
{
    return (MappingMode)(int)this->theMappingsNode[SvkProperty::mappingMode];
}

MappingStyle SvkPreset::getMappingStyle() const
{
    return (MappingStyle)(int)this->theMappingsNode[SvkProperty::autoMappingStyle];
}

bool SvkPreset::restoreFromNode(ValueTree presetNodeIn, bool sendChangeMessage)
{
    if (isValidPresetNode(presetNodeIn))
    {
        // Update root properties in-place so named child references remain valid.
        // Never reassign parentNode — child refs would be orphaned.
        parentNode.copyPropertiesFrom(presetNodeIn, nullptr);
        parentNode.setProperty(SvkProperty::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);

        // Update each named child section individually in-place.
        auto updateChild = [&](ValueTree& dest, const Identifier& type)
        {
            auto src = presetNodeIn.getChildWithName(type);
            if (src.isValid())
                dest.copyPropertiesAndChildrenFrom(src, nullptr);
            else
                dest.removeAllChildren(nullptr);
        };

        updateChild(thePropertiesNode,   SvkProperty::presetProperties);
        updateChild(theModeSlots,        SvkProperty::modeSlotsNode);
        updateChild(theModeSelectors,    SvkProperty::modeSelectorsNode);
        updateChild(theCustomMode,       SvkProperty::modeCustomNode);
        updateChild(theKeyboardNode,     SvkProperty::pianoNode);
        updateChild(theMidiSettingsNode, SvkProperty::midiSettingsNode);
        updateChild(theMappingsNode,     SvkProperty::midiMapNode);

        slotNumbersInUse.clear();
        for (int i = 0; i < theModeSlots.getNumChildren(); i++)
        {
            ValueTree modeNode = theModeSlots.getChild(i);

            int slotNum = jlimit(0, MAX_MODE_SLOTS_INDEX, (int)modeNode[SvkProperty::modeSlotNumber]);
            slotNumbersInUse.addUsingDefaultSort(slotNum);
        }

        // Ensure this has at least 2 mode selectors
        while (theModeSelectors.getNumChildren() < 2)
        {
            ValueTree selectorNode(SvkProperty::modeSelectorsNode);
            selectorNode.setProperty(SvkProperty::modeSlotNumber, theModeSelectors.getNumChildren(), nullptr);
            selectorNode.setProperty(SvkProperty::modeSelectorRootNote, 60, nullptr);
            theModeSelectors.appendChild(selectorNode, nullptr);
        }

        // Make sure mode selectors have a midi root note
        for (auto selector : theModeSelectors)
        {
            if (!selector.hasProperty(SvkProperty::modeSelectorRootNote))
                selector.setProperty(SvkProperty::modeSelectorRootNote, 60, nullptr);
        }

        if (sendChangeMessage)
            listeners.call(&Listener::presetReloaded, *this);

        return true;
    }

    return false;
}

Array<int> SvkPreset::getSlotNumbersInUse() const
{
    return slotNumbersInUse;
}

int SvkPreset::getNumSlotsInUse() const
{
    return slotNumbersInUse.size();
}

int SvkPreset::getModeSelectorViewed() const
{
    return thePropertiesNode[SvkProperty::modeSelectorViewed];
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
    return theModeSelectors.getChild(selectorNumIn)[SvkProperty::modeSlotNumber];
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

int SvkPreset::getSlotNumberIndex(int slotNumIn) const
{
    for (int i = 0; i < theModeSlots.getNumChildren(); i++)
    {
        if (slotNumIn == (int)(theModeSlots.getChild(i)[SvkProperty::modeSlotNumber]))
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
    return theModeSelectors.getChild(modeSelectorNum)[SvkProperty::modeSelectorRootNote];
}

ValueTree SvkPreset::getModeSlots()
{
    return theModeSlots;
}

Mode* SvkPreset::getMode1() const
{
    return getModeBySelector(0);
}

Mode* SvkPreset::getMode2() const
{
    return getModeBySelector(1);
}

Mode* SvkPreset::getModeViewed()
{
    return getModeBySelector(thePropertiesNode[SvkProperty::modeSelectorViewed]);
}

Mode* SvkPreset::getCustomMode()
{
    if (modeCustom != nullptr)
        return modeCustom.get();
    return nullptr;
}

Mode* SvkPreset::getModeInSlot(int modeSlotNumIn) const
{
    int slotIndex = getSlotNumberIndex(modeSlotNumIn);

    if (slotIndex >= 0)
    {
        jassert(slotIndex < getNumSlotsInUse());

        Mode* mode = modeSlots[slotIndex];
        DBG("GRABBING MODE " + String(modeSlotNumIn) + " which is: " + mode->getDescription());

        return mode;
    }
    else if (modeSlotNumIn > 0)
    {
        return modeCustom.get();
    }

    return nullptr;
}

Mode* SvkPreset::getModeBySelector(int selectorNumber) const
{
    int slotNumber = getSlotNumberBySelector(selectorNumber);
    if (isSlotNumberInUse(slotNumber))
    {
        auto index = getSlotNumberIndex(slotNumber);
        return getModeInSlot(index);
    }

    else if (slotNumber > MAX_MODE_SLOTS_INDEX)
    {
        return modeCustom.get();
    }

    return nullptr;
}

Mode* SvkPreset::setModeCustom(ValueTree modeNodeIn)
{
    setCustomMode(modeNodeIn);
    return getCustomMode();
}

Mode* SvkPreset::setModeCustom(String stepsIn, String familyIn, String nameIn, String infoIn, int rootNoteIn)
{
    return setModeCustom(Mode::createNode(stepsIn, familyIn, nameIn, infoIn, rootNoteIn));
}

int SvkPreset::setSlotAndSelection(int modeSlotNum, int modeSelectorNum, ValueTree modeNode, bool sendChangeMessage)
{
    int slotIndex = setModeSlot(modeNode, modeSlotNum, false);
    setModeSelectorSlotNum(modeSelectorNum, modeSlotNum, sendChangeMessage);
    return slotIndex;
}

int SvkPreset::addSlotAndSetSelection(int modeSelectorNumber, ValueTree modeNode, bool sendChangeMessage)
{
    int modeSlotNum = addModeSlot(modeNode, false);
    setModeSelectorSlotNum(modeSelectorNumber, modeSlotNum, sendChangeMessage);
    return modeSlotNum;
}

int SvkPreset::setModeSelectorSlotNum(int selectorNumIn, int slotNumIn, bool sendChangeMessage)
{
    if (selectorNumIn >= 0 && slotNumIn >= 0)
    {
        ValueTree modeSelectorNode = ValueTree(SvkProperty::modeSelectorsNode);

        if (selectorNumIn < theModeSelectors.getNumChildren())
            modeSelectorNode = theModeSelectors.getChild(selectorNumIn);
    
        else
            theModeSelectors.appendChild(modeSelectorNode, nullptr);
        
        modeSelectorNode.setProperty(SvkProperty::modeSlotNumber, slotNumIn, nullptr);
        return slotNumIn;
    }

    return -1;
}

void SvkPreset::setMode1SlotNumber(int slotNumIn, bool sendChangeMessage)
{
    setModeSelectorSlotNum(0, slotNumIn);
}

void SvkPreset::setMode2SlotNumber(int slotNumIn, bool sendChangeMessage)
{
    setModeSelectorSlotNum(1, slotNumIn);
}

void SvkPreset::setModeSelectorRootNote(int modeSelectorNumIn, int rootNoteIn, bool sendChangeMessage)
{
    ValueTree node = theModeSelectors.getChild(modeSelectorNumIn);

    if (node.isValid() && rootNoteIn >= 0 && rootNoteIn < 128)
        node.setProperty(SvkProperty::modeSelectorRootNote, rootNoteIn, nullptr);
}

int SvkPreset::setModeSlot(ValueTree modeNodeIn, int slotNumberIn, bool sendChangeMessage)
{
    if (Mode::isValidMode(modeNodeIn) && slotNumberIn >= 0 && slotNumberIn <= MAX_MODE_SLOTS_INDEX)
    {
        ValueTree slotNode = ValueTree(SvkProperty::modeSlotsNode);
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

        auto modeNode = modeNodeIn.createCopy();
        slotNode.setProperty(SvkProperty::modeSlotNumber, slotNumberIn, nullptr);
        slotNode.appendChild(modeNode, nullptr);

        modeSlots.set(slotNumberIn, new Mode(modeNode));

        if (slotNumbersInUse.indexOf(slotNumberIn) < 0)
            slotNumbersInUse.addUsingDefaultSort(slotNumberIn);

        return slotIndex;
    }

    return -1;
}

int SvkPreset::addModeSlot(ValueTree modeNodeIn, bool sendChangeMessage)
{
    int slotIndex = getNextFreeSlotNumber();
    return setModeSlot(modeNodeIn, slotIndex);
}

ValueTree SvkPreset::setCustomMode(ValueTree customModeNodeIn, bool sendChangeMessage)
{
    if (Mode::isValidMode(customModeNodeIn))
    {
        ValueTree nodeToAdd = customModeNodeIn.createCopy();
        theCustomMode.removeAllChildren(nullptr);
        theCustomMode.appendChild(nodeToAdd, nullptr);

        modeCustom = std::make_unique<Mode>(nodeToAdd, false);
        return nodeToAdd;
    }

    return ValueTree();
}

int SvkPreset::removeModeSlot(int slotNumberIn, bool sendChangeMessage)
{
    int slotIndex = getSlotNumberIndex(slotNumberIn);

    if (slotIndex >= 0)
    {
        theModeSlots.removeChild(slotIndex, nullptr);
        slotNumbersInUse.removeFirstMatchingValue(slotNumberIn);
    }

    return slotIndex;
}

void SvkPreset::resetModeSlots(bool sendChangeMessage)
{
    theModeSelectors.removeAllChildren(nullptr);
    slotNumbersInUse.clear();
    
    addModeSlot(STD_TUNING_MODE_NODE, false);
    addModeSlot(STD_TUNING_MODE_NODE, false);

    setMode1SlotNumber(0, false);
    setMode2SlotNumber(1, false);

    if (sendChangeMessage)
    {
        // todo
    }
}

juce::String SvkPreset::getMidiInputName() const
{
    return juce::String();
}

bool SvkPreset::setMidiInputDevice(MidiDeviceInfo deviceInfo)
{
    listeners.call(&Listener::midiInputDeviceChanged, deviceInfo);
    return false;
}

juce::String SvkPreset::getMidiOutputName() const
{
    return juce::String();
}

bool SvkPreset::setMidiOutputDevice(MidiDeviceInfo deviceInfo)
{
    listeners.call(&Listener::midiOutputDeviceChanged, deviceInfo);
    return false;
}

int SvkPreset::getPeriodShift() const
{
    return 0;
}

void SvkPreset::setPeriodShift(int shiftAmt)
{
    listeners.call(&Listener::periodShiftAmountChanged, shiftAmt);
}

bool SvkPreset::periodShiftIsModeSize() const
{
    return false;
}

void SvkPreset::setPeriodShiftToModeSize(bool isModeSize)
{
    listeners.call(&Listener::periodShiftSizeChanged, isModeSize);
}

int SvkPreset::getTransposeAmount() const
{
    return 0;
}

void SvkPreset::setTransposeAmount(int steps)
{
    listeners.call(&Listener::transposeAmountChanged, steps);
}

int SvkPreset::getVoiceLimit() const
{
    return 128;
}

void SvkPreset::setVoiceLimit(int maxVoices)
{
    listeners.call(&Listener::voiceLimitChanged, maxVoices);
}

int SvkPreset::getMpePitchbendRange() const
{
    return 2;
}

void SvkPreset::setMpePitchbendRange(int semitones)
{
    listeners.call(&Listener::mpePitchbendRangeChanged, semitones);
}

int SvkPreset::getGlobalPitchbendRange() const
{
    return 4;
}

void SvkPreset::setGlobalPitchbendRange(int semitones)
{
    listeners.call(&Listener::globalPitchbendRangeChanged, semitones);
}

NoteMap SvkPreset::getMidiInputMap() const
{
    return NoteMap();
}

void SvkPreset::setMidiInputMap(const NoteMap& mapIn, bool updateNode, bool sendChangeMessage)
{
    //inputNoteMap = std::make_unique<NoteMap>(mapIn);
    //if (sendChangeMessage)
    //    listeners.call(&Listener::inputMappingChanged, inputNoteMap.get());
}

VirtualKeyboard::HighlightStyle SvkPreset::getKeyHighlightStyle() const
{
    return VirtualKeyboard::HighlightStyle((int)theKeyboardNode[SvkProperty::keyboardHighlightStyle]);
}

void SvkPreset::setKeyHighlightStyle(VirtualKeyboard::HighlightStyle styleIn)
{
    listeners.call(&Listener::keyboardHighlightStyleChanged, styleIn);
}

//VirtualKeyboard::KeyPlacementType SvkPreset::getKeyPlacementType() const
//{
//    return VirtualKeyboard::KeyPlacementType();// (int)theKeyboardNode[SvkProperty::pl]);
//
//}

void SvkPreset::setKeyPlacementType(VirtualKeyboard::KeyPlacementType placementTypeIn)
{
    listeners.call(&Listener::keyboardKeyPlacementTypeChanged, placementTypeIn);
}

float SvkPreset::getKeySizeRatio() const
{
    return (bool)theKeyboardNode[SvkProperty::pianoWHRatio];
}

void SvkPreset::setKeySizeRatio(float keyRatio)
{
    listeners.call(&Listener::keyboardKeyRatioChanged, keyRatio);
}

bool SvkPreset::areNoteNumbersShown() const
{
    return (bool)theKeyboardNode[SvkProperty::pianoKeysShowNoteNumbers];
}

void SvkPreset::showNoteNumbers(bool showNumbers)
{
    listeners.call(&Listener::keyParametersShown, (int)showNumbers);
}

bool SvkPreset::isValidPresetNode(ValueTree presetNodeIn)
{
    bool isValid = presetNodeIn.hasType(SvkProperty::presetNode);

    if (isValid)
    {
        isValid *= presetNodeIn.getChildWithName(SvkProperty::modeSlotsNode).getNumChildren() > 0;
            
        // TODO: more checks?
    }

    return isValid;
}

SvkPreset SvkPreset::getDefaultPreset()
{
    SvkPreset defaultPreset;

    ValueTree propertiesNode = defaultPreset.parentNode.getOrCreateChildWithName(SvkProperty::presetProperties, nullptr);
    propertiesNode.setProperty(SvkProperty::modeSelectorViewed, 1, nullptr);

    defaultPreset.resetModeSlots();

    ValueTree pianoNode = defaultPreset.parentNode.getOrCreateChildWithName(SvkProperty::pianoNode, nullptr);
    pianoNode.setProperty(SvkProperty::keyboardKeysStyle, 1, nullptr);
    pianoNode.setProperty(SvkProperty::pianoKeysShowNoteNumbers, 0, nullptr);
    pianoNode.setProperty(SvkProperty::keyboardHighlightStyle, 1, nullptr);
    
    ValueTree mappingMode = defaultPreset.parentNode.getOrCreateChildWithName(SvkProperty::midiMapNode, nullptr);
    mappingMode.setProperty(SvkProperty::mappingMode, 1, nullptr);
    mappingMode.setProperty(SvkProperty::autoMappingStyle, 1, nullptr);

    ValueTree customMode = defaultPreset.parentNode.getOrCreateChildWithName(SvkProperty::modeCustomNode, nullptr);
    customMode.appendChild(Mode::createNode("1"), nullptr);

    return defaultPreset;
}

String SvkPreset::toString()
{
    return parentNode.toXmlString();
}
