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
    
    midiProcessor.reset(new SvkMidiProcessor(svkTree));
    midiSettingsNode = midiProcessor->midiSettingsNode;
    
	presetManager.reset(new SvkPresetManager(pluginSettingsNode));
	modeLibraryNode = presetManager->modeLibraryNode;
	presetManager->addChangeListener(this);

	virtualKeyboard.reset(new VirtualKeyboard::Keyboard(pianoNode));
	pianoNode = virtualKeyboard->getNode();
    virtualKeyboard->addListener(midiProcessor.get());
    
    setTuningToET(2, 12);

	textFilterIntOrSpace.reset(new TextFilterIntOrSpace());
	textFilterInt.reset(new TextFilterInt());
    
	for (auto child : svkTree.state)
	{
		svkTree.addParameterListener(child["id"].toString(), this);
	}

	DBG("PluginState listening to parameters");
	midiProcessor->connectToParameters();

	// TODO: set up default settings
 //   setModeSelectorViewed(1);
	//updateModeViewed(false);
}

void SvkPluginState::recallState(ValueTree nodeIn, bool fallbackToDefaultSettings)
{
	DBG("PluginState recalling " + nodeIn.toXmlString());
	if (nodeIn.hasType(IDs::pluginStateNode))
	{
		if (!presetManager->loadPreset(nodeIn.getChildWithName(IDs::presetNode), false))
		{
			// Might not be good - will actually delete existing properties that are set properly
			nodeIn = defaultPluginStateNode;
			presetManager->loadPreset(nodeIn.getChildWithName(IDs::presetNode), false);
		}

		pluginStateNode = nodeIn.createCopy();
		revertToSavedPreset(fallbackToDefaultSettings);
	}
}

void SvkPluginState::revertToSavedPreset(bool fallbackToDefaultSettings, bool sendChange)
{
	presetEdited = false;
	presetManager->resetToSavedPreset();

	pluginStateNode.getOrCreateChildWithName(IDs::presetNode, nullptr) = presetManager->getPreset();
	svkPreset = pluginStateNode.getChildWithName(IDs::presetNode);

	midiSettingsNode = svkPreset.getChildWithName(IDs::midiSettingsNode);
	midiProcessor->restoreFromNode(midiSettingsNode);

	pluginEditorNode = pluginStateNode.getOrCreateChildWithName(IDs::pluginEditorNode, nullptr);

	// TODO revise
	midiProcessor->setMode1(getMode1());
	midiProcessor->setMode2(getMode2());

	loadPropertiesOfNode(pluginStateNode, fallbackToDefaultSettings);
    
	// TODO: Move to UI side of things
    pianoNode = svkPreset.getChildWithName(IDs::pianoNode);
	virtualKeyboard->restoreNode(pianoNode);
    virtualKeyboard->applyMode(modeViewed);

	doMapping();
    
	if (sendChange)
		sendChangeMessage();
}

