/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SvkAudioProcessor::SvkAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    svkUndo(new UndoManager()),
    svkValueTree(*this, svkUndo.get()),//, IDs::svkParentNode, createParameters())
    pluginSettings(std::make_unique<SvkPluginSettings>()),
    presetManager(std::make_unique<SvkPresetManager>(pluginSettings->getSettingsNode())),
    workingPreset(presetManager->getPreset())
{
    // Temporary
    svkValueTree.state = ValueTree(IDs::svkParentNode);

    //pluginSettings.reset(new SvkPluginSettings());

    //presetManager.reset(new SvkPresetManager(pluginSettings->getSettingsNode()));
    presetManager->addListener(this);

    //workingPreset = presetManager->getPreset();

    // TODO: Factory default MIDI settings
    midiProcessor.reset(new SvkMidiProcessor(workingPreset));

    modeMapper.reset(new ModeMapper());

    buildFactoryDefaultState();
    buildUserDefaultState();
}

SvkAudioProcessor::~SvkAudioProcessor()
{
    
}

//==============================================================================
const String SvkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SvkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SvkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SvkAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SvkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SvkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SvkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SvkAudioProcessor::setCurrentProgram (int index)
{
}

const String SvkAudioProcessor::getProgramName (int index)
{
    return {};
}

void SvkAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SvkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sendChangeMessage();
}

void SvkAudioProcessor::releaseResources()
{
    midiProcessor->allNotesOff();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SvkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // No audio inputs or outputs needed
    ignoreUnused (layouts);
    return true;
}
#endif

void SvkAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data
    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    midiProcessor->processMidi(midiMessages);
}

//==============================================================================
bool SvkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SvkAudioProcessor::createEditor()
{
    return new SvkPluginEditor(*this);
}

//==============================================================================
void SvkAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    MemoryOutputStream memOut(destData, true);
    auto stateTree = buildStateValueTree();
    svkValueTree.state.copyPropertiesAndChildrenFrom(stateTree, nullptr);
    svkValueTree.state.writeToStream(memOut);
    DBG("Saving Plugin State node to internal memory:" + svkValueTree.state.toXmlString());
}

void SvkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    MemoryInputStream memIn(data, sizeInBytes, false);
    ValueTree presetRecall = ValueTree::readFromStream(memIn);
    DBG("Found this in memory:\n" + presetRecall.toXmlString());

    //presetRecall.removeChild(presetRecall.getChildWithName(IDs::pluginStateNode), nullptr); // uncomment this line to test new instantiation

    //svkValueTree.replaceState(presetRecall);    
    //recordParamIDs();
    recallState(presetRecall, true);
}

//==============================================================================

UndoManager* SvkAudioProcessor::getUndoManager()
{
    return svkUndo.get();
}

//const Array<String>& SvkAudioProcessor::getParamIDs() const
//{
//    return paramIDs;
//}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SvkAudioProcessor();
}

//==============================================================================

