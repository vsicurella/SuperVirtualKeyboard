/*
 ==============================================================================
 
 SvkState.cpp
 Created: 22 Apr 2019 12:05:22pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#include "SvkState.h"

#include "../Constants.h"
#include "../Structures/Mode.h"
#include "../Structures/NoteMap.h"
#include "../CommonFunctions.h"


SvkState::SvkState(juce::ValueTree stateIn)
    : SvkStateBase(SvkProperty::svkParentNode.toString())
    , modeSlots(std::make_shared<juce::OwnedArray<Mode>>())
    , slotNumbersInUse(std::make_shared<juce::Array<int>>())
{
    state = loadStateProperties(stateIn);
    state.setProperty(SvkProperty::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);
    pullChildrenFromParentNode(state);
    state.addListener(this);
}

SvkState::SvkState(juce::String nameIn, const SvkState& stateToCopy)
    : SvkStateBase(nameIn)
    , modeSlots(std::make_shared<juce::OwnedArray<Mode>>())
    , slotNumbersInUse(std::make_shared<juce::Array<int>>())
{
    state = loadStateProperties(juce::ValueTree(SvkProperty::svkParentNode));
    state.setProperty(SvkProperty::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);
    pullChildrenFromParentNode(state);
    restoreFromNode(stateToCopy.state, false);
    state.addListener(this);
}

SvkState::~SvkState() 
{

}


float SvkState::getPresetVersion() const
{
    return (float)state[SvkProperty::pluginPresetVersion];
}

juce::ValueTree SvkState::getMidiSettingsNode()
{
    return midiProperties;
}

juce::ValueTree SvkState::getMappingsNode()
{
    return mappingProperties;
}

// ValueTree SvkState::getPresetNode(bool sortModeSlots)
// {
//     if (sortModeSlots)
//     {
//         // TODO: Maybe rethink the context of this...
//         // If sorted, PresetManager's ModeSlots will need to be resynched

//         HashMap<int, ValueTree> slotMap;
//         for (auto mode : theModeSlots)
//             slotMap.set(mode[SvkProperty::modeSlotNumber], mode);

//         theModeSlots.removeAllChildren(nullptr);

//         for (auto slotNum : slotNumbersInUse)
//             theModeSlots.appendChild(slotMap[slotNum], nullptr);
//     }

//     return parentNode;
// }


MappingMode SvkState::getMappingMode() const
{
    return (MappingMode)(int)mappingProperties[SvkProperty::mappingMode];
}

MappingStyle SvkState::getMappingStyle() const
{
    return (MappingStyle)(int)mappingProperties[SvkProperty::autoMappingStyle];
}

bool SvkState::restoreFromNode(ValueTree presetNodeIn, bool sendChangeMessage)
{
    if (!isValidPresetNode(presetNodeIn))
        return false;

    // Update root properties in-place — never reassign state, child refs stay valid.
    state.copyPropertiesFrom(presetNodeIn, nullptr);
    state.setProperty(SvkProperty::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);

    auto updateChild = [&](juce::ValueTree& dest, const juce::Identifier& type)
    {
        auto src = presetNodeIn.getChildWithName(type);
        if (src.isValid())
            dest.copyPropertiesAndChildrenFrom(src, nullptr);
        else
            dest.removeAllChildren(nullptr);
    };

    updateChild(presetProperties,       SvkProperty::presetProperties);
    updateChild(modeSlotProperties,     SvkProperty::modeSlotsNode);
    updateChild(modeSelectorProperties, SvkProperty::modeSelectorsNode);
    updateChild(customModeProperties,   SvkProperty::modeCustomNode);
    updateChild(keyboardProperties,     SvkProperty::pianoNode);
    updateChild(midiProperties,         SvkProperty::midiSettingsNode);
    updateChild(mappingProperties,      SvkProperty::midiMapNode);

    // Rebuild in-memory mode slot objects from the updated tree.
    modeSlots->clear();
    slotNumbersInUse->clear();
    for (int i = 0; i < modeSlotProperties.getNumChildren(); i++)
    {
        juce::ValueTree slotNode = modeSlotProperties.getChild(i);
        int slotNum = jlimit(0, MAX_MODE_SLOTS_INDEX, (int)slotNode[SvkProperty::modeSlotNumber]);
        slotNumbersInUse->addUsingDefaultSort(slotNum);

        auto modeNode = slotNode.getChild(0); // mode data is the first (only) child of the slot node
        if (Mode::isValidMode(modeNode))
            modeSlots->set(slotNum, new Mode(modeNode));
    }

    // Restore custom mode from tree.
    {
        auto customNode = customModeProperties.getChild(0);
        if (Mode::isValidMode(customNode))
            modeCustom = std::make_unique<Mode>(customNode, false);
        else
            modeCustom = nullptr;
    }

    // Ensure at least 2 mode selectors.
    while (modeSelectorProperties.getNumChildren() < 2)
    {
        juce::ValueTree selectorNode(SvkProperty::modeSelectorsNode);
        selectorNode.setProperty(SvkProperty::modeSlotNumber, modeSelectorProperties.getNumChildren(), nullptr);
        selectorNode.setProperty(SvkProperty::modeSelectorRootNote, 60, nullptr);
        modeSelectorProperties.appendChild(selectorNode, nullptr);
    }

    for (auto selector : modeSelectorProperties)
    {
        if (!selector.hasProperty(SvkProperty::modeSelectorRootNote))
            selector.setProperty(SvkProperty::modeSelectorRootNote, 60, nullptr);
    }

    if (sendChangeMessage)
        listeners.call(&Listener::presetReloaded, *this);

    return true;
}

const Array<int>* SvkState::getSlotNumbersInUse() const
{
    return slotNumbersInUse.get();
}

int SvkState::getNumSlotsInUse() const
{
    return slotNumbersInUse->size();
}

int SvkState::getModeSelectorViewed() const
{
    return presetProperties[SvkProperty::modeSelectorViewed];
}

bool SvkState::isSlotNumberInUse(int modeSlotNumberIn) const
{
    return slotNumbersInUse->contains(modeSlotNumberIn);
}

int SvkState::getModeSlotsSize() const
{
    return modeSlotProperties.getNumChildren();
}

int SvkState::getSlotNumberBySelector(int selectorNumIn) const
{
    return (int)modeSelectorProperties.getChild(selectorNumIn)[SvkProperty::modeSlotNumber];
}

int SvkState::getMode1SlotNumber() const
{
    return getSlotNumberBySelector(0);
}
int SvkState::getMode2SlotNumber() const
{
    return getSlotNumberBySelector(1);
}

int SvkState::getModeViewedSlotNumber() const
{
    return getSlotNumberBySelector(getModeSelectorViewed());
}

int SvkState::getSlotNumberIndex(int slotNumIn)
{
    for (int i = 0; i < modeSlotProperties.getNumChildren(); i++)
    {
        if (slotNumIn == (int)(modeSlotProperties.getChild(i)[SvkProperty::modeSlotNumber]))
            return i;
    }

    return -1;
}

int SvkState::getNextFreeSlotNumber()
{
    for (int i = 1; i < slotNumbersInUse->size(); i++)
    {
        if (slotNumbersInUse->getUnchecked(i) - slotNumbersInUse->getUnchecked(i - 1) > 1)
            return slotNumbersInUse->getUnchecked(i - 1) + 1;
    }

    if (slotNumbersInUse->size() < MAX_MODE_SLOTS_INDEX + 1)
        return slotNumbersInUse->size();

    return -1;
}

int SvkState::getModeSelectorRootNote(int modeSelectorNum) const
{
    return modeSelectorProperties.getChild(modeSelectorNum)[SvkProperty::modeSelectorRootNote];
}

// ValueTree SvkState::getModeSlots()
// {
//     return modeSlotProperties;
// }

Mode* SvkState::getMode1()
{
    return getModeBySelector(0);
}

Mode* SvkState::getMode2()
{
    return getModeBySelector(1);
}

Mode* SvkState::getModeViewed()
{
    return getModeBySelector((int)presetProperties[SvkProperty::modeSelectorViewed]);
}

Mode* SvkState::getCustomMode()
{
    if (modeCustom != nullptr)
        return modeCustom.get();
    return nullptr;
}

Mode* SvkState::getModeInSlot(int modeSlotNumIn)
{
    int slotIndex = getSlotNumberIndex(modeSlotNumIn);

    if (slotIndex >= 0)
    {
        jassert(slotIndex < getNumSlotsInUse());

        Mode* mode = modeSlots->getUnchecked(slotIndex);
        DBG("GRABBING MODE " + String(modeSlotNumIn) + " which is: " + mode->getDescription());

        return mode;
    }
    else if (modeSlotNumIn > 0)
    {
        return modeCustom.get();
    }

    return nullptr;
}

Mode* SvkState::getModeBySelector(int selectorNumber)
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

Mode* SvkState::setModeCustom(ValueTree modeNodeIn)
{
    setCustomMode(modeNodeIn);
    return getCustomMode();
}

Mode* SvkState::setModeCustom(String stepsIn, String familyIn, String nameIn, String infoIn, int rootNoteIn)
{
    return setModeCustom(Mode::createNode(stepsIn, familyIn, nameIn, infoIn, rootNoteIn));
}

int SvkState::setSlotAndSelection(int modeSlotNum, int modeSelectorNum, ValueTree modeNode, bool sendChangeMessage)
{
    int slotIndex = setModeSlot(modeNode, modeSlotNum, false);
    setModeSelectorSlotNum(modeSelectorNum, modeSlotNum, sendChangeMessage);
    return slotIndex;
}

int SvkState::addSlotAndSetSelection(int modeSelectorNumber, ValueTree modeNode, bool sendChangeMessage)
{
    int modeSlotNum = addModeSlot(modeNode, false);
    setModeSelectorSlotNum(modeSelectorNumber, modeSlotNum, sendChangeMessage);
    return modeSlotNum;
}

int SvkState::setModeSelectorSlotNum(int selectorNumIn, int slotNumIn, bool sendChangeMessage)
{
    if (selectorNumIn >= 0 && slotNumIn >= 0)
    {
        juce::ValueTree modeSelectorNode(SvkProperty::modeSelectorsNode);

        if (selectorNumIn < modeSelectorProperties.getNumChildren())
            modeSelectorNode = modeSelectorProperties.getChild(selectorNumIn);
        else
            modeSelectorProperties.appendChild(modeSelectorNode, nullptr);

        modeSelectorNode.setProperty(SvkProperty::modeSlotNumber, slotNumIn, nullptr);
        return slotNumIn;
    }

    return -1;
}

void SvkState::setMode1SlotNumber(int slotNumIn, bool sendChangeMessage)
{
    setModeSelectorSlotNum(0, slotNumIn);
}

void SvkState::setMode2SlotNumber(int slotNumIn, bool sendChangeMessage)
{
    setModeSelectorSlotNum(1, slotNumIn);
}

void SvkState::setModeSelectorRootNote(int modeSelectorNumIn, int rootNoteIn, bool sendChangeMessage)
{
    juce::ValueTree node = modeSelectorProperties.getChild(modeSelectorNumIn);

    if (node.isValid() && rootNoteIn >= 0 && rootNoteIn < 128)
        node.setProperty(SvkProperty::modeSelectorRootNote, rootNoteIn, nullptr);
}

int SvkState::setModeSlot(ValueTree modeNodeIn, int slotNumberIn, bool sendChangeMessage)
{
    if (Mode::isValidMode(modeNodeIn) && slotNumberIn >= 0 && slotNumberIn <= MAX_MODE_SLOTS_INDEX)
    {
        ValueTree slotNode = ValueTree(SvkProperty::modeSlotsNode);
        int slotIndex = getSlotNumberIndex(slotNumberIn);
        if (slotIndex >= 0)
        {
            slotNode = modeSlotProperties.getChild(slotIndex);
            slotNode.removeAllChildren(nullptr);
        }
        else
        {
            slotIndex = modeSlotProperties.getNumChildren();
            modeSlotProperties.appendChild(slotNode, nullptr);
        }

        auto modeNode = modeNodeIn.createCopy();
        slotNode.setProperty(SvkProperty::modeSlotNumber, slotNumberIn, nullptr);
        slotNode.appendChild(modeNode, nullptr);

        modeSlots->set(slotNumberIn, new Mode(modeNode));

        if (slotNumbersInUse->indexOf(slotNumberIn) < 0)
            slotNumbersInUse->addUsingDefaultSort(slotNumberIn);

        return slotIndex;
    }

    return -1;
}

int SvkState::addModeSlot(ValueTree modeNodeIn, bool sendChangeMessage)
{
    int slotIndex = getNextFreeSlotNumber();
    return setModeSlot(modeNodeIn, slotIndex);
}

ValueTree SvkState::setCustomMode(ValueTree customModeNodeIn, bool sendChangeMessage)
{
    if (Mode::isValidMode(customModeNodeIn))
    {
        ValueTree nodeToAdd = customModeNodeIn.createCopy();
        customModeProperties.removeAllChildren(nullptr);
        customModeProperties.appendChild(nodeToAdd, nullptr);

        modeCustom = std::make_unique<Mode>(nodeToAdd, false);
        return nodeToAdd;
    }

    return ValueTree();
}

int SvkState::removeModeSlot(int slotNumberIn, bool sendChangeMessage)
{
    int slotIndex = getSlotNumberIndex(slotNumberIn);

    if (slotIndex >= 0)
    {
        modeSlotProperties.removeChild(slotIndex, nullptr);
        slotNumbersInUse->removeFirstMatchingValue(slotNumberIn);
    }

    return slotIndex;
}

void SvkState::resetModeSlots(bool sendChangeMessage)
{
    modeSlotProperties.removeAllChildren(nullptr);
    slotNumbersInUse->clear();
    
    addModeSlot(STD_TUNING_MODE_NODE, false);
    addModeSlot(STD_TUNING_MODE_NODE, false);

    setMode1SlotNumber(0, false);
    setMode2SlotNumber(1, false);

    if (sendChangeMessage)
    {
        // todo
    }
}

juce::String SvkState::getMidiInputName() const
{
    // MIDI device names are non-preset app settings; stored separately from preset state.
    return juce::String();
}

bool SvkState::setMidiInputDevice(MidiDeviceInfo deviceInfo)
{
    listeners.call(&Listener::midiInputDeviceChanged, deviceInfo);
    return false;
}

juce::String SvkState::getMidiOutputName() const
{
    return juce::String();
}

bool SvkState::setMidiOutputDevice(MidiDeviceInfo deviceInfo)
{
    listeners.call(&Listener::midiOutputDeviceChanged, deviceInfo);
    return false;
}

int SvkState::getPeriodShift() const
{
    return (int)midiProperties[SvkProperty::periodShift];
}

void SvkState::setPeriodShift(int shiftAmt)
{
    midiProperties.setProperty(SvkProperty::periodShift, shiftAmt, nullptr);
}

bool SvkState::periodShiftIsModeSize() const
{
    return (bool)midiProperties[SvkProperty::periodShiftModeSize];
}

void SvkState::setPeriodShiftToModeSize(bool isModeSize)
{
    midiProperties.setProperty(SvkProperty::periodShiftModeSize, isModeSize, nullptr);
}

int SvkState::getTransposeAmount() const
{
    return (int)midiProperties[SvkProperty::transposeAmt];
}

void SvkState::setTransposeAmount(int steps)
{
    midiProperties.setProperty(SvkProperty::transposeAmt, steps, nullptr);
}

int SvkState::getVoiceLimit() const
{
    return (int)midiProperties.getProperty(SvkProperty::maxVoices, 128);
}

void SvkState::setVoiceLimit(int maxVoices)
{
    midiProperties.setProperty(SvkProperty::maxVoices, maxVoices, nullptr);
}

int SvkState::getMpePitchbendRange() const
{
    return (int)midiProperties.getProperty(SvkProperty::pitchBendNoteMax, 48);
}

void SvkState::setMpePitchbendRange(int semitones)
{
    midiProperties.setProperty(SvkProperty::pitchBendNoteMax, semitones, nullptr);
}

int SvkState::getGlobalPitchbendRange() const
{
    return (int)midiProperties.getProperty(SvkProperty::pitchBendGlobalMax, 2);
}

void SvkState::setGlobalPitchbendRange(int semitones)
{
    midiProperties.setProperty(SvkProperty::pitchBendGlobalMax, semitones, nullptr);
}

int SvkState::getMidiChannelOut() const
{
    return jlimit(1, 16, (int)midiProperties.getProperty(SvkProperty::keyboardMidiChannel, 1));
}

void SvkState::setMidiChannelOut(int channel)
{
    midiProperties.setProperty(SvkProperty::keyboardMidiChannel, jlimit(1, 16, channel), nullptr);
}

NoteMap SvkState::getMidiInputMap() const
{
    auto mapNode = mappingProperties.getChildWithName(SvkProperty::midiInputRemap);
    if (mapNode.isValid())
        return NoteMap(mapNode);
    return NoteMap();
}

void SvkState::setMidiInputMap(const NoteMap& mapIn, bool updateNode, bool sendChangeMessage)
{
    if (updateNode)
    {
        auto mapNode = mapIn.getAsValueTree(SvkProperty::midiInputRemap);
        auto existing = mappingProperties.getChildWithName(SvkProperty::midiInputRemap);
        if (existing.isValid())
            existing.copyPropertiesAndChildrenFrom(mapNode, nullptr);
        else
            mappingProperties.appendChild(mapNode, nullptr);
    }

    if (sendChangeMessage)
        listeners.call(&Listener::inputMappingChanged, &mapIn);
}

VirtualKeyboard::HighlightStyle SvkState::getKeyHighlightStyle() const
{
    return VirtualKeyboard::HighlightStyle((int)keyboardProperties[SvkProperty::keyboardHighlightStyle]);
}

void SvkState::setKeyHighlightStyle(VirtualKeyboard::HighlightStyle styleIn)
{
    keyboardProperties.setProperty(SvkProperty::keyboardHighlightStyle, (int)styleIn, nullptr);
}

VirtualKeyboard::Orientation SvkState::getKeyboardOrientation() const
{
    return VirtualKeyboard::Orientation((int)keyboardProperties[SvkProperty::keyboardOrientation]);
}

VirtualKeyboard::HighlightStyle SvkState::getHighlightStyle() const
{
    return VirtualKeyboard::HighlightStyle((int)keyboardProperties[SvkProperty::keyboardHighlightStyle]);
}

VirtualKeyboard::ScrollingStyle SvkState::getScrollingStyle() const
{
    return VirtualKeyboard::ScrollingStyle((int)keyboardProperties[SvkProperty::keyboardScrollingStyle]);
}

VirtualKeyboard::KeyPlacementType SvkState::getKeyPlacementType() const
{
    return VirtualKeyboard::KeyPlacementType((int)keyboardProperties[SvkProperty::keyboardKeysStyle]);
}

void SvkState::setKeyPlacementType(VirtualKeyboard::KeyPlacementType placementTypeIn)
{
    keyboardProperties.setProperty(SvkProperty::keyboardKeysStyle, (int)placementTypeIn, nullptr);
}

float SvkState::getKeySizeRatio() const
{
    float ratio = (float)keyboardProperties[SvkProperty::pianoWHRatio];
    return (ratio < 0.01f) ? 0.25f : ratio;
}

void SvkState::setKeySizeRatio(float keyRatio)
{
    keyboardProperties.setProperty(SvkProperty::pianoWHRatio, keyRatio, nullptr);
}

bool SvkState::areNoteNumbersShown() const
{
    return (bool)keyboardProperties[SvkProperty::pianoKeysShowNoteNumbers];
}

void SvkState::setShowNumbers(bool showNumbers)
{
    keyboardProperties.setProperty(SvkProperty::pianoKeysShowNoteNumbers, showNumbers, nullptr);
}

juce::ValueTree SvkState::getPresetNode() const
{
    juce::ValueTree node(SvkProperty::presetNode);
    node.setProperty(SvkProperty::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);
    node.appendChild(presetProperties.createCopy(),       nullptr);
    node.appendChild(modeSlotProperties.createCopy(),     nullptr);
    node.appendChild(modeSelectorProperties.createCopy(), nullptr);
    node.appendChild(customModeProperties.createCopy(),   nullptr);
    node.appendChild(keyboardProperties.createCopy(),     nullptr);
    node.appendChild(midiProperties.createCopy(),         nullptr);
    node.appendChild(mappingProperties.createCopy(),      nullptr);
    return node;
}

const SvkPreset& SvkState::getSavedPreset() const
{
    return savedPreset;
}

bool SvkState::isPresetEdited() const
{
    return !getPresetNode().isEquivalentTo(savedPreset.parentNode);
}

void SvkState::commitPreset()
{
    savedPreset.restoreFromNode(getPresetNode(), false);
}

void SvkState::revertPreset(bool sendChangeMessage)
{
    restoreFromNode(savedPreset.getPresetNode(), sendChangeMessage);
}

bool SvkState::isValidPresetNode(ValueTree presetNodeIn)
{
    bool isValid = presetNodeIn.hasType(SvkProperty::presetNode);

    if (isValid)
    {
        isValid *= presetNodeIn.getChildWithName(SvkProperty::modeSlotsNode).getNumChildren() > 0;
            
        // TODO: more checks?
    }

    return isValid;
}

// SvkState SvkState::getDefaultPreset()
// {
//     SvkState defaultPreset;

//     ValueTree propertiesNode = defaultPreset.parentNode.getOrCreateChildWithName(SvkProperty::presetProperties, nullptr);
//     propertiesNode.setProperty(SvkProperty::modeSelectorViewed, 1, nullptr);

//     defaultPreset.resetModeSlots();

//     ValueTree pianoNode = defaultPreset.parentNode.getOrCreateChildWithName(SvkProperty::pianoNode, nullptr);
//     pianoNode.setProperty(SvkProperty::keyboardKeysStyle, 1, nullptr);
//     pianoNode.setProperty(SvkProperty::pianoKeysShowNoteNumbers, 0, nullptr);
//     pianoNode.setProperty(SvkProperty::keyboardHighlightStyle, 1, nullptr);
    
//     ValueTree mappingMode = defaultPreset.parentNode.getOrCreateChildWithName(SvkProperty::midiMapNode, nullptr);
//     mappingMode.setProperty(SvkProperty::mappingMode, 1, nullptr);
//     mappingMode.setProperty(SvkProperty::autoMappingStyle, 1, nullptr);

//     ValueTree customMode = defaultPreset.parentNode.getOrCreateChildWithName(SvkProperty::modeCustomNode, nullptr);
//     customMode.appendChild(Mode::createNode("1"), nullptr);

//     return defaultPreset;
// }

String SvkState::toString()
{
    return state.toXmlString();
}

juce::ValueTree SvkState::loadStateProperties(juce::ValueTree stateIn)
{
    return stateIn.hasType(SvkProperty::svkParentNode)
               ? stateIn
               : juce::ValueTree(SvkProperty::svkParentNode);
}

void SvkState::pullChildrenFromParentNode(juce::ValueTree parentNode)
{
    presetProperties    = parentNode.getOrCreateChildWithName(SvkProperty::presetProperties,   nullptr);
    modeSlotProperties  = parentNode.getOrCreateChildWithName(SvkProperty::modeSlotsNode,      nullptr);
    modeSelectorProperties = parentNode.getOrCreateChildWithName(SvkProperty::modeSelectorsNode, nullptr);
    customModeProperties= parentNode.getOrCreateChildWithName(SvkProperty::modeCustomNode,     nullptr);
    keyboardProperties  = parentNode.getOrCreateChildWithName(SvkProperty::pianoNode,          nullptr);
    midiProperties      = parentNode.getOrCreateChildWithName(SvkProperty::midiSettingsNode,   nullptr);
    mappingProperties   = parentNode.getOrCreateChildWithName(SvkProperty::midiMapNode,        nullptr);
}

void SvkState::handleStatePropertyChange(juce::ValueTree treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (treeWhosePropertyHasChanged == midiProperties)
    {
        if (property == SvkProperty::periodShift)
            listeners.call(&Listener::periodShiftAmountChanged, (int)midiProperties[property]);
        else if (property == SvkProperty::periodShiftModeSize)
            listeners.call(&Listener::periodShiftSizeChanged, (bool)midiProperties[property]);
        else if (property == SvkProperty::transposeAmt)
            listeners.call(&Listener::transposeAmountChanged, (int)midiProperties[property]);
        else if (property == SvkProperty::maxVoices)
            listeners.call(&Listener::voiceLimitChanged, (int)midiProperties[property]);
        else if (property == SvkProperty::pitchBendNoteMax)
            listeners.call(&Listener::mpePitchbendRangeChanged, (int)midiProperties[property]);
        else if (property == SvkProperty::pitchBendGlobalMax)
            listeners.call(&Listener::globalPitchbendRangeChanged, (int)midiProperties[property]);
        else if (property == SvkProperty::keyboardMidiChannel)
            listeners.call(&Listener::midiChannelOutChanged, jlimit(1, 16, (int)midiProperties[property]));
    }
    else if (treeWhosePropertyHasChanged == keyboardProperties)
    {
        if (property == SvkProperty::keyboardKeysStyle)
            listeners.call(&Listener::keyboardKeyPlacementTypeChanged,
                (VirtualKeyboard::KeyPlacementType)(int)keyboardProperties[property]);
        else if (property == SvkProperty::keyboardHighlightStyle)
            listeners.call(&Listener::keyboardHighlightStyleChanged,
                (VirtualKeyboard::HighlightStyle)(int)keyboardProperties[property]);
        else if (property == SvkProperty::pianoWHRatio)
            listeners.call(&Listener::keyboardKeyRatioChanged,
                (float)keyboardProperties[property]);
        else if (property == SvkProperty::pianoKeysShowNoteNumbers)
            listeners.call(&Listener::keyParametersShown,
                (int)keyboardProperties[property]);
    }
}

void SvkState::loadPropertiesFile(juce::PropertiesFile* properties)
{
    // TODO: load non-preset app settings (MIDI devices, directories) from PropertiesFile
}
