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
#include "../CommonFunctions.h"

struct SvkPreset
{
	ValueTree parentNode;
	ValueTree theModeNode;
	ValueTree theKeyboardNode;
    ValueTree theMapNode;

	SvkPreset();
	SvkPreset(ValueTree presetNodeIn);
	SvkPreset(SvkPreset& presetToCopy);
	~SvkPreset();

    bool updateModeNode(ValueTree modeNodeIn);
    
    bool updateKeyboardNode(ValueTree modeNodeIn);
    
    bool updateMapNode(ValueTree mapNodeIn);

	bool writeToFile(String absoluteFilePath="");
       
	String toString();	

};
