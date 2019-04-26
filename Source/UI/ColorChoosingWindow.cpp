/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "ColorChoosingWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ColorChooserWindow::ColorChooserWindow (Mode* modeIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    component.reset (new ColourSelector());
    addAndMakeVisible (component.get());
    component->setName ("new component");


    //[UserPreSize]
	colorSelector.reset(new ColourSelector());
	colorSelector->setName("Color Selector");
	colorSelector->setSize(300, 300);

	setOpaque(true);
    //[/UserPreSize]

    setSize (450, 450);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ColorChooserWindow::~ColorChooserWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    component = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ColorChooserWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ColorChooserWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    component->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..
	colorSelector->setBoundsRelative(0, 0.2, 0.8, 0.8);
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ColorChooserWindow::setCurrentMode(Mode* modeIn)
{
	mode = modeIn;
}

void ColorChooserWindow::closeButtonPressed()
{
	setVisible(false);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ColorChooserWindow" componentName=""
                 parentClasses="public Component" constructorParams="Mode* modeIn"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="450" initialHeight="450">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="new component" id="d9ba9435617a7d65" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="0 0 100% 100%" class="ColourSelector"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

