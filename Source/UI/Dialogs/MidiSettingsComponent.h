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
#include "../../../JuceLibraryCode/JuceHeader.h"

#include "../../CommonFunctions.h"
#include "../../PluginState.h"
#include "../../Structures/ModeMapper.h"
#include "../Components/MidiRemapTableModel.h"
#include "../Components/ReferencedComboBox.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiSettingsComponent  : public Component,
                               public Button::Listener,
                               public Slider::Listener,
                               public ComboBox::Listener
{
public:
    //==============================================================================
    MidiSettingsComponent (SvkPluginState* pluginStateIn);
    ~MidiSettingsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setMode1SelectedId(int selectedIdIn);
	void setMode2SelectedId(int selectedIdIn);
    void setMode1RootNote(int rootNoteIn);
    void setMode2RootNote(int rootNoteIn);

    void refreshTables();
    void updateModeMapper();

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	/*
    std::unique_ptr<MidiRemapTableModel> remapTableModel;
	std::unique_ptr<TableListBox> remapTable;
	std::unique_ptr<ModeMapper> modeMapper;
     */

    std::unique_ptr<ModeMapper> modeMapper;

	std::unique_ptr<Mode> modeSelected1;
	std::unique_ptr<Mode> modeSelected2;

	int boxW = 10;
	int boxH = 8;

	SvkPluginState* pluginState;
	MidiFilter* midiInputFilter;
	MidiFilter* midiOutputFilter;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> modeFromLbl;
    std::unique_ptr<Label> modeToLbl;
    std::unique_ptr<Label> rootNoteLabel;
    std::unique_ptr<ReferencedComboBox> presetBox1;
    std::unique_ptr<ReferencedComboBox> presetBox2;
    std::unique_ptr<TextButton> mapModesBtn;
    std::unique_ptr<ToggleButton> mapFullBtn;
    std::unique_ptr<ToggleButton> mapModeToScaleBtn;
    std::unique_ptr<ToggleButton> mapModeOrdersBtn;
    std::unique_ptr<Slider> rootFromSld;
    std::unique_ptr<Slider> rootToSld;
    std::unique_ptr<Label> rootNoteFromLbl;
    std::unique_ptr<Label> rootNoteToLbl;
    std::unique_ptr<Label> ModeMapTypeLbl;
    std::unique_ptr<ComboBox> mode1OrderBox;
    std::unique_ptr<ComboBox> mode2OrderBox;
    std::unique_ptr<Label> orderSelectionLbl;
    std::unique_ptr<Slider> orderOffsetSld1;
    std::unique_ptr<Slider> orderOffsetSld2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiSettingsComponent)
};

//[EndFile] You can add extra defines here...

//[/EndFile]

