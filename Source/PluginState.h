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
	static Identifier processorNode("Processor Node");
	static Identifier selectedModeName("Selecetd Mode Name");
	static Identifier externalMidiState("MIDI Input State");

	// ModeLayout
	static Identifier modeLayoutNode("ModeLayout Node");
	static Identifier scaleSize("Scale Size");
	static Identifier modeSize("Mode Size");
	static Identifier stepString("Steps (String)");
	static Identifier MOSClass("MOS Class");
	static Identifier stepArray("Steps (Array)");
	static Identifier keyboardOrderArray("Order Array");
	static Identifier keyboardModeDegrees("Mode Degree Array");

	// Keyboard Window
	static Identifier keyboardWindowNode("Keyboard Window Node");
	static Identifier windowBounds("Window Bounds");
	static Identifier viewportPosition("Viewport Position");
	static Identifier selectedPresetName("Selected Preset Name");
	
	// Piano Component
	static Identifier pianoNode("Piano Node");
	static Identifier pianoUIMode("Piano UI Mode");
	static Identifier pianoOrientation("Piano Orientation");
	static Identifier pianoKeyboardState("Piano Keyboard State");
	static Identifier pianoMidiChannel("Selected MIDI Channel");
	static Identifier pianoMidiNoteOffset("MIDI Node Offset");
	static Identifier pianoMPEToggle("MPE Toggle");
	static Identifier pianoKeyOrderPlacement("Placement of Ordered Keys");
	static Identifier pianoKeyOrderRatios("Key Size Ratios by Order");
	static Identifier pianoKeyOrderColors("Key Colors by Order");

	// Piano Key Component
	static Identifier pianoKeyNode("Piano Key Node");
	static Identifier pianokeyNumber("Key Number");
	static Identifier pianoKeyModeDegree("Key Mode Degree");
	static Identifier pianoKeyOrder("Key Order");
	static Identifier pianokeyMidiNote("Mapped MIDI Note");
	static Identifier pianoKeyWidthMod("Key Width Modifier");
	static Identifier pianoKeyHeightMod("Key Height Modifier");
	static Identifier pianoKeyXOffset("Key X Offset");
	static Identifier pianoKeyYOffset("Key Y Offset");
	static Identifier pianoKeyColorDefault("Key Color Default");
	static Identifier pianoKeyColorHighlighted("Key Color Highlighted");
	static Identifier pianoKeyColorPressed("Key Color Pressed");
	static Identifier pianoKeyColorExternal("Key Color MIDI Input");

	template <class T>
	static Array<T> vector_to_juce_array(std::vector<T> vectorIn)
	{
		Array<T> arrayOut;

		for (auto item : vectorIn)
			arrayOut.add(item);

		return arrayOut;
	}
}

struct SuperVirtualKeyboardPluginState
{
	ValueTree processorNode;
	ValueTree modeLayoutNode;
	ValueTree keyboardWindowNode;
	ValueTree pianoNode;
	ValueTree pianoKeyNode;

	SuperVirtualKeyboardPluginState(UndoManager* undoManagerIn)
		:	undoManager(undoManagerIn),
			processorNode(IDs::processorNode),
			modeLayoutNode(IDs::modeLayoutNode),
			keyboardWindowNode(IDs::keyboardWindowNode),
			pianoNode(IDs::pianoNode),
			pianoKeyNode(IDs::pianoKeyNode)
	{
	}

private:
	UndoManager* undoManager;
};