//void SvkAudioProcessor::recordParamIDs()
//{
//    for (auto param : getParameters())
//    {
//        paramIDs.add(dynamic_cast<AudioProcessorParameterWithID*>(param)->paramID);
//    }
//    DBG("Parameters are initialized");
//    DBGArray(paramIDs);
//}
//
AudioProcessorValueTreeState::ParameterLayout SvkAudioProcessor::createParameters()
{ 

    return AudioProcessorValueTreeState::ParameterLayout({ std::make_unique<AudioParameterBool>("NotifyHostDummy", "SameAsInitialValue", true) });
//    std::vector<std::unique_ptr<RangedAudioParameter>> paramsInit;
//    
//    //paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::presetSlotViewed.toString(), "Preset Slot Viewed", 0, 1, 0));
// //   
// //   paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode1SlotNum.toString(), "Mode 1 Slot Number", 0, 128, 0));
// //   paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode2SlotNum.toString(), "Mode 2 Slot Number", 0, 128, 1));
// //   paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode1RootNote.toString(), "Mode 1 Root Note", 0, 127, 60));
// //   paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode2RootNote.toString(), "Mode 2 Root Note", 0, 127, 60));
// //   paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode1RootChannel.toString(), "Mode 1 Root Channel", 1, 16, 1));
// //   paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode2RootChannel.toString(), "Mode 2 Root Channel", 1, 16, 1));
//    
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mappingMode.toString(), "Mapping Mode", 1, 3, 1));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::autoMappingStyle.toString(), "Mapping Style", 1, 3, 1));
//    
//    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::mpeOn.toString(), "MPE On", false));
//    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::mpeLegacyOn.toString(), "MPE Legacy Mode", false));
//    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::mpeTuningPreserveMidiNote.toString(), "Preserve Midi Notes", true));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pitchBendGlobalMax.toString(), "Global Pitch Bend", 0, 8192, 2));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pitchBendNoteMax.toString(), "Note Pitch Bend", 0, 8192, 48));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::maxVoices.toString(), "Max Polyphony", 1, 16, 15));
//
//    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::retuneOn.toString(), "Retune On", false));
//    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::retuneAuto.toString(), "Retune Auto", false));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::tuningRootNote.toString(), "Tuning Reference Note", 0, 127, 69));
//    paramsInit.push_back(std::make_unique<AudioParameterFloat>(IDs::tuningRootFreq.toString(), "Tuning Reference Frequency", 0.01f, 24000.0f, 440.0f));
//
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::periodShift.toString(), "Period Shift", -10, 10, 0));
//    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::periodShiftModeSize.toString(), "Shift By Mode Size", 0));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::transposeAmt.toString(), "Transpose", -127, 127, 0));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardMidiChannel.toString(), "Midi Channel Out", 1, 16, 1));
//    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::pianoKeysShowNoteNumbers.toString(), "Show Note Numbers", false));
//    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::pianoKeysShowFilteredNotes.toString(), "Show Filtered Numbers", false));
//    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::pianoKeyShowNoteLabel.toString(), "Show Pitch Names", false));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardScrollingMode.toString(), "Scrolling Mode", 0, 3, 1));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardScrollingStyle.toString(), "Scrolling Style", 0, 3, 1));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::numKeysInWidth.toString(), "Num Keys in Width", 1, 128, 16));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardNumRows.toString(), "Keyboard Rows", 1, 16, 1));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardOrientation.toString(), "Keyboard Orientation", 0, 3, 0));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardKeysStyle.toString(), "Key Style", 1, 4, 1));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardHighlightStyle.toString(), "Highlight Style", 1, 4, 1));
//    
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pianoVelocityBehavior.toString(), "Keyboard Velocity Mode", 0, 2, 0));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pianoVelocityValue.toString(), "Default Velocity", 0, 127, 100));
//    paramsInit.push_back(std::make_unique<AudioParameterFloat>(IDs::pianoWHRatio.toString(), "Key Size Ratio", 0.01f, 1.62f, 0.25f));
//    
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::modeSlotToEdit.toString(), "Mode Slot Debug", 0, 1, 0));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::modeLibraryIndex.toString(), "Mode Debug Index", 0, 1, 0));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::modeRootNote.toString(), "Mode Debug Root", 0, 127, 60));
//    
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyNumberToEdit.toString(), "Key To Debug", 0, 127, 60));
//    paramsInit.push_back(std::make_unique<AudioParameterFloat>(IDs::pianoKeyWidthMod.toString(), "Key Debug Width Mod", 0.001f, 10.0f, 1.0f));
//    paramsInit.push_back(std::make_unique<AudioParameterFloat>(IDs::pianoKeyHeightMod.toString(), "Key Debug Height Mod", 0.001f, 10.0f, 1.0f));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pianoKeyXOffset.toString(), "Key Debug X Offset", -1000, 1000, 0));
//    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pianoKeyYOffset.toString(), "Key Debug Y Offset", -1000, 1000, 0));
//    
//
//    return {paramsInit.begin(), paramsInit.end()};
}


void SvkAudioProcessor::recallState(ValueTree nodeIn, bool fallbackToDefaultSettings)
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

        revertToSavedPreset(fallbackToDefaultSettings);
    }

    auto pluginStateNode = buildStateValueTree();
    svkValueTree.state.copyPropertiesAndChildrenFrom(pluginStateNode, nullptr);

    DBG("Plugin State Node after recall:\n" + svkValueTree.state.toXmlString());
}

void SvkAudioProcessor::revertToSavedPreset(bool fallbackToDefaultSettings, bool sendChange)
{
    presetManager->resetToSavedPreset(sendChange);

    auto pluginStateNode = svkValueTree.state.getChildWithName(IDs::pluginStateNode);

    // svkTree.state.appendChild(pluginStateNode, nullptr);
    
    auto preset = presetManager->getPreset();
    modeSelectorViewedNum = preset.getModeSelectorViewed();

    modeMapper.reset(new ModeMapper(preset.getMappingsNode()));
    // TODO: fallbacks
    midiProcessor->restoreSettingsNode(preset.getMidiSettingsNode());
    midiProcessor->restoreMappingNode(preset.getMappingsNode());
    midiProcessor->restoreDevicesNode(pluginStateNode.getOrCreateChildWithName(IDs::midiDeviceSettingsNode, nullptr));

    onModeUpdate(!sendChange, !sendChange);
}

