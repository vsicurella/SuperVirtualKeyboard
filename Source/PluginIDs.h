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
	// Parent Nodes
	static Identifier pluginStateNode("PluginState");
	static Identifier modeLibraryNode("ModeLibrary");
	static Identifier presetNode("Preset");
	static Identifier modePresetNode("Mode");
	static Identifier pluginSettingsNode("PluginSettings");
	static Identifier midiSettingsNode("MidiSettings");

	// Midi Settings
    static Identifier midiInputName("InputDeviceName");
    static Identifier midiOutputName("OutputDeviceName");
	static Identifier rootMidiNote("RootNote");
	static Identifier midiInputMap("MidiInputMap");
	static Identifier midiOutputMap("MidiInputMap");

	// ModeLayout
	static Identifier scaleSize("ScaleSize");
	static Identifier modeSize("ModeSize");
	static Identifier stepString("StepsString");
	static Identifier MOSClass("MosClass");
	static Identifier family("Family");
    static Identifier modeName("ModeName");
	static Identifier factoryPreset("FactoryPreset");
	static Identifier libraryIndexOfMode("ModeLibraryIndex");

	// Keyboard Window
	static Identifier keyboardWindowNode("KeyboardWindow");
	static Identifier windowBoundsW("WindowW");
	static Identifier windowBoundsH("WindowH");
	static Identifier viewportPosition("ViewportPosition");

	// Piano Component
	static Identifier pianoNode("PianoNode");
	static Identifier pianoUIMode("PianoUIMode");
	static Identifier pianoOrientation("PianoOrientation");
	static Identifier pianoKeyboardState("PianoKeyboardState");
	static Identifier pianoLastKeyClicked("LastKeyClicked");
	static Identifier pianoMidiChannel("SelectedMidiChannel");
	static Identifier pianoMPEToggle("MpeToggle");
	static Identifier pianoWHRatio("WidthToHeightRatio");
	static Identifier pianoHasCustomColor("PresetHasCustomColor");
	static Identifier pianoKeyPlacementType("KeyPlacementType");
	static Identifier pianoKeyColorsOrder("KeyOrderColors");
	static Identifier pianoKeyColorsDegree("KeyDegreeColors");
	static Identifier pianoKeyColorSingle("KeySingleColors");
	static Identifier pianoKeyPlaceOrder("KeyOrderPlaces");
	static Identifier pianoKeyPlaceDegree("KeyDegreePlaces");
	static Identifier pianoKeyPlaceSingle("KeySinglePlaces");
	static Identifier pianoKeyRatioOrder("KeyOrderRatios");
	static Identifier pianoKeyRatioDegree("KeyDegreeRatios");
	static Identifier pianoKeyRatioSingle("KeySingleRatios");


	// Piano Key Component
	static Identifier pianoKeyColorIsCustom("KeyColorIsCustom");
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
		setKeysAdjacent,
		showMidiNoteNumbers
	};
}

enum SortType
{
	scaleSizeSort,
	modeSizeSort,
	familySort
};
