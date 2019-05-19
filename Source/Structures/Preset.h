/*
  ==============================================================================

    FractionalGrid.h
    Created: 24 Mar 2019 1:58:51pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginIDs.h"

struct SvkPreset
{
	ValueTree parentNode;
	ValueTree theModeNode;
	ValueTree theKeyboardNode;

	SvkPreset();
	SvkPreset(ValueTree presetNodeIn);
	SvkPreset(SvkPreset& presetToCopy);
	~SvkPreset();

	bool updateModeNode(ValueTree modeNodeIn);
	bool updateKeyboardNode(ValueTree keyboardNodeIn);

	bool writeToFile(String absoluteFilePath="");
    
    // make sure this is deleted when not needed
	static SvkPreset* loadFromFile(String absoluteFilePath="");
   
	String toString();	
};
