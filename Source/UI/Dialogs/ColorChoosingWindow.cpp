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

	colorChooser.reset(new ColourSelector());
    colorChooser->setName("Color Chooser");
    colorChooser->setBounds(0, 0, 450, 450);
    //setContentOwned(colorChooser.get(), true);

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
	colorChooser->setBounds(0, getHeight() - proportionOfHeight(0.9493f), proportionOfWidth(1.0000f), proportionOfHeight(0.9493f));
}

void ColorChooserWindow::closeButtonPressed()
{
	sendChangeMessage();
	setVisible(false);
}

Colour ColorChooserWindow::getColorSelected()
{
	return colorChooser->getCurrentColour();
}
