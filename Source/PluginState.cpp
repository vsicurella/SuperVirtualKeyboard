/*
  ==============================================================================

    PluginState.cpp
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PluginState.h"

SvkPluginState::SvkPluginState()
{
    appCmdMgr.reset(new ApplicationCommandManager());
    undoManager.reset(new UndoManager());
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
    
    initializeParameters();
    
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

void SvkPluginState::initializeParameters()
{
    svkParameters.stash(IDs::presetSlotViewed, new AudioParameterInt(IDs::presetSlotViewed.toString(),"Preset Slot Viewed", 0, 1, 0));
    svkParameters.stash(IDs::modeSlotNumViewed, new AudioParameterInt(IDs::modeSlotNumViewed.toString(), "Mode Slot Viewed", 0, 1, 0));
    
    svkParameters.stash(IDs::mappingMode, new AudioParameterInt(IDs::mappingMode.toString(), "Mapping Mode", 1, 3, 1));
    svkParameters.stash(IDs::modeMappingStyle, new AudioParameterInt(IDs::modeMappingStyle.toString(), "Mapping Style", 1, 3, 1));
    
    svkParameters.stash(IDs::mpeOn, new AudioParameterBool(IDs::mpeOn.toString(), "MPE On", false));
    svkParameters.stash(IDs::mpeLegacyOn, new AudioParameterBool(IDs::mpeLegacyOn.toString(), "MPE Legacy Mode", false));
    svkParameters.stash(IDs::mpeTuningPreserveMidiNote, new AudioParameterBool(IDs::mpeTuningPreserveMidiNote.toString(), "Preserve Midi Notes", true));
    svkParameters.stash(IDs::pitchBendGlobalMax, new AudioParameterInt(IDs::pitchBendGlobalMax.toString(), "Global Pitch Bend", 0, 8192, 2));
    svkParameters.stash(IDs::pitchBendNoteMax, new AudioParameterInt(IDs::pitchBendNoteMax.toString(), "Note Pitch Bend", 0, 8192, 48));
    svkParameters.stash(IDs::maxVoices, new AudioParameterInt(IDs::maxVoices.toString(), "Max Polyphony", 1, 16, 15));

    svkParameters.stash(IDs::retuneOn, new AudioParameterBool(IDs::retuneOn.toString(), "Retune On", false));
    svkParameters.stash(IDs::retuneAuto, new AudioParameterBool(IDs::retuneAuto.toString(), "Retune Auto", false));
    svkParameters.stash(IDs::tuningRootNote, new AudioParameterInt(IDs::tuningRootNote.toString(), "Tuning Reference Note", 0, 127, 69));
    svkParameters.stash(IDs::tuningRootFreq, new AudioParameterFloat(IDs::tuningRootFreq.toString(), "Tuning Reference Frequency", 0.01f, 24000.0f, 440.0f));

    svkParameters.stash(IDs::periodShift, new AudioParameterInt(IDs::periodShift.toString(), "Period Shift", -10, 10, 0));
    svkParameters.stash(IDs::transposeAmt, new AudioParameterInt(IDs::transposeAmt.toString(), "Transpose", -127, 127, 0));
    svkParameters.stash(IDs::keyboardMidiChannel, new AudioParameterInt(IDs::keyboardMidiChannel.toString(), "Midi Channel Out", 1, 16, 1));
    svkParameters.stash(IDs::pianoKeysShowNoteNumbers, new AudioParameterBool(IDs::pianoKeysShowNoteNumbers.toString(), "Show Note Numbers", false));
    svkParameters.stash(IDs::pianoKeysShowFilteredNotes, new AudioParameterBool(IDs::pianoKeysShowFilteredNotes.toString(), "Show Filtered Numbers", false));
    svkParameters.stash(IDs::pianoKeyShowNoteLabel, new AudioParameterBool(IDs::pianoKeyShowNoteLabel.toString(), "Show Pitch Names", false));
    svkParameters.stash(IDs::keyboardScrollingMode, new AudioParameterInt(IDs::keyboardScrollingMode.toString(), "Scrolling Mode", 0, 3, 1));
    svkParameters.stash(IDs::keyboardScrollingStyle, new AudioParameterInt(IDs::keyboardScrollingStyle.toString(), "Scrolling Style", 0, 3, 1));
    svkParameters.stash(IDs::numKeysInWidth, new AudioParameterInt(IDs::numKeysInWidth.toString(), "Num Keys in Width", 1, 128, 16));
    svkParameters.stash(IDs::keyboardNumRows, new AudioParameterInt(IDs::keyboardNumRows.toString(), "Keyboard Rows", 1, 16, 1));
    svkParameters.stash(IDs::keyboardOrientation, new AudioParameterInt(IDs::keyboardOrientation.toString(), "Keyboard Orientation", 0, 3, 0));
    svkParameters.stash(IDs::keyboardKeysStyle, new AudioParameterInt(IDs::keyboardKeysStyle.toString(), "Key Style", 1, 4, 1));
    svkParameters.stash(IDs::keyboardHighlightStyle, new AudioParameterInt(IDs::keyboardHighlightStyle.toString(), "Highlight Style", 1, 4, 1));
    
    svkParameters.stash(IDs::pianoVelocityBehavior, new AudioParameterInt(IDs::pianoVelocityBehavior.toString(), "Keyboard Velocity Mode", 0, 2, 0));
    svkParameters.stash(IDs::pianoVelocityValue, new AudioParameterInt(IDs::pianoVelocityValue.toString(), "Default Velocity", 0, 127, 100));
    svkParameters.stash(IDs::pianoWHRatio, new AudioParameterFloat(IDs::pianoWHRatio.toString(), "Key Size Ratio", 0.01f, 1.62f, 0.25f));
    
    svkParameters.stash(IDs::modeSlotToEdit, new AudioParameterInt(IDs::modeSlotToEdit.toString(), "Mode Slot Debug", 0, 1, 0));
    svkParameters.stash(IDs::modeLibraryIndex, new AudioParameterInt(IDs::modeLibraryIndex.toString(), "Mode Debug Index", 0, 1, 0));
    svkParameters.stash(IDs::modeRootNote, new AudioParameterInt(IDs::modeRootNote.toString(), "Mode Debug Root", 0, 127, 60));
    
    svkParameters.stash(IDs::keyNumberToEdit, new AudioParameterInt(IDs::keyNumberToEdit.toString(), "Key To Debug", 0, 127, 60));
    svkParameters.stash(IDs::pianoKeyWidthMod, new AudioParameterFloat(IDs::pianoKeyWidthMod.toString(), "Key Debug Width Mod", 0.001f, 10.0f, 1.0f));
    svkParameters.stash(IDs::pianoKeyHeightMod, new AudioParameterFloat(IDs::pianoKeyHeightMod.toString(), "Key Debug Height Mod", 0.001f, 10.0f, 1.0f));
    svkParameters.stash(IDs::pianoKeyXOffset, new AudioParameterInt(IDs::pianoKeyXOffset.toString(), "Key Debug X Offset", -1000, 1000, 0));
    svkParameters.stash(IDs::pianoKeyYOffset, new AudioParameterInt(IDs::pianoKeyYOffset.toString(), "Key Debug Y Offset", -1000, 1000, 0));
}

void SvkPluginState::updateToPreset(bool sendChange)
{
	presetEdited = false;

	presetViewed = presetManager->getPresetLoaded(getPresetSlotNumViewed());
    
	svkParameters.grab(IDs::modeSlotNumViewed)->setValue((int)presetViewed->thePropertiesNode[IDs::modeSlotNumViewed]);
	svkParameters.grab(IDs::mappingMode)->setValue((int)presetViewed->thePropertiesNode[IDs::mappingMode]);
	svkParameters.grab(IDs::modeMappingStyle)->setValue((int)presetViewed->thePropertiesNode[IDs::modeMappingStyle]);
    
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
	if (paramId == IDs::keyboardMidiChannel)
	{
		svkParameters.grab(IDs::keyboardMidiChannel)->setValue(midiProcessor->getMidiChannelOut());
	}
	else if (paramId == IDs::pianoKeysShowNoteNumbers)
	{
		svkParameters.grab(IDs::pianoKeysShowNoteNumbers)->setValue(virtualKeyboard->isShowingNoteNumbers());
	}
	else if (paramId == IDs::pianoKeysShowFilteredNotes)
	{
		svkParameters.grab(IDs::pianoKeysShowFilteredNotes)->setValue(virtualKeyboard->isShowingFilteredNumbers());
	}
	else if (paramId == IDs::pianoKeyShowNoteLabel)
	{
		svkParameters.grab(IDs::pianoKeyShowNoteLabel)->setValue(virtualKeyboard->isShowingNoteNames());
	}
	else if (paramId == IDs::keyboardOrientation)
	{
		svkParameters.grab(IDs::keyboardOrientation)->setValue(virtualKeyboard->getOrientation());
	}
	else if (paramId == IDs::keyboardKeysStyle)
	{
		svkParameters.grab(IDs::keyboardKeysStyle)->setValue(virtualKeyboard->getKeyPlacementStyle());
	}
	else if (paramId == IDs::keyboardHighlightStyle)
	{
		svkParameters.grab(IDs::keyboardHighlightStyle)->setValue(virtualKeyboard->getHighlightStyle());
	}
	else if (paramId == IDs::pianoVelocityBehavior)
	{
		svkParameters.grab(IDs::pianoVelocityBehavior)->setValue(virtualKeyboard->getVelocityStyle());
	}
	else if (paramId == IDs::pianoVelocityValue)
	{
		svkParameters.grab(IDs::pianoVelocityValue)->setValue((int)(virtualKeyboard->getVelocityFixed() * 127));
	}
	else if (paramId == IDs::pianoWHRatio)
	{
		svkParameters.grab(IDs::pianoWHRatio)->setValue(virtualKeyboard->getKeySizeRatio());
	}
}

void SvkPluginState::updateParameters()
{
	for (SvkParameters::TheHash::Iterator param(svkParameters); param.next();)
	{
		updateParameter(param.getKey());
	}
}

void SvkPluginState::updateFromParameter(Identifier paramId)
{
    
}

//==============================================================================

ApplicationCommandManager* SvkPluginState::getAppCmdMgr()
{
	return appCmdMgr.get();
}

UndoManager* SvkPluginState::getUndoManager()
{
	return undoManager.get();
}

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

SvkParameters* SvkPluginState::getParameters()
{
    return &svkParameters;
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
	RangedAudioParameter* param = svkParameters.grab(paramId);
	
	if (param)
		return param->convertFrom0to1(param->getValue());
	
	return 0.0f;
}

float SvkPluginState::getParameterMin(Identifier paramId)
{
	RangedAudioParameter* param = svkParameters.grab(paramId);

	if (param)
		return param->convertFrom0to1(0.0f);

	return 0.0f;
}

float SvkPluginState::getParameterMax(Identifier paramId)
{
	RangedAudioParameter* param = svkParameters.grab(paramId);

	if (param)
		return param->convertFrom0to1(1.0f);

	return 0.0f;
}

float SvkPluginState::getParameterDefaultValue(Identifier paramId)
{
	RangedAudioParameter* param = svkParameters.grab(paramId);

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
	RangedAudioParameter* param = svkParameters.grab(paramIdIn);

	if (param)
	{
		param->setValue(valueIn);
	}
}

void SvkPluginState::setPresetViewed(int presetViewedIn)
{
	svkParameters.grab(IDs::presetSlotViewed)->setValue(presetViewedIn);
    presetViewed = presetManager->getPresetLoaded(presetViewedIn);

	midiProcessor->setMode1(getMode1());
	midiProcessor->setMode2(getMode2());
}

void SvkPluginState::setModeViewed(int modeViewedIn)
{
	svkParameters.grab(IDs::modeSlotNumViewed)->setValue(modeViewedIn);
	presetViewed->thePropertiesNode.setProperty(IDs::modeSlotNumViewed, modeViewedIn, nullptr);
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
	svkParameters.grab(IDs::mappingMode)->setValue(mapModeSelectionIn);
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
	svkParameters.grab(IDs::modeMappingStyle)->setValue(mapStyleIn);
	presetViewed->thePropertiesNode.setProperty(IDs::modeMappingStyle, mapStyleIn, nullptr);

	if (isAutoMapping())
	{
		doMapping();
	}
}

void SvkPluginState::setMPEOn(bool mpeOnIn)
{
    setParameterValue(IDs::mpeOn, mpeOnIn);
    presetViewed->theMidiSettingsNode.setProperty(IDs::mpeOn, mpeOnIn, nullptr);
    
    midiProcessor->setMPEOn(mpeOnIn);
}

void SvkPluginState::setMPELegacy(bool mpeLegacyIn)
{
    setParameterValue(IDs::mpeLegacyOn, mpeLegacyIn);
    presetViewed->theMidiSettingsNode.setProperty(IDs::mpeLegacyOn, mpeLegacyIn, nullptr);
    
    if (mpeLegacyIn)
        midiProcessor->getMPEInstrument()->enableLegacyMode();
    else
        midiProcessor->setMPEOn(true); // bad if called when MPE is actually off
    
}

void SvkPluginState::setGlobalPitchBendMax(int globalPitchBendMax)
{
    setParameterValue(IDs::pitchBendGlobalMax, globalPitchBendMax);
    presetViewed->theMidiSettingsNode.setProperty(IDs::pitchBendGlobalMax, globalPitchBendMax, nullptr);
    midiProcessor->setPitchBendGlobalMax(globalPitchBendMax);
}

void SvkPluginState::setNotePitchBendMax(int notePitchBendMax)
{
    setParameterValue(IDs::pitchBendNoteMax, notePitchBendMax);
    presetViewed->theMidiSettingsNode.setProperty(IDs::pitchBendNoteMax, notePitchBendMax, nullptr);
    midiProcessor->setPitchBendGlobalMax(notePitchBendMax);
}

void SvkPluginState::setMaxPolyphony(int maxVoicesIn)
{
    setParameterValue(IDs::maxVoices, maxVoicesIn);
    presetViewed->theMidiSettingsNode.setProperty(IDs::maxVoices, maxVoicesIn, nullptr);
    midiProcessor->setVoiceLimit(maxVoicesIn);
}

void SvkPluginState::setRetuneOn(bool toRetuneIn)
{
    setParameterValue(IDs::retuneOn, toRetuneIn);
    presetViewed->theMidiSettingsNode.setProperty(IDs::retuneOn, toRetuneIn, nullptr);
    midiProcessor->setRetuneOn(toRetuneIn);
}

void SvkPluginState::setRetuneAuto(bool toRetuneAutoIn)
{
    autoRetuneOn = toRetuneAutoIn;
    
    if (midiProcessor->isRetuning() && autoRetuneOn)
        setTuningToModeET(2);
}

void SvkPluginState::setRetuneMidiNotePreserved(bool preseveMidiNoteRetune)
{
    setParameterValue(IDs::mpeTuningPreserveMidiNote, preseveMidiNoteRetune);
    presetViewed->theMidiSettingsNode.setProperty(IDs::mpeTuningPreserveMidiNote, preseveMidiNoteRetune, nullptr);
    midiProcessor->setTuningPreservesMidiNote(preseveMidiNoteRetune);
}

void SvkPluginState::setPeriodShift(int shiftIn)
{
	svkParameters.grab(IDs::periodShift)->setValue(shiftIn);
	midiProcessor->setPeriodShift(shiftIn);
}

void SvkPluginState::setTransposeAmt(int stepsIn)
{
    setParameterValue(IDs::transposeAmt, stepsIn);
    midiProcessor->setTransposeAmt(stepsIn);
}

void SvkPluginState::setMidiChannel(int midiChannelIn)
{
	svkParameters.grab(IDs::keyboardMidiChannel)->setValue(midiChannelIn);
	// Can be improved, don't think this needs to be set in both objects
	midiProcessor->setMidiChannelOut(midiChannelIn);
	virtualKeyboard->setMidiChannelOut(midiChannelIn);
}

void SvkPluginState::setShowNoteNums(bool showNoteNumsIn)
{
	svkParameters.grab(IDs::pianoKeysShowNoteNumbers)->setValue(showNoteNumsIn);
	virtualKeyboard->setShowNoteNumbers(showNoteNumsIn);
}

void SvkPluginState::setShowFilteredNoteNums(bool showFilteredNoteNumsIn)
{
	svkParameters.grab(IDs::pianoKeysShowFilteredNotes)->setValue(showFilteredNoteNumsIn);
	virtualKeyboard->setShowFilteredNumbers(showFilteredNoteNumsIn);
}

void SvkPluginState::setShowNoteLabels(bool showNoteLabelsIn)
{
	svkParameters.grab(IDs::keyboardShowsNoteLabels)->setValue(showNoteLabelsIn);
	virtualKeyboard->setShowNoteLabels(showNoteLabelsIn);
}

void SvkPluginState::setScrollingMode(int modeIn)
{
	svkParameters.grab(IDs::keyboardScrollingMode)->setValue(modeIn);
	//TODO: add to virtualKeyboard
}

void SvkPluginState::setScrollingStyle(int styleIn)
{
	svkParameters.grab(IDs::keyboardScrollingStyle)->setValue(styleIn);
	virtualKeyboard->setScrollingStyle(styleIn);
}

void SvkPluginState::setNumKeysInWidth(int numKeysIn)
{
	svkParameters.grab(IDs::numKeysInWidth)->setValue(numKeysIn);
	//TODO: add to virtualKeyboard
}

void SvkPluginState::setNumKeyboardRows(int rowsIn)
{
	svkParameters.grab(IDs::keyboardNumRows)->setValue(rowsIn);
	virtualKeyboard->setNumRows(rowsIn);
}

void SvkPluginState::setKeyboardOrientation(int orientationIn)
{
	svkParameters.grab(IDs::keyboardOrientation)->setValue(orientationIn);
	virtualKeyboard->setShowNoteLabels(orientationIn);
}

void SvkPluginState::setKeyStyle(int keyStyleIn)
{
	svkParameters.grab(IDs::keyboardKeysStyle)->setValue(keyStyleIn);
	virtualKeyboard->setKeyStyle(keyStyleIn);
}

void SvkPluginState::setHighlightStyle(int highlightStyleIn)
{
	svkParameters.grab(IDs::keyboardHighlightStyle)->setValue(highlightStyleIn);
	virtualKeyboard->setHighlightStyle(highlightStyleIn);
}

void SvkPluginState::setVelocityBehavior(int velocityIdIn)
{
	svkParameters.grab(IDs::pianoVelocityBehavior)->setValue(velocityIdIn);
	virtualKeyboard->setVelocityBehavior(velocityIdIn);
}

void SvkPluginState::setVelocityScalar(int velocityScalar)
{
	//TODO
	//svkParameters.grab(IDs::keyboardHighlightStyle)->setValue(highlightStyleIn);
	//virtualKeyboard->setHighlightStyle(highlightStyleIn);
}

void SvkPluginState::setVelocityFixed(int velocityFixed)
{
	svkParameters.grab(IDs::pianoVelocityValue)->setValue(velocityFixed);
	virtualKeyboard->setHighlightStyle(velocityFixed);
}

void SvkPluginState::setModeSlotRoot(int modeSlotIn, int rootNoteIn)
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

void SvkPluginState::setMode1Root(int rootNoteIn)
{
	setModeSlotRoot(0, rootNoteIn);
}

void SvkPluginState::setMode2Root(int rootNoteIn)
{
	setModeSlotRoot(1, rootNoteIn);
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
	svkParameters.grab(IDs::modeSlotNumViewed)->setValue(getModeViewedNum());

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
