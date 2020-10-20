/*
  ==============================================================================

    PluginState.cpp
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PluginState.h"

SvkPluginState::SvkPluginState(AudioProcessorValueTreeState& svkTreeIn)
    : svkTree(svkTreeIn)
{
    pluginStateNode = svkTree.state.getOrCreateChildWithName(IDs::pluginStateNode, nullptr);
    pluginStateNode.setProperty(IDs::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);

    // TODO: Factory default settings, use a function call to load user settings
    pluginSettings.reset(new SvkPluginSettings());

    presetManager.reset(new SvkPresetManager(pluginSettings->getSettingsNode()));
    presetManager->addChangeListener(this);

    // TODO: Factory default MIDI settings
    midiProcessor.reset(new SvkMidiProcessor(svkTree));

    modeMapper.reset(new ModeMapper());

    buildFactoryDefaultState();
    buildUserDefaultState();

    revertToSavedPreset(true, false);
}

void SvkPluginState::recallState(ValueTree nodeIn, bool fallbackToDefaultSettings)
{
    ValueTree stateIn = nodeIn;
    int status = isValidStateNode(stateIn);
    DBG("Validity status returned: " + String(status));

    if (status & PluginStateNodeStatus::IsAPVTSNode)
    {
        if (status & 1)
            stateIn = stateIn.getChildWithName(IDs::pluginStateNode);
        
        // TODO
        else
        {
            //stateIn = stateIn.getChildWithName(IDs::alphaPluginStateNode);
            stateIn = factoryDefaultPluginStateNode.createCopy();
        }
    }

    if (stateIn.isValid())
    {
        bool loaded = presetManager->loadPreset(stateIn.getChildWithName(IDs::presetNode), false);
        if (!loaded)
        {
            // Might not be good - will actually delete existing properties that are set properly
            stateIn = defaultPluginStateNode;
            presetManager->loadPreset(stateIn.getChildWithName(IDs::presetNode), false);
        }

        pluginStateNode = stateIn.createCopy();
        revertToSavedPreset(fallbackToDefaultSettings);
    }

    DBG("Plugin State Node after recall:\n" + pluginStateNode.toXmlString());
}

ValueTree SvkPluginState::getPluginEditorNode() const
{
    return pluginStateNode.getChildWithName(IDs::pluginEditorNode);
}

void SvkPluginState::revertToSavedPreset(bool fallbackToDefaultSettings, bool sendChange)
{
    presetEdited = false;
    presetManager->resetToSavedPreset();

    svkPreset = &presetManager->getPreset();
    presetNode = svkPreset->getPresetNode();

    pluginStateNode.getOrCreateChildWithName(IDs::presetNode, nullptr)
                   .copyPropertiesAndChildrenFrom(presetNode, nullptr);

    for (auto child : svkTree.state)
    {
        if (child.hasType(IDs::pluginStateNode))
        {
            svkTree.state.removeChild(child, nullptr);
            break;
        }
    }

    svkTree.state.appendChild(pluginStateNode, nullptr);
    
    modeSelectorViewedNum = svkPreset->getModeSelectorViewed();

    modeMapper.reset(new ModeMapper(svkPreset->getMappingsNode()));
    // TODO: fallbacks
    midiProcessor->restoreSettingsNode(svkPreset->getMidiSettingsNode());
    midiProcessor->restoreMappingNode(svkPreset->getMappingsNode());
    midiProcessor->restoreDevicesNode(pluginStateNode.getOrCreateChildWithName(IDs::midiDeviceSettingsNode, nullptr));

    onModeUpdate(!sendChange, !sendChange);

    if (sendChange)
        listeners.call(&Listener::presetLoaded, presetNode);
}

void SvkPluginState::commitStateNode()
{
    // TODO: make it so it's not necessary to call this before saving (?)

    midiProcessor->updateNodes();

    presetManager->commitPreset();
    svkPreset = &presetManager->getPreset();
    presetNode = svkPreset->getPresetNode();

    pluginStateNode = svkTree.state.getOrCreateChildWithName(IDs::pluginStateNode, nullptr);
    pluginStateNode.getOrCreateChildWithName(IDs::presetNode, nullptr).copyPropertiesAndChildrenFrom(presetNode, nullptr);
    presetNode.getOrCreateChildWithName(IDs::pianoNode, nullptr).copyPropertiesAndChildrenFrom(pluginStateNode.getChildWithName(IDs::pianoNode), nullptr);

    onModeUpdate(false, false);

    pluginStateNode.getOrCreateChildWithName(IDs::midiDeviceSettingsNode, nullptr).copyPropertiesAndChildrenFrom(midiProcessor->midiDeviceNode, nullptr);

    DBG("COMMITTED NODE:\n" + pluginStateNode.toXmlString());

    presetEdited = false;
}

int SvkPluginState::isValidStateNode(ValueTree pluginStateNodeIn)
{
    int status = 0; // TODO: determine status codes

    // Determine if it's a SVK state node or a APVTS node
    ValueTree stateNode;

    if (pluginStateNodeIn.hasType(IDs::svkParentNode))
    {
        status = PluginStateNodeStatus::IsAPVTSNode;

        stateNode = pluginStateNodeIn.getChildWithName(IDs::pluginStateNode);

        if (!stateNode.isValid())
        {
            // Check if it's an alpha preset
            stateNode = pluginStateNodeIn.getChildWithName(IDs::alphaPluginStateNode);
        }
    }

    else if (pluginStateNodeIn.hasType(IDs::pluginStateNode))
    {
        stateNode = pluginStateNodeIn;
    }

    if (stateNode.isValid())
    {
        // Determine if it's the current preset version or not
        if ((float)stateNode[IDs::pluginPresetVersion] == SVK_PRESET_VERSION)
        {
            status += PluginStateNodeStatus::CurrentVersion;
        }
        else // Can add more conditions for certain versions if necessary
        {
            status += PluginStateNodeStatus::AlphaVersion;
        }

        if (status == 1)
        {
            // TODO: Various validity checks
        }
    }

    return status;
}

//==============================================================================

SvkPresetManager* SvkPluginState::getPresetManager()
{
    return presetManager.get();
}

SvkMidiProcessor* SvkPluginState::getMidiProcessor()
{
    return midiProcessor.get();
}

SvkPluginSettings* SvkPluginState::getPluginSettings()
{
    return pluginSettings.get();
}

ModeMapper* SvkPluginState::getModeMapper()
{
    return modeMapper.get();
}

NoteMap* SvkPluginState::getMidiInputFilterMap()
{
    return midiProcessor->getInputRemapMidiFilter()->getNoteMap();
}

NoteMap* SvkPluginState::getMidiOutputFilterMap()
{
    return midiProcessor->getOutputMidiFilter()->getNoteMap();
}

ValueTree SvkPluginState::getPresetNode()
{
    return presetNode;
}

Mode* SvkPluginState::getModeInSlot(int slotNumIn) const
{
    return presetManager->getModeInSlot(slotNumIn);
}

Mode* SvkPluginState::getModeViewed()
{
    return modeViewed;
}

Mode* SvkPluginState::getMode1()
{
    return presetManager->getModeBySelector(0);
}

Mode* SvkPluginState::getMode2()
{
    return presetManager->getModeBySelector(1);
}

Mode* SvkPluginState::getModeCustom()
{
    return presetManager->getModeCustom();
}

//==============================================================================

//float SvkPluginState::getParameterValue(Identifier paramId)
//{
//    RangedAudioParameter* param = svkTree.getParameter(paramId);
//    
//    if (param)
//    {
//        return param->convertFrom0to1(param->getValue());
//    }
//    
//    return 0.0f;
//}
//
//float SvkPluginState::getParameterMin(Identifier paramId)
//{
//    RangedAudioParameter* param = svkTree.getParameter(paramId);
//
//    if (param)
//        return param->convertFrom0to1(0.0f);
//
//    return 0.0f;
//}
//
//float SvkPluginState::getParameterMax(Identifier paramId)
//{
//    RangedAudioParameter* param = svkTree.getParameter(paramId);
//
//    if (param)
//        return param->convertFrom0to1(1.0f);
//
//    return 0.0f;
//}
//
//float SvkPluginState::getParameterDefaultValue(Identifier paramId)
//{
//    RangedAudioParameter* param = svkTree.getParameter(paramId);
//
//    if (param)
//        return param->convertFrom0to1(param->getDefaultValue());
//
//    return 0.0f;
//}

int SvkPluginState::getNumModesLoaded() const
{
    return presetManager->getNumModeSlots();
}

int SvkPluginState::getModeSelectorViewed() const
{
    return modeSelectorViewedNum;
}

int SvkPluginState::getModeViewedSlotNumber() const
{
    return presetManager->getModeSlotOfSelector(modeSelectorViewedNum);
}

int SvkPluginState::getMappingMode() const
{
    return presetNode.getChildWithName(IDs::midiMapNode)[IDs::mappingMode];
}

int SvkPluginState::getAutoMappingStyle() const
{
    return presetNode.getChildWithName(IDs::midiMapNode)[IDs::autoMappingStyle];
}

bool SvkPluginState::isAutoMapping() const
{
    return getMappingMode() == 2.0f;
}

int SvkPluginState::getModeSlotRoot(int slotNum) const
{
    return getModeInSlot(slotNum)->getRootNote();
}

int SvkPluginState::getMode1Root() const
{
    return getModeSlotRoot(0);
}

int SvkPluginState::getMode2Root() const
{
    return getModeSlotRoot(1);
}

bool SvkPluginState::isPresetEdited()
{
    return presetEdited;
}

//==============================================================================

void SvkPluginState::setParameterValue(Identifier paramIdIn, float valueIn)
{
    RangedAudioParameter* param = svkTree.getParameter(paramIdIn);

    if (param)
    {
        param->setValueNotifyingHost(param->convertTo0to1(valueIn));
    }
}

void SvkPluginState::setModeSelectorViewed(int modeSelectorViewedIn)
{
    modeSelectorViewedNum = modeSelectorViewedIn;
    presetNode.getChildWithName(IDs::presetProperties).setProperty(IDs::modeSelectorViewed, modeSelectorViewedNum, nullptr);
    updateModeViewed();
}

void SvkPluginState::handleModeSelection(int modeBoxNum, int idIn)
{
    presetManager->handleModeSelection(modeBoxNum, idIn);
    onModeUpdate();
    presetEdited = true;
}

void SvkPluginState::setMapMode(int mapModeSelectionIn)
{
    ValueTree mappingNode = presetNode.getChildWithName(IDs::midiMapNode);
    mappingNode.setProperty(IDs::mappingMode, mapModeSelectionIn, nullptr);
    DBG("Plugin State Map Mode Selection: " + String(mapModeSelectionIn));
    
    if (mapModeSelectionIn == 2) // Auto Mapping
        doAutoMapping();
    
    else
    {
        if (mapModeSelectionIn == 3) // Manual Mapping
        {
            midiProcessor->restoreMappingNode(midiProcessor->midiMapNode);

            // TODO: control state with button
            mappingNode.setProperty(IDs::manualMappingEditOn, 1, nullptr);
        }

        else // Mapping Off
        {
            midiProcessor->setInputRemap(NoteMap(), false);
            midiProcessor->setOutputFilter(NoteMap(), false);
            setModeSelectorViewed(1);
        }

        listeners.call(&Listener::inputMappingChanged, midiProcessor->getInputNoteRemap());
    }
}

void SvkPluginState::setAutoMapStyle(int mapStyleIn)
{
    modeMapper->setMappingStyle(mapStyleIn);
    presetNode.getChildWithName(IDs::midiMapNode).setProperty(IDs::autoMappingStyle, mapStyleIn, nullptr);
    DBG("mapStyle index = " + String(mapStyleIn));
    if (isAutoMapping())
    {
        doAutoMapping();
    }
}

void SvkPluginState::setModeSelectorRoot(int modeSelectorNumIn, int rootNoteIn, bool updateParameter)
{
    rootNoteIn = totalModulus(rootNoteIn, 128);
    svkPreset->setModeSelectorRootNote(modeSelectorNumIn, rootNoteIn);
    
    Mode* mode = presetManager->getModeBySelector(modeSelectorNumIn);
    mode->setRootNote(rootNoteIn);

    presetEdited = true;

    if (modeSelectorViewedNum == modeSelectorNumIn)
        updateModeViewed();

    if (isAutoMapping())
        doAutoMapping();
}

void SvkPluginState::onModeUpdate(bool sendModeViewedChangeMessage, bool sendMappingChangeMessage)
{
    midiProcessor->setMode1(getMode1());
    midiProcessor->setMode2(getMode2());

    updateModeViewed(sendMappingChangeMessage);
    
    if (isAutoMapping())
        doAutoMapping(sendMappingChangeMessage);
}

//==============================================================================

void SvkPluginState::setMidiInputMap(NoteMap noteMapIn, bool updateNode, bool sendChangeMessage)
{
    midiProcessor->setInputRemap(noteMapIn, updateNode);

    if (sendChangeMessage)
        listeners.call(&Listener::inputMappingChanged, midiProcessor->getInputNoteRemap());
}

void SvkPluginState::setMidiOutputMap(NoteMap noteMapIn, bool updateNode)
{
    midiProcessor->setOutputFilter(noteMapIn, updateNode);
}

void SvkPluginState::setModeCustom(String stepsIn, bool sendChangeMessage)
{
    presetManager->setModeCustom(stepsIn);
    handleModeSelection(getModeSelectorViewed(), 999);
    
    if (sendChangeMessage)
        listeners.call(&Listener::customModeChanged, presetManager->getModeCustom());
}

void SvkPluginState::setMapOrder1(int orderIn)
{
    modeMapper->setMode1OrderNum(orderIn);
}

void SvkPluginState::setMapOrder2(int orderIn)
{
    modeMapper->setMode2OrderNum(orderIn);
}

void SvkPluginState::setMapOrderOffset1(int offsetIn)
{
    modeMapper->setMode1OrderOffset(offsetIn);
}

void SvkPluginState::setMapOrderOffset2(int offsetIn)
{
    modeMapper->setMode2OrderOffset(offsetIn);
}

void SvkPluginState::doAutoMapping(const Mode* mode1, const Mode* mode2, bool sendChangeMessage)
{
    NoteMap noteMap = modeMapper->map(*mode1, *mode2, *midiProcessor->getInputNoteRemap());

    setMidiInputMap(noteMap, false);

    if (sendChangeMessage)
        listeners.call(&Listener::inputMappingChanged, midiProcessor->getInputNoteRemap());
}

void SvkPluginState::doAutoMapping(bool sendChangeMessage)
{
    if (getMappingMode() == 2)
    {
        DBG("Applying new MIDI mapping");
        doAutoMapping(getMode1(), getMode2());
    }
    else
    {
        midiProcessor->setInputRemap(NoteMap(), false);

        if (sendChangeMessage)
            listeners.call(&Listener::inputMappingChanged, midiProcessor->getInputNoteRemap());
    }
}

//==============================================================================

void SvkPluginState::updateModeViewed(bool sendChange)
{
    modeViewed = presetManager->getModeBySelector(modeSelectorViewedNum);
    midiProcessor->setModeViewed(modeViewed);

    if (sendChange)
        listeners.call(&Listener::modeViewedChanged, modeViewed, modeSelectorViewedNum, presetManager->getModeSlotOfSelector(modeSelectorViewedNum));
}

void SvkPluginState::commitModeInfo(bool sendChangeMessage)
{
    if (modeViewed == presetManager->getModeCustom())
    {
        DBG("Custom mode edited:" + presetManager->getModeCustom()->modeNode.toXmlString());
    }

    presetManager->setSlotToMode(modeSelectorViewedNum, modeViewed->modeNode);
    
    onModeUpdate(sendChangeMessage, sendChangeMessage);

    if (sendChangeMessage)
        listeners.call(&Listener::modeInfoChanged, modeViewed);
}

bool SvkPluginState::savePresetToFile()
{
    commitStateNode();
    return presetManager->savePresetToFile(pluginSettings->getPresetPath());
}

bool SvkPluginState::loadPresetFromFile()
{
    ValueTree presetLoaded = presetManager->presetFromFile(pluginSettings->getPresetPath());
    recallState(presetLoaded);
    return true;
}

bool SvkPluginState::saveModeViewedToFile()
{
    return presetManager->saveModeToFile(
        presetManager->getModeSlotOfSelector(modeSelectorViewedNum), pluginSettings->getModePath());
}

bool SvkPluginState::loadModeFromFile()
{
    ValueTree modeNode = presetManager->nodeFromFile("Open Mode", "*.svk", pluginSettings->getModePath());

    if (Mode::isValidMode(modeNode))
    {
        presetManager->addSlotAndSetSelection(getModeSelectorViewed(), modeNode);
        onModeUpdate();
        return true;
    }

    return false;
}

//==============================================================================

void SvkPluginState::buildFactoryDefaultState()
{
    ModeMapper mapper;

    factoryDefaultPluginStateNode = ValueTree(IDs::pluginStateNode);
    factoryDefaultPluginStateNode.setProperty(IDs::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);
    factoryDefaultPluginStateNode.appendChild(pluginSettings->getSettingsNode().createCopy(), nullptr);
    factoryDefaultPluginStateNode.appendChild(presetManager->getPreset().getPresetNode(), nullptr);
}

void SvkPluginState::buildUserDefaultState()
{
    ValueTree defaultSettings = pluginSettings->getSettingsNode().getChildWithName(IDs::presetNode);
    
    // TODO: connect to actual user default settings

    if (!defaultSettings.isValid())
        defaultSettings = factoryDefaultPluginStateNode.createCopy();
}

//==============================================================================

//void SvkPluginState::parameterChanged(const String& paramID, float newValue)
//{
//    if (paramID == IDs::mappingMode.toString())
//    {
//        setMapMode(newValue);
//    }
//    else if (paramID == IDs::autoMappingStyle.toString())
//    {
//        setAutoMapStyle(newValue);
//    }
//}

void SvkPluginState::changeListenerCallback(ChangeBroadcaster* source)
{
    // Preset loaded
    if (source == presetManager.get())
    {
        revertToSavedPreset();
    }
}
