/*
 ==============================================================================
 
 VirtualKeyboardKey.h
 Created: 17 Apr 2019 2:11:53pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../PluginIDs.h"

namespace VirtualKeyboard
{
    struct Key : public Button
    {        
        // Parameters
        int keyNumber;
        
        float widthMod = -1;
        float heightMod = -1;
        float xOffset = 0;
        float yOffset = 0;
        
        //Pitch* mappedPitch = nullptr;
        int mappedMIDInote;
        int externalMidiState = 0;
        
        // Properties
        int order; // front most keys are 0, "black" keys are 1, etc
		int scaleDegree; // corresponds to chromatic degree in the period
        float modeDegree; // fractional degree. in Meantone[7], F# would be 3.5
        int step; // the interval step that this note is a part of
        
        float degreeWidthRatio;
        float degreeHeightRatio;
        float velocity = 0;

		bool customColor = false;
        
        // more of a "status" flag
		// 0 = no interaction, 1 = mouseover, 2 = mouse down, 3 = color selecting
        int activeState = 0;
        
        // Methods
        
        Key(String nameIn, int keyNumIn);
        
        // returns the decimal part of the mode degree
        float getDegree();
        
        void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
        
		//==============================================================================

		void mouseExit(const MouseEvent& e) override;
    };
}
