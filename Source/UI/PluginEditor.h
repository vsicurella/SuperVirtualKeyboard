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
#include "../Structures/Mode.h"
#include "VirtualKeyboard/Keyboard.h"
#include "ColorChoosingWindow.h"

#include "../File IO/ReaperWriter.h"

using namespace VirtualKeyboard;

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
    SuperVirtualKeyboardAudioProcessorEditor (SuperVirtualKeyboardAudioProcessor&, ApplicationCommandManager*);
    ~SuperVirtualKeyboardAudioProcessorEditor();

    //==============================================================================
   
	void paint (Graphics&) override;
    void resized() override;

	//==============================================================================

	void timerCallback() override;
	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;
	void focusGained(FocusChangeType changeType) override;
	void userTriedToCloseWindow() override;
    
    //==============================================================================

    void init_node_data();
    void restore_node_data(ValueTree nodeIn);

	bool save_preset(const File& fileOut);
	bool load_preset(const File& fileIn);

	void update_children_to_preset();

	bool write_reaper_file();

	//==============================================================================

	bool keyPressed(const KeyPress& key) override;

	//==============================================================================

	 void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;

	 void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;

	 void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;

	 void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override;

	 void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

	 //==============================================================================

	 void filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged) override;

	 File fileDialog(String message, bool forSaving);

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

	ApplicationCommandManager* appCmdMgr;

	std::unique_ptr<FilenameComponent> openFileBox;
	std::unique_ptr<FilenameComponent> saveFileBox;
	
	std::unique_ptr<Viewport> view;
	std::unique_ptr<Keyboard> piano;
	std::unique_ptr<KeyboardEditorBar> keyboardEditorBar;

	std::unique_ptr<DocumentWindow> colorChooserWindow;
    std::unique_ptr<ColourSelector> colorSelector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperVirtualKeyboardAudioProcessorEditor)
};
