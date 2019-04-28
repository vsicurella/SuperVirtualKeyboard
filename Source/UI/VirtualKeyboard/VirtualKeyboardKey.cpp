/*
  ==============================================================================

    VirtualKeyboardKey.cpp
    Created: 17 Apr 2019 2:11:53pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "VirtualKeyboardKey.h"

using namespace VirtualKeyboard;

Key::Key(String nameIn, int keyNumIn)
: Button(nameIn)
{
    keyNumber = keyNumIn;
    mappedMIDInote = keyNumber;
	
	setColour(0, Colours::transparentBlack);
    setOpaque(true);
}

float Key::getDegree()
{
    return degree;
}

void Key::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	Rectangle<int> fillBounds = getLocalBounds();

    Colour color = findColour(activeState);
    
	/* // for specific "pressed" color, currently deprecated 
    if (activeState == 2)
    {
        color = findColour(0).interpolatedWith(findColour(2), 0.618);
    }
    */

    if (externalMidiState > 0)
        color = color.interpolatedWith(Colours::lightblue, 0.75);
    
    g.setColour(color);
    g.fillRect(fillBounds);

	g.setColour(Colours::black);
	g.drawRect(fillBounds);
}

void Key::restore_from_node(ValueTree keyNodeIn)
{
    pianoKeyNode = keyNodeIn;
	keyNumber = pianoKeyNode[IDs::pianoKeyNumber];
    mappedMIDInote = pianoKeyNode[IDs::pianoKeyMidiNote];
    widthMod = pianoKeyNode[IDs::pianoKeyWidthMod];
    heightMod = pianoKeyNode[IDs::pianoKeyHeightMod];
    xOffset = pianoKeyNode[IDs::pianoKeyXOffset];
    yOffset = pianoKeyNode[IDs::pianoKeyYOffset];
	customColor = (bool)pianoKeyNode[IDs::pianoKeyColorIsCustom];
	setColour(0, Colour::fromString(pianoKeyNode[IDs::pianoKeyColorDefault].toString()));
}
//==============================================================================

void Key::mouseExit(const MouseEvent& e)
{
	if (activeState <= 1 && !(e.mods.isShiftDown() && activeState == 2))
		activeState = 0;
}