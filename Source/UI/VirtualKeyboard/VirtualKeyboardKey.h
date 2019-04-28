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
        ValueTree pianoKeyNode;
        
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
        float degree; 
        int step;
        
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
        
        void restore_from_node(ValueTree parentNodeIn);

		//==============================================================================

		void mouseExit(const MouseEvent& e) override;
    };
}
