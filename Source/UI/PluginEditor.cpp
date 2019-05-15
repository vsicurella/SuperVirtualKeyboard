/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "../PluginProcessor.h"

using namespace VirtualKeyboard;

//==============================================================================
SuperVirtualKeyboardAudioProcessorEditor::SuperVirtualKeyboardAudioProcessorEditor(SuperVirtualKeyboardAudioProcessor& p, ApplicationCommandManager* cmdMgr)
	: AudioProcessorEditor(&p), processor(p), appCmdMgr(cmdMgr), pluginState(processor.get_plugin_state())
{
	setName("Super Virtual Keyboard");
	setResizable(true, true);
	setBroughtToFrontOnMouseClick(true);
	
	keyboardEditorBar.reset(new KeyboardEditorBar(pluginState, appCmdMgr));
	keyboardEditorBar.get()->setName("Keyboard Editor Bar");
	keyboardEditorBar.get()->setSize(640, 48);
	addAndMakeVisible(keyboardEditorBar.get());
    
	piano.reset(new Keyboard(pluginState, appCmdMgr));
	piano.get()->setName("The Piano");

	view.reset(new Viewport("Piano Viewport"));
	addAndMakeVisible(view.get());
	view.get()->setViewedComponent(piano.get());
	view.get()->setTopLeftPosition(1, 49);

	processor.set_midi_input_state(&externalMidi);
	externalMidi.addListener(piano.get());

	openFileBox.reset(new FilenameComponent("OpenFile", {}, false, false, false, ".svk", "", "Load preset..."));
	saveFileBox.reset(new FilenameComponent("SaveFile", {}, true, false, true, ".svk", "", "Load preset..."));
    
    colorChooserWindow.reset(new ColorChooserWindow("Color Chooser", Colours::slateblue, DocumentWindow::closeButton));
    colorChooserWindow->setSize(450, 450);
    colorChooserWindow->addToDesktop();
	colorChooserWindow->addChangeListener(this);

    pluginState->addChangeListener(this);
	pluginState->pluginStateNode.addListener(this);
	initNodeData();

	appCmdMgr->registerAllCommandsForTarget(this);
	appCmdMgr->registerAllCommandsForTarget(piano.get());

	setMouseClickGrabsKeyboardFocus(true);
	addMouseListener(this, true);

    setSize(1000, 250);
	setResizeLimits(640, 100, 10e4, 10e4);

	startTimerHz(20);
}

