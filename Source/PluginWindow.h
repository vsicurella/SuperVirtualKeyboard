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
#include "../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Sketch  : public Component,
                public ComboBox::Listener,
                public Slider::Listener,
                public Button::Listener
{
public:
    //==============================================================================
    Sketch ();
    ~Sketch();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Component> component;
    std::unique_ptr<ComboBox> comboBox;
    std::unique_ptr<ComboBox> comboBox2;
    std::unique_ptr<Slider> slider;
    std::unique_ptr<Slider> slider2;
    std::unique_ptr<TextEditor> textEditor;
    std::unique_ptr<TextButton> textButton;
    std::unique_ptr<TextButton> textButton2;
    std::unique_ptr<Slider> slider3;
    std::unique_ptr<ToggleButton> toggleButton;
    std::unique_ptr<ToggleButton> toggleButton2;
    std::unique_ptr<Label> label4;
    std::unique_ptr<Label> label5;
    std::unique_ptr<ComboBox> comboBox3;
    std::unique_ptr<Slider> slider4;
    std::unique_ptr<Label> label6;
    std::unique_ptr<TextButton> textButton3;
    std::unique_ptr<Label> label7;
    std::unique_ptr<TextButton> textButton4;
    std::unique_ptr<TextButton> textButton5;
    std::unique_ptr<ComboBox> comboBox4;
    std::unique_ptr<TextButton> textButton6;
    std::unique_ptr<TextButton> textButton7;
    std::unique_ptr<TextButton> textButton8;
    std::unique_ptr<TextButton> textButton9;
    std::unique_ptr<TextButton> textButton10;
    std::unique_ptr<Label> label8;
    std::unique_ptr<ComboBox> comboBox5;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sketch)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

