/*
  ==============================================================================

    PluginState.cpp
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PluginState.h"

SuperVirtualKeyboardPluginState::SuperVirtualKeyboardPluginState()
{
    appCmdMgr.reset(new ApplicationCommandManager());
    undoManager.reset(new UndoManager());

	pluginSettings.reset(new SvkPluginSettings());
    presetCurrent.reset(new SvkPreset());
    midiProcessor.reset(new SvkMidiProcessor());

	textFilterIntOrSpace.reset(new TextFilterIntOrSpace());
	textFilterInt.reset(new TextFilterInt());

    // setup data nodes
    pluginStateNode = ValueTree(IDs::pluginStateNode);
    modeLibraryNode = ValueTree(IDs::modeLibraryNode);
    modePresetNode = ValueTree(IDs::modePresetNode);

	pluginSettingsNode = pluginSettings->pluginSettingsNode;
	midiSettingsNode = midiProcessor->midiSettingsNode;
    
    pluginStateNode.addChild(modeLibraryNode, 0, nullptr);
    pluginStateNode.addChild(pluginSettingsNode, -1, nullptr);
    pluginStateNode.addChild(midiSettingsNode, -1, nullptr);

    createPresets();
}

SvkMidiProcessor* SuperVirtualKeyboardPluginState::getMidiProcessor()
{
    return midiProcessor.get();
}

UndoManager* SuperVirtualKeyboardPluginState::getUndoManager()
{
	return undoManager.get();
}

OwnedArray<Mode>* SuperVirtualKeyboardPluginState::getPresets()
{
	return &presets;
}

Array<Array<Mode*>>* SuperVirtualKeyboardPluginState::get_presets_sorted()
{
	return &presetsSorted;
}

SvkPreset* SuperVirtualKeyboardPluginState::getCurrentPreset()
{
	return presetCurrent.get();
}

int SuperVirtualKeyboardPluginState::get_current_preset_index()
{
	Mode* mode;

	for (int i = 0; i < presets.size(); i++)
	{
		mode = presets.getUnchecked(i);

		if (mode == modeCurrent)
			return i;
	}

	return -1;
}

Mode* SuperVirtualKeyboardPluginState::getCurrentMode()
{
	return modeCurrent;
}

int* SuperVirtualKeyboardPluginState::getMidiInputMap()
{
    return midiProcessor->getInputRemapper()->getRemappedNotes();
}
int* SuperVirtualKeyboardPluginState::getMidiOutputMap()
{
    return midiProcessor->getOutputRemapper()->getRemappedNotes();
}

int SuperVirtualKeyboardPluginState::is_mode_in_presets(String stepsStringIn)
{
	int index = 0;

	for (auto mode : presets)
	{
		if (stepsStringIn == mode->getStepsString())
			return index;
		index++;
	}

	return 0;
}

void SuperVirtualKeyboardPluginState::setCurrentMode(int presetIndexIn)
{
	Mode* mode = presets.getUnchecked(presetIndexIn);
	
	if (mode)
	{
		modeCurrent = mode;
		modeCurrent->setRootNote(midiProcessor->getRootNote());

		if (presetCurrent->theKeyboardNode.isValid())
			presetCurrent->theKeyboardNode.setProperty(IDs::pianoHasCustomColor, false, nullptr);

		presetCurrent->updateModeNode(modeCurrent->modeNode);
		presetCurrent->parentNode.setProperty(IDs::libraryIndexOfMode, presetIndexIn, undoManager.get());

		modePresetNode.copyPropertiesAndChildrenFrom(modeCurrent->modeNode, nullptr);
        sendChangeMessage();
	}
}

void SuperVirtualKeyboardPluginState::setCurrentMode(Mode* modeIn)
{
	if (modeIn)
	{
		modeCurrent = modeIn;
		modeCurrent->setRootNote(midiProcessor->getRootNote());
		presets.set(0, modeCurrent, true);

		if (presetCurrent->theKeyboardNode.isValid())
			presetCurrent->theKeyboardNode.setProperty(IDs::pianoHasCustomColor, false, nullptr);

		presetCurrent->updateModeNode(modeCurrent->modeNode);
		presetCurrent->parentNode.setProperty(IDs::libraryIndexOfMode, 0, undoManager.get());

		modePresetNode = modeCurrent->modeNode;
        sendChangeMessage();
	}
}

void SuperVirtualKeyboardPluginState::setMidiRootNote(int rootNoteIn)
{
	rootNoteIn = totalModulus(rootNoteIn, 128);
	midiProcessor->setRootNote(rootNoteIn);
	modeCurrent->setRootNote(rootNoteIn);
}

void SuperVirtualKeyboardPluginState::updateKeyboardSettingsPreset()
{
	if (pianoNode.isValid())
	{
		presetCurrent->updateKeyboardNode(pianoNode);
	}
}


bool SuperVirtualKeyboardPluginState::savePreset()
{
    midiProcessor->updateNode();
	modePresetNode = modeCurrent->modeNode;
	presetCurrent->updateParentNode(pluginStateNode);

	return presetCurrent->writeToFile();
}


bool SuperVirtualKeyboardPluginState::loadPreset()
{
    std::unique_ptr<SvkPreset> newPreset(SvkPreset::loadFromFile());
    
	if (newPreset.get())
	{
        presetCurrent.swap(newPreset);

		
		if (presetCurrent->thePluginSettingsNode.isValid())
		{
			pluginSettingsNode.copyPropertiesAndChildrenFrom(presetCurrent->thePluginSettingsNode, nullptr);
		}

		if (presetCurrent->theMidiSettingsNode.isValid())
		{
            midiProcessor->restoreFromNode(presetCurrent->theMidiSettingsNode);
		}

		if (presetCurrent->theModeNode.isValid())
		{
			modeCurrent->restoreNode(presetCurrent->theModeNode, midiProcessor->getRootNote());
			modePresetNode = modeCurrent->modeNode;
		}
		
		if (presetCurrent->theKeyboardNode.isValid())
		{
			pianoNode.copyPropertiesAndChildrenFrom(presetCurrent->theKeyboardNode, nullptr);
		}

		return true;
	}

	return false;
}

//==============================================================================


void SuperVirtualKeyboardPluginState::createPresets()
{
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 1 }), "Custom", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 2, 1, 2, 1 }), "Mavila", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 2, 1 }), "Father", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 1, 2, 1, 1, 1, 2 }), "Mavila", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 2, 1, 2, 1, 2, 1 }), "Dicot", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 2, 2, 1 }), "Machine", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 1, 2, 1, 2 }), "Orgone", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 1, 3, 1, 3, 1 }), "Augmented", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 1, 2, 2, 2, 1 }), "Meantone", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1 }), "Diminished", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 1, 2, 2, 1, 2, 1 }), "Father", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1, 1 }), "Orwell", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 2, 1, 2, 2, 1 }), "Hedgehog", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 1, 2, 1, 2, 1, 2, 1 }), "Titanium", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 3, 3, 3 }), "Blackwood", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 1, 3, 1, 3, 1 }), "Hanson", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 1, 3, 1, 3, 1, 3 }), "Orgone", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 2, 2, 2, 2, 1 }), "Porcupine", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1 }), "Orgone", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 4, 1, 4, 1, 4, 1 }), "Magic", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 1, 3, 3, 3 }), "Gorgo", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 1, 2, 2, 2, 2, 1 }), "Mavila", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 1, 2, 2, 1, 2, 1, 2 }), "Lemba", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 1, 3, 3, 3, 1 }), "Superpyth", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 2, 3, 2, 3, 2, 2 }), "Maqamic", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 1, 2, 2, 1, 2, 2, 2, 1 }), "Maqamic", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2 }), "Machine", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 4, 1, 4, 4, 1 }), "Bicycle", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 1, 3, 3, 1, 3, 3, 1 }), "Father", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 2, 3, 3, 3, 2 }), "Meantone", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 3, 2, 2, 3, 2, 3, 2 }), "Keemun", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 2, 3, 2, 2, 2, 2 }), "Negri", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 1, 4, 1, 4, 1, 4 }), "Hanson", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 2, 2, 5, 2, 2, 2 }), "Mavila", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 1, 3, 1, 3, 1, 3, 1, 3, 1 }), "Blackwood", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 3, 2, 2, 3, 2, 2, 3, 2 }), "Miracle", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 4, 1, 4, 4, 4, 1 }), "Superpyth", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 3, 3, 2, 3, 3, 2 }), "Hedgehog", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 2, 3, 2, 3, 2, 3, 2, 2 }), "Orwell", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 3, 1, 3, 3, 3, 3, 1 }), "Mavila", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 3, 2, 2, 3, 2, 2, 3 }), "Sephiroth", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 5, 4, 5, 5 }), "Godzilla", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 3, 5, 3, 5, 3 }), "Triforce", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 3, 4, 3, 3, 4 }), "Dastgah-e Sehgah / Maqam Nairuz", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 4, 2, 4, 4, 4, 2 }), "Meantone", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 4, 3, 4, 3, 3 }), "Mohajira", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 4, 3, 4, 4, 4, 3 }), "Meantone", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 3, 2, 2, 3, 2, 3, 2, 2, 3, 2 }), "Orgone", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 3 }), "Injera", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 5, 1, 5, 5, 5, 1 }), "Superpyth", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 4, 4, 4, 4, 4, 7 }), "Nusecond", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 5, 3, 5, 5, 5, 3 }), "Meantone", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 2, 3, 2, 3, 3, 2, 3, 2, 3, 2 }), "Meantone", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 4, 3, 4, 3, 4, 3, 3 }), "Orwell", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 1 }), "Mothra", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 4, 5, 4, 5, 4, 4 }), "Mohajira", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1 }), "Miracle", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 7, 7, 3, 7, 7, 7, 3 }), "Schismatic", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 4, 3, 3, 4, 3, 4, 3, 4, 3, 3 }), "Schismatic", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 4, 3, 4, 3, 4, 3, 3 }), "Miracle", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 1 }), "Magic", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 3, 5, 3, 3, 3, 5, 3, 3, 3, 5, 3, 3, 5 }), "Hanson", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3 }), "Tricot", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 1, 2, 1, 2, 1, 2, 1 }), "BP Lambda", true), -1, nullptr);
		modeLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 1, 2, 1, 2, 1, 1, 2 }), "BP Dur II", true), -1, nullptr);

	
	if (pluginSettings->getCreatePresetFolder())
	{
		File presetDirectory = File(pluginSettings->getPresetPath());
		Array<File> filesToLoad = presetDirectory.findChildFiles(File::TypesOfFileToFind::findFiles, "*.svk");

		std::unique_ptr<XmlElement> xml;
		ValueTree loadedModeNode;
		Mode* loadedMode;

		while (filesToLoad.size > 0)
		{
			xml = parseXML(filesToLoad.removeAndReturn(0));
			loadedModeNode = ValueTree::fromXml(*(xml.get()));

			if (loadedModeNode.hasType(IDs::modePresetNode))
			{
				modeLibraryNode.addChild(loadedModeNode, -1, nullptr);
			}
		}
	}
}
