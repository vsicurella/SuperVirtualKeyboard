/*
  ==============================================================================

    FractionalGrid.h
    Created: 24 Mar 2019 1:58:51pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Mode.h"
#include "../PluginState.h"
#include "../UI/VirtualKeyboard/Keyboard.h"

#pragma once

struct SvkPreset
{
	ValueTree parentNode;
	ValueTree theModeNode;
	ValueTree theKeyPlaceNode;
	ValueTree theKeyRatiosNode;
	ValueTree theKeyColorsNode;

	SvkPreset(ValueTree modeNodeIn, ValueTree pianoNodeIn);
	SvkPreset(Mode* modeIn, VirtualKeyboard::Keyboard* pianoIn);
	SvkPreset(ValueTree presetNodeIn);
	SvkPreset(SvkPreset& presetToCopy);
	~SvkPreset();

	bool writeToFile(String absoluteFilePath="");
	static SvkPreset loadFromFile(String absoluteFilePath = "");
   
	String toString();

private:

	
};
