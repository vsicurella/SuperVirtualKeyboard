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
	setDefaultColor(Colours::transparentBlack);
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
	showNoteLabel = showNoteNameIn;
	setDefaultColor(colorIn);
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
      keyToCopy.showNoteNumber, keyToCopy.showNoteLabel, keyToCopy.defaultColor)
{
    node = keyToCopy.node.createCopy();
}

void Key::setPath(Path keyPathIn)
{
	keyPath = keyPathIn;
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
		else if (id == IDs::keyboardShowsNoteLabels)
			showNoteLabel = nodeIn.getProperty(id);
		else if (id == IDs::pianoKeyColor)
			setDefaultColor(Colour::fromString(nodeIn.getProperty(id).toString()));
	}
}

Colour Key::getDefaultColor() const
{
	return defaultColor;
}

Colour Key::getCustomColor() const
{
	return customColor;
}

void Key::paint(Graphics& g)
{
	Colour colorToUse = customColor.isOpaque() 
		? customColor
		: defaultColor;

	float contrast = 0;

	if (isMouseOver())
	{
		contrast = 0.25f;
	}
	if (isClicked)
	{
		contrast = 0.75f;
	}
	else if (exPressed)
	{
		colorToUse = exInputColor;
	}

	g.setColour(colorToUse.contrasting(contrast));
	g.fillRect(getLocalBounds());

	if (order == 0)
	{
		Rectangle<int> outline = getLocalBounds();
		g.setColour(Colours::black);
		g.drawRect(outline, 1.0f);
	}

	if (showNoteNumber)
	{
		g.setColour(defaultColor.contrasting());
		String numTxt = String(keyNumber);
		Rectangle<int> txtArea = getLocalBounds();
		txtArea.removeFromBottom(6);
		g.drawText(numTxt, txtArea,	Justification::centredBottom);
	}

	isDirty = false;
}


void Key::resized()
{
    
}

void Key::mouseExit(const MouseEvent& e)
{
	//// is mouse outside of parent?
	//bool hold = getParentComponent()->reallyContains(e.getEventRelativeTo(getParentComponent()).getPosition(), true);
	//if (!e.mods.isShiftDown())
	//{
	//	isClicked = false;
	//	repaint();
	//}

}

void Key::setDefaultColor(Colour colorIn)
{
	defaultColor = colorIn;
	customColor = Colours::transparentBlack;
	repaint();
}

void Key::setCustomColor(Colour colorIn)
{
	customColor = colorIn;
	repaint();
}