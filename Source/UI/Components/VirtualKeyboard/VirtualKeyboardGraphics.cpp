/*
  ==============================================================================

    VirtualKeyboardGraphics.cpp
    Created: 25 Jul 2020 1:49:41am
    Author:  Vincenzo

  ==============================================================================
*/

#include "VirtualKeyboardGraphics.h"

VirtualKeyboard::KeyGraphics::KeyGraphics(const Mode& modeIn, int baseKeyHeightIn, int baseKeyWidthIn, float heightLayerMultIn, float widthLayerMultIn)
    :    mode(modeIn), 
        baseKeyHeight(baseKeyHeightIn),
        baseKeyWidth(baseKeyWidthIn),
        heightLayerMult(heightLayerMultIn),
        widthLayerMult(widthLayerMultIn)
{
    
}

void VirtualKeyboard::KeyGraphics::setBaseKeyWidth(int keyWidthIn)
{
    baseKeyWidth = keyWidthIn;
}

void VirtualKeyboard::KeyGraphics::setBaseKeyHeight(int keyHeightIn)
{
    baseKeyHeight = keyHeightIn;
}

void VirtualKeyboard::KeyGraphics::setWidthMultiplier(float widthMultIn)
{
    widthLayerMult = widthMultIn;
}

void VirtualKeyboard::KeyGraphics::setHeightMultiplier(float heightMultIn)
{
    heightLayerMult = heightMultIn;
}

void VirtualKeyboard::KeyGraphics::setKeyPlacementStyle(VirtualKeyboard::KeyPlacementType placementTypeIn)
{
    keyPlacement = placementTypeIn;
}

Array<Path> VirtualKeyboard::KeyGraphics::getKeyboardOctave(int scaleDegreeOffset)
{
    Array<Path> octavePathOut;

    int scaleIndex = mode.getRootNote() + scaleDegreeOffset;
    for (auto step : mode.getSteps())
    {
        for (int i = 0; i < step; i++)
        {
            octavePathOut.add(getKey(scaleIndex++));
        }
    }

    return octavePathOut;
}

Array<Path> VirtualKeyboard::KeyGraphics::getFullKeyboard()
{
    Array<Path> keyboardPathOut;

    //Array<Path> keyboardPeriod = getKeyboardOctave();
    //Array<Path> keyboardBegin = getKeyboardOctave(-(mode.getRootNote() * 2));
    //Array<Path> keyboardEnd = getKeyboardOctave(128 - mode.getRootNote() - mode.getScaleSize());

    //keyboardPathOut.addArray(keyboardBegin, mode.getRootNote() & mode.getScaleSize());
    //
    //for (int period = 1; period < 128 / mode.getScaleSize(); period++)
    //{
    //    keyboardPathOut.addArray(keyboardPeriod);
    //}

    //int endStart = (128 - mode.getRootNote() % mode.getScaleSize());
    //keyboardPathOut.addArray(keyboardEnd, endStart, mode.getScaleSize() - endStart);

    for (int i = 0; i < 128; i++)
        keyboardPathOut.add(getKey(i));


    return keyboardPathOut;
}

Path VirtualKeyboard::KeyGraphics::getKey(int keyNumber)
{
    int order = mode.getOrders()[keyNumber];
    int step = mode.getStepsOfOrders()[keyNumber];

    int prevOrder = mode.getOrders()[keyNumber - 1];
    int prevStep = mode.getStepsOfOrders()[keyNumber - 1];

    int nextOrder = mode.getOrders()[keyNumber + 1];
    int nextStep = mode.getStepsOfOrders()[keyNumber + 1];

    // get key style
    int keyShape = 0;
    if (order == 0)
    {
        // has left notch
        if (prevStep > 1)
        {
            keyShape += VirtualKeyboard::NotchPrevious;
        }

        // has right notch
        if (nextStep > 1)
        {
            keyShape += VirtualKeyboard::NotchNext;
        }
    }
    // Not a top layer key
    else if (order < step - 1)
    {
        if (keyPlacement == VirtualKeyboard::nestedCenter)
            keyShape = VirtualKeyboard::nestedCenter;
        else
            keyShape += VirtualKeyboard::NotchNext;
    }
    else
    {
        // Key is a rectangle
    }

    float keyWidth = baseKeyWidth - (order * baseKeyWidth * widthLayerMult);
    float keyHeight = baseKeyHeight - (order * baseKeyHeight * heightLayerMult);

    Array<Point<float>> keyPathPoints;
    keyPathPoints.add({ 0, keyHeight });
    keyPathPoints.add({ keyWidth, keyHeight });

    if (keyShape & VirtualKeyboard::NotchPrevious != 0)
    {
        float notchWidth = (baseKeyWidth - (prevOrder * baseKeyWidth * widthLayerMult)) / 2.0f;
        float prevKeyHeight = baseKeyHeight - (prevOrder * baseKeyHeight * heightLayerMult);

        keyPathPoints.insert(0, { 0, prevKeyHeight });
        keyPathPoints.insert(0, { notchWidth, prevKeyHeight });
        keyPathPoints.insert(0, { notchWidth, 0 });
    }
    else
    {
        keyPathPoints.insert(0, { 0, 0 });
    }

    if (keyShape & VirtualKeyboard::NotchNext != 0)
    {
        float notchX = keyWidth - (baseKeyWidth - (nextOrder * baseKeyWidth * widthLayerMult)) / 2.0f;
        float nextKeyHeight = baseKeyHeight - (nextOrder * baseKeyHeight * heightLayerMult);

        keyPathPoints.add({ keyWidth, nextKeyHeight });
        keyPathPoints.add({ notchX, nextKeyHeight });
        keyPathPoints.add({ notchX, 0 });
    }
    else
    {
        keyPathPoints.add({ keyWidth, 0 });
    }

    if (keyShape == VirtualKeyboard::NotchCenter)
    {
        float nextKeyWidth = (baseKeyWidth - (nextOrder * baseKeyWidth * widthLayerMult)) / 2.0f;
        float notchX0 = (keyWidth - nextKeyWidth) / 2;
        float notchX1 = keyWidth - notchX0;
        float nextKeyHeight = baseKeyHeight - (nextOrder * baseKeyHeight * heightLayerMult);

        keyPathPoints.insert(0, { 0, 0 });
        keyPathPoints.insert(0, { notchX0, 0 });
        keyPathPoints.insert(0, { notchX0, nextKeyHeight });
        keyPathPoints.add({ keyWidth, 0 });
        keyPathPoints.add({ notchX1, 0 });
        keyPathPoints.add({ notchX1, nextKeyHeight });
    }

    // Build Path
    Path keyPath;
    keyPath.startNewSubPath(keyPathPoints.removeAndReturn(0));
    for (auto point : keyPathPoints)
    {
        keyPath.lineTo(point);
    }

    keyPath.closeSubPath();

    return keyPath;
}