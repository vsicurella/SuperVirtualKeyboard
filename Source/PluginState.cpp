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
    pluginStateNode = ValueTree(IDs::pluginStateNode);
    
    modeMapper.reset(new ModeMapper());

	pluginSettings.reset(new SvkPluginSettings());
	pluginSettingsNode = pluginSettings->pluginSettingsNode;
	pluginStateNode.addChild(pluginSettingsNode, -1, nullptr);
    
    midiProcessor.reset(new SvkMidiProcessor(svkTree));
    midiSettingsNode = midiProcessor->midiSettingsNode;
    pluginStateNode.addChild(midiSettingsNode, -1, nullptr);
    
	presetManager.reset(new SvkPresetManager(pluginSettingsNode));
	modeLibraryNode = presetManager->modeLibraryNode;
	presetManager->addChangeListener(this);

	virtualKeyboard.reset(new VirtualKeyboard::Keyboard(pianoNode));
	pianoNode = virtualKeyboard->getNode();
    virtualKeyboard->addListener(midiProcessor.get());
    
    setTuningToET(2, 12);

	textFilterIntOrSpace.reset(new TextFilterIntOrSpace());
	textFilterInt.reset(new TextFilterInt());
    
	setPresetViewed(0);
    setModeViewed(1);
	updateModeViewed(false);
}

void SvkPluginState::recallState(ValueTree nodeIn)
{
	if (nodeIn.isValid() && nodeIn.hasType(IDs::pluginStateNode))
	{
        pluginStateNode = nodeIn;

        ValueTree childNode = pluginStateNode.getChildWithName(IDs::presetNode);
        
        if (childNode.isValid())
        {
            presetManager->loadPreset(0, childNode, false);
        }
        
		childNode = nodeIn.getChildWithName(IDs::globalSettingsNode);

		if (childNode.isValid())
        {
			pluginSettings->restoreNode(childNode);
            pluginSettingsNode = pluginSettings->pluginSettingsNode;
        }

		childNode = nodeIn.getChildWithName(IDs::pluginEditorNode);

		if (childNode.isValid() && childNode.getNumProperties() > 0)
        {
			pluginEditorNode = childNode;
        }
        
		resetToPreset();
	}
    else
    {
        svkTree.state.addChild(pluginStateNode, -1, nullptr);
    }
    
    // Default settings
	
    if (!presetViewed->thePropertiesNode.hasProperty(IDs::mappingMode))
		setMapMode(1);
    
    if (!presetViewed->thePropertiesNode.hasProperty(IDs::modeMappingStyle))
		setMapStyle(1);
    
    if (!presetViewed->theKeyboardNode.hasProperty(IDs::keyboardKeysStyle))
		setKeyStyle(1);
    
    if (!presetViewed->theKeyboardNode.hasProperty(IDs::keyboardHighlightStyle))
		setHighlightStyle(1);
    
    if (!presetViewed->thePropertiesNode.hasProperty(IDs::modeSlotNumViewed))
		setModeViewed(1);
    
    if (!presetViewed->thePropertiesNode.hasProperty(IDs::mode1OrderMapping))
        setMapOrder1(0);
    
    if (!presetViewed->thePropertiesNode.hasProperty(IDs::mode2OrderMapping))
        setMapOrder2(0);
    
    if (!presetViewed->thePropertiesNode.hasProperty(IDs::mode1OrderOffsetMapping))
        setMapOrderOffset1(0);
    
    if (!presetViewed->thePropertiesNode.hasProperty(IDs::mode2OrderOffsetMapping))
        setMapOrderOffset2(0);
    
    DBG("FINAL RECALL:");
    DBG(svkTree.state.toXmlString());
}

