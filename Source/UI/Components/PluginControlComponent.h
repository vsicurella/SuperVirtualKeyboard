/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//#include "../SvkUiPanel.h"
#include "../../PresetManager.h"
#include "../VectorResources.h"
#include "ReferencedComboBox.h"
#include "VirtualKeyboard/KeyboardComponent.h"
#include "VirtualKeyboard/KeyboardViewport.h"

typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class PluginControlComponent  : public Component,
                                public TextEditor::Listener,
                                public ComboBox::Listener,
                                public Slider::Listener,
                                public Button::Listener
{
public:
    //==============================================================================
    PluginControlComponent (AudioProcessorValueTreeState& processorTreeIn, ApplicationCommandManager* appCmdMgrIn, SvkPresetManager* presetManagerIn);
    ~PluginControlComponent();

	//==============================================================================

	void paint(Graphics& g) override;
	void resized() override;
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	void sliderValueChanged(Slider* sliderThatWasMoved) override;
	void buttonClicked(Button* buttonThatWasClicked) override;

	void textEditorTextChanged(TextEditor& textEditor) override;
	void textEditorEscapeKeyPressed(TextEditor& textEditor) override;
	void textEditorReturnKeyPressed(TextEditor& textEditor) override;
	void textEditorFocusLost(TextEditor& textEditor) override;

	void mouseDown(const MouseEvent& e) override;

    //==============================================================================

    void connectToProcessor();

	String getScaleEntryText();
	void setScaleEntryText(String textIn, NotificationType notify = NotificationType::dontSendNotification);

	VirtualKeyboard::Keyboard* getKeyboard();

	TextEditor* getScaleTextEditor();
	Viewport* getViewport();
	ComboBox* getMappingStyleBox();

	ReferencedComboBox* getMode1Box();
	ReferencedComboBox* getMode2Box();
	void updateModeBoxMenus();

	ImageButton* getSettingsButton();
	TextButton* getModeInfoButton();

	/*
		Sets controls to preset settings
	*/
	void loadPresetNode(ValueTree presetNodeIn);

	/*
		Updates UI to the new mode
	*/
	void onModeViewedChange(Mode* modeViewed);

	/*
		Sets the X position of the viewport
	*/
	void setViewPosition(int xIn);

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

	int getModeSelectorViewed();

	int getMappingMode();
	void setMappingMode(int mappingModeId, NotificationType notify = NotificationType::dontSendNotification);

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

private:
    AudioProcessorValueTreeState& processorTree;
	ApplicationCommandManager* appCmdMgr;
    SvkPresetManager* presetManager;

	ValueTree presetNode;

    std::unique_ptr<Image> saveIcon;
    std::unique_ptr<Image> openIcon;
    std::unique_ptr<Image> settingsIcon;

    std::unique_ptr<PopupMenu> saveMenu;
    std::unique_ptr<PopupMenu> loadMenu;
    std::unique_ptr<PopupMenu> exportMenu;

    OwnedArray<ButtonAttachment> buttonAttachments;
    OwnedArray<ComboBoxAttachment> comboBoxAttachments;
    OwnedArray<SliderAttachment> sliderAttachments;

    TextFilterIntOrSpace txtFilter;

    bool inMappingMode = false;

	int barHeight = 24;
	int gap = 8;

    //==============================================================================
    std::unique_ptr<TextEditor> scaleTextBox;
    std::unique_ptr<ReferencedComboBox> mode1Box;
    std::unique_ptr<ReferencedComboBox> mode2Box;
    std::unique_ptr<Slider> mode1RootSld;
    std::unique_ptr<Slider> mode2RootSld;
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
    std::unique_ptr<Label> periodShiftLbl;
    std::unique_ptr<Label> mapStyleLbl;
    std::unique_ptr<TextButton> mapOrderEditBtn;
    std::unique_ptr<ComboBox> mapModeBox;
    std::unique_ptr<TextButton> mapApplyBtn;
	std::unique_ptr<VirtualKeyboard::Keyboard> keyboard;
    std::unique_ptr<Viewport> keyboardViewport;
    std::unique_ptr<ImageButton> saveButton;
    std::unique_ptr<ImageButton> openButton;
    std::unique_ptr<ImageButton> settingsButton;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginControlComponent)
};
