/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "../PluginProcessor.h"

//==============================================================================
SuperVirtualKeyboardAudioProcessorEditor::SuperVirtualKeyboardAudioProcessorEditor(SuperVirtualKeyboardAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p),
	pluginState(processor.get_plugin_state()),
	piano(new VirtualKeyboard(&appCmdMgr, nullptr)),
	view(new Viewport("Piano Viewport")),
	keyboardEditorBar(new KeyboardEditorBar(pluginState, &appCmdMgr))
{
	setName("Super Virtual Piano");
	setResizable(true, true);
	setResizeLimits(640, 100, 10e4, 10e4);
	setBroughtToFrontOnMouseClick(true);
	
	keyboardEditorBar.get()->setName("Scale Edit Popup");
	keyboardEditorBar.get()->setSize(640, 48);
	addAndMakeVisible(keyboardEditorBar.get());
    
	piano.get()->setName("The Piano");

	addAndMakeVisible(view.get());
	view.get()->setViewedComponent(piano.get());
	view.get()->setTopLeftPosition(1, 49);

	view.get()->setViewPositionProportionately(0.6, 0);
	processor.set_midi_input_state(&externalMidi);
	externalMidi.addListener(piano.get());
    
    if (!pluginState->keyboardWindowNode.isValid())
    {
        init_node_data();
    }

    restore_node_data(pluginState->keyboardWindowNode);

	appCmdMgr.registerAllCommandsForTarget(piano.get());
	appCmdMgr.registerAllCommandsForTarget(this);
    
    setSize(1000, 250);
	
	startTimerHz(20);
}

SuperVirtualKeyboardAudioProcessorEditor::~SuperVirtualKeyboardAudioProcessorEditor()
{
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::paint(Graphics& g)
{
	
}

void SuperVirtualKeyboardAudioProcessorEditor::resized()
{
	AudioProcessorEditor::resized();

	keyboardEditorBar->setBounds(0, 6, getWidth(), 36);
	view.get()->setBounds(0, keyboardEditorBar->getBottom(), getWidth(), getHeight() - keyboardEditorBar->getHeight() - view.get()->getScrollBarThickness());
	piano.get()->setSize(piano.get()->getWidth(), view.get()->getMaximumVisibleHeight());

	view.get()->setViewPositionProportionately(0.618, 0);
	
	// update node
	if (keyboardWindowNode.isValid())
	{
		keyboardWindowNode.setProperty(IDs::windowBoundsW, getWidth(), nullptr);
		keyboardWindowNode.setProperty(IDs::windowBoundsH, getHeight(), nullptr);
		keyboardWindowNode.setProperty(IDs::viewportPosition, view.get()->getViewArea().getX(), nullptr);
	}
	
}

bool SuperVirtualKeyboardAudioProcessorEditor::keyPressed(const KeyPress& key)
{
	if (key == KeyPress::returnKey)
	{

		return true;
	}

	return false;
}


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

void SuperVirtualKeyboardAudioProcessorEditor::focusGained(FocusChangeType changeType)
{
	setWantsKeyboardFocus(true);
}

void SuperVirtualKeyboardAudioProcessorEditor::userTriedToCloseWindow()
{
	setVisible(false);
}

void SuperVirtualKeyboardAudioProcessorEditor::visibilityChanged()
{
	if (isVisible())
	{
		setWantsKeyboardFocus(true);
	}
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

	load_preset(pluginState->presetCurrentNode);
}

//==============================================================================


bool SuperVirtualKeyboardAudioProcessorEditor::load_preset(ValueTree presetIn, bool updateKeyboardMenuBar)
{
    if (presetIn.hasType(IDs::modePresetNode))
    {
        
    }
    
	return false;
}


//==============================================================================


ApplicationCommandTarget* SuperVirtualKeyboardAudioProcessorEditor::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void SuperVirtualKeyboardAudioProcessorEditor::getAllCommands(Array< CommandID > &c)
{
	Array<CommandID> commands{
		IDs::CommandIDs::saveCustomLayout,
		IDs::CommandIDs::loadCustomLayout,
		IDs::CommandIDs::saveReaperMap };

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
	default:
		break;
	}
}

bool SuperVirtualKeyboardAudioProcessorEditor::perform(const InvocationInfo &info)
{
	switch (info.commandID)
	{
	case IDs::CommandIDs::saveCustomLayout:
		// TBI
		break;
	case IDs::CommandIDs::loadCustomLayout:
		// TBI
		break;
	case IDs::CommandIDs::saveReaperMap:
		// TBI
		break;
	default:
		return false;
	}
	return true;
}
