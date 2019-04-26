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
	piano(new Keyboard(pluginState, appCmdMgr)),
	view(new Viewport("Piano Viewport")),
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

    if (!pluginState->keyboardWindowNode.isValid())
    {
        init_node_data();
    }
	else
	{
		restore_node_data(pluginState->keyboardWindowNode);
	}

	update_children_to_preset();

	colorChooserWindow.reset(new DocumentWindow("Color Chooser Window", Colours::slategrey, DocumentWindow::closeButton));
	addChildComponent(colorChooserWindow.get());
	colorChooserWindow->setSize(300, 650);
	colorChooserWindow->addToDesktop();

	colorChooser.reset(new ColorChooserWindow(pluginState->get_current_mode()));
	colorChooserWindow->setContentComponent(colorChooser.get());

	appCmdMgr->registerAllCommandsForTarget(this);
	appCmdMgr->registerAllCommandsForTarget(piano.get());

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

	piano->init_data_node();
	pluginState->pianoNode = piano->get_node();
	keyboardWindowNode.addChild(pluginState->pianoNode, 0, nullptr);

	pluginState->keyboardWindowNode = keyboardWindowNode;
	pluginState->pluginStateNode.addChild(keyboardWindowNode, 2, nullptr);
}

void SuperVirtualKeyboardAudioProcessorEditor::restore_node_data(ValueTree nodeIn)
{
	keyboardWindowNode = nodeIn;
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

	piano->apply_mode_layout(modeCurrent);
	
	keyboardEditorBar->set_mode_readout_text(modeCurrent->getStepsString());

	//if ((int)pluginState->presetCurrentNode[IDs::indexOfMode] != 0)
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
	piano.get()->get_keyboard_state()->processNextMidiBuffer(
		*processor.get_midi_buffer(), 0, 4096, true);

	piano.get()->repaint();
}

void SuperVirtualKeyboardAudioProcessorEditor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	externalMidi.processNextMidiEvent(message);
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::focusGained(FocusChangeType changeType)
{
	setWantsKeyboardFocus(false);
}

void SuperVirtualKeyboardAudioProcessorEditor::userTriedToCloseWindow()
{
	setVisible(false);
}

bool SuperVirtualKeyboardAudioProcessorEditor::keyPressed(const KeyPress& key)
{
	if (key == KeyPress::returnKey)
	{

		return true;
	}

	return false;
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged.hasType(IDs::presetNode) && property == IDs::modeOffset)
    {
        piano->apply_mode_layout(pluginState->get_current_mode());
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
		break;
	default:
		return false;
	}
	return true;
}

