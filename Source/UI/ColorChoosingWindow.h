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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Structures/Mode.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ColorChooserWindow  : public Component
{
public:
    //==============================================================================
    ColorChooserWindow (Mode* modeIn);
    ~ColorChooserWindow();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void setCurrentMode(Mode* modeIn);

	void closeButtonPressed();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	Mode* mode;

	std::unique_ptr<ColourSelector> colorSelector;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ColourSelector> component;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColorChooserWindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

