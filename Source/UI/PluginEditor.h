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
													public ApplicationCommandTarget,
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

	//==============================================================================

	bool load_preset(ValueTree presetIn);

	//==============================================================================

	ApplicationCommandTarget* getNextCommandTarget();

	void getAllCommands(Array< CommandID > &commands);

	void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result);

	bool perform(const InvocationInfo &info);

	//==============================================================================

	bool keyPressed(const KeyPress& key) override;

	//==============================================================================



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SuperVirtualKeyboardAudioProcessor& processor;
	SuperVirtualKeyboardPluginState* pluginState;
	MidiKeyboardState externalMidi;
    ValueTree keyboardWindowNode;
	ValueTree presetCurrentNode;

	std::unique_ptr<ApplicationCommandManager> appCmdMgr;


	std::unique_ptr<Viewport> view;
	std::unique_ptr<VirtualKeyboard> piano;
	std::unique_ptr<KeyboardMenuBar> scaleEdit;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperVirtualKeyboardAudioProcessorEditor)
};
