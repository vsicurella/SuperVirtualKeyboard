/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

#include "Components/KeyboardEditorBar.h"
#include "../Structures/Mode.h"
#include "Components/VirtualKeyboard/Keyboard.h"

#include "Dialogs/ColorChoosingWindow.h"
#include "Dialogs/MidiSettingsWindow.h"
#include "Dialogs/MidiSettingsComponent.h"

#include "../File IO/ReaperWriter.h"

using namespace VirtualKeyboard;

//==============================================================================
/**
*/
class SuperVirtualKeyboardAudioProcessorEditor  : public AudioProcessorEditor,
													public ApplicationCommandTarget,
													private ChangeListener,
													private Timer
{
public:
    SuperVirtualKeyboardAudioProcessorEditor (SuperVirtualKeyboardAudioProcessor&, ApplicationCommandManager*);
    ~SuperVirtualKeyboardAudioProcessorEditor();

    //==============================================================================
   
	void paint (Graphics&) override;
    void resized() override;

	//==============================================================================

	void timerCallback() override;
	void userTriedToCloseWindow() override;
    
    //==============================================================================

    void initNodeData();
	void updateNodeData();

	void update_children_to_preset();
	void beginColorEditing();

	//===============================================================================================

	bool save_preset();
	bool load_preset();
	bool write_reaper_file();

	//===============================================================================================

	void mouseDown(const MouseEvent& e) override;

	void mouseDrag(const MouseEvent& e) override;

	void mouseUp(const MouseEvent& e) override;

	void mouseMove(const MouseEvent& e) override;

	//==============================================================================

	void changeListenerCallback(ChangeBroadcaster* source) override;

	 //==============================================================================

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
	
	std::unique_ptr<Viewport> view;
	std::unique_ptr<Keyboard> piano;
	std::unique_ptr<KeyboardEditorBar> keyboardEditorBar;

	std::unique_ptr<ColorChooserWindow> colorChooserWindow;
    std::unique_ptr<ColourSelector> colorSelector;

	std::unique_ptr<MidiSettingsWindow> midiSettingsWindow;
    std::unique_ptr<MidiSettingsComponent> midiSettingsComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperVirtualKeyboardAudioProcessorEditor)
};
