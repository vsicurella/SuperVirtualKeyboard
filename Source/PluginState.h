/*
  ==============================================================================

    PluginState.h
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginIDs.h"
#include "Structures/Preset.h"
#include "Structures/MidiRemapper.h"
#include "CommonFunctions.h"

struct SuperVirtualKeyboardPluginState : public ChangeBroadcaster
{
	ValueTree pluginStateNode;
	ValueTree modeLibraryNode;
	ValueTree keyboardWindowNode;

	ValueTree pluginSettingsNode;
	ValueTree midiSettingsNode;
	ValueTree modePresetNode;
	ValueTree pianoNode;
    
	SuperVirtualKeyboardPluginState() :
		pluginStateNode(IDs::pluginStateNode),
		modeLibraryNode(IDs::modeLibraryNode),
		modePresetNode(IDs::modePresetNode),
		pluginSettingsNode(IDs::pluginSettingsNode),
		midiSettingsNode(IDs::midiSettingsNode),
		undoManager(new UndoManager()),
		appCmdMgr(new ApplicationCommandManager())

	{
		createPresets();
		pluginStateNode.addChild(modeLibraryNode, 0, nullptr);
		pluginStateNode.addChild(pluginSettingsNode, -1, nullptr);
		pluginStateNode.addChild(midiSettingsNode, -1, nullptr);
	
		presetCurrent.reset(new SvkPreset());
        midiStateIn.reset(new MidiKeyboardState());
        
        noteInputMap = MidiRemapper::getStandardMap();
        noteOutputMap = MidiRemapper::getStandardMap();
        
        midiInputFilter = MidiRemapper(&noteInputMap);
        midiOutputFilter = MidiRemapper(&noteOutputMap);

		updateMidiSettingsNode();
	}

	~SuperVirtualKeyboardPluginState() {}

	//==============================================================================

	OwnedArray<Mode>* get_presets();
	Array<Array<Mode*>>* get_presets_sorted();
	UndoManager* getUndoManager();

	SvkPreset* getCurrentPreset();
	int get_current_preset_index();
	Mode* getCurrentMode();

    
    void setRootNote(int rootNoteIn);
    int getRootNote();
    
	// going to move these over to a midi processor class at some point
	Array<int>* getInputNoteMap();
	Array<int>* getOutputNoteMap();
    MidiRemapper midiInputFilter;
    MidiRemapper midiOutputFilter;
	void setMidiInputMap(Array<int> mapIn);
	void setMidiOutputMap(Array<int> mapIn);
	Array<int> mapInputNote(int noteIn, int noteOut);
	void mapOutputNode(int noteIn, int noteOut);

	int is_mode_in_presets(String stepsStringIn);

	void setCurrentMode(int presetIndexIn);
	void setCurrentMode(Mode* modeIn);
    
	void updateKeyboardSettingsPreset();
	void updateMidiSettingsNode();

	bool savePreset();
	bool loadPreset();

    void pauseMidiInput(bool setPaused=true);
    bool isMidiPaused();
    
	//==============================================================================
	
	std::unique_ptr<ApplicationCommandManager> appCmdMgr;
    std::unique_ptr<MidiKeyboardState> midiStateIn;

private:

	void createPresets();

	//==============================================================================

	std::unique_ptr<UndoManager> undoManager;
	OwnedArray<Mode> presets;
	Array<Array<Mode*>> presetsSorted;

	std::unique_ptr<SvkPreset> presetCurrent;
	Mode* modeCurrent;
    
    Array<int> noteInputMap;
    Array<int> noteOutputMap;
    
    bool midiInputPaused = false;
    
    int rootMidiNote = 60;
};
