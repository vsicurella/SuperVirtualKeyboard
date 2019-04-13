/*
  ==============================================================================

    PluginState.h
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

// IDENTIFIERS

namespace IDs
{
	// Generic IDs
	static Identifier itemName("Name");
	
	// Processor
	static Identifier processorNode("ProcessorNode");
	static Identifier selectedModeName("SelectedModeName");
	static Identifier externalMidiState("MidiInputState");
	static Identifier presetsNode("Presets");

	// ModeLayout
	static Identifier modeLayoutNode("ModeLayoutNode");
	static Identifier scaleSize("ScaleSize");
	static Identifier modeSize("ModeSize");
	static Identifier stepString("StepsString");
	static Identifier MOSClass("MosClass");
	static Identifier temperamentFamily("Family");
	static Identifier modeFullName("ModeName");
	static Identifier modeScaleName("NameScaleSize");
	static Identifier modeModeName("NameModeSize");
	static Identifier stepArray("StepsArray");
	static Identifier keyboardOrderArray("OrderArray");
	static Identifier keyboardModeDegrees("ModeDegreeArray");
	static Identifier stepValue("StepsValue");
	static Identifier orderValue("OrderValue");
	static Identifier degreeValue("DegreeValue");


	// Keyboard Window
	static Identifier keyboardWindowNode("KeyboardWindowNode");
	static Identifier windowBoundsW("WindowW");
	static Identifier windowBoundsH("WindowH");
	static Identifier viewportPosition("ViewportPosition");
	static Identifier selectedPresetName("SelectedPresetName");
    static Identifier selectedPresetIndex("SelectedPresetIndex");
    static Identifier selectedPresetComboID("SelectedPresetComboboxIndex");
	
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
		loadCustomLayout = 1,
		saveCustomLayout,
		saveReaperMap,
		setKeyColor,
		setMidiNoteOffset,
		pianoPlayMode,
		pianoEditMode,
		setPianoHorizontal,
		setPianoVerticalL,
		setPianoVerticalR,
	};

	template <class T>
	static Array<T> vector_to_juce_array(std::vector<T> vectorIn)
	{
		Array<T> arrayOut;

		for (auto item : vectorIn)
			arrayOut.add(item);

		return arrayOut;
	}
}


enum SortType
{
	scaleSize,
	modeSize,
	family,
	alphabetical
};

struct SuperVirtualKeyboardPluginState
{
	ValueTree processorNode;
	ValueTree presetsNode;
	ValueTree modeLayoutNode;
	ValueTree keyboardWindowNode;
	ValueTree pianoNode;
	ValueTree pianoKeyNode;

	SuperVirtualKeyboardPluginState(UndoManager* undoManagerIn)
		:	undoManager(undoManagerIn),
			processorNode(IDs::processorNode),
			presetsNode(IDs::presetsNode),
			modeLayoutNode(IDs::modeLayoutNode),
			pianoKeyNode(IDs::pianoKeyNode)
	{
	}

	~SuperVirtualKeyboardPluginState() {}

private:
	UndoManager* undoManager;
};
