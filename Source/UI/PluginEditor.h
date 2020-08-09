/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../PluginProcessor.h"
#include "../PluginModes.h"
#include "../Structures/Mode.h"
#include "../Structures/MappingHelper.h"
#include "Components/PluginControlComponent.h"
#include "Components/VirtualKeyboard/KeyboardComponent.h"
//#include "Components/Settings/SettingsContainer.h"
#include "Dialogs/ColorChoosingWindow.h"
#include "Dialogs/MidiSettingsWindow.h"
#include "Dialogs/MapByOrderDialog.h"
#include "Dialogs/ModeInfoDialog.h"
//#include "Dialogs/PluginSettingsDialog.h"
#include "../File IO/ReaperWriter.h"

using namespace VirtualKeyboard;

//==============================================================================
/**
*/
class SvkPluginEditor : public AudioProcessorEditor,
						public ApplicationCommandTarget,
						private SvkPluginState::Listener,
						private ChangeListener,
                        private AudioProcessorValueTreeState::Listener,
						private Timer
{
public:
    SvkPluginEditor (SvkAudioProcessor&);
    ~SvkPluginEditor();

    //==============================================================================
   
	void paint (Graphics&) override;
    void resized() override;

	//==============================================================================

	void timerCallback() override;
	void userTriedToCloseWindow() override;
    
    //==============================================================================

	bool savePresetToFile();
	bool saveMode();
    void showSaveMenu();

	bool loadPreset();
	bool loadMode();
    void showLoadMenu();

	bool exportReaperMap();
	bool exportAbletonMap();
    void showExportMenu();
    
    void showSettingsDialog();

	void commitCustomScale();

	void setMode1();
	void setMode1(int idIn);

	void setMode2();
	void setMode2(int idIn);

	void setMode1Root();
	void setMode1Root(int rootIn);
	
	void setMode2Root();
	void setMode2Root(int rootIn);

	void setModeView();
	void setModeView(int modeNumberIn);

	void showModeInfo();

	void setMappingMode();
	void setMappingMode(int mappingModeId);

	void setMappingStyle();
	void setMappingStyle(int mapStyleId);
    
    void showMapOrderEditDialog();

	void applyMap();

	void beginColorEditing();

	//==============================================================================

	void presetLoaded(ValueTree presetNodeIn) override;

	void modeViewedChanged(Mode* modeIn, int selectorNumber, int slotNumber) override;

	void inputMappingChanged(NoteMap* inputNoteMap) override;

	//==============================================================================

	void changeListenerCallback(ChangeBroadcaster* source) override;
    
	//==============================================================================

	 File fileDialog(String message, bool forSaving);
    
    //==============================================================================
    
    void parameterChanged (const String& paramID, float newValue) override;

	//==============================================================================

	 ApplicationCommandTarget* getNextCommandTarget() override;

	 void getAllCommands(Array< CommandID > &commands) override;

	 void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) override;

	 bool perform(const InvocationInfo &info) override;


private:
    SvkAudioProcessor& processor;
	SvkPluginState* pluginState;
	MidiKeyboardState externalMidi;
    ValueTree pluginEditorNode;

    ApplicationCommandManager* appCmdMgr;

	std::unique_ptr<PluginControlComponent> controlComponent;
    std::unique_ptr<MappingHelper> mappingHelper;

	Keyboard* virtualKeyboard;

	std::unique_ptr<ColorChooserWindow> colorChooserWindow;
    std::unique_ptr<ColourSelector> colorSelector;
    bool isColorEditing = false;

    //std::unique_ptr<SettingsContainer> settingsContainer;
    //PluginSettingsDialog* pluginSettingsDialog;
	ModeInfoDialog* modeInfo;
    MapByOrderDialog* mapByOrderDialog;

	TooltipWindow tooltip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SvkPluginEditor)
};
