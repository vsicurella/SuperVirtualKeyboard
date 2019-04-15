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
	appCmdMgr(processor.get_app_cmd_mgr()),
	piano(new VirtualKeyboard(appCmdMgr.get(), nullptr)),
	view(new Viewport("Piano Viewport")),
	scaleEdit(new KeyboardMenuBar(processor.get_presets(), processor.get_presets_sorted(), appCmdMgr.get()))
{
	setName("Super Virtual Piano");
	setResizable(true, true);
	setResizeLimits(640, 100, 10e4, 10e4);
	setBroughtToFrontOnMouseClick(true);
	
	scaleEdit.get()->setName("Scale Edit Popup");
	scaleEdit.get()->addChangeListener(this);
	scaleEdit.get()->setSize(640, 48);
	addAndMakeVisible(scaleEdit.get());
    
	piano.get()->setName("The Piano");

	addAndMakeVisible(view.get());
	view.get()->setViewedComponent(piano.get());
	view.get()->setTopLeftPosition(1, 49);

	pluginState = processor.get_plugin_state();

	view.get()->setViewPositionProportionately(0.6, 0);
	processor.set_midi_input_state(&externalMidi);
	externalMidi.addListener(piano.get());
    
    if (!pluginState->keyboardWindowNode.isValid())
    {
        init_node_data();
    }

    restore_node_data();

	appCmdMgr->registerAllCommandsForTarget(piano.get());
	appCmdMgr->registerAllCommandsForTarget(this);
    
    setSize(1000, 250);
	
	startTimerHz(20);
}

SuperVirtualKeyboardAudioProcessorEditor::~SuperVirtualKeyboardAudioProcessorEditor()
{
//	appCmdMgr->clearCommands();
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::paint(Graphics& g)
{
	
}

void SuperVirtualKeyboardAudioProcessorEditor::resized()
{
	AudioProcessorEditor::resized();

	scaleEdit->setBounds(0, 6, getWidth(), 36);
	view.get()->setBounds(0, scaleEdit->getBottom(), getWidth(), getHeight() - scaleEdit->getHeight() - view.get()->getScrollBarThickness());
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

void SuperVirtualKeyboardAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
    ModeLayout* newPreset = scaleEdit.get()->presetSelected;
    
	if (keyboardWindowNode.isValid() && newPreset)
	{
		keyboardWindowNode.setProperty(IDs::selectedPresetName, scaleEdit.get()->get_preset_name(), nullptr);
		keyboardWindowNode.setProperty(IDs::selectedPresetIndex, scaleEdit.get()->get_preset_id(newPreset->get_full_name()), nullptr);
		keyboardWindowNode.setProperty(IDs::selectedPresetComboID, scaleEdit.get()->get_selected_preset_id(), nullptr);

		if (scaleEdit.get()->presetSelected->family == "Custom")
			processor.set_current_preset(0);
		else
			processor.set_current_preset(keyboardWindowNode[IDs::selectedPresetIndex]);
        
        piano.get()->apply_layout(newPreset);
	}
    std::cout << keyboardWindowNode.toXmlString() << std::endl;
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
	pluginState->keyboardWindowNode = ValueTree(IDs::keyboardWindowNode);
	keyboardWindowNode = pluginState->keyboardWindowNode;
	presetCurrentNode = pluginState->presetCurrentNode;

	keyboardWindowNode.setProperty(IDs::windowBoundsW, getWidth(), nullptr);
	keyboardWindowNode.setProperty(IDs::windowBoundsH, getHeight(), nullptr);
	keyboardWindowNode.setProperty(IDs::viewportPosition, view.get()->getViewPositionX(), nullptr);

    keyboardWindowNode.setProperty(IDs::selectedPresetName, presetCurrentNode.getChild(0)[IDs::modeFullName], nullptr);
    keyboardWindowNode.setProperty(IDs::selectedPresetIndex, pluginState->processorNode[IDs::selectedPresetIndex], nullptr);
    keyboardWindowNode.setProperty(IDs::selectedPresetComboID, 8, nullptr);

	processor.connect_editor_node(keyboardWindowNode);
    
    piano->init_data_node();
    pluginState->pianoNode = piano->get_node();
    keyboardWindowNode.addChild(pluginState->pianoNode, 0, nullptr);
}

void SuperVirtualKeyboardAudioProcessorEditor::restore_node_data()
{
    keyboardWindowNode = pluginState->keyboardWindowNode;
	setSize(keyboardWindowNode[IDs::windowBoundsW], keyboardWindowNode[IDs::windowBoundsH]);
	view.get()->setViewPosition((int)keyboardWindowNode[IDs::viewportPosition], 0);

	load_preset(pluginState->presetCurrentNode);

    piano->restore_data_node(pluginState->pianoNode);
}

//==============================================================================


bool SuperVirtualKeyboardAudioProcessorEditor::load_preset(ValueTree presetIn)
{

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