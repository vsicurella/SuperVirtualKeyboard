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
	: AudioProcessorEditor(&p), processor(p), appCmdMgr(cmdMgr),
	pluginState(processor.get_plugin_state()),
    view(new Viewport("Piano Viewport")),
	piano(new Keyboard(pluginState, appCmdMgr)),
	keyboardEditorBar(new KeyboardEditorBar(pluginState, appCmdMgr))
{
	setName("Super Virtual Keyboard");
	setResizable(true, true);
	setResizeLimits(640, 100, 10e4, 10e4);
	setBroughtToFrontOnMouseClick(true);
	
	keyboardEditorBar.get()->setName("Keyboard Editor Bar");
	keyboardEditorBar.get()->setSize(640, 48);
	addAndMakeVisible(keyboardEditorBar.get());
    
	piano.get()->setName("The Piano");

	addAndMakeVisible(view.get());
	view.get()->setViewedComponent(piano.get());
	view.get()->setTopLeftPosition(1, 49);

	view.get()->setViewPositionProportionately(0.6, 0);
	processor.set_midi_input_state(&externalMidi);
	externalMidi.addListener(piano.get());

	openFileBox.reset(new FilenameComponent("OpenFile", {}, false, false, false, ".svk", "", "Load preset..."));
	saveFileBox.reset(new FilenameComponent("SaveFile", {}, true, false, true, ".svk", "", "Load preset..."));

	pluginState->presetCurrentNode.addListener(this);
	piano->getNode().addListener(this);

    if (!pluginState->keyboardWindowNode.isValid())
    {
        init_node_data();
    }
	else
	{
		restore_node_data(pluginState->keyboardWindowNode);
	}
    
    colorChooserWindow.reset(new ColorChooserWindow("Color Chooser", Colours::slateblue, DocumentWindow::closeButton));
    colorChooserWindow->setSize(450, 450);
    colorChooserWindow->addToDesktop();
	colorChooserWindow->addChangeListener(this);
	
	appCmdMgr->registerAllCommandsForTarget(this);
	appCmdMgr->registerAllCommandsForTarget(piano.get());

	setMouseClickGrabsKeyboardFocus(true);
	addMouseListener(this, true);

    setSize(1000, 250);
	
	startTimerHz(20);
}