ValueTree SvkAudioProcessor::buildStateValueTree()
{
    // TODO: make it so it's not necessary to call this before saving (?)

    auto midiNodes = midiProcessor->getNode();
    // pluginStateNode.getOrCreateChildWithName(IDs::midiDeviceSettingsNode, nullptr).copyPropertiesAndChildrenFrom(midiProcessor->midiDeviceNode, nullptr);

    auto presetManagerNode = presetManager->commitPreset();
    auto presetNode = presetManager->getPreset().getPresetNode();
    
    // presetNode.getOrCreateChildWithName(IDs::pianoNode, nullptr).copyPropertiesAndChildrenFrom(pluginStateNode.getChildWithName(IDs::pianoNode), nullptr);

    ValueTree pluginStateNode(IDs::pluginStateNode);
    pluginStateNode.setProperty(IDs::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);

    pluginStateNode.addChild(presetNode, 0, nullptr);

    onModeUpdate(false, false);

    DBG("COMMITTED NODE:\n" + pluginStateNode.toXmlString());

    return pluginStateNode;
}

int SvkAudioProcessor::isValidStateNode(ValueTree pluginStateNodeIn)
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

SvkPresetManager* SvkAudioProcessor::getPresetManager() const
{
    return presetManager.get();
}

SvkMidiProcessor* SvkAudioProcessor::getMidiProcessor() const
{
    return midiProcessor.get();
}

SvkPluginSettings* SvkAudioProcessor::getPluginSettings() const
{
    return pluginSettings.get();
}

ModeMapper* SvkAudioProcessor::getModeMapper() const
{
    return modeMapper.get();
}

NoteMap* SvkAudioProcessor::getMidiInputFilterMap() const
{
    return midiProcessor->getInputRemapMidiFilter()->getNoteMap();
}

NoteMap* SvkAudioProcessor::getMidiOutputFilterMap() const
{
    return midiProcessor->getOutputMidiFilter()->getNoteMap();
}

ValueTree SvkAudioProcessor::getPresetNode() const
{
    return presetManager->getPreset().getPresetNode();
}

Mode* SvkAudioProcessor::getModeInSlot(int slotNumIn) const
{
    return presetManager->getModeInSlot(slotNumIn);
}

Mode* SvkAudioProcessor::getModeViewed() const
{
    return modeViewed;
}

Mode* SvkAudioProcessor::getMode1() const
{
    return presetManager->getModeBySelector(0);
}

Mode* SvkAudioProcessor::getMode2() const
{
    return presetManager->getModeBySelector(1);
}

Mode* SvkAudioProcessor::getModeCustom() const
{
    return presetManager->getModeCustom();
}

//==============================================================================

//float SvkAudioProcessor::getParameterValue(Identifier paramId)
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
//float SvkAudioProcessor::getParameterMin(Identifier paramId)
//{
//    RangedAudioParameter* param = svkTree.getParameter(paramId);
//
//    if (param)
//        return param->convertFrom0to1(0.0f);
//
//    return 0.0f;
//}
//
//float SvkAudioProcessor::getParameterMax(Identifier paramId)
//{
//    RangedAudioParameter* param = svkTree.getParameter(paramId);
//
//    if (param)
//        return param->convertFrom0to1(1.0f);
//
//    return 0.0f;
//}
//
//float SvkAudioProcessor::getParameterDefaultValue(Identifier paramId)
//{
//    RangedAudioParameter* param = svkTree.getParameter(paramId);
//
//    if (param)
//        return param->convertFrom0to1(param->getDefaultValue());
//
//    return 0.0f;
//}

void SvkAudioProcessor::addPresetManagerListener(SvkPresetManager::Listener* listenerIn)
{
    presetManager->addListener(listenerIn);
}
void SvkAudioProcessor::removePresetManagerListener(SvkPresetManager::Listener* listenerIn)
{
    presetManager->removeListener(listenerIn);
}

int SvkAudioProcessor::getNumModesLoaded() const
{
    return presetManager->getNumModeSlots();
}

int SvkAudioProcessor::getModeSelectorViewed() const
{
    return modeSelectorViewedNum;
}

int SvkAudioProcessor::getModeViewedSlotNumber() const
{
    return presetManager->getModeSlotOfSelector(modeSelectorViewedNum);
}

int SvkAudioProcessor::getMappingMode() const
{
    return getPresetNode().getChildWithName(IDs::midiMapNode)[IDs::mappingMode];
}

