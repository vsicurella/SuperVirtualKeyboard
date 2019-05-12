/*
  ==============================================================================

    FractionalGrid.h
    Created: 24 Mar 2019 1:58:51pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginIDs.h"
#pragma once

struct SvkPreset
{
	ValueTree parentNode;
	ValueTree theModeNode;
	ValueTree theKeyboardNode;

	SvkPreset();
	SvkPreset(ValueTree modeNodeIn, ValueTree keyboardNodeIn);
	SvkPreset(ValueTree presetNodeIn);
	SvkPreset(SvkPreset& presetToCopy);
	~SvkPreset();

	bool updateModeNode(ValueTree modeNodeIn);
	bool updateKeyboardNode(ValueTree keyboardNodeIn);

	bool writeToFile(String absoluteFilePath="");
	static SvkPreset loadFromFile(String absoluteFilePath="");
   
	String toString();	
};