SuperVirtualKeyboardAudioProcessorEditor::~SuperVirtualKeyboardAudioProcessorEditor()
{
	pluginState->presetCurrentNode.removeListener(this);
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::init_node_data()
{
	keyboardWindowNode = ValueTree(IDs::keyboardWindowNode);

	keyboardWindowNode.setProperty(IDs::windowBoundsW, getWidth(), nullptr);
	keyboardWindowNode.setProperty(IDs::windowBoundsH, getHeight(), nullptr);
	keyboardWindowNode.setProperty(IDs::viewportPosition, view.get()->getViewPositionX(), nullptr);

	// setup initial preset, needed for piano node to initiate
	pluginState->set_current_mode(8);

	piano->initiateDataNode();
	keyboardWindowNode.addChild(pluginState->pianoNode, 0, nullptr);

	pluginState->keyboardWindowNode = keyboardWindowNode;
	pluginState->pluginStateNode.addChild(keyboardWindowNode, 2, nullptr);
}

void SuperVirtualKeyboardAudioProcessorEditor::restore_node_data(ValueTree nodeIn)
{
	keyboardWindowNode = nodeIn;
	update_children_to_preset();
	setSize(keyboardWindowNode[IDs::windowBoundsW], keyboardWindowNode[IDs::windowBoundsH]);
	view.get()->setViewPosition((int)keyboardWindowNode[IDs::viewportPosition], 0);
}

bool SuperVirtualKeyboardAudioProcessorEditor::save_preset(const File& fileOut)
{
	std::unique_ptr<XmlElement> xml(pluginState->presetCurrentNode.createXml());
	return xml->writeToFile(fileOut, "");
}
	

bool SuperVirtualKeyboardAudioProcessorEditor::load_preset(const File& fileIn)
{
	ValueTree presetIn;

	if (fileIn.exists())
	{
		std::unique_ptr<XmlElement> xml = parseXML(fileIn);
		presetIn = ValueTree::fromXml(*(xml.get()));
		
		if (presetIn.hasType(IDs::presetNode))
		{
			pluginState->presetCurrentNode.removeAllChildren(pluginState->get_undo_mgr());
			pluginState->presetCurrentNode = presetIn;
			pluginState->get_current_mode()->restore_from_node(presetIn.getChildWithName(IDs::modePresetNode));
		}

		update_children_to_preset();
	}

	return presetIn.hasType(IDs::presetNode);
}

bool SuperVirtualKeyboardAudioProcessorEditor::write_reaper_file()
{
	ReaperWriter rpp = ReaperWriter(pluginState->get_current_mode());
	return rpp.write_file();
}

void SuperVirtualKeyboardAudioProcessorEditor::update_children_to_preset()
{
	Mode* modeCurrent = pluginState->get_current_mode();

	if (pluginState->presetCurrentNode.getChild(1).isValid())
	{
		piano->restoreDataNode(pluginState->presetCurrentNode.getChild(1));
	}
	else
		piano->resetKeyColors();

	piano->applyMode(modeCurrent);

	keyboardEditorBar->set_mode_readout_text(modeCurrent->getStepsString());
	keyboardEditorBar->set_mode_library_text(modeCurrent->getDescription());
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::paint(Graphics& g)
{
	
}

void SuperVirtualKeyboardAudioProcessorEditor::resized()
{
	AudioProcessorEditor::resized();

	keyboardEditorBar->setBounds(0, 6, getWidth(), 36);
	view->setBounds(0, keyboardEditorBar->getBottom(), getWidth(), getHeight() - keyboardEditorBar->getHeight() - view.get()->getScrollBarThickness());
	piano->setBounds(0, 0, piano->getWidthFromHeight(view->getMaximumVisibleHeight()), view->getMaximumVisibleHeight());

	view->setViewPositionProportionately(0.618, 0);
	
	// update node
	if (keyboardWindowNode.isValid())
	{
		keyboardWindowNode.setProperty(IDs::windowBoundsW, getWidth(), nullptr);
		keyboardWindowNode.setProperty(IDs::windowBoundsH, getHeight(), nullptr);
		keyboardWindowNode.setProperty(IDs::viewportPosition, view.get()->getViewArea().getX(), nullptr);
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
			if (e.mods.isShiftDown())
				piano->setKeyColorOrder(key->order, 3, colorChooserWindow->getColorSelected());
			else if (e.mods.isCtrlDown())
				piano->setKeyColor(key->keyNumber, 3, colorChooserWindow->getColorSelected());
			else
				piano->setKeyColorDegree(key->keyNumber, 3, colorChooserWindow->getColorSelected());
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
	Component* changeSource = dynamic_cast<Component*>(source);
	DBG("color change callback");
	if (changeSource->getName() == "Color Chooser")
	{
		if (piano->getUIMode() == UIMode::editMode)
		{
			// Commit Color Changes
			for (int i = 0; i < 128; i++)
			{
				piano->setKeyColor(i, 0, piano->getKey(i)->findColour(3));
				piano->setKeyColor(i, 1, piano->getKey(i)->findColour(0).contrasting(0.25));
				piano->setKeyColor(i, 2, piano->getKey(i)->findColour(0).contrasting(0.75));
			}

			piano->updateKeyNodes();
			
			// Update Preset Node
			pluginState->presetCurrentNode.removeChild(1, pluginState->get_undo_mgr());
			pluginState->presetCurrentNode.addChild(piano->getNode().createCopy(), 1, pluginState->get_undo_mgr());

			piano->setUIMode(UIMode::playMode);
		}
	}
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged.hasType(IDs::presetNode) && property == IDs::modeOffset)
    {
        piano->applyMode(pluginState->get_current_mode());
    }
}

void SuperVirtualKeyboardAudioProcessorEditor::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
{
	if (parentTree.hasType(IDs::presetNode))
	{
		if (childWhichHasBeenAdded.hasType(IDs::modePresetNode))
		{
			update_children_to_preset();

			if (getWidth() > piano->getWidth())
			{
				//setSize(getWidth(), (piano->getWidth() / (pluginState->get_current_mode()->get_num_modal_notes() * (float)keyboardWindowNode[IDs::pianoWHRatio])));
			}
		}
	}
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
		save_preset(fileDialog("Save your preset..", true));
		break;
	case IDs::CommandIDs::loadCustomLayout:
		load_preset(fileDialog("Please select a preset to load...", false));
		break;
	case IDs::CommandIDs::saveReaperMap:
		write_reaper_file();
		break;
	case IDs::CommandIDs::setKeyColor:
		colorChooserWindow->setVisible(true);
		piano->setUIMode(UIMode::editMode);
		break;
	default:
		return false;
	}
	return true;
}

