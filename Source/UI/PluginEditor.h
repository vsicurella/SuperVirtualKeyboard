/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

#include "KeyboardEditorBar.h"
#include "../Structures/ModeLayout.h"
#include "VirtualKeyboard.h"


//==============================================================================
/**
*/
class SuperVirtualKeyboardAudioProcessorEditor  : public AudioProcessorEditor,
													public ApplicationCommandTarget,
													public ValueTree::Listener,
													public FilenameComponentListener,
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


	void timerCallback() override;
	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;
	void focusGained(FocusChangeType changeType) override;
	void userTriedToCloseWindow() override;
	void visibilityChanged() override;
    
    //==============================================================================

    void init_node_data();
    void restore_node_data(ValueTree nodeIn);

	bool load_preset(File& presetIn);

	void update_children_to_preset();

	//==============================================================================

	bool keyPressed(const KeyPress& key) override;

	//==============================================================================

	 void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property);

	 void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded);

	 void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved);

	 void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex);

	 void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged);

	 //==============================================================================

	 void filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged);

	 //==============================================================================

	 ApplicationCommandTarget* getNextCommandTarget() override;

	 void getAllCommands(Array< CommandID > &commands) override;

	 void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) override;

	 bool perform(const InvocationInfo &info) override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SuperVirtualKeyboardAudioProcessor& processor;
	SuperVirtualKeyboardPluginState* pluginState;
	MidiKeyboardState externalMidi;
    ValueTree keyboardWindowNode;

	ApplicationCommandManager appCmdMgr;

	std::unique_ptr<FilenameComponent> openFileChooser;
	std::unique_ptr<FilenameComponent> saveFileChooser;
	
	std::unique_ptr<Viewport> view;
	std::unique_ptr<VirtualKeyboard> piano;
	std::unique_ptr<KeyboardEditorBar> keyboardEditorBar;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperVirtualKeyboardAudioProcessorEditor)
};
