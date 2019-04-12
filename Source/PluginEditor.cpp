/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SuperVirtualKeyboardAudioProcessorEditor::SuperVirtualKeyboardAudioProcessorEditor(SuperVirtualKeyboardAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p),
	piano(new ViewPianoComponent(appCmdMgr, nullptr)),
	view(new Viewport("Piano Viewport")),
	scaleEdit(new ScaleEditPopup(processor.get_presets(), processor.get_presets_sorted()))
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

	setSize(1000, 250);

	view.get()->setViewPositionProportionately(0.6, 0);
	processor.set_midi_input_state(&externalMidi);
	externalMidi.addListener(piano.get());
    
    if (!keyboardWindowNode.isValid())
        init_node_data();
    restore_node_data();
	  
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

	scaleEdit->setSize(getWidth(), 48);
	view.get()->setSize(getWidth(), getHeight() - 48);
	piano.get()->setSize(piano.get()->getWidth(), view.get()->getMaximumVisibleHeight() - 10);

	view.get()->setViewPositionProportionately(0.618, 0);
	
	// update node
	if (keyboardWindowNode.isValid())
	{
		keyboardWindowNode.setProperty(IDs::windowBoundsW, getWidth(), nullptr);
		keyboardWindowNode.setProperty(IDs::windowBoundsH, getHeight(), nullptr);
		keyboardWindowNode.setProperty(IDs::viewportPosition, view.get()->getViewPositionX(), nullptr);
	}
	
}

void SuperVirtualKeyboardAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
    ModeLayout* newPreset = scaleEdit.get()->presetSelected;
    
	if (newPreset)
	{
		piano.get()->apply_layout(newPreset);

		if (scaleEdit.get()->presetSelected->family == "Custom")
			processor.set_preset(0);
		else
			processor.set_preset(scaleEdit.get()->get_preset_id(newPreset->get_full_name()));

		keyboardWindowNode.setProperty(IDs::selectedPresetName, scaleEdit.get()->get_preset_name(), nullptr);
		keyboardWindowNode.setProperty(IDs::selectedPresetIndex, scaleEdit.get()->get_preset_id(newPreset->get_full_name()), nullptr);
		keyboardWindowNode.setProperty(IDs::selectedPresetComboID, scaleEdit.get()->get_selected_preset_id(), nullptr);
	}
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

    keyboardWindowNode.setProperty(IDs::selectedPresetName, processor.get_preset_selected()->get_full_name(), nullptr);
    keyboardWindowNode.setProperty(IDs::selectedPresetIndex, 8, nullptr);
    keyboardWindowNode.setProperty(IDs::selectedPresetComboID, 8, nullptr);

	processor.connect_editor_node(keyboardWindowNode);
}

void SuperVirtualKeyboardAudioProcessorEditor::restore_node_data()
{
	setSize(keyboardWindowNode[IDs::windowBoundsW], keyboardWindowNode[IDs::windowBoundsH]);
	view.get()->setViewPosition((int)keyboardWindowNode[IDs::viewportPosition], 0);

	scaleEdit.get()->set_selected_preset(processor.get_preset_selected());
	int id = (int)keyboardWindowNode[IDs::selectedPresetComboID];

    scaleEdit.get()->set_selected_preset(id);
    scaleEdit.get()->set_text_boxes(keyboardWindowNode[IDs::selectedPresetName].toString(), processor.get_preset_selected()->strSteps);
}
