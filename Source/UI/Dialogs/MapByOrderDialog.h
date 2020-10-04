/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../../JuceLibraryCode/JuceHeader.h"

#include "../../CommonFunctions.h"
#include "../../Structures/ModeMapper.h"
#include "../Components/ReferencedComboBox.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MapByOrderDialog  : public Component,
                          public ComboBox::Listener,
                          public Slider::Listener
{
public:
    //==============================================================================
    MapByOrderDialog (ModeMapper* modeMapperIn, Mode* mode1In, Mode* mode2In);
    ~MapByOrderDialog();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    ModeMapper* modeMapper;
    Mode* mode1;
    Mode* mode2;

    int boxW = 10;
    int boxH = 8;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ComboBox> mode1OrderBox;
    std::unique_ptr<ComboBox> mode2OrderBox;
    std::unique_ptr<Label> orderSelectionLbl;
    std::unique_ptr<Slider> orderOffsetSld1;
    std::unique_ptr<Slider> orderOffsetSld2;
    std::unique_ptr<Label> label;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MapByOrderDialog)
};

//[EndFile] You can add extra defines here...

//[/EndFile]

