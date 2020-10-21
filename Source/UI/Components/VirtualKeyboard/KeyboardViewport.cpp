/*
  ==============================================================================

    KeyboardViewPort.cpp
    Created: 31 Oct 2019 2:41:37pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "KeyboardViewport.h"

KeyboardViewport::KeyboardViewport(VirtualKeyboard::Keyboard* keyboardIn, const String& nameIn, int scrollingModeIn, int scrollingStyleIn)
: Viewport(nameIn)
{
    stepRightLarge.reset(new ImageButton());
    stepRightLarge->addListener(this);
    addChildComponent(stepRightLarge.get());
    
    stepRightSmall.reset(new ImageButton());
    stepRightSmall->addListener(this);
    addChildComponent(stepRightSmall.get());

    stepLeftLarge.reset(new ImageButton());
    stepLeftLarge->addListener(this);
    addChildComponent(stepLeftLarge.get());

    stepLeftSmall.reset(new ImageButton());
    stepLeftSmall->addListener(this);
    addChildComponent(stepLeftSmall.get());

    setScrollingMode(scrollingModeIn);
    setScrollingStyle(scrollingStyleIn);

    keyboard = keyboardIn;
    setViewedComponent(keyboard);
}

void KeyboardViewport::viewedComponentChanged(Component* newComponent)
{
    keyboard = dynamic_cast<Keyboard*>(newComponent);
    resizeKeyboard();
}

void KeyboardViewport::visibleAreaChanged(const Rectangle<int>&)
{
    if (keyboard->getHeight() > getMaximumVisibleHeight())
    {
        resizeKeyboard();
    }
}

int KeyboardViewport::getStepSmall()
{
    return stepSmall;
}

int KeyboardViewport::getStepLarge()
{
    return stepLarge;
}

int KeyboardViewport::getButtonWidth()
{
    if (isShowingButtons())
        return buttonWidth;
    
    return 0;
}

bool KeyboardViewport::isShowingButtons()
{
    return scrollingModeSelected > 1;
}

float KeyboardViewport::getCenterKeyProportion() const
{
    int center = getViewPositionX() + getMaximumVisibleWidth() / 2;
    Key* key = keyboard->getKeyFromPosition({ center, 0 });
    if (key)
        return key->keyNumber + (center % key->getWidth()) / (float)key->getWidth();

    return -1;
}

void KeyboardViewport::setButtonWidth(int widthIn)
{
    buttonWidth = widthIn;
}

void KeyboardViewport::setScrollingMode(int modeIdIn)
{
    scrollingModeSelected = modeIdIn;

    if (scrollingModeSelected > 1)
    {
        stepRightLarge->setVisible(true);
        stepRightSmall->setVisible(true);
        stepLeftLarge->setVisible(true);
        stepLeftSmall->setVisible(true);
    }
    else
    {
        stepRightLarge->setVisible(false);
        stepRightSmall->setVisible(false);
        stepLeftLarge->setVisible(false);
        stepLeftSmall->setVisible(false);
    }

}
void KeyboardViewport::setScrollingStyle(int styleIdIn)
{
    scrollingStyleSelected = styleIdIn;
    // do stuff
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

void KeyboardViewport::centerOnKey(int keyNumberIn)
{
    Key* key = keyboard->getKey(keyNumberIn);
    if (key)
    {
        int position = key->getBoundsInParent().getX() - getMaximumVisibleWidth() / 2;
        Viewport::setViewPosition(position, getViewPositionY());
    }
}

void KeyboardViewport::centerOnKey(float keyNumberProportionIn)
{
    Key* key = keyboard->getKey((int)keyNumberProportionIn);
    if (key)
    {
        int proportion = (keyNumberProportionIn - (int)keyNumberProportionIn) * key->getWidth();
        int position = key->getBoundsInParent().getX() + proportion - getMaximumVisibleWidth() / 2;
        Viewport::setViewPosition(position, getViewPositionY());
    }
}

int KeyboardViewport::drawAngleBracket(Graphics& g, bool isRightPointing, int sideLength, int xCenter, int yCenter, float thickness)
{
    float halfSide = sideLength / 2.0f;
    float altitude = sqrtf(powf(sideLength, 2.0f) - powf(halfSide, 2.0f));
    float halfAltitude = altitude / 2.0f;

    int x1 = xCenter - halfAltitude;
    int x2 = xCenter + halfAltitude;
    int y1 = yCenter + halfSide;
    int y2 = yCenter - halfSide;

    if (isRightPointing)
    {
        g.drawLine(Line<float>(x1, y1, x2, yCenter), thickness);
        g.drawLine(Line<float>(x1, y2, x2, yCenter), thickness);
    }
    else
    {
        g.drawLine(Line<float>(x2, y1, x1, yCenter), thickness);
        g.drawLine(Line<float>(x2, y2, x1, yCenter), thickness);
    }

    return altitude;
}


void KeyboardViewport::redrawButtons(int heightIn)
{
    Image singleBracketImage = Image(Image::PixelFormat::ARGB, buttonWidth, heightIn, false);
    Image doubleBracketImage = singleBracketImage.createCopy();
    Image singleBracketFlipped = singleBracketImage.createCopy();
    Image doubleBracketFlipped = singleBracketImage.createCopy();

    Colour buttonColour = Colours::steelblue.darker().withSaturation(0.33f);
    Colour bracketColour = Colours::black;
    Colour mouseOverColor = Colours::lightgrey.withAlpha(0.33f);
    Colour mouseClickColor = Colours::darkgrey.withAlpha(0.75f);
    
    int sideLength = round(buttonWidth * 0.33f);
    int altitude;
    int xCenter = buttonWidth / 2;
    int yCenter = getHeight() / 4;
    int doubleSeparation = 3;

    Graphics g1(singleBracketImage);
    g1.setColour(buttonColour);
    g1.fillAll();
    g1.setColour(bracketColour);
    altitude = drawAngleBracket(g1, true, sideLength, xCenter, yCenter, 1);


    Graphics g2(doubleBracketImage);
    g2.setColour(buttonColour);
    g2.fillAll();
    g2.setColour(bracketColour);
    drawAngleBracket(g2, true, sideLength, xCenter - doubleSeparation, yCenter, 1);
    drawAngleBracket(g2, true, sideLength, xCenter + doubleSeparation, yCenter, 1);

    Graphics g3(singleBracketFlipped);
    g3.setColour(buttonColour);
    g3.fillAll();
    g3.setColour(bracketColour);
    altitude = drawAngleBracket(g3, false, sideLength, xCenter, yCenter, 1);


    Graphics g4(doubleBracketFlipped);
    g4.setColour(buttonColour);
    g4.fillAll();
    g4.setColour(bracketColour);
    drawAngleBracket(g4, false, sideLength, xCenter - doubleSeparation, yCenter, 1);
    drawAngleBracket(g4, false, sideLength, xCenter + doubleSeparation, yCenter, 1);


    stepLeftSmall->setImages(false, false, false,
        singleBracketImage, 1.0f, Colours::transparentBlack,
        singleBracketImage, 1.0f, mouseOverColor,
        singleBracketImage, 1.0f, mouseClickColor);

    stepLeftLarge->setImages(false, false, false,
        doubleBracketImage, 1.0f, Colours::transparentBlack,
        doubleBracketImage, 1.0f, mouseOverColor,
        doubleBracketImage, 1.0f, mouseClickColor);
    
    stepRightSmall->setImages(false, false, false,
        singleBracketFlipped, 1.0f, Colours::transparentBlack,
        singleBracketFlipped, 1.0f, mouseOverColor,
        singleBracketFlipped, 1.0f, mouseClickColor);

    stepRightLarge->setImages(false, false, false,
        doubleBracketFlipped, 1.0f, Colours::transparentBlack,
        doubleBracketFlipped, 1.0f, mouseOverColor,
        doubleBracketFlipped, 1.0f, mouseClickColor);
}

void KeyboardViewport::resizeKeyboard()
{
    int heightWithScrollbar = getHeight() - getScrollBarThickness();
    keyboard->setBounds(0, 0, keyboard->getPianoWidth(heightWithScrollbar), heightWithScrollbar);
}

void KeyboardViewport::resized()
{
    Viewport::resized();

    stepSmall = keyboard->getWidth() / keyboard->getKeysByOrder(0).size();
    stepLarge = stepSmall * keyboard->getModeSize();
    
    // draw step buttons
    if (scrollingModeSelected > 1)
    {
        int halfHeight = round(getMaximumVisibleHeight() / 2.0f);
        redrawButtons(halfHeight);

        stepRightLarge->setBounds(0, 0, buttonWidth, halfHeight);
        stepRightSmall->setBounds(0, halfHeight, buttonWidth, halfHeight);
        stepLeftLarge->setBounds(getWidth() - buttonWidth, 0, buttonWidth, halfHeight);
        stepLeftSmall->setBounds(getWidth() - buttonWidth, halfHeight, buttonWidth, halfHeight);
    }
}

void KeyboardViewport::buttonClicked(Button* button)
{
    if (button == stepRightLarge.get())
    {
        stepLargeBackward();
    }
    else if (button == stepRightSmall.get())
    {
        stepSmallBackward();
    }
    else if (button == stepLeftLarge.get())
    {
        stepLargeForward();
    }
    else if (button == stepLeftSmall.get())
    {
        stepSmallForward();
    }
}
