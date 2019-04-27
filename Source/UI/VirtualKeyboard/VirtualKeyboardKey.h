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
        
        // more of a "status" flag
        int activeState = 0;

		std::unique_ptr<PopupMenu> editMenu;
		std::unique_ptr<ColourSelector> colorSelector;
        
        // Methods
        
        Key(String nameIn, int keyNumIn);
        
        // returns the decimal part of the mode degree
        float getDegree();
        
        void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
        
        void restore_from_node(ValueTree parentNodeIn);

		//==============================================================================

		void mouseEnter(const MouseEvent& e) override;
		void mouseExit(const MouseEvent& e) override;
		void mouseDown(const MouseEvent& e) override;
		void mouseUp(const MouseEvent& e) override;

		bool keyStateChanged(bool isKeyDown) override;
    };
}
