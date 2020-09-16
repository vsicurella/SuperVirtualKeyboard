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
	pluginSettingsNode = pluginSettings->getSettingsNode();

	presetManager.reset(new SvkPresetManager(pluginSettingsNode));
	modeLibraryNode = presetManager->modeLibraryNode;
	presetManager->addChangeListener(this);

	// TODO: Factory default MIDI settings
	midiProcessor.reset(new SvkMidiProcessor(svkTree));
	midiSettingsNode = midiProcessor->midiSettingsNode;

	buildFactoryDefaultState();
	buildUserDefaultState();

	// TODO: revise
	textFilterIntOrSpace.reset(new TextFilterIntOrSpace());
	textFilterInt.reset(new TextFilterInt());

	revertToSavedPreset(true, false);
    
	//for (auto child : svkTree.state)
	//{
	//	svkTree.addParameterListener(child["id"].toString(), this);
	//}

	//DBG("PluginState listening to parameters");
	//midiProcessor->connectToParameters();

	// TODO: set up default settings
 //   setModeSelectorViewed(1);
	//updateModeViewed(false);
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

void SvkPluginState::revertToSavedPreset(bool fallbackToDefaultSettings, bool sendChange)
{
	presetEdited = false;
	presetManager->resetToSavedPreset();

	svkPreset = &presetManager->getPreset();
	presetNode = svkPreset->getPresetNode();

	pluginStateNode.getOrCreateChildWithName(IDs::presetNode, nullptr).copyPropertiesAndChildrenFrom(presetNode, nullptr);
	
	modeSelectorViewedNum = svkPreset->getModeSelectorViewed();

	pianoNode = svkPreset->getKeyboardNode();
	pluginEditorNode = pluginStateNode.getOrCreateChildWithName(IDs::pluginEditorNode, nullptr);

	modeMapper.reset(new ModeMapper(svkPreset->getMappingsNode()));
	midiSettingsNode = svkPreset->getMidiSettingsNode();
	midiProcessor->restoreFromNode(midiSettingsNode);
	midiProcessor->setMidiMaps(svkPreset->getMappingsNode());

	onModeUpdate(!sendChange, !sendChange);

	if (sendChange)
		listeners.call(&Listener::presetLoaded, presetNode);
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
	return presetNode.getChildWithName(IDs::presetProperties)[IDs::mappingMode];
}

int SvkPluginState::getMappingStyle() const
{
	return presetNode.getChildWithName(IDs::midiMapNode)[IDs::modeMappingStyle];
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
	presetNode.getChildWithName(IDs::presetProperties).setProperty(IDs::mappingMode, mapModeSelectionIn, nullptr);
	DBG("Plugin State Map Mode Selection: " + String(mapModeSelectionIn));
	
	if (mapModeSelectionIn == 2) // Auto Mapping
		doMapping();
	
	else
	{
		if (mapModeSelectionIn == 3) // Manual Mapping
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

		listeners.call(&Listener::inputMappingChanged, midiProcessor->getInputNoteRemap());
	}
}

void SvkPluginState::setMapStyle(int mapStyleIn)
{
	modeMapper->setMappingStyle(mapStyleIn);
	presetNode.getChildWithName(IDs::midiMapNode).setProperty(IDs::modeMappingStyle, mapStyleIn, nullptr);
	DBG("mapStyle index = " + String(mapStyleIn));
	if (isAutoMapping())
	{
		doMapping();
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
		doMapping();
}

void SvkPluginState::onModeUpdate(bool sendModeViewedChangeMessage, bool sendMappingChangeMessage)
{
	midiProcessor->setMode1(getMode1());
	midiProcessor->setMode2(getMode2());

	updateModeViewed(sendMappingChangeMessage);
	
	if (isAutoMapping())
		doMapping(sendMappingChangeMessage);
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

void SvkPluginState::doMapping(const Mode* mode1, const Mode* mode2, bool sendChangeMessage)
{
    NoteMap noteMap = modeMapper->map(*mode1, *mode2, *midiProcessor->getInputNoteRemap());

    setMidiInputMap(noteMap);

	if (sendChangeMessage)
		listeners.call(&Listener::inputMappingChanged, midiProcessor->getInputNoteRemap());
}

void SvkPluginState::doMapping(bool sendChangeMessage)
{
	if (getMappingMode() > 1)
	{
		DBG("Applying new MIDI mapping");
		doMapping(getMode1(), getMode2());
	}
	else
	{
		midiProcessor->setInputRemap(NoteMap());

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

	presetManager->setSlotToMode(modeSelectorViewedNum, presetManager->getModeCustom()->modeNode);
	
	onModeUpdate(sendChangeMessage, sendChangeMessage);

	if (sendChangeMessage)
		listeners.call(&Listener::modeInfoChanged, modeViewed);
}

void SvkPluginState::commitStateNode()
{
	// TODO: make it so it's not necessary to call this before saving
	presetManager->commitPreset();
	svkPreset = &presetManager->getPreset();
	presetNode = svkPreset->getPresetNode();

	pluginStateNode.getOrCreateChildWithName(IDs::presetNode, nullptr).copyPropertiesAndChildrenFrom(presetNode, nullptr);
	presetNode.getOrCreateChildWithName(IDs::pianoNode, nullptr).copyPropertiesAndChildrenFrom(pianoNode, nullptr);

	onModeUpdate();

	// TODO: revise
	midiProcessor->updateNode();
	if (getMappingMode() > 2)
	{
		midiSettingsNode.addChild(midiProcessor->midiMapNode, -1, nullptr);
	}
	else if (midiSettingsNode.getChildWithName(IDs::midiMapNode).isValid())
	{
		midiSettingsNode.removeChild(midiSettingsNode.getChild(0), nullptr);
	}

	pluginStateNode.getOrCreateChildWithName(IDs::midiSettingsNode, nullptr).copyPropertiesAndChildrenFrom(midiSettingsNode, nullptr);
	svkTree.state.getOrCreateChildWithName(IDs::pluginStateNode, nullptr).copyPropertiesAndChildrenFrom(pluginStateNode, nullptr);

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
	factoryDefaultPluginStateNode.appendChild(mapper.getMappingNode(), nullptr);
}

void SvkPluginState::buildUserDefaultState()
{
	ValueTree defaultSettings = pluginSettingsNode.getChildWithName(IDs::presetNode);
	
	// TODO: connect to actual user default settings

	if (!defaultSettings.isValid())
		defaultSettings = factoryDefaultPluginStateNode.createCopy();
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
}

void SvkPluginState::changeListenerCallback(ChangeBroadcaster* source)
{
	// Preset loaded
	if (source == presetManager.get())
	{
        revertToSavedPreset();
	}
}
