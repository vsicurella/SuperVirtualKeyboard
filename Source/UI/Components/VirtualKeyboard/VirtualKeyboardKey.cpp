/*
  ==============================================================================

    VirtualKeyboardKey.cpp
    Created: 17 Apr 2019 2:11:53pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "VirtualKeyboardKey.h"

VirtualKeyboard::Key::Key()
{
    setName("Key");
    node = ValueTree(IDs::pianoKeyNode);
    setDisplayColor(Colours::transparentBlack);
}

VirtualKeyboard::Key::Key(int keyNumIn)
    : Key()
{
    keyNumber = keyNumIn;
}

VirtualKeyboard::Key::Key(int keyNumIn, int orderIn, int scaleDegreeIn, int modeDegreeIn, int stepIn,
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
    color = colorIn;
}

VirtualKeyboard::Key::Key(ValueTree keyNodeIn)
    : Key()
{
    if (keyNodeIn.hasType(IDs::pianoKeyNode))
    {
        applyParameters(keyNodeIn);
        node = keyNodeIn;
    }
}

VirtualKeyboard::Key::Key(const Key& keyToCopy)
: Key(keyToCopy.keyNumber, keyToCopy.order, keyToCopy.scaleDegree, keyToCopy.modeDegree, keyToCopy.step,
      keyToCopy.pitchName, keyToCopy.widthMod, keyToCopy.heightMod, keyToCopy.xOffset, keyToCopy.yOffset,
      keyToCopy.showNoteNumber, keyToCopy.showNoteLabel, keyToCopy.color)
{
    node = keyToCopy.node.createCopy();
}

void VirtualKeyboard::Key::setPath(Path keyPathIn)
{
    keyPath = keyPathIn;
}

void VirtualKeyboard::Key::applyParameters(ValueTree nodeIn)
{
    if (nodeIn.hasType(IDs::pianoKeyNode))
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
            else if (id == IDs::pianoKeyColorsNode)
                color = Colour::fromString(nodeIn.getProperty(id).toString());
        }

        node.copyPropertiesAndChildrenFrom(nodeIn, nullptr);
    }
}

Colour VirtualKeyboard::Key::getDisplayColor() const
{
    return color;
}

void VirtualKeyboard::Key::paint(Graphics& g)
{
    g.fillAll(color);

    Colour colorToUse = color;

    if (isClicked || isMouseOver() || exPressed)
    {
        float contrast = 0;
        if (isClicked)
            contrast = 0.7f;
        else if (isMouseOver())
            contrast = 0.25f;
        
        if (exPressed)
            colorToUse = exInputColor;
        else
            colorToUse = color.contrasting(contrast);

        if (colorToUse.getPerceivedBrightness() < 0.6667f)
            g.setColour(colorToUse.contrasting(0.333f));
        
        g.setColour(colorToUse);

        switch (highlightStyleId)
        {
        case HighlightStyle::full:
        case HighlightStyle::inside:
        {
            Rectangle<int> bounds = getLocalBounds();

            if (highlightStyleId == HighlightStyle::inside)
            {
                int margin = sqrt(getWidth() * getHeight()) * 0.07;
                bounds.reduce(margin, margin);
            }

            g.fillRect(bounds);
            break;
        }

        case HighlightStyle::outline:
        {
            float margin = sqrt(getWidth() * getHeight()) * 0.07f;
            g.drawRect(getLocalBounds(), margin);
            break;
        }

        case HighlightStyle::circles:
        case HighlightStyle::squares:
        {
            float margin = sqrt(getWidth() * getHeight()) * 0.05f;
            float boundsWidth = getWidth() - margin * 2;
            Rectangle<float> shapeBounds = { margin, getHeight() - margin - boundsWidth, boundsWidth, boundsWidth };

            // Circle
            if (highlightStyleId == HighlightStyle::circles)
                g.fillEllipse(shapeBounds);

            // Square
            else
                g.fillRect(shapeBounds);
            break;
        }
        }
    }

    if (order == 0)
    {
        if (colorToUse.getPerceivedBrightness() < 0.6667f)
            g.setColour(colorToUse.contrasting(0.333f));

        else
            g.setColour(Colours::black);

        Rectangle<int> outline = getLocalBounds();    
        g.drawRect(outline, 1.0f);
    }

    if (showNoteNumber)
    {
        g.setColour(colorToUse.contrasting());
        String numTxt = String(keyNumber);
        Rectangle<int> txtArea = getLocalBounds();
        txtArea.removeFromBottom(6);
        g.drawText(numTxt, txtArea,    Justification::centredBottom);
    }

    isDirty = false;
}

void VirtualKeyboard::Key::setDisplayColor(Colour colorIn)
{
    color = colorIn;
    node.setProperty(IDs::pianoKeyColorsNode, color.toString(), nullptr);
    repaint();
}
