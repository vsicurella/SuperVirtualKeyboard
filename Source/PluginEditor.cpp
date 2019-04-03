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
    : AudioProcessorEditor(&p), processor (p), view(new Viewport("Piano Viewport")), piano(new ViewPianoComponent()),
	  scaleEdit(new ScaleEditPopup())
{
	setName("Super Virtual Piano");

	scaleEdit.get()->setName("Scale Edit Popup");
	scaleEdit.get()->setAlwaysOnTop(true);
	scaleEdit.get()->addChangeListener(this);
	scaleEdit.get()->setSize(640, 48);

	addAndMakeVisible(view.get());
	view.get()->setViewedComponent(piano.get());
	view.get()->setTopLeftPosition(1, 49);
	
	piano.get()->setSize(1000, 250);

	addAndMakeVisible(scaleEdit.get());
	addChildComponent(scaleEdit.get());

	setResizable(true, true);
	setSize(1000, 250);
	setResizeLimits(640, 100, 10e4, 10e4);

	view.get()->setViewPositionProportionately(0.6, 0);

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

	if (getHeight() < piano.get()->get_min_height())
		setSize(getWidth(), piano.get()->get_min_height());

	if (getWidth() > piano.get()->getWidth())
		setSize(piano.get()->getWidth(), getHeight());

	AudioProcessorEditor::resized();

	scaleEdit->setSize(getWidth(), 48);
	view.get()->setSize(getWidth(), getHeight() - 48);
	piano.get()->setSize(piano.get()->getWidth(), view.get()->getMaximumVisibleHeight());

	view.get()->setViewPosition(pt);
}

void SuperVirtualKeyboardAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	std::vector<int> layout = ModeLayout::parse_steps(scaleEdit.get()->get_input());

	if (layout.size() > 0)
	{
		piano.get()->apply_steps_layout(layout);
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
