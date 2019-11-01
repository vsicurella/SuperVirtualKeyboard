/*
  ==============================================================================

    KeyboardViewPort.cpp
    Created: 31 Oct 2019 2:41:37pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "KeyboardViewPort.h"

KeyboardViewport::KeyboardViewport(const String& nameIn, bool showStepButtonsIn)
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

	setShowButtons(showStepButtonsIn);
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
	return buttonWidth;
}


void KeyboardViewport::setStepSmall(int smallSizeIn)
{
    stepSmall = smallSizeIn;
}

void KeyboardViewport::setStepLarge(int largeSizeIn)
{
    stepLarge = largeSizeIn;
}

void KeyboardViewport::setButtonWidth(int widthIn)
{
	buttonWidth = widthIn;
}

void KeyboardViewport::setShowButtons(bool toShowButtons)
{
	showStepButtons = toShowButtons;

	if (showStepButtons)
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

int KeyboardViewport::drawAngleBracket(Graphics& g, bool isLeft, int sideLength, int xCenter, int yCenter, float thickness)
{

	float halfSide = sideLength / 2.0f + yCenter;
	float altitude = sqrtf(powf(buttonWidth, 2) - powf(buttonWidth / 2.0f, 2));

	int xc = (int)(xCenter - altitude / 2.0f);
	int xa = altitude + xc;

	if (isLeft)
	{
		g.drawLine(Line<float>(xc, halfSide, xa, yCenter), thickness);
		g.drawLine(Line<float>(xc, -halfSide, xa, yCenter), thickness);
	}
	else
	{
		g.drawLine(Line<float>(xa, halfSide, xc, yCenter), thickness);
		g.drawLine(Line<float>(xa, -halfSide, xc, yCenter), thickness);
	}

	return altitude;
}


void KeyboardViewport::redrawButtons()
{
	singleBracketImage = Image(Image::PixelFormat::ARGB, buttonWidth, getHeight()/4, false);
	doubleBracketImage = Image(Image::PixelFormat::ARGB, buttonWidth, getHeight()/4, false);
	
	Graphics g1(singleBracketImage);
	int sideLength = round(buttonWidth * 0.8f);
	int altitude;
	int xCenter = buttonWidth / 2;
	int yCenter = getHeight() / 4;

	altitude = drawAngleBracket(g1, true, sideLength, xCenter, yCenter, 2);

	Graphics g2(doubleBracketImage);
	int xc = (int)(xCenter - (altitude / 2.0f));
	int yc = (int)(yCenter + (sideLength / 2.0f));
	doubleBracketImage = singleBracketImage;
	doubleBracketImage.moveImageSection(xc + 13, yc, xc, yc, altitude, sideLength);

	stepLeftSmall->setImages(false, false, false,
		singleBracketImage, 0.01f, Colours::lightslategrey,
		singleBracketImage, 0.68f, Colours::lightslategrey,
		singleBracketImage, 1.0f, Colours::darkslategrey);

	stepLeftLarge->setImages(false, false, false,
		doubleBracketImage, 0.01f, Colours::lightslategrey,
		doubleBracketImage, 0.68f, Colours::lightslategrey,
		doubleBracketImage, 1.0f, Colours::darkslategrey);
		
	AffineTransform af = AffineTransform();
	af.rotated(355.0f / 113.0f);
	g1.drawImageTransformed(singleBracketImage, af);
	g2.drawImageTransformed(doubleBracketImage, af);
	
	stepRightSmall->setImages(false, false, false,
		singleBracketImage, 0.01f, Colours::lightslategrey,
		singleBracketImage, 0.68f, Colours::lightslategrey,
		singleBracketImage, 1.0f, Colours::darkslategrey);

	stepRightLarge->setImages(false, false, false,
		doubleBracketImage, 0.01f, Colours::lightslategrey,
		doubleBracketImage, 0.68f, Colours::lightslategrey,
		doubleBracketImage, 1.0f, Colours::darkslategrey);

	g1.drawImageTransformed(singleBracketImage, af);
	g2.drawImageTransformed(doubleBracketImage, af);
}



void KeyboardViewport::resized()
{
    Viewport::resized();
	
	if (showStepButtons)
	{
		// resize the step sizes
		redrawButtons();

		stepRightLarge->setBounds(0, 0, buttonWidth, getHeight() / 2);
		stepRightSmall->setBounds(0, getHeight() / 2, buttonWidth, getHeight() / 2);
		stepLeftLarge->setBounds(getWidth() - buttonWidth, 0, buttonWidth, getHeight() / 2);
		stepLeftSmall->setBounds(getWidth() - buttonWidth, getHeight() / 2, buttonWidth, getHeight() / 2);
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
