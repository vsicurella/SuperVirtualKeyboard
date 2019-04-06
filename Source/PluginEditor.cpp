/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SuperVirtualKeyboardAudioProcessorEditor::SuperVirtualKeyboardAudioProcessorEditor (SuperVirtualKeyboardAudioProcessor& p)
    : AudioProcessorEditor(&p), processor (p), piano(new ViewPianoComponent(appCmdMgr)), view(new Viewport("Piano Viewport")), scaleEdit(new ScaleEditPopup())
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
	view.get()->setViewPositionProportionately(0.6, 0);

	setSize(1000, 250);

	processor.set_midi_input_state(&externalMidi);
	externalMidi.addListener(piano.get());

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
	Point<int> pt = view.get()->getViewPosition();

	AudioProcessorEditor::resized();

	scaleEdit->setSize(getWidth(), 48);
	view.get()->setSize(getWidth(), getHeight() - 48);
	piano.get()->setSize(piano.get()->getWidth(), view.get()->getMaximumVisibleHeight());

	if (view.get()->getMaximumVisibleWidth() > piano.get()->getWidth())
	{
		piano.get()->setSize(piano.get()->getWidth(), view.get()->getMaximumVisibleHeight());
	}

	view.get()->setViewPosition(pt);
}

void SuperVirtualKeyboardAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	ModeLayout layout = ModeLayout(scaleEdit.get()->get_input().toStdString());

	if (layout.scaleSize > 0)
	{
		piano.get()->apply_layout(layout);
		repaint();
	}

	resized();
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

