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
#include "Components/Settings/SettingsContainer.h"

#include "Dialogs/MapByOrderDialog.h"
#include "Dialogs/ModeInfoDialog.h"

#include "../File IO/ReaperWriter.h"
#include "../File IO/AbletonMidiWriter.h"

using namespace VirtualKeyboard;

//==============================================================================
/**
*/
class SvkPluginEditor : public AudioProcessorEditor,
						public ApplicationCommandTarget,
						private SvkPluginState::Listener,
						private SettingsContainer::Listener,
						private ScrollBar::Listener,
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

	bool exportReaperMap();
	bool exportAbletonMap();
    
    void showSettingsDialog();
	void hideSettings();

	void commitCustomScale();

	void setMode1();
	void setMode1(int idIn);

	void setMode2();
	void setMode2(int idIn);

	void setMode1Root();
	void setMode1Root(int rootIn);
	
	void setMode2Root();
	void setMode2Root(int rootIn);

	void setModeSelectorViewed();
	void setModeSelectorViewed(int selectorNumIn);

	void showModeInfo();

	void setMappingMode();
	void setMappingMode(int mappingModeId);

	void setMappingStyle();
	void setMappingStyle(int mapStyleId);
    
    void showMapOrderEditDialog();

	void applyMap();

	void beginColorEditing();

	//==============================================================================

	void scrollBarMoved(ScrollBar* bar, double newRangeStart) override;

	//==============================================================================

	void presetLoaded(ValueTree presetNodeIn) override;

	void modeViewedChanged(Mode* modeIn, int selectorNumber, int slotNumber) override;

	void inputMappingChanged(NoteMap* inputNoteMap) override;

	void customModeChanged(Mode* newCustomMode) override;

	void modeInfoChanged(Mode* modeEdited) override;

	//==============================================================================

	void settingsTabChanged(int tabIndex, const String& tabName, SvkSettingsPanel* panelChangedTo) override;

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
	Viewport* keyboardViewport;

	std::unique_ptr<SettingsContainer> settingsPanel;

	int defaultHeight = 210;
	bool settingsPanelOpen = false;
    bool isColorEditing = false;

    //std::unique_ptr<SettingsContainer> settingsContainer;
	//std::unique_ptr<PluginSettingsDialog> pluginSettingsDialog;
	ModeInfoDialog* modeInfo;
    MapByOrderDialog* mapByOrderDialog;

	TooltipWindow tooltip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SvkPluginEditor)
};
