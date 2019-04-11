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
	piano(new ViewPianoComponent(processor.get_preset_selected(), appCmdMgr)),
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

    keyboardWindowNode = processor.get_plugin_state()->keyboardWindowNode;
    
    if (!keyboardWindowNode.isValid())
        update_node_data();
    else
        restore_node_data();
    
	startTimer(1);
}

SuperVirtualKeyboardAudioProcessorEditor::~SuperVirtualKeyboardAudioProcessorEditor()
{
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::paint(Graphics& g)
{
	piano.get()->repaint();
}

void SuperVirtualKeyboardAudioProcessorEditor::resized()
{
	//Point<int> pt = view.get()->getViewPosition();

	AudioProcessorEditor::resized();

	scaleEdit->setSize(getWidth(), 48);
	view.get()->setSize(getWidth(), getHeight() - 48);
	piano.get()->setSize(piano.get()->getWidth(), view.get()->getMaximumVisibleHeight() - 10);

	//view.get()->setViewPosition(pt);
	view.get()->setViewPositionProportionately(0.618, 0);
}

void SuperVirtualKeyboardAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
    ModeLayout* newPreset = scaleEdit.get()->presetSelected;
    
	if (newPreset)
	{
		piano.get()->apply_layout(newPreset);
        
        if (!scaleEdit.get()->isUserPreset)
        {
            processor.set_preset(scaleEdit.get()->get_preset_id(newPreset->get_full_name()));
            keyboardWindowNode.setProperty(IDs::selectedPresetName, scaleEdit.get()->get_preset_name(), nullptr);
            keyboardWindowNode.setProperty(IDs::selectedPresetIndex, scaleEdit.get()->get_preset_id(newPreset->get_full_name()), nullptr);
            keyboardWindowNode.setProperty(IDs::selectedPresetComboID, scaleEdit.get()->get_selected_preset_id(), nullptr);
        }
        else
        {
            int index = processor.get_presets()->size() - 1;
            processor.set_preset(index);
            
            keyboardWindowNode.setProperty(IDs::selectedPresetName, "Custom Mode (will be adding Save l8r!)", nullptr);
            keyboardWindowNode.setProperty(IDs::selectedPresetIndex, index, nullptr);
            keyboardWindowNode.setProperty(IDs::selectedPresetComboID, -1, nullptr);
        }
	}
    
    restore_node_data();
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
	processor.setViewportPositionProportions(view.get()->getViewPosition());
	setVisible(false);
}

void SuperVirtualKeyboardAudioProcessorEditor::visibilityChanged()
{
	if (isVisible())
	{
		view.get()->setViewPosition(processor.getViewportPositionProportions());
	}
}

//==============================================================================

void SuperVirtualKeyboardAudioProcessorEditor::update_node_data()
{
    keyboardWindowNode = ValueTree(IDs::keyboardWindowNode);
    keyboardWindowNode.setProperty(IDs::selectedPresetName, processor.get_preset_selected()->get_full_name(), nullptr);
    keyboardWindowNode.setProperty(IDs::selectedPresetIndex, 8, nullptr);
    keyboardWindowNode.setProperty(IDs::selectedPresetComboID, 8, nullptr);
    
    restore_node_data();
}

void SuperVirtualKeyboardAudioProcessorEditor::restore_node_data()
{
    int id = (int) keyboardWindowNode[IDs::selectedPresetIndex];
    scaleEdit.get()->set_selected_preset(processor.get_presets()->getUnchecked(id));
    
    if ((int) keyboardWindowNode[IDs::selectedPresetComboID] < 0)
        scaleEdit.get()->isUserPreset = true;

    scaleEdit.get()->set_text_boxes(keyboardWindowNode[IDs::selectedPresetName].toString(), processor.get_preset_selected()->strSteps);
}