int SvkAudioProcessor::getAutoMappingStyle() const
{
    return getPresetNode().getChildWithName(IDs::midiMapNode)[IDs::autoMappingStyle];
}

bool SvkAudioProcessor::isAutoMapping() const
{
    return getMappingMode() == 2.0f;
}

int SvkAudioProcessor::getModeSlotRoot(int slotNum) const
{
    return getModeInSlot(slotNum)->getRootNote();
}

int SvkAudioProcessor::getMode1Root() const
{
    return getModeSlotRoot(0);
}

int SvkAudioProcessor::getMode2Root() const
{
    return getModeSlotRoot(1);
}

SvkPreset& SvkAudioProcessor::getPreset() const
{
    return workingPreset;
}

bool SvkAudioProcessor::isPresetEdited()
{
    return presetManager->isPresetEdited();
}

//==============================================================================

void SvkAudioProcessor::setParameterValue(Identifier paramIdIn, float valueIn)
{
    // RangedAudioParameter* param = svkTree.getParameter(paramIdIn);

    // if (param)
    // {
    //     param->setValueNotifyingHost(param->convertTo0to1(valueIn));
    // }
}

void SvkAudioProcessor::setModeSelectorViewed(int modeSelectorViewedIn)
{
    modeSelectorViewedNum = modeSelectorViewedIn;
    updateModeViewed();
}

void SvkAudioProcessor::handleModeSelection(int modeBoxNum, int idIn)
{
    presetManager->handleModeSelection(modeBoxNum, idIn);
    onModeUpdate();
}

void SvkAudioProcessor::setMapMode(int mapModeSelectionIn)
{
    DBG("Plugin State Map Mode Selection: " + String(mapModeSelectionIn));
    
    if (mapModeSelectionIn == 2) // Auto Mapping
        doAutoMapping();
    
    else
    {
        if (mapModeSelectionIn == 3) // Manual Mapping
        {
            midiProcessor->restoreMappingNode(midiProcessor->midiMapNode);
        }

        else // Mapping Off
        {
            midiProcessor->setInputRemap(NoteMap(), false);
            midiProcessor->setOutputFilter(NoteMap(), false);
            setModeSelectorViewed(1);
        }

        //listeners.call(&Listener::inputMappingChanged, midiProcessor->getInputNoteRemap());
    }
}

void SvkAudioProcessor::setAutoMapStyle(int mapStyleIn)
{
    modeMapper->setMappingStyle(mapStyleIn);
    DBG("mapStyle index = " + String(mapStyleIn));
    if (isAutoMapping())
    {
        doAutoMapping();
    }
}

void SvkAudioProcessor::setModeSelectorRoot(int modeSelectorNumIn, int rootNoteIn, bool updateParameter)
{
    rootNoteIn = totalModulus(rootNoteIn, 128);
    workingPreset.setModeSelectorRootNote(modeSelectorNumIn, rootNoteIn);
    
    Mode* mode = presetManager->getModeBySelector(modeSelectorNumIn);
    mode->setRootNote(rootNoteIn);

    if (modeSelectorViewedNum == modeSelectorNumIn)
        updateModeViewed();

    if (isAutoMapping())
        doAutoMapping();
}

void SvkAudioProcessor::onModeUpdate(bool sendModeViewedChangeMessage, bool sendMappingChangeMessage)
{
    midiProcessor->setMode1(getMode1());
    midiProcessor->setMode2(getMode2());

    updateModeViewed(sendMappingChangeMessage);
    
    if (isAutoMapping())
        doAutoMapping(sendMappingChangeMessage);
}

//==============================================================================

void SvkAudioProcessor::setMidiInputMap(NoteMap noteMapIn, bool updateNode, bool sendChangeMessage)
{
    midiProcessor->setInputRemap(noteMapIn, updateNode);

    //if (sendChangeMessage)
    //    listeners.call(&Listener::inputMappingChanged, midiProcessor->getInputNoteRemap());
}

void SvkAudioProcessor::setMidiOutputMap(NoteMap noteMapIn, bool updateNode)
{
    midiProcessor->setOutputFilter(noteMapIn, updateNode);
}

void SvkAudioProcessor::setModeCustom(String stepsIn, bool sendChangeMessage)
{
    presetManager->setModeCustom(stepsIn);
    handleModeSelection(getModeSelectorViewed(), 999);
    
    //if (sendChangeMessage)
    //    listeners.call(&Listener::customModeChanged, presetManager->getModeCustom());
}

void SvkAudioProcessor::setMapOrder1(int orderIn)
{
    modeMapper->setMode1OrderNum(orderIn);
}

