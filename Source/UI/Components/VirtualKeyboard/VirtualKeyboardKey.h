/*
 ==============================================================================
 
 VirtualKeyboardKey.h
 Created: 17 Apr 2019 2:11:53pm
 Author:  Vincenzo

 Struct for holding virtual keyboard key data.
 
 ==============================================================================
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../../PluginIDs.h"
#include "../../../PluginModes.h"

namespace VirtualKeyboard
{
    class Key : public Component
    {
        
    public:

        Key();
        Key(int keyNumIn);
        Key(int keyNumIn, int orderIn, int scaleDegreeIn, int modeDegreeIn, int stepIn,
            String pitchNameIn = "", int widthModIn = 0, int heightModIn = 0, int xOff = 0, int yOff = 0,
            bool showNoteNumIn = false, bool showNoteNameIn = false, Colour colorIn = Colours::transparentBlack);
        Key(ValueTree keyNodeIn);
        Key(const Key& keyToCopy);

        Colour getDisplayColor() const;

        void setDisplayColor(Colour colorIn);
        
        void applyParameters(ValueTree nodeIn);

        void setPath(Path keyPathIn);
        
        void paint(juce::Graphics& g) override;

        ValueTree node;

        Path keyPath;

        // Properties
        int keyNumber = 0;
        int order = 0; // front most keys are 0, "black" keys are 1, etc
        int scaleDegree = 0; // corresponds to chromatic degree in the period
        float modeDegree = 0; // fractional degree. in Meantone[7] in 12edo set in C, F# would be 3.5
        int step = 0; // the interval step that this note is a part of

        float velocity = 0; // recorded when key is clicked
        bool isClicked = false; // clicked by mouse
        bool exPressed = false; // pressed by midi input
        Colour exInputColor; // for midi controller input, will use this color if it's opaque
        bool isDirty = true;

        // Parameters
        String pitchName = "";
        float widthMod = -1;
        float heightMod = -1;
        float xOffset = 0;
        float yOffset = 0;
        bool showNoteNumber = false;
        bool showNoteLabel = false;
        float velocityFixed = 1;

        Colour color = Colours::transparentBlack;
        int highlightStyleId = HighlightStyle::full;
    };
}
