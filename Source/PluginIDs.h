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
static Identifier periodShiftModeSize("PeriodShiftModeSize");
static Identifier transposeAmt("TransposeAmt");
static Identifier sustainPedalOn("SustainPedalOn");
static Identifier maxVoices("MaxVoices");
static Identifier midiInputFilter("InputFilter");
static Identifier midiInputRemap("InputMapping");
static Identifier midiOutputFilter("OutputFilter");
static Identifier midiInputName("InputDevice");
static Identifier midiOutputName("OutputDevice");
static Identifier midiMapInputCustom("CustomMidiInputMap");
static Identifier midiMapOutputCustom("CustomMidiOutputMap");
static Identifier modeMapFromRootNote("ModeMapFromRootNote");
static Identifier modeMapFromPreset("ModeMapFromPreset");
static Identifier mappingMode("MappingMode");
static Identifier modeMappingStyle("ModeMappingType");
static Identifier mode1OrderMapping("Mode1OrderMapping");
static Identifier mode2OrderMapping("Mode2OrderMapping");
static Identifier mode1OrderOffsetMapping("Mode1OrderOffsetMapping");
static Identifier mode2OrderOffsetMapping("Mode2OrderOffsetMapping");

// MPE Settings
static Identifier mpeOn("MPEOn");
static Identifier mpeThru("MPEThru");
static Identifier mpeZone("MPEZone");
static Identifier mpeLegacyOn("MPELegacyOn");
static Identifier pitchBendGlobalMax("PitchBendGlobalMax");
static Identifier pitchBendNoteMax("PitchBendNoteMax");
static Identifier mpePitchTrackingMode("MPEPitchTrackingMode");
static Identifier mpePressureTrackingMode("MPEPitchTrackingMode");
static Identifier mpeTimbreTrackingMode("MPEPitchTrackingMode");
static Identifier mpeTuningPreserveMidiNote("MPETunePreserveMidiNote");

// Tuning
static Identifier tuningName("TuningName");
static Identifier retuneOn("RetuneOn");
static Identifier retuneAuto("RetuneAuto");
static Identifier tuningRootNote("TuningRootNote");
static Identifier tuningRootFreq("TuningRootFreq");

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

// TODO: Refactor piano -> keyboard
// Keyboard Component
static Identifier pianoNode("PianoNode");
static Identifier pianoUIMode("PianoUIMode");
static Identifier keyboardOrientation("KeyboardOrientation");
static Identifier pianoLastKeyClicked("PianoLastKeyClicked");
static Identifier keyboardMidiChannel("SelectedMidiChannel");
static Identifier pianoWHRatio("WidthToHeightRatio");
static Identifier keyboardNumRows("KeyboardNumRows");
static Identifier pianoHasCustomColor("PresetHasCustomColor");
static Identifier keyboardKeysStyle("KeyPlacementType");
static Identifier pianoKeysShowNoteNumbers("PianoShowNoteNumbers");
static Identifier pianoKeysShowFilteredNotes("PianoShowFilteredNotes");
static Identifier keyboardShowsNoteLabels("KeyboardShowsNoteLabels");
static Identifier keyboardHighlightStyle("PianoHighlightStyle");
static Identifier keyboardScrollingStyle("KeyboardScrollingStyle");
static Identifier keyboardScrollingMode("KeyboardScrollingMode");
static Identifier numKeysInWidth("NumKeysInWidth");
static Identifier pianoVelocityBehavior("PianoVelocityBehavior");
static Identifier pianoVelocityScaleInput("PianoVelocityScaleInput");
static Identifier pianoVelocityValue("PianoVelocityValue");
static Identifier expressionCCVal("CCForExpression");
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
static Identifier pianoKeyTreeNode("PianoKeyTree");
static Identifier pianoKeyNode("PianoKeyNode");
static Identifier pianoKeyNumber("KeyNumber");
static Identifier pianoKeyNoteLabel("NoteLabel");
static Identifier pianoKeyWidthMod("WidthMod");
static Identifier pianoKeyHeightMod("HeightMod");
static Identifier pianoKeyXOffset("XOffset");
static Identifier pianoKeyYOffset("YOffset");
static Identifier pianoKeyShowNumber("ShowNumber");
static Identifier pianoKeyShowNoteLabel("ShowKeyNoteLabel");
static Identifier pianoKeyColor("KeyColor");

// Debug
static Identifier modeSlotToEdit("ModeSlotToDebug");
static Identifier modeLibraryIndex("ModeLibraryIndex");
static Identifier modeRootNote("ModeRootNote");
static Identifier keyNumberToEdit("KeyToDebug");


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
    setModeViewed,
    setMode1SlotNumber,
    setMode2SlotNumber,
    showModeInfo,
    setMappingMode,
    setMappingStyle,
    setMappingModeToMode,
    setMappingModeToScale,
    setMappingOrders,
    showMapOrderEdit,
    applyMapping,
    beginMapEditing,
    setPeriodShift,
    setMidiChannelOut,
    beginColorEditing,
    showMidiNoteNumbers,
    setKeyboardOrientation,
    setPianoHorizontal,
    setPianoVerticalL,
    setPianoVerticalR,
    setKeyboardRows,
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
