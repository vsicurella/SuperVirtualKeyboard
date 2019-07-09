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

#include "Sketch.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Sketch::Sketch ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    component.reset (new Component());
    addAndMakeVisible (component.get());
    component->setName ("new component");

    component->setBounds (24, 80, 800, 128);

    comboBox.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (comboBox.get());
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (String());
    comboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox->addListener (this);

    comboBox->setBounds (640, 16, 150, 24);

    comboBox2.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (comboBox2.get());
    comboBox2->setEditableText (false);
    comboBox2->setJustificationType (Justification::centredLeft);
    comboBox2->setTextWhenNothingSelected (String());
    comboBox2->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox2->addListener (this);

    comboBox2->setBounds (640, 48, 150, 24);

    slider.reset (new Slider ("new slider"));
    addAndMakeVisible (slider.get());
    slider->setRange (0, 10, 1);
    slider->setSliderStyle (Slider::IncDecButtons);
    slider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider->addListener (this);

    slider->setBounds (556, 16, 71, 24);

    slider2.reset (new Slider ("new slider"));
    addAndMakeVisible (slider2.get());
    slider2->setRange (0, 10, 1);
    slider2->setSliderStyle (Slider::IncDecButtons);
    slider2->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider2->addListener (this);

    slider2->setBounds (556, 48, 71, 24);

    textEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (textEditor.get());
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setText (String());

    textEditor->setBounds (264, 16, 207, 24);

    textButton.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton.get());
    textButton->setButtonText (TRANS("OK"));
    textButton->addListener (this);

    textButton->setBounds (481, 16, 31, 24);

    textButton2.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton2.get());
    textButton2->setButtonText (TRANS("i"));
    textButton2->addListener (this);

    textButton2->setBounds (232, 16, 23, 24);

    slider3.reset (new Slider ("new slider"));
    addAndMakeVisible (slider3.get());
    slider3->setRange (0, 10, 1);
    slider3->setSliderStyle (Slider::IncDecButtons);
    slider3->setTextBoxStyle (Slider::TextBoxLeft, false, 70, 20);
    slider3->addListener (this);

    slider3->setBounds (108, 216, 87, 24);

    toggleButton.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton.get());
    toggleButton->setButtonText (String());
    toggleButton->addListener (this);

    toggleButton->setBounds (792, 16, 31, 24);

    toggleButton2.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton2.get());
    toggleButton2->setButtonText (String());
    toggleButton2->addListener (this);
    toggleButton2->setToggleState (true, dontSendNotification);

    toggleButton2->setBounds (792, 48, 31, 24);

    label4.reset (new Label ("new label",
                             TRANS("C4")));
    addAndMakeVisible (label4.get());
    label4->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4->setBounds (524, 16, 32, 24);

    label5.reset (new Label ("new label",
                             TRANS("F5")));
    addAndMakeVisible (label5.get());
    label5->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label5->setBounds (524, 48, 32, 24);

    comboBox3.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (comboBox3.get());
    comboBox3->setEditableText (false);
    comboBox3->setJustificationType (Justification::centredLeft);
    comboBox3->setTextWhenNothingSelected (String());
    comboBox3->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox3->addListener (this);

    comboBox3->setBounds (320, 48, 150, 24);

    slider4.reset (new Slider ("new slider"));
    addAndMakeVisible (slider4.get());
    slider4->setRange (0, 10, 1);
    slider4->setSliderStyle (Slider::IncDecButtons);
    slider4->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider4->addListener (this);

    slider4->setBounds (298, 216, 71, 24);

    label6.reset (new Label ("new label",
                             TRANS("MIDI Channel")));
    addAndMakeVisible (label6.get());
    label6->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label6->setBounds (203, 216, 96, 24);

    textButton3.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton3.get());
    textButton3->setButtonText (TRANS("#"));
    textButton3->addListener (this);

    textButton3->setBounds (384, 216, 24, 24);

    label7.reset (new Label ("new label",
                             TRANS("Period Shift")));
    addAndMakeVisible (label7.get());
    label7->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label7->setBounds (24, 216, 88, 24);

    textButton4.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton4.get());
    textButton4->setButtonText (TRANS("GO"));
    textButton4->addListener (this);

    textButton4->setBounds (480, 48, 32, 24);

    textButton5.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton5.get());
    textButton5->setButtonText (TRANS("Edit Colors"));
    textButton5->addListener (this);

    textButton5->setBounds (648, 216, 79, 24);

    comboBox4.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (comboBox4.get());
    comboBox4->setEditableText (false);
    comboBox4->setJustificationType (Justification::centredLeft);
    comboBox4->setTextWhenNothingSelected (String());
    comboBox4->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox4->addListener (this);

    comboBox4->setBounds (424, 216, 96, 24);

    textButton6.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton6.get());
    textButton6->setButtonText (TRANS("Save"));
    textButton6->addListener (this);

    textButton6->setBounds (24, 16, 39, 24);

    textButton7.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton7.get());
    textButton7->setButtonText (TRANS("Load"));
    textButton7->addListener (this);

    textButton7->setBounds (72, 16, 40, 24);

    textButton8.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton8.get());
    textButton8->setButtonText (TRANS("Export"));
    textButton8->addListener (this);

    textButton8->setBounds (120, 16, 88, 24);

    textButton9.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton9.get());
    textButton9->setButtonText (TRANS("Auto Map"));
    textButton9->addListener (this);

    textButton9->setBounds (24, 48, 88, 24);

    textButton10.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton10.get());
    textButton10->setButtonText (TRANS("Manual Map"));
    textButton10->addListener (this);

    textButton10->setBounds (120, 48, 88, 24);

    label8.reset (new Label ("new label",
                             TRANS("Mapping Style:")));
    addAndMakeVisible (label8.get());
    label8->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label8->setBounds (224, 48, 96, 24);

    comboBox5.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (comboBox5.get());
    comboBox5->setEditableText (false);
    comboBox5->setJustificationType (Justification::centredLeft);
    comboBox5->setTextWhenNothingSelected (String());
    comboBox5->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox5->addListener (this);

    comboBox5->setBounds (536, 216, 96, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Sketch::~Sketch()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    component = nullptr;
    comboBox = nullptr;
    comboBox2 = nullptr;
    slider = nullptr;
    slider2 = nullptr;
    textEditor = nullptr;
    textButton = nullptr;
    textButton2 = nullptr;
    slider3 = nullptr;
    toggleButton = nullptr;
    toggleButton2 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    comboBox3 = nullptr;
    slider4 = nullptr;
    label6 = nullptr;
    textButton3 = nullptr;
    label7 = nullptr;
    textButton4 = nullptr;
    textButton5 = nullptr;
    comboBox4 = nullptr;
    textButton6 = nullptr;
    textButton7 = nullptr;
    textButton8 = nullptr;
    textButton9 = nullptr;
    textButton10 = nullptr;
    label8 = nullptr;
    comboBox5 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Sketch::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Sketch::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void Sketch::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox.get())
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox]
    }
    else if (comboBoxThatHasChanged == comboBox2.get())
    {
        //[UserComboBoxCode_comboBox2] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox2]
    }
    else if (comboBoxThatHasChanged == comboBox3.get())
    {
        //[UserComboBoxCode_comboBox3] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox3]
    }
    else if (comboBoxThatHasChanged == comboBox4.get())
    {
        //[UserComboBoxCode_comboBox4] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox4]
    }
    else if (comboBoxThatHasChanged == comboBox5.get())
    {
        //[UserComboBoxCode_comboBox5] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox5]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void Sketch::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider.get())
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        //[/UserSliderCode_slider]
    }
    else if (sliderThatWasMoved == slider2.get())
    {
        //[UserSliderCode_slider2] -- add your slider handling code here..
        //[/UserSliderCode_slider2]
    }
    else if (sliderThatWasMoved == slider3.get())
    {
        //[UserSliderCode_slider3] -- add your slider handling code here..
        //[/UserSliderCode_slider3]
    }
    else if (sliderThatWasMoved == slider4.get())
    {
        //[UserSliderCode_slider4] -- add your slider handling code here..
        //[/UserSliderCode_slider4]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void Sketch::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton.get())
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == textButton2.get())
    {
        //[UserButtonCode_textButton2] -- add your button handler code here..
        //[/UserButtonCode_textButton2]
    }
    else if (buttonThatWasClicked == toggleButton.get())
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == toggleButton2.get())
    {
        //[UserButtonCode_toggleButton2] -- add your button handler code here..
        //[/UserButtonCode_toggleButton2]
    }
    else if (buttonThatWasClicked == textButton3.get())
    {
        //[UserButtonCode_textButton3] -- add your button handler code here..
        //[/UserButtonCode_textButton3]
    }
    else if (buttonThatWasClicked == textButton4.get())
    {
        //[UserButtonCode_textButton4] -- add your button handler code here..
        //[/UserButtonCode_textButton4]
    }
    else if (buttonThatWasClicked == textButton5.get())
    {
        //[UserButtonCode_textButton5] -- add your button handler code here..
        //[/UserButtonCode_textButton5]
    }
    else if (buttonThatWasClicked == textButton6.get())
    {
        //[UserButtonCode_textButton6] -- add your button handler code here..
        //[/UserButtonCode_textButton6]
    }
    else if (buttonThatWasClicked == textButton7.get())
    {
        //[UserButtonCode_textButton7] -- add your button handler code here..
        //[/UserButtonCode_textButton7]
    }
    else if (buttonThatWasClicked == textButton8.get())
    {
        //[UserButtonCode_textButton8] -- add your button handler code here..
        //[/UserButtonCode_textButton8]
    }
    else if (buttonThatWasClicked == textButton9.get())
    {
        //[UserButtonCode_textButton9] -- add your button handler code here..
        //[/UserButtonCode_textButton9]
    }
    else if (buttonThatWasClicked == textButton10.get())
    {
        //[UserButtonCode_textButton10] -- add your button handler code here..
        //[/UserButtonCode_textButton10]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Sketch" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="new component" id="5bb066a5f0df6b3a" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="24 80 800 128" class="Component"
                    params=""/>
  <COMBOBOX name="new combo box" id="197cbd0054b3ea6d" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="640 16 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="e43718c6ce3f175" memberName="comboBox2"
            virtualName="" explicitFocusOrder="0" pos="640 48 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="e732568ad188d067" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="556 16 71 24" min="0.0" max="10.0"
          int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="ae29d1ebf57b7bd7" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="556 48 71 24" min="0.0"
          max="10.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="39f9f4bff4e94802" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="264 16 207 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="bbb112b96c51ecf7" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="481 16 31 24" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c6025abb0c68f4" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="232 16 23 24" buttonText="i"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="89742a2afd3a3325" memberName="slider3"
          virtualName="" explicitFocusOrder="0" pos="108 216 87 24" min="0.0"
          max="10.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="70" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="new toggle button" id="bb818484022dae8c" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="792 16 31 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="a8183216fad07ec6" memberName="toggleButton2"
                virtualName="" explicitFocusOrder="0" pos="792 48 31 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <LABEL name="new label" id="2df91047cc98faf4" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="524 16 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="C4" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="c21534178bfcff74" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="524 48 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="F5" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="217ff7f7b6934ebe" memberName="comboBox3"
            virtualName="" explicitFocusOrder="0" pos="320 48 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="4f2d4d8b9051247f" memberName="slider4"
          virtualName="" explicitFocusOrder="0" pos="298 216 71 24" min="0.0"
          max="10.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="d2587185be475d9c" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="203 216 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI Channel" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="405a79d645f5f8ac" memberName="textButton3"
              virtualName="" explicitFocusOrder="0" pos="384 216 24 24" buttonText="#"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="140764aae2291735" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="24 216 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Period Shift" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="22ef34e38fed1212" memberName="textButton4"
              virtualName="" explicitFocusOrder="0" pos="480 48 32 24" buttonText="GO"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="bc86137b1b6a5176" memberName="textButton5"
              virtualName="" explicitFocusOrder="0" pos="648 216 79 24" buttonText="Edit Colors"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="new combo box" id="292b32e0c6cd0b80" memberName="comboBox4"
            virtualName="" explicitFocusOrder="0" pos="424 216 96 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="new button" id="b3d301956f9f6d50" memberName="textButton6"
              virtualName="" explicitFocusOrder="0" pos="24 16 39 24" buttonText="Save"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="7de794bf7f813f4c" memberName="textButton7"
              virtualName="" explicitFocusOrder="0" pos="72 16 40 24" buttonText="Load"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="7db05d91d674ecfe" memberName="textButton8"
              virtualName="" explicitFocusOrder="0" pos="120 16 88 24" buttonText="Export"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="d0adb9765a43b7e2" memberName="textButton9"
              virtualName="" explicitFocusOrder="0" pos="24 48 88 24" buttonText="Auto Map"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6ead157214bd613f" memberName="textButton10"
              virtualName="" explicitFocusOrder="0" pos="120 48 88 24" buttonText="Manual Map"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="27d88f1ce2d645c1" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="224 48 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mapping Style:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="ddae7c4ef2fca8a5" memberName="comboBox5"
            virtualName="" explicitFocusOrder="0" pos="536 216 96 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

