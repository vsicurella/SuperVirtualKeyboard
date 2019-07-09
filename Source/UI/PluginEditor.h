/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

#include "Components/PluginControlComponent.h"
#include "Components/KeyboardEditorBar.h"
#include "../Structures/Mode.h"
#include "Components/VirtualKeyboard/Keyboard.h"

#include "Dialogs/ColorChoosingWindow.h"
#include "Dialogs/MidiSettingsWindow.h"
#include "Dialogs/MidiSettingsComponent.h"
#include "Dialogs/ModeInfoDialog.h"

#include "../File IO/ReaperWriter.h"

using namespace VirtualKeyboard;

//==============================================================================
/**
*/
class SvkPluginEditor : public AudioProcessorEditor,
						public ApplicationCommandTarget,
						private ChangeListener,
						private Timer
{
public:
    SvkPluginEditor (SvkAudioProcessor&, ApplicationCommandManager*);
    ~SvkPluginEditor();

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

	//===============================================================================================

	bool savePreset();
	bool saveMode();

	bool loadPreset();
	bool loadMode();

	bool exportReaperMap();
	bool exportAbletonMap();

	void commitCustomScale();

	void setMode1();
	void setMode1(ValueTree modeNodeIn);
	void setMode1(int presetId);

	void setMode2();
	void setMode2(ValueTree modeNodeIn);
	void setMode2(int presetId);

	void setMode1Root();
	void setMode1Root(int rootIn);
	
	void setMode2Root();
	void setMode2Root(int rootIn);

	void setModeView();
	void setModeView(int modeNumberIn);

	void showModeInfo();

	void setMappingStyle();
	void setMappingStyle(int mapStyleId);

	void applyMap();

	void setAutoMap();
	void setAutoMap(bool isAutoMapping);
	void beginMapEditing();

	void setPeriodShift();
	void setPeriodShift(int periodsIn);
	
	void setMidiChannel();
	void setMidiChannel(int midiChannelIn);

	void beginColorEditing();

	void setNoteNumsVisible();
	void setNoteNumsVisible(bool noteNumsVisible);
	
	void setKeyStyle();
	void setKeyStyle(int keyStyleId);

	void setHighlightStyle();
	void setHighlightStyle(int highlightStyleId);

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
    SvkAudioProcessor& processor;
	SvkPluginState* pluginState;
	MidiKeyboardState externalMidi;
    ValueTree pluginEditorNode;

	ApplicationCommandManager* appCmdMgr;
	
	std::unique_ptr<PluginControlComponent> controlComponent;

	std::unique_ptr<Viewport> view;
	Keyboard* virtualKeyboard;
	std::unique_ptr<KeyboardEditorBar> keyboardEditorBar;

	std::unique_ptr<ColorChooserWindow> colorChooserWindow;
    std::unique_ptr<ColourSelector> colorSelector;

	std::unique_ptr<MidiSettingsWindow> midiSettingsWindow;
    std::unique_ptr<MidiSettingsComponent> midiSettingsComponent;

	ModeInfoDialog* modeInfo;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SvkPluginEditor)
};
