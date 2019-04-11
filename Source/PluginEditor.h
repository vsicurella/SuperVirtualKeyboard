/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "ScaleEditPopup.h"
#include "Structures/ModeLayout.h"
#include "Views/ViewPianoComponent.h"


//==============================================================================
/**
*/
class SuperVirtualKeyboardAudioProcessorEditor  : public AudioProcessorEditor,
													public ChangeListener,
													public MidiInputCallback,
													public Timer
{
public:
    SuperVirtualKeyboardAudioProcessorEditor (SuperVirtualKeyboardAudioProcessor&);
    ~SuperVirtualKeyboardAudioProcessorEditor();

    //==============================================================================
   
	void paint (Graphics&) override;
    void resized() override;

	//==============================================================================


	void changeListenerCallback(ChangeBroadcaster*) override;
	void timerCallback() override;
	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;
	void focusGained(FocusChangeType changeType) override;
	void userTriedToCloseWindow() override;
	void visibilityChanged() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SuperVirtualKeyboardAudioProcessor& processor;
	MidiKeyboardState externalMidi;
	ApplicationCommandManager appCmdMgr;

	std::unique_ptr<Viewport> view;
	std::unique_ptr<ViewPianoComponent> piano;
	std::unique_ptr<ScaleEditPopup> scaleEdit;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperVirtualKeyboardAudioProcessorEditor)
};
