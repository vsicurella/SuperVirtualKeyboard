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
	
	editMenu.reset(new PopupMenu());
	colorSelector.reset(new ColourSelector(ColourSelector::ColourSelectorOptions::showColourspace));

    setOpaque(true);
}

float Key::getDegree()
{
    return degree;
}

void Key::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    // idk why getBounds() doesn't work properly. maybe it's set in the parent bounds
    Rectangle<int> fillBounds = Rectangle<int>(0, 0, getWidth() - 1, getHeight());
    Colour color = findColour(activeColor);
    
    if (activeColor == 2)
    {
        color = findColour(0).interpolatedWith(findColour(2), 0.618);
        fillBounds.reduce(1, 0);
    }
    
    if (externalMidiState > 0)
        color = color.interpolatedWith(Colours::lightblue, 0.75);
    
    
    g.setColour(color);
    g.fillRect(fillBounds);
}

void Key::restore_from_node(ValueTree parentNodeIn)
{
    pianoKeyNode = parentNodeIn.getChild(keyNumber);
    mappedMIDInote = pianoKeyNode[IDs::pianoKeyMidiNote];
    widthMod = pianoKeyNode[IDs::pianoKeyWidthMod];
    heightMod = pianoKeyNode[IDs::pianoKeyHeightMod];
    xOffset = pianoKeyNode[IDs::pianoKeyXOffset];
    yOffset = pianoKeyNode[IDs::pianoKeyYOffset];
    
    setColour(0, Colour::fromString(pianoKeyNode[IDs::pianoKeyColorDefault].toString()));
    setColour(1, Colour::fromString(pianoKeyNode[IDs::pianoKeyColorHighlighted].toString()));
    setColour(2, Colour::fromString(pianoKeyNode[IDs::pianoKeyColorPressed].toString()));


}
//==============================================================================

void Key::mouseDown(const MouseEvent& e)
{
	if (e.mods.isRightButtonDown())
	{
		
	}
}

void Key::mouseUp(const MouseEvent& e)
{

}

bool Key::keyStateChanged(bool isKeyDown)
{
	return false;
}
