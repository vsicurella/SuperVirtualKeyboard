/*
  ==============================================================================

  Created with Projucer version: 5.4.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"

#include "../../CommonFunctions.h"
#include "../../Structures/ModeMapper.h"
#include "../Components/ReferencedComboBox.h"


//==============================================================================

class MapByOrderDialog  : public Component,
                          public ComboBox::Listener,
                          public Slider::Listener
{
public:
    //==============================================================================
    MapByOrderDialog (ModeMapper* modeMapperIn, Mode* mode1In, Mode* mode2In);
    ~MapByOrderDialog();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

private:

    ModeMapper* modeMapper;
    Mode* mode1;
    Mode* mode2;

    int boxW = 10;
    int boxH = 8;

    //==============================================================================
    std::unique_ptr<ComboBox> mode1OrderBox;
    std::unique_ptr<ComboBox> mode2OrderBox;
    std::unique_ptr<Label> orderSelectionLbl;
    std::unique_ptr<Slider> orderOffsetSld1;
    std::unique_ptr<Slider> orderOffsetSld2;
    std::unique_ptr<Label> offsetLabel;
    std::unique_ptr<Label> inputLabel;
    std::unique_ptr<Label> outputLabel;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MapByOrderDialog)
};


