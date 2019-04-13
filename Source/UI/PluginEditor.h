/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

#include "KeyboardMenuBar.h"
#include "../Structures/ModeLayout.h"
#include "VirtualKeyboard.h"


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
    
    //==============================================================================

    void init_node_data();
    void restore_node_data();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SuperVirtualKeyboardAudioProcessor& processor;
	SuperVirtualKeyboardPluginState* pluginState;
	MidiKeyboardState externalMidi;
	ApplicationCommandManager appCmdMgr;
    ValueTree keyboardWindowNode;


	std::unique_ptr<Viewport> view;
	std::unique_ptr<VirtualKeyboard> piano;
	std::unique_ptr<KeyboardMenuBar> scaleEdit;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperVirtualKeyboardAudioProcessorEditor)
};
