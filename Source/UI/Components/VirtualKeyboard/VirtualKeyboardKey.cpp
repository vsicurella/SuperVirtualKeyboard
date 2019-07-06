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
	
	setColour(0, Colours::transparentBlack);
    setOpaque(true);
}

float Key::getDegree()
{
    return modeDegree;
}

void Key::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	Rectangle<int> fillBounds = getLocalBounds();

    Colour color = findColour(activeState);
    
	/* // for specific "pressed" color, currently unused 
    if (activeState == 2)
    {
        color = findColour(0).interpolatedWith(findColour(2), 0.618);
    }
    */

	if (externalMidiState > 0)
	{
		float hue = ((externalMidiState - 1) / 16.0f) * 0.75f;
		Colour c = Colour(hue, 1.0f, 0.75f, 1.0f);
		color = color.interpolatedWith(c, 0.75f);
		//color = color.withHue((externalMidiState - 1) / 16.0f).withSaturation(1.0f).withBrightness(0.75f);
	}

    g.setColour(color);
    g.fillRect(fillBounds);

	fillBounds.expand(1, 1);
	g.setColour(Colours::black);
	g.drawRect(fillBounds, 1.2);
    
    if (showNoteNumber && mappedNoteIn >= 0)
    {
        g.setColour(color.contrasting());
        String numTxt = String(mappedNoteIn);
        Rectangle<int> txtArea = getLocalBounds();
        txtArea.removeFromBottom(6);
        g.drawText(numTxt,
                   txtArea,
                   Justification::centredBottom);
    }
}

//==============================================================================

void Key::mouseExit(const MouseEvent& e)
{
	if (activeState <= 1 && !(e.mods.isShiftDown() && activeState == 2))
		activeState = 0;
}
