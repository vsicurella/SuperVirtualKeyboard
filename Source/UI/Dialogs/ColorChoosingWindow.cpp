/*
  ==============================================================================

	by Vincenzo Sicurella, 04-26-2019

  ==============================================================================
*/


#include "ColorChoosingWindow.h"


//==============================================================================
ColorChooserWindow::ColorChooserWindow (String windowName, Colour backgroundColor, int windowFlags)
    : DocumentWindow(windowName, backgroundColor, windowFlags)
{
    setAlwaysOnTop(true);
    setOpaque(true);

    setSize (450, 450);
}

ColorChooserWindow::~ColorChooserWindow()
{

}

//==============================================================================
void ColorChooserWindow::paint (Graphics& g)
{

    g.fillAll (Colour (0xff323e44));

}

void ColorChooserWindow::resized()
{
    DocumentWindow::resized();
}

void ColorChooserWindow::changeListenerCallback(ChangeBroadcaster* source)
{
	sendChangeMessage();
}

void ColorChooserWindow::closeButtonPressed()
{
	sendChangeMessage();
}

Colour ColorChooserWindow::getColorSelected()
{
	return colorChooser->getCurrentColour();
}
