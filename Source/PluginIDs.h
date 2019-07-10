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
	static Identifier modePresetNode("ModeLayout");
	static Identifier modeSlotsNode("ModeSlots");
	static Identifier mode1SlotNumber("Mode1SlotNum");
	static Identifier mode2SlotNumber("Mode2SlotNum");

	// Plugin Settings
	static Identifier pluginSettingsNode("Preferences");
	static Identifier settingsDirectory("SettingsDirectory");
	static Identifier presetDirectory("PresetDirectory");
	static Identifier modeDirectory("ModeDirectory");
	static Identifier createPresetFolder("CreatePresetFolder");
	static Identifier saveFactoryPresets("SaveFactoryPresets");

	// Midi Settings
    static Identifier midiSettingsNode("MidiSettings");
    static Identifier midiMapNode("MidiMappings");
	static Identifier rootMidiNote("RootNote");
    static Identifier periodShift("PeriodShift");
    static Identifier midiInputMap("InputMapping");
    static Identifier midiOutputMap("OutputMappint");
    static Identifier midiInputName("InputDevice");
    static Identifier midiOutputName("OutputDevice");
    static Identifier autoRemapOn("AutoRemapOn");
    static Identifier modeMapFromRootNote("ModeMapFromRootNote");
    static Identifier modeMapFromPreset("ModeMapFromPreset");
    static Identifier modeMappingType("ModeMappingType");

	// Mode
	static Identifier scaleSize("ScaleSize");
	static Identifier modeSize("ModeSize");
	static Identifier stepString("StepsString");
	static Identifier MOSClass("MosClass");
	static Identifier family("Family");
    static Identifier modeInfo("Description");
    static Identifier modeName("ModeName");
	static Identifier factoryPreset("FactoryPreset");
	static Identifier libraryIndexOfMode("IndexOfMode");

	// Keyboard Window
	static Identifier pluginEditorNode("PluginWindow");
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
	static Identifier pianoKeyMidiNoteMappings("KeyMappedMidiNotes");


	// Piano Key Component
	static Identifier pianoKeyColorIsCustom("KeyColorIsCustom");
	static Identifier pianoKeyColorHighlighted("KeyColorHighlighted");
	static Identifier pianoKeyColorPressed("KeyColorPressed");
	static Identifier pianoKeyColorExternal("KeyColorMidiInput");

	enum CommandIDs
	{
		savePreset = 1,
		saveMode,
		loadPreset,
		loadMode,
		exportPreset,
		exportReaperMap,
		exportAbletonMap,
		commitCustomScale,
		setMode1,
		setMode2,
		setMode1RootNote,
		setMode2RootNote,
		viewMode1,
		viewMode2,
		showModeInfo,
		setMappingStyle,
		setMappingModeToMode,
		setMappingModeToScale,
		setMappingOrders,
		applyMapping,
		setAutoMap,
		beginMapEditing,
		setPeriodShift,
		setMidiChannelOut,
		beginColorEditing,
		showMidiNoteNumbers,
		setKeyboardOrientation,
		setPianoHorizontal,
		setPianoVerticalL,
		setPianoVerticalR,
		setKeyStyle,
		setKeysNestedRight,
		setKeysFlat,
		setKeysAdjacent,
		setHighlightStyle,
		setHighlightFullKey,
		setHighlightDot,
	};
}

enum SortType
{
	scaleSize = 0,
	modeSize,
	familyName,
	user
};