void SvkPluginState::resetToPreset(bool sendChange)
{
	presetEdited = false;

	presetViewed = presetManager->getPresetLoaded(getPresetSlotNumViewed());

    setModeViewed(presetViewed->thePropertiesNode[IDs::modeSlotNumViewed]);
    setParameterValue(IDs::mappingMode, (int)presetViewed->thePropertiesNode[IDs::mappingMode]);
    setParameterValue(IDs::modeMappingStyle, (int)presetViewed->thePropertiesNode[IDs::modeMappingStyle]);
    
    mapOrder1 = (int) presetViewed->thePropertiesNode[IDs::mode1OrderMapping];
    mapOrder2 = (int) presetViewed->thePropertiesNode[IDs::mode2OrderMapping];
    mapOrderOffset1 = (int) presetViewed->thePropertiesNode[IDs::mode1OrderOffsetMapping];
    mapOrderOffset2 = (int) presetViewed->thePropertiesNode[IDs::mode2OrderOffsetMapping];
    
	updateModeViewed();

	midiProcessor->restoreFromNode(presetViewed->theMidiSettingsNode);
	midiProcessor->setMode1(getMode1());
	midiProcessor->setMode2(getMode2());
    
    pianoNode = presetViewed->theKeyboardNode;
	virtualKeyboard->restoreNode(pianoNode);
    virtualKeyboard->applyMode(modeViewed);

	doMapping();
    
	if (sendChange)
		sendChangeMessage();
}

