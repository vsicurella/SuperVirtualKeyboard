/*
  ==============================================================================

    PluginState.cpp
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PluginState.h"

SvkPluginState::SvkPluginState(ValueTree svkValueTreeIn, SvkParameters* svkParametersIn, const Array<Identifier>* paramIDsIn)
    : pluginStateNode(svkValueTreeIn), svkParameters(svkParametersIn), svkParameterIDs(paramIDsIn)
{
    modeMapper.reset(new ModeMapper());

	pluginStateNode = ValueTree(IDs::pluginStateNode);

	pluginSettings.reset(new SvkPluginSettings());
	pluginSettingsNode = pluginSettings->pluginSettingsNode;
	pluginStateNode.addChild(pluginSettingsNode, -1, nullptr);
    
    midiProcessor.reset(new SvkMidiProcessor());
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
	if (nodeIn.isValid())
	{
		ValueTree childNode = nodeIn.getChildWithName(IDs::presetNode);

		if (nodeIn.hasType(IDs::presetNode))
		{
			presetManager->loadPreset(0, nodeIn, false);
		}
		else if (childNode.hasType(IDs::presetNode))
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
			pluginEditorNode = childNode.createCopy();

		updateToPreset();
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
}

void SvkPluginState::updateToPreset(bool sendChange)
{
	presetEdited = false;

	presetViewed = presetManager->getPresetLoaded(getPresetSlotNumViewed());
    
	svkParameters->grab(IDs::modeSlotNumViewed)->setValue((int)presetViewed->thePropertiesNode[IDs::modeSlotNumViewed]);
	svkParameters->grab(IDs::mappingMode)->setValue((int)presetViewed->thePropertiesNode[IDs::mappingMode]);
	svkParameters->grab(IDs::modeMappingStyle)->setValue((int)presetViewed->thePropertiesNode[IDs::modeMappingStyle]);
    
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
	virtualKeyboard->setMidiChannelOut(midiProcessor->getMidiChannelOut());
    virtualKeyboard->applyMode(modeViewed);

	doMapping();
    
	updateParameters(); // may want to make a different function or pass in a list of params

	if (sendChange)
		sendChangeMessage();
}

void SvkPluginState::updateParameter(Identifier paramId)
{
//	if (paramId == IDs::keyboardMidiChannel)
//	{
//		svkParameters->grab(IDs::keyboardMidiChannel)->setValue(midiProcessor->getMidiChannelOut());
//	}
//	else if (paramId == IDs::pianoKeysShowNoteNumbers)
//	{
//		svkParameters->grab(IDs::pianoKeysShowNoteNumbers)->setValue(virtualKeyboard->isShowingNoteNumbers());
//	}
//	else if (paramId == IDs::pianoKeysShowFilteredNotes)
//	{
//		svkParameters->grab(IDs::pianoKeysShowFilteredNotes)->setValue(virtualKeyboard->isShowingFilteredNumbers());
//	}
//	else if (paramId == IDs::pianoKeyShowNoteLabel)
//	{
//		svkParameters->grab(IDs::pianoKeyShowNoteLabel)->setValue(virtualKeyboard->isShowingNoteNames());
//	}
//	else if (paramId == IDs::keyboardOrientation)
//	{
//		svkParameters->grab(IDs::keyboardOrientation)->setValue(virtualKeyboard->getOrientation());
//	}
//	else if (paramId == IDs::keyboardKeysStyle)
//	{
//		svkParameters->grab(IDs::keyboardKeysStyle)->setValue(virtualKeyboard->getKeyPlacementStyle());
//	}
//	else if (paramId == IDs::keyboardHighlightStyle)
//	{
//		svkParameters->grab(IDs::keyboardHighlightStyle)->setValue(virtualKeyboard->getHighlightStyle());
//	}
//	else if (paramId == IDs::pianoVelocityBehavior)
//	{
//		svkParameters->grab(IDs::pianoVelocityBehavior)->setValue(virtualKeyboard->getVelocityStyle());
//	}
//	else if (paramId == IDs::pianoVelocityValue)
//	{
//		svkParameters->grab(IDs::pianoVelocityValue)->setValue((int)(virtualKeyboard->getVelocityFixed() * 127));
//	}
//	else if (paramId == IDs::pianoWHRatio)
//	{
//		svkParameters->grab(IDs::pianoWHRatio)->setValue(virtualKeyboard->getKeySizeRatio());
//	}
}

void SvkPluginState::updateParameters()
{
//	for (SvkParameters::TheHash::Iterator param(svkParameters); param.next();)
//	{
//		updateParameter(param.getKey());
//	}
}

void SvkPluginState::updateFromParameter(int paramInd)
{
//    if (paramInd < 0 || paramInd >= svkParameterIDs.size())
//        return;
//
//    if (svkParameterIDs[paramInd] == IDs::mappingMode)
//    {
//        setMapMode(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::modeMappingStyle)
//    {
//        setMapStyle(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::keyboardMidiChannel)
//    {
//        setMidiChannel(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::pianoKeysShowNoteNumbers)
//    {
//        setShowNoteNums(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::pianoKeysShowFilteredNotes)
//    {
//        setShowFilteredNoteNums(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::pianoKeyShowNoteLabel)
//    {
//        setShowNoteLabels(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::keyboardOrientation)
//    {
//        setKeyboardOrientation(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::keyboardKeysStyle)
//    {
//        setKeyStyle(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::keyboardHighlightStyle)
//    {
//        setHighlightStyle(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::pianoVelocityBehavior)
//    {
//        setVelocityBehavior(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::pianoVelocityValue)
//    {
//        setVelocityFixed(getParameterValue(paramInd));
//    }
//    else if (svkParameterIDs[paramInd] == IDs::pianoWHRatio)
//    {
//        virtualKeyboard->setKeySizeRatio(getParameterValue(paramInd));
//    }
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

float SvkPluginState::getParameterValue(int paramInd)
{
   // return getParameterValue(svkParameterIDs[paramInd]);
}

float SvkPluginState::getParameterValue(Identifier paramId)
{
	RangedAudioParameter* param = svkParameters->grab(paramId);
	
	if (param)
		return param->convertFrom0to1(param->getValue());
	
	return 0.0f;
}

float SvkPluginState::getParameterMin(Identifier paramId)
{
	RangedAudioParameter* param = svkParameters->grab(paramId);

	if (param)
		return param->convertFrom0to1(0.0f);

	return 0.0f;
}

float SvkPluginState::getParameterMax(Identifier paramId)
{
	RangedAudioParameter* param = svkParameters->grab(paramId);

	if (param)
		return param->convertFrom0to1(1.0f);

	return 0.0f;
}

float SvkPluginState::getParameterDefaultValue(Identifier paramId)
{
	RangedAudioParameter* param = svkParameters->grab(paramId);

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
	return getParameterValue(IDs::modeSlotNumViewed);
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
	RangedAudioParameter* param = svkParameters->grab(paramIdIn);

	if (param)
	{
		param->setValue(valueIn);
	}
}

void SvkPluginState::setPresetViewed(int presetViewedIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::presetSlotViewed)->setValue(presetViewedIn);
    
    presetViewed = presetManager->getPresetLoaded(presetViewedIn);

	midiProcessor->setMode1(getMode1());
	midiProcessor->setMode2(getMode2());
}

void SvkPluginState::setModeViewed(int modeViewedIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::modeSlotNumViewed)->setValue(modeViewedIn);
	
    presetViewed->thePropertiesNode.setProperty(IDs::modeSlotNumViewed, modeViewedIn, nullptr);
	updateModeViewed();
}

void SvkPluginState::handleModeSelection(int modeBoxNum, int idIn, bool updateParameter)
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

void SvkPluginState::setMapMode(int mapModeSelectionIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::mappingMode)->setValue(mapModeSelectionIn);
	
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

void SvkPluginState::setMapStyle(int mapStyleIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::modeMappingStyle)->setValue(mapStyleIn);
	
    presetViewed->thePropertiesNode.setProperty(IDs::modeMappingStyle, mapStyleIn, nullptr);

	if (isAutoMapping())
	{
		doMapping();
	}
}

void SvkPluginState::setMPEOn(bool mpeOnIn, bool updateParameter)
{
    if (updateParameter)
        setParameterValue(IDs::mpeOn, mpeOnIn);
    
    presetViewed->theMidiSettingsNode.setProperty(IDs::mpeOn, mpeOnIn, nullptr);
    
    midiProcessor->setMPEOn(mpeOnIn);
}

void SvkPluginState::setMPELegacy(bool mpeLegacyIn, bool updateParameter)
{
    if (updateParameter)
        setParameterValue(IDs::mpeLegacyOn, mpeLegacyIn);
    
    presetViewed->theMidiSettingsNode.setProperty(IDs::mpeLegacyOn, mpeLegacyIn, nullptr);
    
    if (mpeLegacyIn)
        midiProcessor->getMPEInstrument()->enableLegacyMode();
    else
        midiProcessor->setMPEOn(true); // bad if called when MPE is actually off
    
}

void SvkPluginState::setGlobalPitchBendMax(int globalPitchBendMax, bool updateParameter)
{
    if (updateParameter)
        setParameterValue(IDs::pitchBendGlobalMax, globalPitchBendMax);
    
    presetViewed->theMidiSettingsNode.setProperty(IDs::pitchBendGlobalMax, globalPitchBendMax, nullptr);
    midiProcessor->setPitchBendGlobalMax(globalPitchBendMax);
}

void SvkPluginState::setNotePitchBendMax(int notePitchBendMax, bool updateParameter)
{
    if (updateParameter)
        setParameterValue(IDs::pitchBendNoteMax, notePitchBendMax);
    
    presetViewed->theMidiSettingsNode.setProperty(IDs::pitchBendNoteMax, notePitchBendMax, nullptr);
    midiProcessor->setPitchBendGlobalMax(notePitchBendMax);
}

void SvkPluginState::setMaxPolyphony(int maxVoicesIn, bool updateParameter)
{
    if (updateParameter)
        setParameterValue(IDs::maxVoices, maxVoicesIn);
    
    presetViewed->theMidiSettingsNode.setProperty(IDs::maxVoices, maxVoicesIn, nullptr);
    midiProcessor->setVoiceLimit(maxVoicesIn);
}

void SvkPluginState::setRetuneOn(bool toRetuneIn, bool updateParameter)
{
    if (updateParameter)
        setParameterValue(IDs::retuneOn, toRetuneIn);
    
    presetViewed->theMidiSettingsNode.setProperty(IDs::retuneOn, toRetuneIn, nullptr);
    midiProcessor->setRetuneOn(toRetuneIn);
}

void SvkPluginState::setRetuneAuto(bool toRetuneAutoIn, bool updateParameter)
{
    if (updateParameter)
        setParameterValue(IDs::retuneAuto, toRetuneAutoIn);
    
    presetViewed->theMidiSettingsNode.setProperty(IDs::retuneAuto, toRetuneAutoIn, nullptr);
    autoRetuneOn = toRetuneAutoIn;
    
    if (midiProcessor->isRetuning() && autoRetuneOn)
        setTuningToModeET(2);
}

void SvkPluginState::setRetuneMidiNotePreserved(bool preseveMidiNoteRetune, bool updateParameter)
{
    if (updateParameter)
        setParameterValue(IDs::mpeTuningPreserveMidiNote, preseveMidiNoteRetune);
    
    presetViewed->theMidiSettingsNode.setProperty(IDs::mpeTuningPreserveMidiNote, preseveMidiNoteRetune, nullptr);
    midiProcessor->setTuningPreservesMidiNote(preseveMidiNoteRetune);
}

void SvkPluginState::setPeriodShift(int shiftIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::periodShift)->setValue(shiftIn);
	
    midiProcessor->setPeriodShift(shiftIn);
}

void SvkPluginState::setTransposeAmt(int stepsIn, bool updateParameter)
{
    if (updateParameter)
        setParameterValue(IDs::transposeAmt, stepsIn);
    
    midiProcessor->setTransposeAmt(stepsIn);
}

void SvkPluginState::setMidiChannel(int midiChannelIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::keyboardMidiChannel)->setValue(midiChannelIn);
	
    // Can be improved, don't think this needs to be set in both objects
	midiProcessor->setMidiChannelOut(midiChannelIn);
	virtualKeyboard->setMidiChannelOut(midiChannelIn);
}

void SvkPluginState::setShowNoteNums(bool showNoteNumsIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::pianoKeysShowNoteNumbers)->setValue(showNoteNumsIn);
	
    virtualKeyboard->setShowNoteNumbers(showNoteNumsIn);
}

void SvkPluginState::setShowFilteredNoteNums(bool showFilteredNoteNumsIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::pianoKeysShowFilteredNotes)->setValue(showFilteredNoteNumsIn);
	
    virtualKeyboard->setShowFilteredNumbers(showFilteredNoteNumsIn);
}

void SvkPluginState::setShowNoteLabels(bool showNoteLabelsIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::keyboardShowsNoteLabels)->setValue(showNoteLabelsIn);
	
    virtualKeyboard->setShowNoteLabels(showNoteLabelsIn);
}

void SvkPluginState::setScrollingMode(int modeIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::keyboardScrollingMode)->setValue(modeIn);
	//TODO: add to virtualKeyboard
}

void SvkPluginState::setScrollingStyle(int styleIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::keyboardScrollingStyle)->setValue(styleIn);
	
    virtualKeyboard->setScrollingStyle(styleIn);
}

void SvkPluginState::setNumKeysInWidth(int numKeysIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::numKeysInWidth)->setValue(numKeysIn);
	//TODO: add to virtualKeyboard
}

void SvkPluginState::setNumKeyboardRows(int rowsIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::keyboardNumRows)->setValue(rowsIn);
	
    virtualKeyboard->setNumRows(rowsIn);
}

void SvkPluginState::setKeyboardOrientation(int orientationIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::keyboardOrientation)->setValue(orientationIn);
	
    virtualKeyboard->setShowNoteLabels(orientationIn);
}

void SvkPluginState::setKeyStyle(int keyStyleIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::keyboardKeysStyle)->setValue(keyStyleIn);
	
    virtualKeyboard->setKeyStyle(keyStyleIn);
}

void SvkPluginState::setHighlightStyle(int highlightStyleIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::keyboardHighlightStyle)->setValue(highlightStyleIn);
	
    virtualKeyboard->setHighlightStyle(highlightStyleIn);
}

void SvkPluginState::setVelocityBehavior(int velocityIdIn, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::pianoVelocityBehavior)->setValue(velocityIdIn);
	
    virtualKeyboard->setVelocityBehavior(velocityIdIn);
}

void SvkPluginState::setVelocityScalar(int velocityScalar, bool updateParameter)
{
	//TODO
	//svkParameters->grab(IDs::keyboardHighlightStyle)->setValue(highlightStyleIn);
	//virtualKeyboard->setHighlightStyle(highlightStyleIn);
}

void SvkPluginState::setVelocityFixed(int velocityFixed, bool updateParameter)
{
    if (updateParameter)
        svkParameters->grab(IDs::pianoVelocityValue)->setValue(velocityFixed);
	
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

void SvkPluginState::setMode1Root(int rootNoteIn, bool updateParameter)
{
	setModeSlotRoot(0, rootNoteIn, updateParameter);
}

void SvkPluginState::setMode2Root(int rootNoteIn, bool updateParameter)
{
	setModeSlotRoot(1, rootNoteIn, updateParameter);
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
	svkParameters->grab(IDs::modeSlotNumViewed)->setValue(getModeViewedNum());

	modeViewed = presetManager->getModeInSlots(getPresetSlotNumViewed(), getModeViewedNum());

	midiProcessor->setModeViewed(modeViewed);
	virtualKeyboard->applyMode(modeViewed);

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

void SvkPluginState::commitPresetChanges()
{
	presetViewed->parentNode.removeChild(presetViewed->theKeyboardNode, nullptr);
	presetViewed->theKeyboardNode = pianoNode;
	presetViewed->parentNode.addChild(presetViewed->theKeyboardNode, -1, nullptr);

	presetViewed->parentNode.removeChild(presetViewed->parentNode.getChildWithName(IDs::modeCustomNode), nullptr);
	ValueTree customModeNode = ValueTree(IDs::modeCustomNode);
	customModeNode.addChild(presetManager->getModeCustom()->modeNode, -1, nullptr);
	presetViewed->parentNode.addChild(customModeNode, -1, nullptr);

	if (isAutoMapping())
	{
		midiSettingsNode.removeChild(midiSettingsNode.getChildWithName(IDs::midiMapNode), nullptr);
	}

    presetManager->commitPreset(getPresetSlotNumViewed(), presetViewed->parentNode);

	pluginStateNode.removeChild(pluginStateNode.getChildWithName(IDs::presetNode), nullptr);
	pluginStateNode.addChild(presetViewed->parentNode, -1, nullptr);

    presetEdited = false;
}

bool SvkPluginState::savePresetViewedToFile()
{
    commitPresetChanges();
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

void SvkPluginState::changeListenerCallback(ChangeBroadcaster* source)
{
	// Preset loaded
	if (source == presetManager.get())
	{
        updateToPreset();
	}
}
