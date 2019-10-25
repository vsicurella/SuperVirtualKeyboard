/*
  ==============================================================================

    VirtualKeyboardKey.cpp
    Created: 17 Apr 2019 2:11:53pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "VirtualKeyboardKey.h"

using namespace VirtualKeyboard;

Key::Key()
{
    setName("Key");
    node = ValueTree(IDs::pianoKeyNode);
}

Key::Key(int keyNumIn)
: Key()
{
    keyNumber = keyNumIn;
}

Key::Key(int keyNumIn, int orderIn, int scaleDegreeIn, int modeDegreeIn, int stepIn,
	String pitchNameIn, int widthModIn, int heightModIn, int xOff, int yOff,
	bool showNoteNumIn, bool showNoteNameIn, Colour colorIn)
: Key()
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

Key::Key(ValueTree keyNodeIn)
    : Key()
{
    if (keyNodeIn.hasType(IDs::pianoKeyNode))
    {
        applyParameters(keyNodeIn);
        node = keyNodeIn;
    }
}

Key::Key(const Key& keyToCopy)
: Key(keyToCopy.keyNumber, keyToCopy.order, keyToCopy.scaleDegree, keyToCopy.modeDegree, keyToCopy.step,
      keyToCopy.pitchName, keyToCopy.widthMod, keyToCopy.heightMod, keyToCopy.xOffset, keyToCopy.yOffset,
      keyToCopy.showNoteNumber, keyToCopy.showNoteName, keyToCopy.color)
{
    node = keyToCopy.node.createCopy();
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
			color = Colour::fromString(nodeIn.getProperty(id).toString());
	}
}

void Key::paint(Graphics& g)
{
	Colour colorToUse = color;
	float contrast = 0;

	if (isMouseOver())
	{
		contrast = 0.25f;
	}

	if (isPressed)
	{
		contrast = 0.75f;
	}
	else if (exInputColor.isOpaque())
	{
		colorToUse = exInputColor;
	}

	g.setColour(colorToUse.contrasting(contrast));
	g.fillRect(getLocalBounds());

	Rectangle<int> outline = getLocalBounds().expanded(1, 1);
	g.setColour(Colours::black);
	g.drawRect(outline, 1.2f);

	if (showNoteNumber)
	{
		g.setColour(color.contrasting());
		String numTxt = String(keyNumber);
		Rectangle<int> txtArea = getLocalBounds();
		txtArea.removeFromBottom(6);
		g.drawText(numTxt, txtArea,	Justification::centredBottom);
	}
}


void Key::resized()
{
    
}

void Key::mouseExit(const MouseEvent& e)
{
	if (!isPressed && !e.mods.isShiftDown())
	{
		repaint();
	}
}