void SvkPluginState::loadPropertiesOfNode(ValueTree pluginStateNodeIn, bool fallbackToDefault)
{
	for (auto prop : IDs::pluginStateSettings)
	{
		var value;

		if (!pluginStateNodeIn.getChildWithProperty(prop, value).isValid())
		{
			if (!fallbackToDefault)
				continue;

			else
				defaultPluginStateNode.getChildWithProperty(prop, value);
		}

		if (prop == IDs::modeSelectorViewed)
		{
			setModeSelectorViewed(value);
		}
		
		else if (prop == IDs::mappingMode)
		{
			setMapMode(value);
		}
		
		else if (prop == IDs::modeMappingStyle)
		{
			setMapStyle(value);
		}

		// MOVE TO MODE MAPPING CLASS ?
		else if (prop == IDs::mode1OrderMapping)
		{
			setMapOrder1(0);
		}

		else if (prop == IDs::mode2OrderMapping)
		{
			setMapOrder2(0);
		}

		else if (prop == IDs::mode1OrderOffsetMapping)
		{
			setMapOrderOffset1(0);
		}

		else if (prop == IDs::mode2OrderOffsetMapping)
		{
			setMapOrderOffset2(0);
		}

		// MOVE TO KEYBOARD NODE
		else if (prop == IDs::keyboardKeysStyle)
		{
			setKeyStyle(value);
		}

		else if (prop == IDs::keyboardHighlightStyle)
		{
			setHighlightStyle(value);
		}
	}
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

ValueTree SvkPluginState::getPreset()
{
    return presetManager->getPreset();
}

Mode* SvkPluginState::getModeInSlot(int slotNumIn)
{
	return presetManager->getModeInSlot(slotNumIn);
}

Mode* SvkPluginState::getModeViewed()
{
	return modeViewed;
}

Mode* SvkPluginState::getMode1()
{
	return presetManager->getModeInSlot(0);
}

Mode* SvkPluginState::getMode2()
{
	return presetManager->getModeInSlot(1);
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

int SvkPluginState::getNumModesLoaded()
{
	return presetManager->getNumModeSlots();
}

int SvkPluginState::getModeSelectorViewed()
{
	return modeSelectorViewedNum;
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
		param->setValueNotifyingHost(param->convertTo0to1(valueIn));
	}
}

void SvkPluginState::setModeSelectorViewed(int modeSelectorViewedIn)
{
    modeSelectorViewedNum = modeSelectorViewedIn;
    svkPreset.getChildWithName(IDs::presetProperties).setProperty(IDs::modeSelectorViewed, modeSelectorViewedNum, nullptr);
	updateModeViewed();
}

void SvkPluginState::handleModeSelection(int modeBoxNum, int idIn)
{
    presetManager->handleModeSelection(modeBoxNum, idIn);
    
	midiProcessor->setMode1(getMode1());
	midiProcessor->setMode2(getMode2());

	if (getModeSelectorViewed() == modeBoxNum)
		updateModeViewed();
	
    sendChangeMessage();

	if (isAutoMapping())
		doMapping();

    presetEdited = true;
}

void SvkPluginState::setMapMode(int mapModeSelectionIn)
{
    svkPreset.getChildWithName(IDs::presetProperties).setProperty(IDs::mappingMode, mapModeSelectionIn, nullptr);
	DBG("Plugin State Map Mode Selection: " + String(mapModeSelectionIn));
	if (mapModeSelectionIn == 2) // Auto Mapping
	{
		doMapping();
	}

	else if (mapModeSelectionIn == 3) // Manual Mapping
	{
		ValueTree midiMaps = midiSettingsNode.getChildWithName(IDs::midiMapNode);
		midiProcessor->setMidiMaps(midiMaps);
	}

	else // Mapping Off
	{
		midiProcessor->setInputRemap(NoteMap());
		midiProcessor->setOutputFilter(NoteMap());
		setModeSelectorViewed(1);
	}

	sendChangeMessage();
}

void SvkPluginState::setMapStyle(int mapStyleIn)
{
    svkPreset.getChildWithName(IDs::presetProperties).setProperty(IDs::modeMappingStyle, mapStyleIn, nullptr);
	DBG("mapStyle index = " + String(mapStyleIn));
	if (isAutoMapping())
	{
		doMapping();
	}
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

void SvkPluginState::setModeSelectorRoot(int modeSelectorNumIn, int rootNoteIn, bool updateParameter)
{
	svkPreset.getChildWithName(IDs::modeSelectorsNode)
		.getChild(modeSelectorNumIn)
		.setProperty(IDs::rootMidiNote, rootNoteIn, nullptr);

	Mode* mode = presetManager->getModeBySelector(modeSelectorNumIn);
	rootNoteIn = totalModulus(rootNoteIn, 128);
	mode->setRootNote(rootNoteIn);

	presetEdited = true;

	if (modeSelectorViewedNum == modeSelectorNumIn)
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
	handleModeSelection(getModeSelectorViewed(), 999);
}

void SvkPluginState::setMapOrder1(int orderIn)
{
    mapOrder1 = orderIn;
    svkPreset.getChildWithName(IDs::presetProperties).setProperty(IDs::mode1OrderMapping, mapOrder1, nullptr);
}

void SvkPluginState::setMapOrder2(int orderIn)
{
    mapOrder2 = orderIn;
	svkPreset.getChildWithName(IDs::presetProperties).setProperty(IDs::mode2OrderMapping, mapOrder2, nullptr);
}

void SvkPluginState::setMapOrderOffset1(int offsetIn)
{
    mapOrderOffset1 = offsetIn;
	svkPreset.getChildWithName(IDs::presetProperties).setProperty(IDs::mode1OrderOffsetMapping, mapOrderOffset1, nullptr);
}

void SvkPluginState::setMapOrderOffset2(int offsetIn)
{
    mapOrderOffset2 = offsetIn;
	svkPreset.getChildWithName(IDs::presetProperties).setProperty(IDs::mode2OrderOffsetMapping, mapOrderOffset2, nullptr);
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
		DBG("Applygind new MIDI mapping");
		doMapping(getMode1(), getMode2(), getMappingStyle(), mapOrder1, mapOrder2, mapOrderOffset1, mapOrderOffset2);
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
	// not sure why this was here, but I don't think it's needed
	//midiProcessor->setMidiMaps(mapNodeIn); 
	sendMappingToKeyboard();
}

void SvkPluginState::updateModeViewed(bool sendChange)
{
	modeViewed = presetManager->getModeBySelector(modeSelectorViewedNum);

	midiProcessor->setModeViewed(modeViewed);

	// MOVE TO UI
	virtualKeyboard->applyMode(modeViewed);
    virtualKeyboard->resized();

	getModeSelectorViewed() ? virtualKeyboard->displayKeyboardState(midiProcessor->getRemappedKeyboardState())
					   : virtualKeyboard->displayKeyboardState(midiProcessor->getOriginalKeyboardState());
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

	presetManager->setSlotToMode(modeSelectorViewedNum, presetManager->getModeCustom()->modeNode);
	//presetManager->refreshModeSlot();
	updateModeViewed();
	doMapping();
}

void SvkPluginState::commitStateNode()
{
	// TODO: make it so it's not necessary to call this before saving
	presetManager->commitPreset();
	svkPreset = presetManager->getPreset();
	pluginStateNode.getOrCreateChildWithName(IDs::presetNode, nullptr) = svkPreset;

    pluginStateNode.getOrCreateChildWithName(IDs::pianoNode, nullptr).copyPropertiesAndChildrenFrom(pianoNode, nullptr);

	// TODO: revise
	midiProcessor->updateNode();
	if (getParameterValue(IDs::mappingMode) > 2)
	{
		midiSettingsNode.addChild(midiProcessor->midiMapNode, -1, nullptr);
	}
	else if (midiSettingsNode.getChildWithName(IDs::midiMapNode).isValid())
	{
		midiSettingsNode.removeChild(midiSettingsNode.getChild(0), nullptr);
	}
   
	pluginStateNode.getOrCreateChildWithName(IDs::midiSettingsNode, nullptr).copyPropertiesAndChildrenFrom(midiSettingsNode, nullptr);
    
	svkTree.state.getOrCreateChildWithName(IDs::pluginStateNode, nullptr).copyPropertiesAndChildrenFrom(pluginStateNode, nullptr);
    
	presetEdited = false;
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
		updateModeViewed();
		doMapping();
		return true;
	}

	return false;
}

//==============================================================================

void SvkPluginState::parameterChanged(const String& paramID, float newValue)
{
	if (paramID == IDs::mappingMode.toString())
	{
		setMapMode(newValue);
	}
	else if (paramID == IDs::modeMappingStyle.toString())
	{
		setMapStyle(newValue);
	}
	else if (paramID == IDs::pianoWHRatio.toString())
	{
		virtualKeyboard->setKeySizeRatio(newValue);
	}
	else if (paramID == IDs::keyboardNumRows.toString())
	{
		virtualKeyboard->setNumRows(newValue);
	}
}

void SvkPluginState::changeListenerCallback(ChangeBroadcaster* source)
{
	// Preset loaded
	if (source == presetManager.get())
	{
        revertToSavedPreset();
	}
}