void SvkPluginState::resetParameterToPresetValue(Identifier paramId)
{
//	if (paramId == IDs::keyboardMidiChannel)
//	{
//		setParameterValue(IDs::keyboardMidiChannel, midiProcessor->getMidiChannelOut());
//	}
//	else if (paramId == IDs::pianoKeysShowNoteNumbers)
//	{
//		setParameterValue(IDs::pianoKeysShowNoteNumbers, virtualKeyboard->isShowingNoteNumbers());
//	}
//	else if (paramId == IDs::pianoKeysShowFilteredNotes)
//	{
//		setParameterValue(IDs::pianoKeysShowFilteredNotes, virtualKeyboard->isShowingFilteredNumbers());
//	}
//	else if (paramId == IDs::pianoKeyShowNoteLabel)
//	{
//		setParameterValue(IDs::pianoKeyShowNoteLabel, virtualKeyboard->isShowingNoteNames());
//	}
//	else if (paramId == IDs::keyboardOrientation)
//	{
//		setParameterValue(IDs::keyboardOrientation, virtualKeyboard->getOrientation());
//	}
//	else if (paramId == IDs::keyboardKeysStyle)
//	{
//		setParameterValue(IDs::keyboardKeysStyle, virtualKeyboard->getKeyPlacementStyle());
//	}
//	else if (paramId == IDs::keyboardHighlightStyle)
//	{
//		setParameterValue(IDs::keyboardHighlightStyle, virtualKeyboard->getHighlightStyle());
//	}
//	else if (paramId == IDs::pianoVelocityBehavior)
//	{
//		setParameterValue(IDs::pianoVelocityBehavior, virtualKeyboard->getVelocityStyle());
//	}
//	else if (paramId == IDs::pianoVelocityValue)
//	{
//		setParameterValue(IDs::pianoVelocityValue, (int)(virtualKeyboard->getVelocityFixed() * 127));
//	}
//	else if (paramId == IDs::pianoWHRatio)
//	{
//		setParameterValue(IDs::pianoWHRatio, virtualKeyboard->getKeySizeRatio());
//	}
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

VirtualKeyboard::Keyboard* SvkPluginState::getKeyboard()
{
	return virtualKeyboard.get();
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

SvkPreset* SvkPluginState::getPresetinSlot(int slotNumIn)
{
    return presetManager->getPresetLoaded(slotNumIn);
}

SvkPreset* SvkPluginState::getPresetViewed()
{
    return presetViewed;
}

Mode* SvkPluginState::getModeInSlot(int slotNumIn)
{
	return presetManager->getModeInSlots(getPresetSlotNumViewed(), slotNumIn);
}

Mode* SvkPluginState::getModeViewed()
{
	return modeViewed;
}

Mode* SvkPluginState::getMode1()
{
	return presetManager->getModeInSlots(getPresetSlotNumViewed(), presetViewed->getMode1SlotNumber());
}

Mode* SvkPluginState::getMode2()
{
	return presetManager->getModeInSlots(getPresetSlotNumViewed(), presetViewed->getMode2SlotNumber());
}

Mode* SvkPluginState::getModeCustom()
{
	return presetManager->getModeCustom();
}

Tuning* SvkPluginState::getTuning()
{
	return nullptr;
}

//==============================================================================

float SvkPluginState::getParameterValue(Identifier paramId)
{
	RangedAudioParameter* param = svkTree.getParameter(paramId);
    
	if (param)
    {
        return param->convertFrom0to1(param->getValue());
    }
	
	return 0.0f;
}

float SvkPluginState::getParameterMin(Identifier paramId)
{
    RangedAudioParameter* param = svkTree.getParameter(paramId);

	if (param)
		return param->convertFrom0to1(0.0f);

	return 0.0f;
}

float SvkPluginState::getParameterMax(Identifier paramId)
{
    RangedAudioParameter* param = svkTree.getParameter(paramId);

	if (param)
		return param->convertFrom0to1(1.0f);

	return 0.0f;
}

float SvkPluginState::getParameterDefaultValue(Identifier paramId)
{
    RangedAudioParameter* param = svkTree.getParameter(paramId);

	if (param)
		return param->convertFrom0to1(param->getDefaultValue());

	return 0.0f;
}

int SvkPluginState::getPresetSlotNumViewed()
{
	return getParameterValue(IDs::presetSlotViewed);
}

int SvkPluginState::getNumModesInPresetViewed()
{
	return presetViewed->getModeSlotsSize();
}

int SvkPluginState::getModeViewedNum()
{
	return modeViewedNum;
}

int SvkPluginState::getMappingMode()
{
	return getParameterValue(IDs::mappingMode);
}

int SvkPluginState::getMappingStyle()
{
	return getParameterValue(IDs::modeMappingStyle);
}

bool SvkPluginState::isAutoMapping()
{
	return getMappingMode() == 2.0f;
}

int SvkPluginState::getModeSlotRoot(int slotNum)
{
	return getModeInSlot(slotNum)->getRootNote();
}

int SvkPluginState::getMode1Root()
{
	return getModeSlotRoot(0);
}

int SvkPluginState::getMode2Root()
{
	return getModeSlotRoot(1);
}

int SvkPluginState::getMapOrder1()
{
    return mapOrder1;
}

int SvkPluginState::getMapOrder2()
{
    return mapOrder2;
}

int SvkPluginState::getMapOrderOffset1()
{
    return mapOrderOffset1;
}

int SvkPluginState::getMapOrderOffset2()
{
    return mapOrderOffset2;
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
		param->setValue(param->convertTo0to1(valueIn));
	}
}

void SvkPluginState::setPresetViewed(int presetViewedIn)
{
    presetViewed = presetManager->getPresetLoaded(presetViewedIn);

	midiProcessor->setMode1(getMode1());
	midiProcessor->setMode2(getMode2());
}

void SvkPluginState::setModeViewed(int modeViewedIn)
{
    modeViewedNum = modeViewedIn;
    presetViewed->thePropertiesNode.setProperty(IDs::modeSlotNumViewed, modeViewedNum, nullptr);
	updateModeViewed();
}

void SvkPluginState::handleModeSelection(int modeBoxNum, int idIn)
{
    presetManager->handleModeSelection(getPresetSlotNumViewed(), modeBoxNum, idIn);
    
	midiProcessor->setMode1(getMode1());
	midiProcessor->setMode2(getMode2());

	if (getModeViewedNum() == modeBoxNum)
		updateModeViewed();
	
    sendChangeMessage();

	if (isAutoMapping())
		doMapping();

	if (midiProcessor->isMPEOn())
	{
		setTuningToET(2, getMode2()->getScaleSize());
	}

    presetEdited = true;
}

void SvkPluginState::setMapMode(int mapModeSelectionIn)
{
    presetViewed->thePropertiesNode.setProperty(IDs::mappingMode, mapModeSelectionIn, nullptr);

	if (mapModeSelectionIn == 2) // Auto Mapping
	{
		doMapping();
	}

	else if (mapModeSelectionIn == 3) // Manual Mapping
	{
		ValueTree midiMaps = presetViewed->theMidiSettingsNode.getChildWithName(IDs::midiMapNode);
		midiProcessor->setMidiMaps(midiMaps);
	}

	else // Mapping Off
	{
		midiProcessor->setInputRemap(NoteMap());
		midiProcessor->setOutputFilter(NoteMap());
		setModeViewed(1);
	}

	sendChangeMessage();
}

void SvkPluginState::setMapStyle(int mapStyleIn)
{
    presetViewed->thePropertiesNode.setProperty(IDs::modeMappingStyle, mapStyleIn, nullptr);

	if (isAutoMapping())
	{
		doMapping();
	}
}

void SvkPluginState::setGlobalPitchBendMax(int globalPitchBendMax)
{
    presetViewed->theMidiSettingsNode.setProperty(IDs::pitchBendGlobalMax, globalPitchBendMax, nullptr);
    midiProcessor->setPitchBendGlobalMax(globalPitchBendMax);
}

void SvkPluginState::setNotePitchBendMax(int notePitchBendMax)
{
    presetViewed->theMidiSettingsNode.setProperty(IDs::pitchBendNoteMax, notePitchBendMax, nullptr);
    midiProcessor->setPitchBendGlobalMax(notePitchBendMax);
}

void SvkPluginState::setMaxPolyphony(int maxVoicesIn)
{
    presetViewed->theMidiSettingsNode.setProperty(IDs::maxVoices, maxVoicesIn, nullptr);
    midiProcessor->setVoiceLimit(maxVoicesIn);
}

void SvkPluginState::setRetuneOn(bool toRetuneIn)
{
    presetViewed->theMidiSettingsNode.setProperty(IDs::retuneOn, toRetuneIn, nullptr);
    midiProcessor->setRetuneOn(toRetuneIn);
}

void SvkPluginState::setRetuneAuto(bool toRetuneAutoIn)
{
    presetViewed->theMidiSettingsNode.setProperty(IDs::retuneAuto, toRetuneAutoIn, nullptr);
    autoRetuneOn = toRetuneAutoIn;
    
    if (midiProcessor->isRetuning() && autoRetuneOn)
        setTuningToModeET(2);
}

void SvkPluginState::setRetuneMidiNotePreserved(bool preseveMidiNoteRetune)
{
    presetViewed->theMidiSettingsNode.setProperty(IDs::mpeTuningPreserveMidiNote, preseveMidiNoteRetune, nullptr);
    midiProcessor->setTuningPreservesMidiNote(preseveMidiNoteRetune);
}

void SvkPluginState::setShowNoteNums(bool showNoteNumsIn)
{
    virtualKeyboard->setShowNoteNumbers(showNoteNumsIn);
}

void SvkPluginState::setShowFilteredNoteNums(bool showFilteredNoteNumsIn)
{
    virtualKeyboard->setShowFilteredNumbers(showFilteredNoteNumsIn);
}

void SvkPluginState::setShowNoteLabels(bool showNoteLabelsIn)
{
    virtualKeyboard->setShowNoteLabels(showNoteLabelsIn);
}

void SvkPluginState::setScrollingMode(int modeIn)
{
	//TODO: add to virtualKeyboard
}

void SvkPluginState::setScrollingStyle(int styleIn)
{
    virtualKeyboard->setScrollingStyle(styleIn);
}

void SvkPluginState::setNumKeysInWidth(int numKeysIn)
{
	//TODO: add to virtualKeyboard
}

void SvkPluginState::setNumKeyboardRows(int rowsIn)
{
    virtualKeyboard->setNumRows(rowsIn);
}

void SvkPluginState::setKeyboardOrientation(int orientationIn)
{
    virtualKeyboard->setShowNoteLabels(orientationIn);
}

void SvkPluginState::setKeyStyle(int keyStyleIn)
{
    virtualKeyboard->setKeyStyle(keyStyleIn);
}

void SvkPluginState::setHighlightStyle(int highlightStyleIn)
{
    virtualKeyboard->setHighlightStyle(highlightStyleIn);
}

void SvkPluginState::setVelocityBehavior(int velocityIdIn)
{
    virtualKeyboard->setVelocityBehavior(velocityIdIn);
}

void SvkPluginState::setVelocityScalar(int velocityScalar)
{
	//TODO
	//setParameterValue(IDs::keyboardHighlightStyle, highlightStyleIn);
	//virtualKeyboard->setHighlightStyle(highlightStyleIn);
}

void SvkPluginState::setVelocityFixed(int velocityFixed)
{
    virtualKeyboard->setHighlightStyle(velocityFixed);
}

void SvkPluginState::setModeSlotRoot(int modeSlotIn, int rootNoteIn, bool updateParameter)
{
	Mode* mode = getModeInSlot(modeSlotIn);
	rootNoteIn = totalModulus(rootNoteIn, 128);
	mode->setRootNote(rootNoteIn);

	presetEdited = true;

	// May need some tweaking
	if (getModeViewedNum() == modeSlotIn)
		updateModeViewed();

	if (isAutoMapping())
		doMapping();
}

//==============================================================================

void SvkPluginState::setMidiInputMap(NoteMap noteMapIn)
{
	midiProcessor->setInputRemap(noteMapIn);
}

void SvkPluginState::setMidiOutputMap(NoteMap noteMapIn)
{
	midiProcessor->setOutputFilter(noteMapIn);
}

void SvkPluginState::setModeCustom(String stepsIn)
{
	presetManager->setModeCustom(stepsIn);
	handleModeSelection(getModeViewedNum(), 999);
}

void SvkPluginState::setMapOrder1(int orderIn)
{
    mapOrder1 = orderIn;
    presetViewed->thePropertiesNode.setProperty(IDs::mode1OrderMapping, mapOrder1, nullptr);
}

void SvkPluginState::setMapOrder2(int orderIn)
{
    mapOrder2 = orderIn;
    presetViewed->thePropertiesNode.setProperty(IDs::mode2OrderMapping, mapOrder2, nullptr);
}

void SvkPluginState::setMapOrderOffset1(int offsetIn)
{
    mapOrderOffset1 = offsetIn;
    presetViewed->thePropertiesNode.setProperty(IDs::mode1OrderOffsetMapping, mapOrderOffset1, nullptr);
}

void SvkPluginState::setMapOrderOffset2(int offsetIn)
{
    mapOrderOffset2 = offsetIn;
    presetViewed->thePropertiesNode.setProperty(IDs::mode2OrderOffsetMapping, mapOrderOffset2, nullptr);
}

void SvkPluginState::doMapping(const Mode* mode1, const Mode* mode2, int mappingType,
                               int mode1OrderIn, int mode2OrderIn, int mode1OrderOffsetIn, int mode2OrderOffsetIn)
{
    virtualKeyboard->allNotesOff();

    NoteMap noteMap = modeMapper->map(*mode1, *mode2, mappingType,
                                      mode1OrderIn, mode2OrderIn, mode1OrderOffsetIn, mode2OrderOffsetIn,
                                      *midiProcessor->getInputNoteRemap());

    setMidiInputMap(noteMap);
    sendMappingToKeyboard();
}

void SvkPluginState::doMapping()
{
	if (getMappingMode() > 1)
	{
		DBG("Mapping new settings");
		doMapping(getMode1(), getMode2(), getMappingMode(), mapOrder1, mapOrder2, mapOrderOffset1, mapOrderOffset2);
	}
	else
	{
		midiProcessor->setInputRemap(NoteMap());
		sendMappingToKeyboard();
	}
}

void SvkPluginState::setTuning(const Tuning* tuningToCopy)
{
	tuning.reset(new Tuning(*tuningToCopy));
	midiProcessor->setTuning(tuning.get());
}

void SvkPluginState::setTuningToET(double period, double divisions)
{
	tuning.reset(new Tuning(period, divisions));
	midiProcessor->setTuning(tuning.get());
}

void SvkPluginState::setTuningToModeET(double period)
{
    tuning.reset(new Tuning(period, getMode2()->getScaleSize()));
    midiProcessor->setTuning(tuning.get());
}


//==============================================================================

void SvkPluginState::sendMappingToKeyboard()
{
	virtualKeyboard->setInputNoteMap(midiProcessor->getInputNoteRemap());
}

void SvkPluginState::sendMappingToKeyboard(ValueTree mapNodeIn)
{
	midiProcessor->setMidiMaps(mapNodeIn);
	sendMappingToKeyboard();
}

void SvkPluginState::updateModeViewed(bool sendChange)
{
	modeViewed = presetManager->getModeInSlots(getPresetSlotNumViewed(), getModeViewedNum());

	midiProcessor->setModeViewed(modeViewed);
	virtualKeyboard->applyMode(modeViewed);
    virtualKeyboard->resized();

	//TODO: Change VirtualKeyboard displayed keyboard state
}

void SvkPluginState::commitModeInfo()
{
	if (modeViewed == presetManager->getModeCustom())
	{
		// what i need to do here is to make it so that custom modes don't get added to slots
		// when automation is implemented, the custom mode will be accessible with value 0
		// this might also make menu related (selection id) things easier
		DBG("Custom mode edited:" + presetManager->getModeCustom()->modeNode.toXmlString());
	}

	presetManager->refreshModeSlot(getPresetSlotNumViewed());
	updateModeViewed();
	doMapping();
}

void SvkPluginState::commitParametersToPreset()
{
    presetViewed->theKeyboardNode.copyPropertiesAndChildrenFrom(pianoNode, nullptr);

	presetViewed->parentNode.removeChild(presetViewed->parentNode.getChildWithName(IDs::modeCustomNode), nullptr);
	ValueTree customModeNode = ValueTree(IDs::modeCustomNode);
	customModeNode.addChild(presetManager->getModeCustom()->modeNode.createCopy(), -1, nullptr);
	presetViewed->parentNode.addChild(customModeNode, -1, nullptr);

	if (getParameterValue(IDs::mappingMode) < 3)
	{
		presetViewed->theMidiSettingsNode.removeChild(midiSettingsNode.getChildWithName(IDs::midiMapNode), nullptr);
	}
    
    presetManager->commitPreset(getPresetSlotNumViewed(), presetViewed->parentNode);
    
    ValueTree presetChild = pluginStateNode.getChildWithName(IDs::presetNode);
    if (presetChild.isValid())
    {
        presetChild.copyPropertiesAndChildrenFrom(presetViewed->parentNode, nullptr);
    }
    else
    {
        pluginStateNode.addChild(presetViewed->parentNode, -1, nullptr);

    }
    
    presetEdited = false;
}

bool SvkPluginState::savePresetViewedToFile()
{
    commitParametersToPreset();
    return presetManager->savePresetToFile(getPresetSlotNumViewed(), pluginSettings->getPresetPath());
}

bool SvkPluginState::loadPresetFromFile(bool replaceViewed)
{
    // multi preset loading not yet implemented
	int slotNumber = getPresetSlotNumViewed();

	if (!replaceViewed)
		slotNumber = presetManager->getNumPresetsLoaded();

	ValueTree presetLoaded = presetManager->presetFromFile(pluginSettings->getPresetPath());

	recallState(presetLoaded);

	return true;
}

bool SvkPluginState::saveModeViewedToFile()
{
	int modeSlotNumber = modeViewed ? 
		presetViewed->getMode2SlotNumber() : presetViewed->getMode1SlotNumber();

	return presetManager->saveModeToFile(getPresetSlotNumViewed(), modeSlotNumber, pluginSettings->getModePath());
}

bool SvkPluginState::loadModeFromFile()
{
	ValueTree modeNode = presetManager->nodeFromFile("Open Mode", "*.svk", pluginSettings->getModePath());

	if (Mode::isValidMode(modeNode))
	{
		presetManager->addSlotAndSetSelection(getPresetSlotNumViewed(), getModeViewedNum(), modeNode);
		updateModeViewed();
		doMapping();
		return true;
	}

	return false;
}

//==============================================================================

void SvkPluginState::parameterChanged(const String& paramID, float newValue)
{
    DBG("The parameter " + paramID + " has changed to " + String(newValue));
}

void SvkPluginState::changeListenerCallback(ChangeBroadcaster* source)
{
	// Preset loaded
	if (source == presetManager.get())
	{
        resetToPreset();
	}
}
