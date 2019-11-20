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

	virtualKeyboard.reset(new VirtualKeyboard::Keyboard(*midiProcessor->getOriginalKeyboardState(), pianoNode));
	virtualKeyboard->addListener(midiProcessor.get());
	pianoNode = virtualKeyboard->getNode();
    
    midiProcessor->getRemappedKeyboardState()->addListener(virtualKeyboard.get());

	textFilterIntOrSpace.reset(new TextFilterIntOrSpace());
	textFilterInt.reset(new TextFilterInt());
    
    initializeParameters();
    
	setPresetViewed(0);
	modeViewedNum = 1;
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
    
    if (!presetViewed->theKeyboardNode.hasProperty(IDs::pianoKeyPlacementType))
		setKeyStyle(1);
    
    if (!presetViewed->theKeyboardNode.hasProperty(IDs::pianoKeysHighlightStyle))
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
    svkParameters.stash(IDs::presetSlotViewed, new AudioParameterInt(IDs::presetSlotViewed.toString(),
                                            "Preset Slot Viewed",
                                             0, 1, 0));
    svkParameters.stash(IDs::modeSlotNumViewed, new AudioParameterInt(IDs::modeSlotNumViewed.toString(),
                                            "Mode Slot Viewed",
                                             0, 1, 0));
    svkParameters.stash(IDs::modeSlotDebug, new AudioParameterInt(IDs::modeSlotDebug.toString(),
                                            "Mode Slot Debug",
                                             0, 1, 0));
    svkParameters.stash(IDs::modeLibraryIndex, new AudioParameterInt(IDs::modeLibraryIndex.toString(),
                                            "Mode Debug Index",
                                             0, 1, 0));
    svkParameters.stash(IDs::modeRootNote, new AudioParameterInt(IDs::modeRootNote.toString(),
                                            "Mode Debug Root",
                                             0, 127, 60));
    svkParameters.stash(IDs::periodShift, new AudioParameterInt(IDs::periodShift.toString(),
                                            "Period Shift",
                                             -10, 10, 0));
    svkParameters.stash(IDs::pianoMidiChannel, new AudioParameterInt(IDs::pianoMidiChannel.toString(),
                                            "Midi Channel Out",
                                             1, 16, 1));
    svkParameters.stash(IDs::pianoKeysShowNoteNumbers, new AudioParameterBool(IDs::pianoKeysShowNoteNumbers.toString(),
                                            "Show Note Numbers",
                                              false));
    svkParameters.stash(IDs::pianoKeysShowFilteredNotes, new AudioParameterBool(IDs::pianoKeysShowFilteredNotes.toString(),
                                            "Show Filtered Numbers",
                                              false));
    svkParameters.stash(IDs::pianoKeyShowName, new AudioParameterBool(IDs::pianoKeyShowName.toString(),
                                            "Show Pitch Names",
                                              false));
    svkParameters.stash(IDs::pianoOrientation, new AudioParameterInt(IDs::pianoOrientation.toString(),
                                            "Keyboard Orientation",
                                             0, 3, 0));
    svkParameters.stash(IDs::pianoKeyPlacementType, new AudioParameterInt(IDs::pianoKeyPlacementType.toString(),
                                            "Key Style",
                                             0, 4, 0));
    svkParameters.stash(IDs::pianoKeysHighlightStyle, new AudioParameterInt(IDs::pianoKeysHighlightStyle.toString(),
                                            "Highlight Style",
                                             0, 4, 0));
    svkParameters.stash(IDs::pianoKeysHighlightStyle, new AudioParameterInt(IDs::mappingMode.toString(),
                                            "Mapping Mode",
                                             0, 2, 0));
    svkParameters.stash(IDs::modeMappingStyle, new AudioParameterInt(IDs::modeMappingStyle.toString(),
                                            "Mapping Style",
                                             0, 2, 0));
    svkParameters.stash(IDs::pianoVelocityBehavior, new AudioParameterInt(IDs::pianoVelocityBehavior.toString(),
                                            "Keyboard Velocity Mode",
                                             0, 2, 0));
    svkParameters.stash(IDs::pianoVelocityValue, new AudioParameterInt(IDs::pianoVelocityValue.toString(),
                                            "Default Velocity",
                                             0, 127, 100));
    svkParameters.stash(IDs::pianoWHRatio, new AudioParameterFloat(IDs::pianoWHRatio.toString(),
                                            "Key Size Ratio",
                                             0.001f, 10.0f, 0.25f));
    svkParameters.stash(IDs::keyNumberDebug, new AudioParameterInt(IDs::keyNumberDebug.toString(),
                                            "Key To Debug",
                                             0, 127, 60));
    svkParameters.stash(IDs::pianoKeyWidthMod, new AudioParameterFloat(IDs::pianoKeyWidthMod.toString(),
                                            "Key Debug Width Mod",
                                             0.001f, 10.0f, 1.0f));
    svkParameters.stash(IDs::pianoKeyHeightMod, new AudioParameterFloat(IDs::pianoKeyHeightMod.toString(),
                                            "Key Debug Height Mod",
                                             0.001f, 10.0f, 1.0f));
    svkParameters.stash(IDs::pianoKeyXOffset, new AudioParameterInt(IDs::pianoKeyXOffset.toString(),
                                            "Key Debug X Offset",
                                             -1000, 1000, 0));
    svkParameters.stash(IDs::pianoKeyYOffset, new AudioParameterInt(IDs::pianoKeyYOffset.toString(),
                                            "Key Debug Y Offset",
                                             -1000, 1000, 0));
    
    for (int i = 0; i < svkParameters.getSize(); i++)
    {
        svkParameters.getUnchecked(i)->addListener(this);
    }
}

