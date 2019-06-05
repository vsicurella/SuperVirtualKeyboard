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
                               public ComboBox::Listener,
                               public Slider::Listener,
                               public Button::Listener
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

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	std::unique_ptr<MidiRemapTableModel> remapTableModel;
	std::unique_ptr<TableListBox> remapTable;
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
    std::unique_ptr<ComboBox> midiInputBox;
    std::unique_ptr<Label> midiInputLabel;
    std::unique_ptr<Slider> noteRangeSld;
    std::unique_ptr<Label> noteRangeLabel;
    std::unique_ptr<ToggleButton> manualMapToggle;
    std::unique_ptr<ToggleButton> modeMapToggle;
    std::unique_ptr<Label> modeBoxOriginal;
    std::unique_ptr<Label> modeBoxRemap;
    std::unique_ptr<Slider> modeOriginalRootSld;
    std::unique_ptr<Slider> modeRemapRootSld;
    std::unique_ptr<Label> rootNoteLabel;
    std::unique_ptr<ReferencedComboBox> presetBox1;
    std::unique_ptr<ReferencedComboBox> presetBox2;
    std::unique_ptr<TextButton> mapModesBtn;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiSettingsComponent)
};

//[EndFile] You can add extra defines here...

//[/EndFile]