SuperVirtualKeyboardAudioProcessorEditor::~SuperVirtualKeyboardAudioProcessorEditor()
{
	pluginState->pluginStateNode.removeListener(this);
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::initNodeData()
{
	if (pluginState->keyboardWindowNode.isValid())
	{
		keyboardWindowNode = pluginState->keyboardWindowNode;
		update_children_to_preset();
		setSize(keyboardWindowNode[IDs::windowBoundsW], keyboardWindowNode[IDs::windowBoundsH]);
		view.get()->setViewPosition((int)keyboardWindowNode[IDs::viewportPosition], 0);
	}
	else
	{
		keyboardWindowNode = ValueTree(IDs::keyboardWindowNode);
		
		pluginState->setCurrentMode(8);
		updateNodeData();

		keyboardWindowNode.addChild(pluginState->pianoNode, 0, nullptr);
		pluginState->keyboardWindowNode = keyboardWindowNode;
		pluginState->pluginStateNode.addChild(keyboardWindowNode, 2, nullptr);

		view.get()->setViewPositionProportionately(0.52, 0);
	}
}

void SuperVirtualKeyboardAudioProcessorEditor::updateNodeData()
{
	keyboardWindowNode.setProperty(IDs::windowBoundsW, getWidth(), nullptr);
	keyboardWindowNode.setProperty(IDs::windowBoundsH, getHeight(), nullptr);
	keyboardWindowNode.setProperty(IDs::viewportPosition, view.get()->getViewPositionX(), nullptr);
}



void SuperVirtualKeyboardAudioProcessorEditor::update_children_to_preset()
{
	Mode* modeCurrent = pluginState->getCurrentMode();
	
	if (pluginState->getCurrentPreset()->theModeNode.getProperty(IDs::factoryPreset))
	{
	}
		
	piano->applyMode(modeCurrent);

	keyboardEditorBar->setModeReadoutText(modeCurrent->getStepsString());
	keyboardEditorBar->setModeLibraryText(modeCurrent->getDescription());
    keyboardEditorBar->setOffsetReadout(modeCurrent->getOffset());
	DBG("Children Updated");
}

void SuperVirtualKeyboardAudioProcessorEditor::beginColorEditing()
{
	colorChooserWindow->setVisible(true);
	piano->setUIMode(UIMode::editMode);
	keyboardEditorBar->allowUserInput(false);
}

//==============================================================================


bool SuperVirtualKeyboardAudioProcessorEditor::save_preset()
{
	piano->updatePianoNode();
	pluginState->updateKeyboardSettingsPreset();
	bool written = pluginState->savePreset();
	if (written)
		DBG("file was saved");
	else
		DBG("not saved");

	return written;
}

bool SuperVirtualKeyboardAudioProcessorEditor::load_preset()
{
	bool loaded = pluginState->loadPreset();

	if (loaded)
	{
		piano->restoreDataNode(pluginState->getCurrentPreset()->theKeyboardNode);
		update_children_to_preset();
	}

	return loaded;
}

bool SuperVirtualKeyboardAudioProcessorEditor::write_reaper_file()
{
	ReaperWriter rpp = ReaperWriter(pluginState->getCurrentMode());
	return rpp.write_file();
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::paint(Graphics& g)
{
	
}

void SuperVirtualKeyboardAudioProcessorEditor::resized()
{
	int viewPositionKeyboardX = view->getViewPositionX();
	AudioProcessorEditor::resized();

	keyboardEditorBar->setBounds(0, 6, getWidth(), 36);
	view->setBounds(0, keyboardEditorBar->getBottom(), getWidth(), getHeight() - keyboardEditorBar->getHeight() - view.get()->getScrollBarThickness());
	piano->setBounds(0, 0, piano->getWidthFromHeight(view->getMaximumVisibleHeight()), view->getMaximumVisibleHeight()-1);
	
	view->setViewPosition(viewPositionKeyboardX, 0);

	// update node
	if (keyboardWindowNode.isValid())
	{
		updateNodeData();
	}
	
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::timerCallback()
{
	piano.get()->getMidiKeyboardState()->processNextMidiBuffer(
		*processor.get_midi_buffer(), 0, 4096, true);

	piano.get()->repaint();
}

void SuperVirtualKeyboardAudioProcessorEditor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	externalMidi.processNextMidiEvent(message);
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::userTriedToCloseWindow()
{
	if (colorChooserWindow->isVisible())
		colorChooserWindow->closeButtonPressed();

	setVisible(false);
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::mouseDown(const MouseEvent& e)
{
	if (piano->getUIMode() == UIMode::playMode)
	{
		Key* key = piano->getKeyFromPosition(e);
		if (key)
		{
			if (e.mods.isShiftDown() && !e.mods.isAltDown() && key->activeState == 2)
			{
				// note off
				//piano->lastKeyClicked = 0;
				piano->triggerKeyNoteOff(key);
			}
			else
			{
				if (e.mods.isAltDown())
				{
					Key* oldKey = piano->getKey(piano->getLastKeyClicked());
					piano->triggerKeyNoteOff(oldKey);
				}

				piano->triggerKeyNoteOn(key, piano->getKeyVelocity(key, e));
				piano->setLastKeyClicked(key->keyNumber);
			}
		}
	}
	else if (piano->getUIMode() == UIMode::editMode)
	{
		Key* key = piano->getKeyFromPosition(e);
		if (key)
		{
			if (e.mods.isRightButtonDown())
			{
				if (e.mods.isShiftDown())
				{
					piano->resetKeyOrderColors(key->order, true);
				}
				else if (e.mods.isCtrlDown())
				{
					piano->resetKeySingleColor(key->keyNumber);
				}
				else
				{
					piano->resetKeyDegreeColors(key->keyNumber);
				}
			}

			else if (e.mods.isShiftDown())
			{
				if (piano->getKeyDegreeColor(key->scaleDegree).isOpaque())
					piano->resetKeyDegreeColors(key->scaleDegree);

				else if (piano->getKeySingleColor(key->keyNumber).isOpaque())
					piano->resetKeySingleColor(key->keyNumber);

				piano->setKeyColorOrder(key->order, 3, colorChooserWindow->getColorSelected());
			}
			else if (e.mods.isCtrlDown())
				piano->setKeyColor(key->keyNumber, 3, colorChooserWindow->getColorSelected());
			else
				piano->setKeyColorDegree(key->scaleDegree, 3, colorChooserWindow->getColorSelected());
		}
	}
}

void SuperVirtualKeyboardAudioProcessorEditor::mouseDrag(const MouseEvent& e)
{
	if (piano->getUIMode() == UIMode::playMode)
	{
		Key* key = piano->getKeyFromPosition(e);

		if (key)
		{
			if (key->keyNumber != piano->getLastKeyClicked())
			{
				Key* oldKey = piano->getKey(piano->getLastKeyClicked());
				if (!e.mods.isShiftDown() || e.mods.isAltDown())
				{
					piano->triggerKeyNoteOff(oldKey);
				}

				piano->triggerKeyNoteOn(key, piano->getKeyVelocity(key, e));
				piano->setLastKeyClicked(key->keyNumber);
				repaint();
			}
		}
	}
}

void SuperVirtualKeyboardAudioProcessorEditor::mouseUp(const MouseEvent& e)
{
	if (piano->getUIMode() == UIMode::playMode)
	{
		Key* key = piano->getKeyFromPosition(e);

		if (key)
		{
			if (!e.mods.isShiftDown())
			{
				piano->triggerKeyNoteOff(key);
				key->activeState = 1;
				repaint();
			}
		}
	}
}

void SuperVirtualKeyboardAudioProcessorEditor::mouseMove(const MouseEvent& e)
{
	if (piano->getUIMode() == UIMode::playMode)
	{
		Key* key = piano->getKeyFromPosition(e);

		if (key)
		{
			if (key->activeState == 0)
			{
				key->activeState = 1;
				repaint();
			}
		}
	}
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	//Component* changeSource = dynamic_cast<Component*>(source);

    if (source == pluginState)
    {
        piano->resetKeyColors(true);
        piano->updatePianoNode();
        update_children_to_preset();
    }
    
	if (source == colorChooserWindow.get())
	{
		if (piano->getUIMode() == UIMode::editMode)
		{
			piano->updatePianoNode();
			pluginState->updateKeyboardSettingsPreset();

			piano->applyMode(pluginState->getCurrentMode());
			piano->setUIMode(UIMode::playMode);
			keyboardEditorBar->allowUserInput();
		}
	}
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
	// The Mode offset is changed
	if (treeWhosePropertyHasChanged.hasType(IDs::modePresetNode) && property == IDs::modeOffset)
	{
		update_children_to_preset();
	}
}

void SuperVirtualKeyboardAudioProcessorEditor::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
{

}

void SuperVirtualKeyboardAudioProcessorEditor::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved)
{

}

void SuperVirtualKeyboardAudioProcessorEditor::valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{

}

void SuperVirtualKeyboardAudioProcessorEditor::valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged)
{

}

//==============================================================================


void SuperVirtualKeyboardAudioProcessorEditor::filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged)
{
	if (fileComponentThatHasChanged == openFileBox.get())
	{
		
	}

	if (fileComponentThatHasChanged == saveFileBox.get())
	{

	}
}

File SuperVirtualKeyboardAudioProcessorEditor::fileDialog(String message, bool forSaving)
{
	FileChooser chooser(message, File::getSpecialLocation(File::userDocumentsDirectory), "*.svk");

	if (forSaving)
		chooser.browseForFileToSave(true);
	else
		chooser.browseForFileToOpen();

	return chooser.getResult();
}

//==============================================================================

ApplicationCommandTarget* SuperVirtualKeyboardAudioProcessorEditor::getNextCommandTarget()
{
	return piano.get();// findFirstTargetParentComponent();
}

void SuperVirtualKeyboardAudioProcessorEditor::getAllCommands(Array< CommandID > &c)
{
	Array<CommandID> commands{
		IDs::CommandIDs::saveCustomLayout,
		IDs::CommandIDs::loadCustomLayout,
		IDs::CommandIDs::saveReaperMap,
		IDs::CommandIDs::setKeyColor,
	};

	c.addArray(commands);
}

void SuperVirtualKeyboardAudioProcessorEditor::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
{
	switch (commandID)
	{
	case IDs::CommandIDs::saveCustomLayout:
		result.setInfo("Save Layout", "Save your custom layout to a file.", "Piano", 0);
		//result.setTicked(pianoOrientationSelected == PianoOrientation::horizontal);
		//result.addDefaultKeypress('c', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::loadCustomLayout:
		result.setInfo("Load Layout", "Load a custom layout from a file.", "Piano", 0);
		//result.setTicked(pianoOrientationSelected == PianoOrientation::verticalLeft);
		//result.addDefaultKeypress('a', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::saveReaperMap:
		result.setInfo("Save Reaper Note Names", "Save the current layout as a Reaper MIDI Note Name text file.", "Piano", 0);
		//result.setTicked(pianoOrientationSelected == PianoOrientation::verticalRight);
		//result.addDefaultKeypress('d', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::setKeyColor:
		result.setInfo("Change Keyboard Colors", "Allows you to change the default colors for the rows of keys.", "Piano", 0);
		//result.addDefaultKeypress('c', ModifierKeys::shiftModifier);
		break;
	default:
		break;
	}
}

bool SuperVirtualKeyboardAudioProcessorEditor::perform(const InvocationInfo &info)
{
	switch (info.commandID)
	{
	case IDs::CommandIDs::saveCustomLayout:
		save_preset();
		break;
	case IDs::CommandIDs::loadCustomLayout:
		load_preset();
		break;
	case IDs::CommandIDs::saveReaperMap:
		write_reaper_file();
		break;
	case IDs::CommandIDs::setKeyColor:
		beginColorEditing();
		break;
	default:
		return false;
	}
	return true;
}

