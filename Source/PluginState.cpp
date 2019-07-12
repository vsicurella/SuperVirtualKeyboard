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

	virtualKeyboard.reset(new VirtualKeyboard::Keyboard(midiProcessor.get()));
	virtualKeyboard->addListener(midiProcessor.get());
	pianoNode = virtualKeyboard->getNode();
	pluginStateNode.addChild(pianoNode, -1, nullptr);

	textFilterIntOrSpace.reset(new TextFilterIntOrSpace());
	textFilterInt.reset(new TextFilterInt());
    
    updateToPreset();
}

void SvkPluginState::recallState(ValueTree nodeIn)
{
    ValueTree childNode = nodeIn.getChildWithName(IDs::presetNode);
    
    if (nodeIn.hasType(IDs::presetNode))
    {
        presetManager->loadPreset(0, nodeIn, dontSendNotification);
    }
    else
    {
        presetManager->loadPreset(0, childNode, dontSendNotification);
    }
    
    childNode = nodeIn.getChildWithName(IDs::pluginSettingsNode);
    
    if (childNode.isValid())
        pluginSettings->restoreNode(childNode);
    
    childNode = nodeIn.getChildWithName(IDs::pluginEditorNode);
    
    if (childNode.isValid())
        pluginEditorNode = childNode.createCopy();
    
    updateToPreset();
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
    return presetManager->getModeInSlots(presetSlotNumViewed, slotNumIn);
}

Mode* SvkPluginState::getModeViewed()
{
    return modeViewed;
}

Mode* SvkPluginState::getMode1()
{
    return presetManager->getModeInSlots(presetSlotNumViewed, presetViewed->getMode1SlotNumber());
}

Mode* SvkPluginState::getMode2()
{
    return presetManager->getModeInSlots(presetSlotNumViewed, presetViewed->getMode2SlotNumber());
}

//==============================================================================

bool SvkPluginState::isPresetEdited()
{
    return presetEdited;
}

void SvkPluginState::setPresetViewed(int presetViewedIn)
{
    presetSlotNumViewed = presetViewedIn;
    presetViewed = &presetManager->getPresetLoaded()[presetSlotNumViewed];
}

void SvkPluginState::setModeViewed(int modeViewedIn)
{
    modeViewedNum = modeViewedIn;
    modeViewed = presetManager->getModeInSlots(presetSlotNumViewed, modeViewedIn);
    presetViewed->parentNode.setProperty(IDs::modeSlotNumViewed, modeViewedNum, nullptr);
}

void SvkPluginState::changeModeInCurrentSlot(int modeLibraryIndexIn)
{
    ValueTree modeNode = presetManager->getModeInLibrary(modeLibraryIndexIn);
    presetViewed->setModeSlot(modeNode, modePresetSlotNum);
    
    presetEdited = true;
}

void SvkPluginState::addModeToNewSlot(int modeLibraryIndexIn)
{
    ValueTree modeNode = presetManager->getModeInLibrary(modeLibraryIndexIn);
    addModeToNewSlot(modeNode);
}

void SvkPluginState::addModeToNewSlot(ValueTree modePresetNodeIn)
{
    presetViewed->addMode(modePresetNodeIn);
    
    presetEdited = true;
}

void SvkPluginState::setModeViewedSlotNumber(int slotNumberIn)
{
    if (modeViewed) // Mode index 1, AKA Mode2
    {
        presetViewed->setMode2SlotNumber(slotNumberIn);
    }
    else // Mode1
    {
        presetViewed->setMode1SlotNumber(slotNumberIn);
    }
    
    presetEdited = true;
}

void SvkPluginState::setModeViewedRoot(int rootNoteIn)
{
    rootNoteIn = totalModulus(rootNoteIn, 128);
    modeViewed->setRootNote(rootNoteIn);
    
    presetEdited = true;
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
    modeMapper->setMapType(mapStyleIn);
    presetViewed->parentNode.setProperty(IDs::modeMappingType, mapStyleIn, nullptr);
    
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

void SvkPluginState::updateToPreset()
{
    presetEdited = false;
    
    setPresetViewed(0);
    setModeViewed(presetViewed->parentNode[IDs::modeSlotNumViewed]);
    
	pluginStateNode.removeChild(pluginStateNode.getChildWithName(IDs::presetNode), nullptr);
	pluginStateNode.addChild(presetViewed->parentNode, -1, nullptr);
	
	midiProcessor->restoreFromNode(presetViewed->theMidiSettingsNode);
    midiProcessor->setScaleSize(modeViewed->getScaleSize());
	
	virtualKeyboard->restoreDataNode(presetViewed->theKeyboardNode);
    
    sendChangeMessage();
}

void SvkPluginState::commitPresetChanges()
{
    presetManager->commitPreset(presetSlotNumViewed, presetViewed->parentNode);
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
		slotNumber = presetManager->getNumPresetsLoaded();

    return presetManager->loadPreset(slotNumber);
}

void SvkPluginState::changeListenerCallback(ChangeBroadcaster* source)
{
	// Preset loaded
	if (source == presetManager.get())
	{
        updateToPreset();
	}
}
