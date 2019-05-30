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
#include "../../PluginState.h"
#include "../../Structures/Mode.h"
#include "PresetLibraryBox.h"

#include <iostream>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class KeyboardEditorBar  : public Component,
                           public ChangeBroadcaster,
                           public TextEditor::Listener,
                           public ComboBox::Listener,
                           public Button::Listener,
                           public Slider::Listener
{
public:
    //==============================================================================
    KeyboardEditorBar (SvkPluginState* pluginStateIn, ApplicationCommandManager* managerIn);
    ~KeyboardEditorBar();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    int getModePresetIndex(String anyNameIn);
    int getModeLibraryIndex();
    void setModeReadoutText(String steps);
	void setModeLibraryText(String presetName);
    void setOffsetReadout(int offIn);
	int getOffsetReadout();

	void createAndSendMode();

    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

	//void populate_preset_menu();

	void textEditorReturnKeyPressed(TextEditor& editor) override;

	void allowUserInput(bool isAllowed=true);

    bool isMapButtonOn();

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

	struct KeyboardMenu : public Component,
						public MenuBarModel
	{
		StringArray options;
		std::unique_ptr<MenuBarComponent> menuParent;
		ApplicationCommandManager* appCmdMgr;

		KeyboardMenu(ApplicationCommandManager* managerIn);
		~KeyboardMenu();

		void resized() override;

		StringArray getMenuBarNames() override;
		void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
		PopupMenu getMenuForIndex(int topLevelMenuIndex, const String &menuName) override;
	};

	std::unique_ptr<KeyboardMenu> pianoMenu;
	SvkPluginState* pluginState;
	ApplicationCommandManager* appCmdMgr;

	// Preset Menus

    OwnedArray<Mode>* presets;
    Array<Array<Mode*>>* presetsSorted;

    HashMap<String, int> menuToPresetIndex;
    std::unique_ptr<PopupMenu> menuSortByScale;
    std::unique_ptr<PopupMenu> menuSortByMode;
    std::unique_ptr<PopupMenu> menuSortByFamily;


    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextEditor> modeTextEditor;
    std::unique_ptr<TextButton> sendScaleBtn;
    std::unique_ptr<Slider> offsetSld;
    std::unique_ptr<Label> offsetLabel;
    std::unique_ptr<TextButton> mapButton;
    std::unique_ptr<PresetLibraryBox> presetLibraryBox;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardEditorBar)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

