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
#include "../PluginState.h"
#include "../Structures/Mode.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class KeyboardEditorBar  : public Component,
                           private TextEditor::Listener,
                           public Button::Listener,
                           public ComboBox::Listener,
                           public Slider::Listener
{
public:
    //==============================================================================
    KeyboardEditorBar (SuperVirtualKeyboardPluginState* pluginStateIn, ApplicationCommandManager* managerIn);
    ~KeyboardEditorBar();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    int get_mode_preset_index(String anyNameIn);
    int get_mode_library_index();
    void set_mode_readout_text(String steps);
	void set_mode_library_text(String presetName);

	void createAndSendMode();

	void populate_preset_menu();

	void textEditorReturnKeyPressed(TextEditor& editor) override;

	void allowUserInput(bool isAllowed=true);

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
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
	SuperVirtualKeyboardPluginState* pluginState;
	ApplicationCommandManager* appCmdMgr;

	// Preset Menus

    OwnedArray<Mode>* presets;
    Array<Array<Mode*>>* presetsSorted;

    HashMap<String, int> menuToPresetIndex;
    std::unique_ptr<PopupMenu> menuSortByScale;
    std::unique_ptr<PopupMenu> menuSortByMode;
    std::unique_ptr<PopupMenu> menuSortByFamily;

	// Key Locks

	bool enterDown = false;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextEditor> modeTextEditor;
    std::unique_ptr<TextButton> sendScaleBtn;
    std::unique_ptr<ComboBox> modeLibraryBox;
    std::unique_ptr<Slider> offsetSld;
    std::unique_ptr<Label> offsetLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardEditorBar)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

