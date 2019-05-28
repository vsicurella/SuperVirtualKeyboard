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

	pluginStateNode = ValueTree(IDs::pluginStateNode);

	pluginSettings.reset(new SvkPluginSettings());
	pluginSettingsNode = pluginSettings->pluginSettingsNode;
	pluginStateNode.addChild(pluginSettingsNode, -1, nullptr);

	presetManager.reset(new SvkPresetManager(pluginSettingsNode));
	presetLibraryNode = presetManager->presetLibraryNode;
	pluginStateNode.addChild(presetLibraryNode, -1, nullptr);

	presetManager->createFactoryPresets();
	modeLoaded = presetManager->getModeLoaded();
	modePresetNode = modeLoaded->modeNode;

    midiProcessor.reset(new SvkMidiProcessor());
	midiSettingsNode = midiProcessor->midiSettingsNode;
	pluginStateNode.addChild(midiSettingsNode, -1, nullptr);

	textFilterIntOrSpace.reset(new TextFilterIntOrSpace());
	textFilterInt.reset(new TextFilterInt());
}

SvkMidiProcessor* SvkPluginState::getMidiProcessor()
{
    return midiProcessor.get();
}

UndoManager* SvkPluginState::getUndoManager()
{
	return undoManager.get();
}

SvkPreset* SvkPluginState::getPresetLoaded()
{
	return presetCurrent.get();
}

Mode* SvkPluginState::getModeLoaded()
{
	return modeLoaded;
}

int* SvkPluginState::getMidiInputMap()
{
    return midiProcessor->getInputRemapper()->getRemappedNotes();
}
int* SvkPluginState::getMidiOutputMap()
{
    return midiProcessor->getOutputRemapper()->getRemappedNotes();
}

void SvkPluginState::loadMode(int presetIndexIn)
{
	/*
	std::unique_ptr<Mode> mode(new Mode(presetLibraryNode.getChild(presetIndexIn), midiProcessor->getRootNote()));
	
	if (mode->modeNode.isValid())
	{
		modeLoaded.swap(mode);

		if (presetCurrent->theKeyboardNode.isValid())
			presetCurrent->theKeyboardNode.setProperty(IDs::pianoHasCustomColor, false, nullptr);

		presetCurrent->updateModeNode(modeLoaded->modeNode);
		presetCurrent->parentNode.setProperty(IDs::libraryIndexOfMode, presetIndexIn, undoManager.get());

		modePresetNode.copyPropertiesAndChildrenFrom(modeLoaded->modeNode, nullptr);
        sendChangeMessage();
	}
	*/
	presetManager->loadPreset(presetIndexIn);
}

void SvkPluginState::loadMode(ValueTree modeNodeIn)
{
	/*
	if (modeNodeIn.hasType(IDs::modePresetNode))
	{
		modeLoaded.reset(new Mode(modeNodeIn, midiProcessor->getRootNote()));

		if (presetCurrent->theKeyboardNode.isValid())
			presetCurrent->theKeyboardNode.setProperty(IDs::pianoHasCustomColor, false, nullptr);
		
		presetCurrent->updateModeNode(modeLoaded->modeNode);
		presetCurrent->parentNode.setProperty(IDs::libraryIndexOfMode, 0, undoManager.get());

		modePresetNode = modeLoaded->modeNode;
		sendChangeMessage();
	}
	*/
	presetManager->loadPreset(modeNodeIn);
}

void SvkPluginState::setMidiRootNote(int rootNoteIn)
{
	rootNoteIn = totalModulus(rootNoteIn, 128);
	midiProcessor->setRootNote(rootNoteIn);
	modeLoaded->setRootNote(rootNoteIn);
}

void SvkPluginState::updateKeyboardSettingsPreset()
{
	if (pianoNode.isValid())
	{
		presetCurrent->updateKeyboardNode(pianoNode);
	}
}


bool SvkPluginState::savePreset()
{
	/*
    midiProcessor->updateNode();
	modePresetNode = modeLoaded->modeNode;
	presetCurrent->updateParentNode(pluginStateNode);

	return presetCurrent->writeToFile();
	*/
	return presetManager->savePreset();
}


bool SvkPluginState::loadPreset()
{
	/*
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
			modeLoaded->restoreNode(presetCurrent->theModeNode, midiProcessor->getRootNote());
			modePresetNode = modeLoaded->modeNode;
		}
		
		if (presetCurrent->theKeyboardNode.isValid())
		{
			pianoNode.copyPropertiesAndChildrenFrom(presetCurrent->theKeyboardNode, nullptr);
		}

		return true;
	}
	*/

	return presetManager->loadPreset();;
}