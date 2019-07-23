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

	virtualKeyboard.reset(new VirtualKeyboard::Keyboard());
	virtualKeyboard->addListener(midiProcessor.get());
	pianoNode = virtualKeyboard->getNode();
	pluginStateNode.addChild(pianoNode, -1, nullptr);

	textFilterIntOrSpace.reset(new TextFilterIntOrSpace());
	textFilterInt.reset(new TextFilterInt());
    
	setPresetViewed(0);
	modeViewedNum = 1;
	updateModeViewed(false);
}

void SvkPluginState::recallState(ValueTree nodeIn)
{
    ValueTree childNode = nodeIn.getChildWithName(IDs::presetNode);
    
    if (nodeIn.hasType(IDs::presetNode))
    {
        presetManager->loadPreset(0, nodeIn, false);
		return;
    }
	else if (childNode.hasType(IDs::presetNode))
    {
        presetManager->loadPreset(0, childNode, false);
    }
	else
	{
		presetManager->loadPreset(0, ValueTree(IDs::presetNode), false);
		return;
	}
    
    childNode = nodeIn.getChildWithName(IDs::globalSettingsNode);
    
    if (childNode.isValid())
        pluginSettings->restoreNode(childNode);
    
    childNode = nodeIn.getChildWithName(IDs::pluginEditorNode);
    
    if (childNode.isValid())
        pluginEditorNode = childNode.createCopy();
    
    updateToPreset();
}

void SvkPluginState::updateToPreset(bool sendChange)
{
	presetEdited = false;

	presetViewed = presetManager->getPresetLoaded(presetSlotNumViewed);
	modeViewedNum = (int) presetViewed->thePropertiesNode.getProperty(IDs::modeSlotNumViewed);
	isAutoMapping = (bool) presetViewed->thePropertiesNode.getProperty(IDs::autoRemapOn);
	mapStyleSelected = (int)presetViewed->thePropertiesNode.getProperty(IDs::modeMappingStyle);

	updateModeViewed();
	
	//pluginStateNode.removeChild(pluginStateNode.getChildWithName(IDs::presetNode), nullptr);
	//pluginStateNode.addChild(presetViewed->parentNode, -1, nullptr);

	midiProcessor->restoreFromNode(presetViewed->theMidiSettingsNode);
	//midiProcessor->setScaleSize(modeViewed->getScaleSize());

	//virtualKeyboard->restoreDataNode(presetViewed->theKeyboardNode);
	virtualKeyboard->setMidiChannelOut(midiProcessor->getMidiChannelOut());

	sendMappingToKeyboard();

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

bool SvkPluginState::isAutoMapOn()
{
	return isAutoMapping;
}

int SvkPluginState::getMappingStyle()
{
	return mapStyleSelected;
}

SvkPreset* SvkPluginState::getPresetinSlot(int slotNumIn)
{
    return presetManager->getPresetLoaded(slotNumIn);
}

SvkPreset* SvkPluginState::getPresetViewed()
{
    return presetViewed;
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
    
    if (modeViewedNum == modeBoxNum)
        updateModeViewed();

	if (isAutoMapping)
		doMapping();

    presetEdited = true;
}


void SvkPluginState::setModeCustom(String stepsIn)
{
    presetManager->setModeCustom(stepsIn);
	handleModeSelection(modeViewedNum, presetManager->getNumMenuItems() + 1);
}

void SvkPluginState::setMode1Root(int rootNoteIn)
{
    Mode* mode = getMode1();
    
    rootNoteIn = totalModulus(rootNoteIn, 128);
    mode->setRootNote(rootNoteIn);
    
    presetEdited = true;
    
	updateModeViewed();
    
    if (isAutoMapping)
        doMapping();
}

void SvkPluginState::setMode2Root(int rootNoteIn)
{
    Mode* mode = getMode2();

    rootNoteIn = totalModulus(rootNoteIn, 128);
    mode->setRootNote(rootNoteIn);
    
    presetEdited = true;
    
	updateModeViewed();

    if (isAutoMapping)
        doMapping();
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

void SvkPluginState::setAutoMapping(bool isAutoMappingIn)
{
    isAutoMapping = isAutoMappingIn;
    presetViewed->thePropertiesNode.setProperty(IDs::autoRemapOn, isAutoMapping, nullptr);
}

void SvkPluginState::setMapStyle(int mapStyleIn)
{
	mapStyleSelected = mapStyleIn;
    presetViewed->thePropertiesNode.setProperty(IDs::modeMappingStyle, mapStyleIn, nullptr);
    
    if (isAutoMapping)
    {
        doMapping();
    }
}

void SvkPluginState::doMapping(const Mode* mode1, const Mode* mode2, int mappingType,
                               int mode1OrderIn, int mode2OrderIn, int mode1OrderOffsetIn, int mode2OrderOffsetIn)
{
    NoteMap noteMap = modeMapper->map(*mode1, *mode2, mappingType,
                                      mode1OrderIn, mode2OrderIn, mode1OrderOffsetIn, mode2OrderOffsetIn,
                                      *midiProcessor->getInputNoteMap());

    setMidiInputMap(noteMap);
}

void SvkPluginState::doMapping()
{
    doMapping(getMode1(), getMode2(), mapStyleSelected, mapOrder1, mapOrder2, mapOrderOffset1, mapOrderOffset2);
}

void SvkPluginState::sendMappingToKeyboard()
{
	virtualKeyboard->updateKeyMapping(midiProcessor->getMidiInputFilter(), midiProcessor->getMidiOutputFilter());
}

void SvkPluginState::sendMappingToKeyboard(ValueTree mapNodeIn)
{
	midiProcessor->setMidiMaps(mapNodeIn);
	sendMappingToKeyboard();
}

void SvkPluginState::setNoteNumsShowing(bool showNoteNumsIn)
{
	virtualKeyboard->setNoteNumbersVisible(showNoteNumsIn);
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

	modeViewed = presetManager->getModeInSlots(presetSlotNumViewed, modePresetSlotNum);

	virtualKeyboard->updateKeyboard(modeViewed);

	if (sendChange)
		sendChangeMessage();
}


void SvkPluginState::commitPresetChanges()
{
    presetManager->commitPreset(presetSlotNumViewed, presetViewed->parentNode);

	pluginStateNode.removeChild(pluginStateNode.getChildWithName(IDs::presetNode), nullptr);
	pluginStateNode.addChild(presetViewed->parentNode, -1, nullptr);

    presetEdited = false;
}

bool SvkPluginState::savePresetViewedToFile()
{
    commitPresetChanges();
    return presetManager->savePresetToFile(presetSlotNumViewed);
}

bool SvkPluginState::loadPresetFromFile(bool replaceViewed)
{
	int slotNumber = presetSlotNumViewed;

	if (!replaceViewed)
		slotNumber = presetManager->getNumModesLoaded();

    return presetManager->loadPreset(slotNumber);
}

bool SvkPluginState::saveModeViewedToFile()
{
	int modeSlotNumber = modeViewed ? 
		presetViewed->getMode2SlotNumber() : presetViewed->getMode1SlotNumber();

	return presetManager->saveModeToFile(presetSlotNumViewed, modeSlotNumber);
}

bool SvkPluginState::loadModeIntoSlot()
{
	// TODO
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
