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
	static Identifier modeCustomNode("CurrentCustomMode");

	// Global Settings
	static Identifier globalSettingsNode("Preferences");
	static Identifier settingsDirectory("SettingsDirectory");
	static Identifier presetDirectory("PresetDirectory");
	static Identifier modeDirectory("ModeDirectory");
	static Identifier createPresetFolder("CreatePresetFolder");
	static Identifier saveFactoryModes("SaveFactoryPresets");
    
    // Preset Properties
    static Identifier presetProperties("PresetProperties");
    static Identifier presetSlotViewed("PresetSlotViewed");
    static Identifier modeSlotsNode("ModeSlots");
	static Identifier modeSlotsNumberNode("ModeSlotNumbers");
    static Identifier modeSlotNumber("SlotNumber");
    static Identifier modeSlotNumViewed("ModeSlotViewed");

	// Midi Settings
    static Identifier midiSettingsNode("MidiSettings");
    static Identifier midiMapNode("MidiMappings");
	static Identifier rootMidiNote("RootNote");
    static Identifier periodShift("PeriodShift");
    static Identifier midiInputMap("InputMapping");
    static Identifier midiOutputMap("OutputMappint");
    static Identifier midiInputName("InputDevice");
    static Identifier midiOutputName("OutputDevice");
    static Identifier midiMapInputCustom("Custom Midi Input Map");
    static Identifier midiMapOutputCustom("Custom Midi Output Map");
    static Identifier modeMapFromRootNote("ModeMapFromRootNote");
    static Identifier modeMapFromPreset("ModeMapFromPreset");
    static Identifier mappingMode("MappingMode");
    static Identifier modeMappingStyle("ModeMappingType");
	static Identifier mode1OrderMapping("Mode1OrderMapping");
	static Identifier mode2OrderMapping("Mode2OrderMapping");
	static Identifier mode1OrderOffsetMapping("Mode1OrderOffsetMapping");
	static Identifier mode2OrderOffsetMapping("Mode2OrderOffsetMapping");

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
	static Identifier pianoKeysShowNoteNumbers("KeysShowNoteNumbers");
	static Identifier pianoKeysHighlightStyle("KeysHighlightStyle");
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
		savePresetToFile = 1,
		saveMode,
        showSaveMenu,
		loadPreset,
		loadMode,
        showLoadMenu,
		exportPreset,
		exportReaperMap,
		exportAbletonMap,
        showExportMenu,
        showSettingsDialog,
		commitCustomScale,
		setMode1,
		setMode2,
		setMode1RootNote,
		setMode2RootNote,
		viewMode1,
		viewMode2,
		setMode1SlotNumber,
		setMode2SlotNumber,
		showModeInfo,
		setMappingStyle,
		setMappingModeToMode,
		setMappingModeToScale,
		setMappingOrders,
        showMapOrderEdit,
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
