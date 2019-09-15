/*
  ==============================================================================

    VirtualKeyboardKey.cpp
    Created: 17 Apr 2019 2:11:53pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "VirtualKeyboardKey.h"

using namespace VirtualKeyboard;

Key::Key() {};

Key::Key(int keyNumIn)
{
    keyNumber = keyNumIn;
}

Key::Key(int keyNumIn, int orderIn, int scaleDegreeIn, int modeDegreeIn, int stepIn,
	String pitchNameIn, int widthModIn, int heightModIn, int xOff, int yOff,
	bool showNoteNumIn, bool showNoteNameIn, Colour colorIn)
{
	keyNumber = keyNumIn;
	order = orderIn;
	scaleDegree = scaleDegreeIn;
	modeDegree = modeDegreeIn;
	step = stepIn;
	pitchName = pitchNameIn;
	widthMod = widthModIn;
	heightMod = heightModIn;
	xOffset = xOff;
	yOffset = yOff;
	showNoteNumber = showNoteNumIn;
	showNoteName = showNoteNameIn;
	color = colorIn;
}

void Key::applyParameters(ValueTree nodeIn)
{
	Identifier id;

	for (int i = 0; i < nodeIn.getNumProperties(); i++)
	{
		id = nodeIn.getPropertyName(i);

		if (id == IDs::pianoKeyNumber)
			keyNumber = nodeIn.getProperty(id);
		else if (id == IDs::pianoKeyWidthMod)
			widthMod = nodeIn.getProperty(id);
		else if (id == IDs::pianoKeyHeightMod)
			heightMod = nodeIn.getProperty(id);
		else if (id == IDs::pianoKeyXOffset)
			xOffset = nodeIn.getProperty(id);
		else if (id == IDs::pianoKeyYOffset)
			yOffset = nodeIn.getProperty(id);
		else if (id == IDs::pianoKeyShowNumber)
			showNoteNumber = nodeIn.getProperty(id);
		else if (id == IDs::pianoKeyShowName)
			showNoteName = nodeIn.getProperty(id);
		else if (id == IDs::pianoKeyColor)
			color = Colour::fromString((String)nodeIn.getProperty(id));
	}
}
