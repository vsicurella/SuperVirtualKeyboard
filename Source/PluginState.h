/*
  ==============================================================================

    PluginState.h
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Structures/ModeLayout.h"

// IDENTIFIERS

namespace IDs
{	
	// Plugin State & Presets
	static Identifier pluginStateNode("PluginState");
	static Identifier modeLibraryNode("ModeLibrary");
	static Identifier presetNode("Preset");
	static Identifier modePresetNode("ModeLayoutNode");
	static Identifier indexOfMode("IndexOfMode");

	// ModeLayout
	static Identifier scaleSize("ScaleSize");
	static Identifier modeSize("ModeSize");
	static Identifier stepString("StepsString");
	static Identifier MOSClass("MosClass");
	static Identifier family("Family");
	static Identifier modeFullName("ModeName");
	static Identifier modeScaleName("NameScaleSize");
	static Identifier modeModeName("NameModeSize");
	static Identifier stepArray("StepsArray");
	static Identifier keyboardOrderArray("OrderArray");
	static Identifier keyboardModeDegrees("ModeDegreeArray");
	static Identifier modeArrayValue("Value");

	// Keyboard Window
	static Identifier keyboardWindowNode("KeyboardWindowNode");
	static Identifier windowBoundsW("WindowW");
	static Identifier windowBoundsH("WindowH");
	static Identifier viewportPosition("ViewportPosition");
	
	// Piano Component
	static Identifier pianoNode("PianoNode");
	static Identifier pianoUIMode("PianoUIMode");
	static Identifier pianoOrientation("PianoOrientation");
	static Identifier pianoKeyboardState("PianoKeyboardState");
	static Identifier pianoMidiChannel("SelectedMidiChannel");
	static Identifier pianoMidiNoteOffset("MidiNodeOffset");
	static Identifier pianoMPEToggle("MpeToggle");
	static Identifier pianoKeyOrderPlacement("OrderedKeysPlacement");
	static Identifier pianoKeyOrderRatios("KeySizeRatios");
	static Identifier pianoKeyOrderColors("KeyColors");

	// Piano Key Component
	static Identifier pianoKeyNode("PianoKeyNode");
	static Identifier pianokeyNumber("KeyNumber");
	static Identifier pianoKeyModeDegree("KeyModeDegree");
	static Identifier pianoKeyOrder("KeyOrder");
	static Identifier pianokeyMidiNote("MappedMidiNote");
	static Identifier pianoKeyWidthMod("KeyWidthModifier");
	static Identifier pianoKeyHeightMod("KeyHeightModifier");
	static Identifier pianoKeyXOffset("KeyXOffset");
	static Identifier pianoKeyYOffset("KeyYOffset");
	static Identifier pianoKeyColorDefault("KeyColorDefault");
	static Identifier pianoKeyColorHighlighted("KeyColorHighlighted");
	static Identifier pianoKeyColorPressed("KeyColorPressed");
	static Identifier pianoKeyColorExternal("KeyColorMidiInput");

	enum CommandIDs
	{
		saveCustomLayout = 1,
		loadCustomLayout,
		saveReaperMap,
		setKeyColor,
		setMidiNoteOffset,
		pianoPlayMode,
		pianoEditMode,
		setPianoHorizontal,
		setPianoVerticalL,
		setPianoVerticalR,
	};
}

enum SortType
{
	scaleSize,
	modeSize,
	family
};

struct SuperVirtualKeyboardPluginState
{
	ValueTree pluginStateNode;
	ValueTree modeLibraryNode;
	ValueTree presetCurrentNode; // Always updated when some preset is selected
	ValueTree keyboardWindowNode;
	ValueTree pianoNode;

	SuperVirtualKeyboardPluginState() :
			pluginStateNode(IDs::pluginStateNode),
			modeLibraryNode(IDs::modeLibraryNode),
			presetCurrentNode(IDs::presetNode),
			undoManager(new UndoManager())

	{
		createPresets();
		pluginStateNode.addChild(modeLibraryNode, 0, nullptr);
		pluginStateNode.addChild(presetCurrentNode, 1, nullptr);
	}

	~SuperVirtualKeyboardPluginState() {}

	//==============================================================================

	OwnedArray<ModeLayout>* get_presets();
	Array<Array<ModeLayout*>>* get_presets_sorted();
	UndoManager* get_undo_mgr();

	ValueTree get_current_preset();
	int get_current_preset_index();
	ModeLayout* get_current_mode();

	void set_current_mode(int presetIndexIn);
	void set_current_mode(ModeLayout* modeIn);

	void set_current_key_settings(ValueTree pianoNodeIn);

	//==============================================================================


private:

	void createPresets();

	//==============================================================================

	std::unique_ptr<UndoManager> undoManager;
	OwnedArray<ModeLayout> presets;
	Array<Array<ModeLayout*>> presetsSorted;

	ModeLayout* modeCurrent;
};