void SvkAudioProcessor::setMapOrder2(int orderIn)
{
    modeMapper->setMode2OrderNum(orderIn);
}

void SvkAudioProcessor::setMapOrderOffset1(int offsetIn)
{
    modeMapper->setMode1OrderOffset(offsetIn);
}

void SvkAudioProcessor::setMapOrderOffset2(int offsetIn)
{
    modeMapper->setMode2OrderOffset(offsetIn);
}

void SvkAudioProcessor::doAutoMapping(const Mode* mode1, const Mode* mode2, bool sendChangeMessage)
{
    NoteMap noteMap = modeMapper->map(*mode1, *mode2, *midiProcessor->getInputNoteRemap());

    setMidiInputMap(noteMap, false);

    //if (sendChangeMessage)
    //    listeners.call(&Listener::inputMappingChanged, midiProcessor->getInputNoteRemap());
}

void SvkAudioProcessor::doAutoMapping(bool sendChangeMessage)
{
    if (getMappingMode() == 2)
    {
        DBG("Applying new MIDI mapping");
        doAutoMapping(getMode1(), getMode2());
    }
    else
    {
        midiProcessor->setInputRemap(NoteMap(), false);

        //if (sendChangeMessage)
        //    listeners.call(&Listener::inputMappingChanged, midiProcessor->getInputNoteRemap());
    }
}

//==============================================================================

void SvkAudioProcessor::updateModeViewed(bool sendChange)
{
    modeViewed = presetManager->getModeBySelector(modeSelectorViewedNum);
    midiProcessor->setModeViewed(modeViewed);

    //if (sendChange)
    //    listeners.call(&Listener::modeViewedChanged, modeViewed, modeSelectorViewedNum, presetManager->getModeSlotOfSelector(modeSelectorViewedNum));
}

void SvkAudioProcessor::commitModeInfo(bool sendChangeMessage)
{
    if (modeViewed == presetManager->getModeCustom())
    {
        DBG("Custom mode edited:" + presetManager->getModeCustom()->getNode().toXmlString());
    }

    presetManager->setSlotToMode(modeSelectorViewedNum, modeViewed->getNode());
    
    onModeUpdate(sendChangeMessage, sendChangeMessage);

    //if (sendChangeMessage)
    //    listeners.call(&Listener::modeInfoChanged, modeViewed);
}

bool SvkAudioProcessor::savePresetToFile()
{
    presetManager->commitPreset();
    return presetManager->savePresetToFile(pluginSettings->getPresetPath());
}

bool SvkAudioProcessor::loadPresetFromFile()
{
    File file(pluginSettings->getPresetPath());
    ValueTree presetLoaded = presetManager->parsePresetFile(file);
    recallState(presetLoaded);
    return true;
}

bool SvkAudioProcessor::saveModeViewedToFile()
{
    return presetManager->saveModeToFile(
        presetManager->getModeSlotOfSelector(modeSelectorViewedNum), pluginSettings->getModePath());
}

bool SvkAudioProcessor::loadModeFromFile()
{
    chooser = std::make_unique<FileChooser>("Open Mode",  pluginSettings->getModePath(), "*.svk");
    
    chooser->launchAsync(FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles,
        [&](const FileChooser& chooser)
        {
            auto result = chooser.getResult();
            if (result.existsAsFile())
            {
                auto modeTree = presetManager->parseModeFile(result);
                if (Mode::isValidMode(modeTree))
                {
                    presetManager->addSlotAndSetSelection(getModeSelectorViewed(), modeTree);
                    onModeUpdate();
                }
            }
        });

    return true;
}

//==============================================================================

void SvkAudioProcessor::buildFactoryDefaultState()
{
    ModeMapper mapper;

    factoryDefaultPluginStateNode = ValueTree(IDs::pluginStateNode);
    factoryDefaultPluginStateNode.setProperty(IDs::pluginPresetVersion, SVK_PRESET_VERSION, nullptr);
    factoryDefaultPluginStateNode.appendChild(pluginSettings->getSettingsNode().createCopy(), nullptr);
    factoryDefaultPluginStateNode.appendChild(presetManager->getPreset().getPresetNode(), nullptr);
}

void SvkAudioProcessor::buildUserDefaultState()
{
    ValueTree defaultSettings = pluginSettings->getSettingsNode().getChildWithName(IDs::presetNode);
    
    // TODO: connect to actual user default settings

    if (!defaultSettings.isValid())
        defaultSettings = factoryDefaultPluginStateNode.createCopy();
}

//==============================================================================

//void SvkAudioProcessor::parameterChanged(const String& paramID, float newValue)
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
