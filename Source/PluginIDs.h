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
static Identifier svkParentNode("SVKValueTreeParent");
static Identifier pluginStateNode("SvkPluginState");
static Identifier modeLibraryNode("ModeLibrary");
static Identifier presetNode("SvkPreset");
static Identifier modePresetNode("SvkModeLayout");
static Identifier modeCustomNode("SvkCustomMode");
static Identifier pluginPresetVersion("SvkPresetVersion");

// Global Settings
static Identifier globalSettingsNode("SvkPreferences");
static Identifier settingsDirectory("SettingsDirectory");
static Identifier presetDirectory("PresetDirectory");
static Identifier modeDirectory("ModeDirectory");
static Identifier createPresetFolder("CreatePresetFolder");
static Identifier saveFactoryModes("SaveFactoryPresets");

// Preset Properties
static Identifier presetProperties("PresetProperties");
static Identifier modeSelectorsNode("ModeSelectors");
static Identifier modeSlotsNode("ModeSlots");
static Identifier modeSlotNumber("SlotNumber");
static Identifier modeSelectorViewed("ModeSelectorViewed");
static Identifier modeSelectorRootNote("ModeSelectorRootNote");

// Midi Settings
static Identifier midiDeviceSettingsNode("MidiDeviceSettings");
static Identifier midiSettingsNode("MidiSettings");
static Identifier rootMidiNote("RootNote");
static Identifier periodShift("PeriodShift");
static Identifier periodShiftModeSize("PeriodShiftModeSize");
static Identifier transposeAmt("TransposeAmt");
static Identifier maxVoices("MaxVoices");
static Identifier midiInputName("InputDevice");
static Identifier midiOutputName("OutputDevice");

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

// Note Mapping
static Identifier midiMapNode("MidiMappings");
static Identifier midiInputFilter("InputFilter");
static Identifier midiInputRemap("InputMapping");
static Identifier midiOutputFilter("OutputFilter");
static Identifier noteMapNode("NoteMapping");
static Identifier mappingSize("MappingSize");
static Identifier mappingMidiNoteIn("MidiNoteIn");
static Identifier mappingMidiChannelIn("MidiChannelIn");
static Identifier mappingMidiNoteOut("MidiNoteOut");
static Identifier mappingMidiChannelOut("MidiChannelOut");
static Identifier midiMapInputCustom("CustomMidiInputMap");
static Identifier midiMapOutputCustom("CustomMidiOutputMap");
static Identifier modeMapFromRootNote("ModeMapFromRootNote");
static Identifier modeMapFromPreset("ModeMapFromPreset");
static Identifier mappingMode("MappingMode");
static Identifier autoMappingStyle("ModeMappingType");
static Identifier mode1OrderMapping("Mode1OrderMapping");
static Identifier mode2OrderMapping("Mode2OrderMapping");
static Identifier mode1OrderOffsetMapping("Mode1OrderOffsetMapping");
static Identifier mode2OrderOffsetMapping("Mode2OrderOffsetMapping");
static Identifier kbmFileLocation("KbmFileLocation");

// Keyboard Window
static Identifier pluginEditorNode("PluginWindow");
static Identifier windowBoundsW("WindowW");
static Identifier windowBoundsH("WindowH");
static Identifier viewportPosition("ViewportPosition");
static Identifier settingsOpen("SettingsOpen");
static Identifier settingsTabName("SettingsTabName");

// TODO: Refactor piano -> keyboard
// Keyboard Component
static Identifier pianoNode("SvkPianoNode");
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
static Identifier pianoKeyColorsNode("KeyColors");
static Identifier pianoKeyColorsLayer("LayerColors");
static Identifier pianoKeyColorsDegree("DegreeColors");
static Identifier pianoKeyColorsIndividual("IndividualColors");
static Identifier pianoKeyColorsNoteOn("NoteOnColors");
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

// Control Related IDs
static Identifier colorSelected("ColorSelected");
static Identifier pianoKeyColorReset("ResetColor");
static Identifier pianoKeyPaintType("PaintType");

// Backwards compatibility
static Identifier alphaPluginStateNode("PluginState");
static Identifier alphaVirtualKeyboardNode("PianoNode");
static Identifier alphaCustomMode("CustomMode");

// Debug
static Identifier modeSlotToEdit("ModeSlotToDebug");
static Identifier modeLibraryIndex("ModeLibraryIndex");
static Identifier modeRootNote("ModeRootNote");
static Identifier keyNumberToEdit("KeyToDebug");


static Array<Identifier> pianoSettings =
{
    pianoUIMode,
    pianoWHRatio,
    keyboardKeysStyle,
    keyboardHighlightStyle,
    pianoKeyShowNumber
};

static Array<Identifier> midiSettings =
{
    periodShift,
    periodShiftModeSize,
    transposeAmt,
    keyboardMidiChannel,
    rootMidiNote // default root note
};

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