void SvkPluginState::updateToPreset(bool sendChange)
{
	presetEdited = false;

	presetViewed = presetManager->getPresetLoaded(presetSlotNumViewed);
    
	modeViewedNum = (int) presetViewed->thePropertiesNode[IDs::modeSlotNumViewed];
	mapModeSelected = (int) presetViewed->thePropertiesNode[IDs::mappingMode];
	mapStyleSelected = (int) presetViewed->thePropertiesNode[IDs::modeMappingStyle];
    
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
    
    // update parameters
    svkParameters.grab(IDs::presetSlotViewed)->setValue(presetSlotNumViewed);
    svkParameters.grab(IDs::modeMappingStyle)->setValue(mapStyleSelected);
    svkParameters.grab(IDs::modeMappingStyle)->setValue(mapStyleSelected);
    svkParameters.grab(IDs::pianoMidiChannel)->setValue(midiProcessor->getMidiChannelOut());
    svkParameters.grab(IDs::pianoKeysShowNoteNumbers)->setValue(virtualKeyboard->isShowingNoteNumbers());
    svkParameters.grab(IDs::pianoKeysShowFilteredNotes)->setValue(virtualKeyboard->isShowingFilteredNumbers());
    svkParameters.grab(IDs::pianoKeyShowName)->setValue(virtualKeyboard->isShowingNoteNames());
    svkParameters.grab(IDs::pianoOrientation)->setValue(virtualKeyboard->getOrientation());
    svkParameters.grab(IDs::pianoKeyPlacementType)->setValue(virtualKeyboard->getKeyPlacementStyle());
    svkParameters.grab(IDs::pianoKeysHighlightStyle)->setValue(virtualKeyboard->getHighlightStyle());
    svkParameters.grab(IDs::pianoVelocityBehavior)->setValue(virtualKeyboard->getVelocityStyle());
    svkParameters.grab(IDs::pianoVelocityValue)->setValue((int) virtualKeyboard->getVelocityFixed() * 127);
    svkParameters.grab(IDs::pianoWHRatio)->setValue(virtualKeyboard->getKeySizeRatio());

	if (sendChange)
		sendChangeMessage();
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

SvkParameters* SvkPluginState::getParameters()
{
    return &svkParameters;
}

ApplicationCommandManager* SvkPluginState::getAppCmdMgr()
{
	return appCmdMgr.get();
}

UndoManager* SvkPluginState::getUndoManager()
{
	return undoManager.get();
}

VirtualKeyboard::Keyboard* SvkPluginState::getKeyboard()
{
	return virtualKeyboard.get();
}

ModeMapper* SvkPluginState::getModeMapper()
{
	return modeMapper.get();
}

NoteMap* SvkPluginState::getMidiInputMap()
{
    return midiProcessor->getMidiInputFilter()->getNoteMap();
}
NoteMap* SvkPluginState::getMidiOutputMap()
{
    return midiProcessor->getMidiOutputFilter()->getNoteMap();
}

int SvkPluginState::getMappingMode()
{
	return mapModeSelected;
}

int SvkPluginState::getMappingStyle()
{
	return mapStyleSelected;
}

bool SvkPluginState::isAutoMapping()
{
    return mapModeSelected == 2;
}


SvkPreset* SvkPluginState::getPresetinSlot(int slotNumIn)
{
    return presetManager->getPresetLoaded(slotNumIn);
}

SvkPreset* SvkPluginState::getPresetViewed()
{
    return presetViewed;
}

int SvkPluginState::getPresetSlotNumViewed()
{
	return presetSlotNumViewed;
}

int SvkPluginState::getNumModesInPresetViewed()
{
	return presetViewed->getModeSlotsSize();
}

Mode* SvkPluginState::getModeInSlot(int slotNumIn)
{
    return presetManager->getModeInSlots(presetSlotNumViewed, slotNumIn);
}

Mode* SvkPluginState::getModeViewed()
{
    return modeViewed;
}

int SvkPluginState::getModeViewedNum()
{
	return modeViewedNum;
}

Mode* SvkPluginState::getMode1()
{
    return presetManager->getModeInSlots(presetSlotNumViewed, presetViewed->getMode1SlotNumber());
}

Mode* SvkPluginState::getMode2()
{
    return presetManager->getModeInSlots(presetSlotNumViewed, presetViewed->getMode2SlotNumber());
}

Mode* SvkPluginState::getModeCustom()
{
    return presetManager->getModeCustom();
}

int SvkPluginState::getMode1Root()
{
	return getMode1()->getRootNote();
}

int SvkPluginState::getMode2Root()
{
	return getMode2()->getRootNote();
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

int SvkPluginState::getPeriodShift()
{
	return midiProcessor->getPeriodShift();
}

int SvkPluginState::getMidiChannelOut()
{
	return midiProcessor->getMidiChannelOut();
}

bool SvkPluginState::isShowingNoteNums()
{
	return virtualKeyboard->isShowingNoteNumbers();
}
int SvkPluginState::getKeyStyle()
{
	return virtualKeyboard->getKeyPlacementStyle();
}
int SvkPluginState::getHighlightStyle()
{
	return virtualKeyboard->getHighlightStyle();
}

//==============================================================================

bool SvkPluginState::isPresetEdited()
{
    return presetEdited;
}

void SvkPluginState::setPresetViewed(int presetViewedIn)
{
    presetSlotNumViewed = presetViewedIn;
    presetViewed = presetManager->getPresetLoaded(presetSlotNumViewed);

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
    presetManager->handleModeSelection(presetSlotNumViewed, modeBoxNum, idIn);
    
	midiProcessor->setMode1(getMode1());
	midiProcessor->setMode2(getMode2());

	if (modeViewedNum == modeBoxNum)
		updateModeViewed();
	
    sendChangeMessage();

	if (isAutoMapping())
		doMapping();

    presetEdited = true;
}


void SvkPluginState::setModeCustom(String stepsIn)
{
    presetManager->setModeCustom(stepsIn);
	handleModeSelection(modeViewedNum, 999);
}

void SvkPluginState::setMode1Root(int rootNoteIn)
{
    Mode* mode = getMode1();
    
    rootNoteIn = totalModulus(rootNoteIn, 128);
    mode->setRootNote(rootNoteIn);
    
    presetEdited = true;
    
	updateModeViewed();
    
    if (isAutoMapping())
        doMapping();
}

void SvkPluginState::setMode2Root(int rootNoteIn)
{
    Mode* mode = getMode2();

    rootNoteIn = totalModulus(rootNoteIn, 128);
    mode->setRootNote(rootNoteIn);
    
    presetEdited = true;
    
	updateModeViewed();

    if (isAutoMapping())
        doMapping();
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

void SvkPluginState::setPeriodShift(int shiftIn)
{
    midiProcessor->setPeriodShift(shiftIn);
}

void SvkPluginState::setMidiChannel(int midiChannelIn)
{
    midiProcessor->setMidiChannelOut(midiChannelIn);
	virtualKeyboard->setMidiChannelOut(midiChannelIn);
}

void SvkPluginState::setMidiInputMap(NoteMap noteMapIn)
{
	midiProcessor->setMidiInputMap(noteMapIn);
}

void SvkPluginState::setMidiOutputMap(NoteMap noteMapIn)
{
	midiProcessor->setMidiOutputMap(noteMapIn);
}

void SvkPluginState::setMapMode(int mapModeSelectionIn)
{
    mapModeSelected = mapModeSelectionIn;
    presetViewed->thePropertiesNode.setProperty(IDs::mappingMode, mapModeSelected, nullptr);
    
    if (mapModeSelected == 2) // Auto Mapping
    {
        doMapping();
    }
    
    else if (mapModeSelected == 3) // Manual Mapping
    {
        ValueTree midiMaps = presetViewed->theMidiSettingsNode.getChildWithName(IDs::midiMapNode);
        midiProcessor->setMidiMaps(midiMaps);
    }
    
    else // Mapping Off
    {
        midiProcessor->setMidiInputMap(NoteMap());
        midiProcessor->setMidiOutputMap(NoteMap());
        setModeViewed(1);
    }

    sendChangeMessage();
}

void SvkPluginState::setMapStyle(int mapStyleIn)
{
	mapStyleSelected = mapStyleIn;
    presetViewed->thePropertiesNode.setProperty(IDs::modeMappingStyle, mapStyleIn, nullptr);
    
    if (isAutoMapping())
    {
        doMapping();
    }
}

void SvkPluginState::doMapping(const Mode* mode1, const Mode* mode2, int mappingType,
                               int mode1OrderIn, int mode2OrderIn, int mode1OrderOffsetIn, int mode2OrderOffsetIn)
{
    virtualKeyboard->allNotesOff();

    NoteMap noteMap = modeMapper->map(*mode1, *mode2, mappingType,
                                      mode1OrderIn, mode2OrderIn, mode1OrderOffsetIn, mode2OrderOffsetIn,
                                      *midiProcessor->getInputNoteMap());

    setMidiInputMap(noteMap);
    sendMappingToKeyboard();
}

void SvkPluginState::doMapping()
{
	if (mapModeSelected > 1)
	{
		DBG("Mapping new settings");
		doMapping(getMode1(), getMode2(), mapStyleSelected, mapOrder1, mapOrder2, mapOrderOffset1, mapOrderOffset2);
	}
	else
	{
		midiProcessor->setMidiInputMap(NoteMap());
		sendMappingToKeyboard();
	}
}

void SvkPluginState::sendMappingToKeyboard()
{
	virtualKeyboard->setInputNoteMap(midiProcessor->getInputNoteMap());
}

void SvkPluginState::sendMappingToKeyboard(ValueTree mapNodeIn)
{
	midiProcessor->setMidiMaps(mapNodeIn);
	sendMappingToKeyboard();
}

void SvkPluginState::setNoteNumsShowing(bool showNoteNumsIn)
{
	virtualKeyboard->setShowNoteNumbers(showNoteNumsIn);
}

void SvkPluginState::setKeyStyle(int keyStyleIn)
{
    virtualKeyboard->setKeyPlacementStyle(keyStyleIn);
}

void SvkPluginState::setHighlightStyle(int highlightStyleIn)
{
	virtualKeyboard->setHighlightStyle(highlightStyleIn);
}

//==============================================================================

void SvkPluginState::updateModeViewed(bool sendChange)
{
	modePresetSlotNum = modeViewedNum ?
		presetViewed->getMode2SlotNumber() : presetViewed->getMode1SlotNumber();

    svkParameters.grab(IDs::modeSlotNumViewed)->setValue(modeViewedNum); // unnecessary call when the parameter triggers this function

	modeViewed = presetManager->getModeInSlots(presetSlotNumViewed, modePresetSlotNum);

	midiProcessor->setModeViewed(modeViewed);
	virtualKeyboard->applyMode(modeViewed);
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

	presetManager->refreshModeSlot(presetSlotNumViewed);
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

    presetManager->commitPreset(presetSlotNumViewed, presetViewed->parentNode);

	pluginStateNode.removeChild(pluginStateNode.getChildWithName(IDs::presetNode), nullptr);
	pluginStateNode.addChild(presetViewed->parentNode, -1, nullptr);

    presetEdited = false;
}

bool SvkPluginState::savePresetViewedToFile()
{
    commitPresetChanges();
    return presetManager->savePresetToFile(presetSlotNumViewed, pluginSettings->getPresetPath());
}

bool SvkPluginState::loadPresetFromFile(bool replaceViewed)
{
    // multi preset loading not yet implemented
	int slotNumber = presetSlotNumViewed;

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

	return presetManager->saveModeToFile(presetSlotNumViewed, modeSlotNumber, pluginSettings->getModePath());
}

bool SvkPluginState::loadModeFromFile()
{
	ValueTree modeNode = presetManager->nodeFromFile("Open Mode", "*.svk", pluginSettings->getModePath());

	if (Mode::isValidMode(modeNode))
	{
		presetManager->addSlotAndSetSelection(presetSlotNumViewed, modeViewedNum, modeNode);
		updateModeViewed();
		doMapping();
		return true;
	}

	return false;
}

//==============================================================================

void SvkPluginState::parameterValueChanged(int parameterIndex, float newValue)
{
    
}
 
void SvkPluginState::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
    
}

void SvkPluginState::changeListenerCallback(ChangeBroadcaster* source)
{
	// Preset loaded
	if (source == presetManager.get())
	{
        updateToPreset();
	}
}
