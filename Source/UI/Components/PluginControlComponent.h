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

#include "../../PluginIDs.h"
#include "../../PluginState.h"
#include "ReferencedComboBox.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginControlComponent  : public Component,
                                public TextEditor::Listener,
                                public ComboBox::Listener,
                                public Slider::Listener,
                                public Button::Listener
{
public:
    //==============================================================================
    PluginControlComponent (SvkPluginState* pluginStateIn);
    ~PluginControlComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	String getScaleEntryText();
	void setScaleEntryText(String textIn, NotificationType notify = NotificationType::dontSendNotification);

	TextEditor* getScaleTextEditor();

	Viewport* getViewport();

	ReferencedComboBox* getMode1Box();
	ReferencedComboBox* getMode2Box();
	void updateModeBoxMenus();

	TextButton* getModeInfoButton();

	int getMode1BoxSelection();
	int getMode2BoxSelection();

	void setMode1BoxText(String textIn, NotificationType notify = NotificationType::dontSendNotification);
	void setMode2BoxText(String textIn, NotificationType notify = NotificationType::dontSendNotification);

	void setMode1BoxId(int idIn, NotificationType notify=NotificationType::dontSendNotification);
	void setMode2BoxId(int idIn, NotificationType notify = NotificationType::dontSendNotification);

	void setMode1View(bool isViewed, NotificationType notify = NotificationType::dontSendNotification);
	bool getMode1View();

	void setMode2View(bool isViewed, NotificationType notify = NotificationType::dontSendNotification);
	bool getMode2View();

	int getModeViewed();

	bool getAutoMapState();
	void setAutoMapState(bool isAutoMapping, NotificationType notify = NotificationType::dontSendNotification);

	int getMappingStyle();
	void setMappingStyleId(int idIn, NotificationType notify = NotificationType::dontSendNotification);

	int getMode1Root();
	void setMode1Root(int rootIn, NotificationType notify = NotificationType::dontSendNotification);

	int getMode2Root();
	void setMode2Root(int rootIn, NotificationType notify = NotificationType::dontSendNotification);

	int getPeriodShift();
	void setPeriodShift(int periodsIn, NotificationType notify = NotificationType::dontSendNotification);

	int getMidiChannel();
	void setMidiChannel(int channelIn, NotificationType notify = NotificationType::dontSendNotification);

	bool getNoteNumsView();
	void setNoteNumsView(bool isViewed, NotificationType notify = NotificationType::dontSendNotification);

	int getKeyStyle();
	void setKeyStyleId(int idIn, NotificationType notify = NotificationType::dontSendNotification);

	int getHighlightStyle();
	void setHighlightStyleId(int idIn, NotificationType notify = NotificationType::dontSendNotification);

	void textEditorTextChanged(TextEditor& textEditor) override;
	void textEditorEscapeKeyPressed(TextEditor& textEditor) override;
	void textEditorReturnKeyPressed(TextEditor& textEditor) override;
	void textEditorFocusLost(TextEditor& textEditor) override;

	void mouseDown(const MouseEvent& e) override;

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	SvkPluginState * pluginState;
	ApplicationCommandManager* appCmdMgr;
    
    std::unique_ptr<PopupMenu> saveMenu;
    std::unique_ptr<PopupMenu> loadMenu;
    std::unique_ptr<PopupMenu> exportMenu;
    
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ReferencedComboBox> mode1Box;
    std::unique_ptr<ReferencedComboBox> mode2Box;
    std::unique_ptr<Slider> mode1RootSld;
    std::unique_ptr<Slider> mode2RootSld;
    std::unique_ptr<TextEditor> scaleTextBox;
    std::unique_ptr<TextButton> scaleEntryBtn;
    std::unique_ptr<TextButton> modeInfoButton;
    std::unique_ptr<Slider> periodShiftSld;
    std::unique_ptr<ToggleButton> mode1ViewBtn;
    std::unique_ptr<ToggleButton> mode2ViewBtn;
    std::unique_ptr<Label> mode1RootLbl;
    std::unique_ptr<Label> mode2RootLbl;
    std::unique_ptr<ComboBox> mapStyleBox;
    std::unique_ptr<Slider> midiChannelSld;
    std::unique_ptr<Label> midiChannelLbl;
    std::unique_ptr<TextButton> noteNumsBtn;
    std::unique_ptr<Label> periodShiftLbl;
    std::unique_ptr<TextButton> mapButton;
    std::unique_ptr<TextButton> editColorsBtn;
    std::unique_ptr<ComboBox> keyStyleBox;
    std::unique_ptr<TextButton> saveBtn;
    std::unique_ptr<TextButton> loadBtn;
    std::unique_ptr<TextButton> exportBtn;
    std::unique_ptr<TextButton> autoMapBtn;
    std::unique_ptr<TextButton> manualMapBtn;
    std::unique_ptr<Label> mapStyleLbl;
    std::unique_ptr<ComboBox> highlightStyleBox;
    std::unique_ptr<Viewport> keyboardViewport;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginControlComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

