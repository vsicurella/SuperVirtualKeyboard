/*
  ==============================================================================

    KeyboardViewPort.cpp
    Created: 31 Oct 2019 2:41:37pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "KeyboardViewPort.h"

KeyboardViewport::KeyboardViewport(const String& nameIn)
: Viewport(nameIn)
{
    
}

int KeyboardViewport::getStepSmall()
{
    return stepSmall;
}

int KeyboardViewport::getStepLarge()
{
    return stepLarge;
}

void KeyboardViewport::setStepSmall(int smallSizeIn)
{
    stepSmall = smallSizeIn;
}

void KeyboardViewport::setStepLarge(int largeSizeIn)
{
    stepLarge = largeSizeIn;
}

void KeyboardViewport::stepSmallForward()
{
    Viewport::setViewPosition(getViewPositionX() + stepSmall, getViewPositionY());
}

void KeyboardViewport::stepSmallBackward()
{
    Viewport::setViewPosition(getViewPositionX() - stepSmall, getViewPositionY());

}

void KeyboardViewport::stepLargeForward()
{
    Viewport::setViewPosition(getViewPositionX() + stepLarge, getViewPositionY());

}

void KeyboardViewport::stepLargeBackward()
{
    Viewport::setViewPosition(getViewPositionX() - stepLarge, getViewPositionY());

}

void KeyboardViewport::paint(Graphics& g)
{
    Viewport::paint(g);
    
    // paint the buttons
}

void KeyboardViewport::resized()
{
    Viewport::resized();
    
    // resize the step sizes
}
