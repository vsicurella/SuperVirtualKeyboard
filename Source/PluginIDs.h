/*
  ==============================================================================

    PluginIDs.h
    Created: 15 Apr 2019 8:27:23pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

// IDENTIFIERS

namespace IDs
{
	// Plugin State & Presets
	static Identifier pluginStateNode("PluginState");
	static Identifier modeLibraryNode("ModeLibrary");
	static Identifier presetNode("Preset");
	static Identifier modePresetNode("ModeLayoutNode");
	static Identifier indexOfMode("IndexOfMode");
	static Identifier presetHasCustomColor("PresetHasCustomColor");

	// ModeLayout
	static Identifier scaleSize("ScaleSize");
	static Identifier modeSize("ModeSize");
	static Identifier stepString("StepsString");
	static Identifier modeOffset("ModeOffset");
	static Identifier MOSClass("MosClass");
	static Identifier family("Family");
    static Identifier modeName("ModeName");

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
	static Identifier pianoLastKeyClicked("PianoLastKeyClicked");
	static Identifier pianoMidiChannel("SelectedMidiChannel");
	static Identifier pianoMidiNoteOffset("MidiNodeOffset");
	static Identifier pianoMPEToggle("MpeToggle");
	static Identifier pianoWHRatio("WidthToHeightRatio");
	static Identifier pianoKeyPlacement("KeyPlacement");
	static Identifier pianoKeySizeRatios("KeySizeRatios");
	static Identifier pianoKeyOrderColors("KeyOrderColors");
	static Identifier pianoKeyDegreeColors("KeyDegreeColors");

	// Piano Key Component
	static Identifier pianoKeyNode("PianoKeyNode");
	static Identifier pianoKeyNumber("KeyNumber");
	static Identifier pianoKeyModeDegree("KeyModeDegree");
	static Identifier pianoKeyOrder("KeyOrder");
	static Identifier pianoKeyMidiNote("MappedMidiNote");
	static Identifier pianoKeyWidthMod("KeyWidthModifier");
	static Identifier pianoKeyHeightMod("KeyHeightModifier");
	static Identifier pianoKeyXOffset("KeyXOffset");
	static Identifier pianoKeyYOffset("KeyYOffset");
	static Identifier pianoKeyColorIsCustom("KeyColorIsCustom");
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
		setKeysNestedRight,
		setKeysNestedCenter,
		setKeysAdjacent
	};
}

enum SortType
{
	scaleSizeSort,
	modeSizeSort,
	familySort
};