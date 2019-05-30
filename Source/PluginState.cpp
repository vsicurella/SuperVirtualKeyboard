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
    
    midiProcessor.reset(new SvkMidiProcessor());
    midiSettingsNode = midiProcessor->midiSettingsNode;
    pluginStateNode.addChild(midiSettingsNode, -1, nullptr);

	presetManager.reset(new SvkPresetManager(pluginSettingsNode));
	presetLibraryNode = presetManager->presetLibraryNode;
	pluginStateNode.addChild(presetLibraryNode, -1, nullptr);
	presetManager->addChangeListener(this);

	modeLoaded.reset(new Mode());

	virtualKeyboard.reset(new VirtualKeyboard::Keyboard(this));
	virtualKeyboard->addListener(midiProcessor.get());
	pianoNode = virtualKeyboard->getNode();

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
	return modeLoaded.get();
}

VirtualKeyboard::Keyboard* SvkPluginState::getKeyboard()
{
	return virtualKeyboard.get();
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
	presetManager->loadPreset(presetIndexIn);
}

void SvkPluginState::loadMode(ValueTree modeNodeIn)
{
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
	return presetManager->savePreset();
}


bool SvkPluginState::loadPreset()
{
	return presetManager->loadPreset();
}

void SvkPluginState::changeListenerCallback(ChangeBroadcaster* source)
{
	// Preset loaded
	if (source = presetManager.get())
	{
		modeLoaded->restoreNode(presetManager->presetNode, midiProcessor->getRootNote());
	}
}
