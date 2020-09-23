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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PluginControlComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PluginControlComponent::PluginControlComponent (AudioProcessorValueTreeState& processorTreeIn, ApplicationCommandManager* appCmdMgrIn, SvkPresetManager* presetManagerIn)
    : processorTree(processorTreeIn), appCmdMgr(appCmdMgrIn), presetManager(presetManagerIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    scaleTextBox.reset (new TextEditor ("Scale Text Box"));
    addAndMakeVisible (scaleTextBox.get());
    scaleTextBox->setMultiLine (false);
    scaleTextBox->setReturnKeyStartsNewLine (false);
    scaleTextBox->setReadOnly (false);
    scaleTextBox->setScrollbarsShown (true);
    scaleTextBox->setCaretVisible (true);
    scaleTextBox->setPopupMenuEnabled (true);
    scaleTextBox->setText (String());

    mode1Box.reset (new ReferencedComboBox ("Mode1 Box"));
    addAndMakeVisible (mode1Box.get());
    mode1Box->setEditableText (false);
    mode1Box->setJustificationType (Justification::centredLeft);
    mode1Box->setTextWhenNothingSelected (String());
    mode1Box->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode1Box->addListener (this);

    mode2Box.reset (new ReferencedComboBox ("Mode2 Box"));
    addAndMakeVisible (mode2Box.get());
    mode2Box->setEditableText (false);
    mode2Box->setJustificationType (Justification::centredLeft);
    mode2Box->setTextWhenNothingSelected (String());
    mode2Box->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode2Box->addListener (this);

    mode1RootSld.reset (new Slider ("Mode1 Root Slider"));
    addAndMakeVisible (mode1RootSld.get());
    mode1RootSld->setRange (0, 127, 1);
    mode1RootSld->setSliderStyle (Slider::IncDecButtons);
    mode1RootSld->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    mode1RootSld->addListener (this);

    mode2RootSld.reset (new Slider ("mode2RootSld"));
    addAndMakeVisible (mode2RootSld.get());
    mode2RootSld->setRange (0, 127, 1);
    mode2RootSld->setSliderStyle (Slider::IncDecButtons);
    mode2RootSld->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    mode2RootSld->addListener (this);

    scaleEntryBtn.reset (new TextButton ("Scale Entry Button"));
    addAndMakeVisible (scaleEntryBtn.get());
    scaleEntryBtn->setButtonText (TRANS("OK"));
    scaleEntryBtn->addListener (this);

    modeInfoButton.reset (new TextButton ("Mode Info Button"));
    addAndMakeVisible (modeInfoButton.get());
    modeInfoButton->setButtonText (TRANS("i"));
    modeInfoButton->addListener (this);

    periodShiftSld.reset (new Slider ("Period Shift Slider"));
    addAndMakeVisible (periodShiftSld.get());
    periodShiftSld->setRange (-10, 10, 1);
    periodShiftSld->setSliderStyle (Slider::IncDecButtons);
    periodShiftSld->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    periodShiftSld->addListener (this);

    mode1ViewBtn.reset (new ToggleButton ("Mode1 View Button"));
    addAndMakeVisible (mode1ViewBtn.get());
    mode1ViewBtn->setButtonText (String());
    mode1ViewBtn->setRadioGroupId (10);
    mode1ViewBtn->addListener (this);
    mode1ViewBtn->setToggleState (true, dontSendNotification);

    mode2ViewBtn.reset (new ToggleButton ("Mode2 View Button"));
    addAndMakeVisible (mode2ViewBtn.get());
    mode2ViewBtn->setButtonText (String());
    mode2ViewBtn->setRadioGroupId (10);
    mode2ViewBtn->addListener (this);

    mode1RootLbl.reset (new Label ("Mode1 Root Label",
                                   TRANS("C4")));
    addAndMakeVisible (mode1RootLbl.get());
    mode1RootLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    mode1RootLbl->setJustificationType (Justification::centredLeft);
    mode1RootLbl->setEditable (false, false, false);
    mode1RootLbl->setColour (TextEditor::textColourId, Colours::black);
    mode1RootLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mode2RootLbl.reset (new Label ("Mode 2 Root Label",
                                   TRANS("F5")));
    addAndMakeVisible (mode2RootLbl.get());
    mode2RootLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    mode2RootLbl->setJustificationType (Justification::centredLeft);
    mode2RootLbl->setEditable (false, false, false);
    mode2RootLbl->setColour (TextEditor::textColourId, Colours::black);
    mode2RootLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mapStyleBox.reset (new ComboBox ("Mapping Style Box"));
    addAndMakeVisible (mapStyleBox.get());
    mapStyleBox->setEditableText (false);
    mapStyleBox->setJustificationType (Justification::centredLeft);
    mapStyleBox->setTextWhenNothingSelected (TRANS("Mode To Mode"));
    mapStyleBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mapStyleBox->addItem (TRANS("Mode To Mode"), 1);
    mapStyleBox->addItem (TRANS("Scale To Mode"), 2);
    mapStyleBox->addItem (TRANS("By Orders"), 3);
    mapStyleBox->addListener (this);

    midiChannelSld.reset (new Slider ("Midi Channel Slider"));
    addAndMakeVisible (midiChannelSld.get());
    midiChannelSld->setRange (1, 16, 1);
    midiChannelSld->setSliderStyle (Slider::IncDecButtons);
    midiChannelSld->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    midiChannelSld->addListener (this);

    midiChannelLbl.reset (new Label ("Midi Channel Label",
                                     TRANS("MIDI Channel:")));
    addAndMakeVisible (midiChannelLbl.get());
    midiChannelLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    midiChannelLbl->setJustificationType (Justification::centredLeft);
    midiChannelLbl->setEditable (false, false, false);
    midiChannelLbl->setColour (TextEditor::textColourId, Colours::black);
    midiChannelLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    periodShiftLbl.reset (new Label ("Period Shift Label",
                                     TRANS("Period Shift:")));
    addAndMakeVisible (periodShiftLbl.get());
    periodShiftLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    periodShiftLbl->setJustificationType (Justification::centredLeft);
    periodShiftLbl->setEditable (false, false, false);
    periodShiftLbl->setColour (TextEditor::textColourId, Colours::black);
    periodShiftLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mapStyleLbl.reset (new Label ("Mapping Style Label",
                                  TRANS("Mapping Style:")));
    addAndMakeVisible (mapStyleLbl.get());
    mapStyleLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    mapStyleLbl->setJustificationType (Justification::centredLeft);
    mapStyleLbl->setEditable (false, false, false);
    mapStyleLbl->setColour (TextEditor::textColourId, Colours::black);
    mapStyleLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mapOrderEditBtn.reset (new TextButton ("Map Order Edit Button"));
    addAndMakeVisible (mapOrderEditBtn.get());
    mapOrderEditBtn->setButtonText (TRANS("Edit Mapping"));
    mapOrderEditBtn->addListener (this);

    mapModeBox.reset (new ComboBox ("Mapping Node Box"));
    addAndMakeVisible (mapModeBox.get());
    mapModeBox->setEditableText (false);
    mapModeBox->setJustificationType (Justification::centredLeft);
    mapModeBox->setTextWhenNothingSelected (TRANS("Mapping Off"));
    mapModeBox->setTextWhenNoChoicesAvailable (TRANS("Mapping Off"));
    mapModeBox->addItem (TRANS("Mapping Off"), 1);
    mapModeBox->addItem (TRANS("Auto Map"), 2);
    mapModeBox->addItem (TRANS("Manual Map"), 3);
    mapModeBox->addListener (this);

    mapApplyBtn.reset (new TextButton ("new button"));
    addAndMakeVisible (mapApplyBtn.get());
    mapApplyBtn->setButtonText (TRANS("Apply"));
    mapApplyBtn->addListener (this);

	keyboard.reset(new VirtualKeyboard::Keyboard());
    keyboardViewport.reset (new Viewport("Keyboard Viewport"));
    addAndMakeVisible (keyboardViewport.get());
    keyboardViewport->setName ("Keyboard Viewport");
	keyboardViewport->setViewedComponent(keyboard.get(), false);

    saveButton.reset (new ImageButton ("Save Button"));
    addAndMakeVisible (saveButton.get());
    saveButton->setButtonText (TRANS("Save"));
    saveButton->addListener (this);
    saveButton->setImages (false, true, true,
                           Image(), 1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000));
	saveButton->setSize(28, 28);

    openButton.reset (new ImageButton ("Open Button"));
    addAndMakeVisible (openButton.get());
    openButton->setButtonText (TRANS("Open"));
    openButton->addListener (this);
    openButton->setImages (false, true, true,
                           Image(), 1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000));
	openButton->setSize(28, 28);

    settingsButton.reset (new ImageButton ("Settings Button"));
    addAndMakeVisible (settingsButton.get());
    settingsButton->setButtonText (TRANS("Settings"));
    settingsButton->addListener (this);
    settingsButton->setImages (false, true, true,
                               Image(), 1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000));
	settingsButton->setSize(28, 28);

    saveIcon.reset(new Image(Image::PixelFormat::RGB, saveButton->getWidth(), saveButton->getHeight(), true));
    openIcon.reset(new Image(Image::PixelFormat::RGB, openButton->getWidth(), openButton->getHeight(), true));

    saveButton->setImages(true, true, true, *saveIcon.get(), 0.0f, Colour(), *saveIcon.get(), 0.0f, Colours::white.withAlpha(0.25f), *saveIcon.get(), 0.0f, Colours::white.withAlpha(0.5f));
    openButton->setImages(true, true, true, *openIcon.get(), 0.0f, Colour(), *openIcon.get(), 0.0f, Colours::white.withAlpha(0.25f), *openIcon.get(), 0.0f, Colours::white.withAlpha(0.5f));

    mapOrderEditBtn->setVisible(true);

	scaleTextBox->addListener(this);

    scaleTextBox->setInputFilter(&txtFilter, false);

    //keyboardViewport->setScrollingMode(3);

	// allows for implementing mouseDown() to update the menus
	mode1Box->setInterceptsMouseClicks(false, false);
	mode2Box->setInterceptsMouseClicks(false, false);


    saveMenu.reset(new PopupMenu());
    saveMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::saveMode);
    saveMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::savePresetToFile);
    saveMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::exportReaperMap);
    saveMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::exportAbletonMap);

    loadMenu.reset(new PopupMenu());
    loadMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::loadMode);
    loadMenu->addCommandItem(appCmdMgr, IDs::CommandIDs::loadPreset);

    setSize (1000, 250);

    settingsIcon.reset(new Image(Image::PixelFormat::RGB, settingsButton->getWidth(), settingsButton->getHeight(), true));
    settingsButton->setImages(true, true, true, *settingsIcon.get(), 0.0f, Colour(), *settingsIcon.get(), 0.0f, Colours::white.withAlpha(0.25f), *settingsIcon.get(), 0.0f, Colours::white.withAlpha(0.5f));
	settingsButton->setClickingTogglesState(true);

	// DISABLED BECAUSE NOT IMPLEMENTED
    mapModeBox->setItemEnabled(3, false);
}

PluginControlComponent::~PluginControlComponent()
{
    buttonAttachments.clear();
    comboBoxAttachments.clear();
    sliderAttachments.clear();

    scaleTextBox = nullptr;
    mode1Box = nullptr;
    mode2Box = nullptr;
    mode1RootSld = nullptr;
    mode2RootSld = nullptr;
    scaleEntryBtn = nullptr;
    modeInfoButton = nullptr;
    periodShiftSld = nullptr;
    mode1ViewBtn = nullptr;
    mode2ViewBtn = nullptr;
    mode1RootLbl = nullptr;
    mode2RootLbl = nullptr;
    mapStyleBox = nullptr;
    midiChannelSld = nullptr;
    midiChannelLbl = nullptr;
    periodShiftLbl = nullptr;
    mapStyleLbl = nullptr;
    mapOrderEditBtn = nullptr;
    mapModeBox = nullptr;
    mapApplyBtn = nullptr;
	keyboard = nullptr;
    keyboardViewport = nullptr;
    saveButton = nullptr;
    openButton = nullptr;
    settingsButton = nullptr;
}

//==============================================================================

void PluginControlComponent::loadPresetNode(ValueTree presetNodeIn)
{
	if (presetNodeIn.hasType(IDs::presetNode) && (float)presetNodeIn[IDs::pluginPresetVersion] == SVK_PRESET_VERSION)
	{
		presetNode = presetNodeIn;

		ValueTree properties = presetNode.getChildWithName(IDs::presetProperties);
		if (properties.isValid())
		{
			setMode2View((int)properties[IDs::modeSelectorViewed]);
			setMappingMode(properties[IDs::mappingMode]);
		}

		ValueTree midiSettings = presetNode.getChildWithName(IDs::midiSettingsNode);
		if (midiSettings.isValid())
		{
			setPeriodShift(midiSettings[IDs::periodShift]);
			setMidiChannel(midiSettings[IDs::keyboardMidiChannel]);
		}

		ValueTree keyboardSettings = presetNode.getChildWithName(IDs::pianoNode);
		if (keyboardSettings.isValid())
			keyboard->restoreNode(keyboardSettings);

		ValueTree mapping = presetNode.getChildWithName(IDs::midiMapNode);
		if (mapping.isValid())
		{
			setMappingStyleId(mapping[IDs::modeMappingStyle]);
		}

		ValueTree modeSelectors = presetNode.getChildWithName(IDs::modeSelectorsNode);
		if (modeSelectors.isValid())
		{
			for (int num = 0; num < modeSelectors.getNumChildren(); num++)
			{
				ValueTree selector = modeSelectors.getChild(num);
				Mode* mode = presetManager->getModeBySelector(num);

				// TODO: generalize
				if (num == 0)
				{
					setMode1Root(selector[IDs::modeSelectorRootNote]);
					setMode1BoxText(mode->getName());
				}
				else if (num == 1)
				{
					setMode2Root(selector[IDs::modeSelectorRootNote]);
					setMode2BoxText(mode->getName());
				}

				if (num == (int)presetNode.getChildWithName(IDs::presetProperties)[IDs::modeSelectorViewed])
				{
					onModeViewedChange(mode);
				}
			}
		}
	}
}

void PluginControlComponent::onModeViewedChange(Mode* modeViewed)
{
	setScaleEntryText(modeViewed->getStepsString());
	keyboard->applyMode(modeViewed, true);
	resized();
}

void PluginControlComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));

    Colour buttonFill = getLookAndFeel().findColour(TextButton::ColourIds::buttonColourId);
    Colour buttonOutline = getLookAndFeel().findColour(TextEditor::ColourIds::outlineColourId);

    drawSaveIcon(g, saveButton->getBoundsInParent(), buttonFill, buttonOutline);
    drawLoadIcon(g, openButton->getBoundsInParent(), buttonFill, buttonOutline);
    drawSettingsIcon(g, settingsButton->getBoundsInParent(), buttonFill, buttonOutline);
}

void PluginControlComponent::resized()
{
	scaleTextBox->setSize(proportionOfWidth(0.2f), barHeight);
	scaleTextBox->setCentrePosition(proportionOfWidth(0.5f), barHeight / 2 + gap);
	
	scaleEntryBtn->setBounds(scaleTextBox->getRight() + 8, scaleTextBox->getY(), 31, 24);
	modeInfoButton->setBounds(scaleTextBox->getX() - 32, scaleTextBox->getY(), 24, 24);

	saveButton->setSize(barHeight, barHeight);
	saveButton->setTopLeftPosition(gap, scaleTextBox->getY());

	openButton->setSize(barHeight, barHeight);
	openButton->setTopLeftPosition(saveButton->getRight() + gap, saveButton->getY());

	settingsButton->setSize(barHeight, barHeight);
	settingsButton->setTopLeftPosition(openButton->getRight() + gap, saveButton->getY());

	mapModeBox->setTopLeftPosition(settingsButton->getRight() + gap, gap);
	mapModeBox->setSize(jmin(proportionOfWidth(0.15f), modeInfoButton->getX() - mapModeBox->getX() - gap), barHeight);

	mode1ViewBtn->setBounds(getWidth() - 32, gap, 32, barHeight);
	mode2ViewBtn->setBounds(getWidth() - 32, mode1ViewBtn->getBottom() + gap, 32, barHeight);

	int keyboardY = barHeight + gap * 2;

    if (inMappingMode)
    {
		mode1Box->setSize(proportionOfWidth(0.15f), barHeight);
		mode1Box->setTopLeftPosition(mode1ViewBtn->getX() - mode1Box->getWidth() - gap / 2, gap);

		mode2Box->setSize(proportionOfWidth(0.15f), barHeight);
		mode2Box->setTopLeftPosition(mode1Box->getX(), mode2ViewBtn->getY());

		mapStyleLbl->setTopLeftPosition(gap / 2, scaleTextBox->getBottom() + gap);
		mapStyleLbl->setSize(mapModeBox->getX() - mapStyleLbl->getX(), barHeight);
		mapStyleBox->setBounds(mapStyleLbl->getRight(), mapStyleLbl->getY(), mapModeBox->getWidth(), barHeight);

		mapOrderEditBtn->setBounds(mapStyleBox->getRight() + gap, mapStyleBox->getY(), 96, barHeight);
		mapApplyBtn->setBounds(mapOrderEditBtn->getRight() + gap, mapOrderEditBtn->getY(), 55, barHeight);

		keyboardY = (keyboardY * 2) - gap;
    }
    else
    {
		mode2Box->setSize(proportionOfWidth(0.15f) + mode2ViewBtn->getWidth(), barHeight);
		mode2Box->setTopLeftPosition(getWidth() - mode2Box->getWidth() - gap, gap);
    }

	mode1RootSld->setBounds(mode1Box->getX() - 80 - gap, mode1Box->getY(), 80, barHeight);
	mode2RootSld->setBounds(mode2Box->getX() - 80 - gap, mode2Box->getY(), 80, barHeight);

	mode1RootLbl->setBounds(mode1RootSld->getX() - 32, mode1Box->getY(), 32, barHeight);
	mode2RootLbl->setBounds(mode2RootSld->getX() - 32, mode2Box->getY(), 32, barHeight);

	int bottomBarY = getHeight() - barHeight - gap;

	periodShiftLbl->setBounds(gap, bottomBarY, 88, barHeight);
	periodShiftSld->setBounds(periodShiftLbl->getRight(), bottomBarY, 88, barHeight);
	midiChannelLbl->setBounds(periodShiftSld->getRight() + gap, bottomBarY, 96, barHeight);
	midiChannelSld->setBounds(midiChannelLbl->getRight(), bottomBarY, 86, barHeight);

	float scrollPosition = (float)getProperties()[IDs::viewportPosition] / keyboard->getWidth();
	
	keyboardViewport->setBounds(gap, keyboardY, getWidth() - gap * 2, jmax(bottomBarY - keyboardY - gap, 1));
    keyboardViewport->setScrollBarThickness(getHeight() / 28.0f);
	
	int height = keyboardViewport->getMaximumVisibleHeight();
	keyboard->setBounds(0, 0, keyboard->getPianoWidth(height), height);
	keyboardViewport->setViewPosition(keyboard->getWidth() * scrollPosition, 0);
}

void PluginControlComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == mode1Box.get())
    {
        appCmdMgr->invokeDirectly(IDs::CommandIDs::setMode1, true);
    }
    else if (comboBoxThatHasChanged == mode2Box.get())
    {
        appCmdMgr->invokeDirectly(IDs::CommandIDs::setMode2, true);
    }
	else if (comboBoxThatHasChanged == mapModeBox.get())
	{
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setMappingMode, true);
		setMappingMode(mapModeBox->getSelectedId());
	}
    else if (comboBoxThatHasChanged == mapStyleBox.get())
    {
        appCmdMgr->invokeDirectly(IDs::CommandIDs::setMappingStyle, true);
		setMappingStyleId(mapStyleBox->getSelectedId());
    }
}

void PluginControlComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == mode1RootSld.get())
    {
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setMode1RootNote, true);
		mode1RootLbl->setText(MidiMessage::getMidiNoteName(mode1RootSld->getValue(), true, true, 4), dontSendNotification);
    }
    else if (sliderThatWasMoved == mode2RootSld.get())
    {
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setMode2RootNote, true);
		mode2RootLbl->setText(MidiMessage::getMidiNoteName(mode2RootSld->getValue(), true, true, 4), dontSendNotification);
    }
    else if (sliderThatWasMoved == periodShiftSld.get())
    {
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setPeriodShift, true);
    }
    else if (sliderThatWasMoved == midiChannelSld.get())
    {
		appCmdMgr->invokeDirectly(IDs::CommandIDs::setMidiChannelOut, true);
    }
}

void PluginControlComponent::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == scaleEntryBtn.get())
    {
		appCmdMgr->invokeDirectly(IDs::CommandIDs::commitCustomScale, true);
    }
    else if (buttonThatWasClicked == modeInfoButton.get())
    {
		appCmdMgr->invokeDirectly(IDs::CommandIDs::showModeInfo, true);
    }
    else if (buttonThatWasClicked == mode1ViewBtn.get())
    {
		if (mode1ViewBtn->getToggleState())
			appCmdMgr->invokeDirectly(IDs::CommandIDs::setModeViewed, true);
    }
    else if (buttonThatWasClicked == mode2ViewBtn.get())
    {
		if (mode2ViewBtn->getToggleState())
			appCmdMgr->invokeDirectly(IDs::CommandIDs::setModeViewed, true);
    }
    else if (buttonThatWasClicked == mapOrderEditBtn.get())
    {
        appCmdMgr->invokeDirectly(IDs::CommandIDs::showMapOrderEdit, true);
    }
    else if (buttonThatWasClicked == mapApplyBtn.get())
    {
        appCmdMgr->invokeDirectly(IDs::CommandIDs::applyMapping, true);
    }
    else if (buttonThatWasClicked == saveButton.get())
    {
        saveMenu->showAt(saveButton.get());
    }
    else if (buttonThatWasClicked == openButton.get())
    {
        loadMenu->showAt(openButton.get());
    }
    else if (buttonThatWasClicked == settingsButton.get())
    {
        appCmdMgr->invokeDirectly(IDs::CommandIDs::showSettingsDialog, true);
    }
}

void PluginControlComponent::textEditorTextChanged(TextEditor& textEditor)
{

}

void PluginControlComponent::textEditorEscapeKeyPressed(TextEditor& textEditor)
{

}

void PluginControlComponent::textEditorReturnKeyPressed(TextEditor& textEditor)
{
	appCmdMgr->invokeDirectly(IDs::CommandIDs::commitCustomScale, true);
}

void PluginControlComponent::textEditorFocusLost(TextEditor& textEditor)
{

}

void PluginControlComponent::mouseDown(const MouseEvent& e)
{
	if (mode1Box->getBounds().contains(e.getPosition()))
	{
		String display = mode1Box->getText();
		presetManager->requestModeMenu(mode1Box->getRootMenu());
		mode1Box->setText(display, dontSendNotification);
		mode1Box->showPopup();
	}

	if (mode2Box->getBounds().contains(e.getPosition()))
	{
		String display = mode2Box->getText();
		presetManager->requestModeMenu(mode2Box->getRootMenu());
		mode2Box->setText(display, dontSendNotification);
		mode2Box->showPopup();
	}
}

void PluginControlComponent::connectToProcessor()
{
    comboBoxAttachments.add(new ComboBoxAttachment(processorTree, IDs::mappingMode.toString(), *mapModeBox.get()));
    comboBoxAttachments.add(new ComboBoxAttachment(processorTree, IDs::modeMappingStyle.toString(), *mapStyleBox.get()));
    sliderAttachments.add(new SliderAttachment(processorTree, IDs::periodShift.toString(), *periodShiftSld.get()));
    sliderAttachments.add(new SliderAttachment(processorTree, IDs::keyboardMidiChannel.toString(), *midiChannelSld.get()));
}

VirtualKeyboard::Keyboard* PluginControlComponent::getKeyboard()
{
	return keyboard.get();
}

Viewport* PluginControlComponent::getViewport()
{
	return  keyboardViewport.get();
}

TextEditor* PluginControlComponent::getScaleTextEditor()
{
	return scaleTextBox.get();
}

ComboBox* PluginControlComponent::getMappingStyleBox()
{
    return mapStyleBox.get();
}

String PluginControlComponent::getScaleEntryText()
{
	return scaleTextBox->getText();
}

void PluginControlComponent::setScaleEntryText(String textIn, NotificationType notify)
{
	scaleTextBox->setText(textIn, notify == sendNotification);
}

ReferencedComboBox* PluginControlComponent::getMode1Box()
{
	return mode1Box.get();
}

ReferencedComboBox* PluginControlComponent::getMode2Box()
{
	return mode2Box.get();
}

void PluginControlComponent::updateModeBoxMenus()
{

}

ImageButton* PluginControlComponent::getSettingsButton()
{
	return settingsButton.get();
}

TextButton* PluginControlComponent::getModeInfoButton()
{
	return modeInfoButton.get();
}

void PluginControlComponent::setViewPosition(int xIn)
{
	keyboardViewport->setViewPosition(xIn, 0);
	getProperties().set(IDs::viewportPosition, xIn);
}

int PluginControlComponent::getMode1BoxSelection()
{
	return mode1Box->getSelectedId();
}

int PluginControlComponent::getMode2BoxSelection()
{
	return mode2Box->getSelectedId();
}

void PluginControlComponent::setMode1BoxText(String textIn, NotificationType notify)
{
	mode1Box->setText(textIn, notify);
}

void PluginControlComponent::setMode2BoxText(String textIn, NotificationType notify)
{
	mode2Box->setText(textIn, notify);
}

void PluginControlComponent::setMode1BoxId(int idIn, NotificationType notify)
{
	mode1Box->setSelectedId(idIn, notify);
}

void PluginControlComponent::setMode2BoxId(int idIn, NotificationType notify)
{
	mode2Box->setSelectedId(idIn, notify);
}

void PluginControlComponent::setMode1View(bool isViewed, NotificationType notify)
{
	mode1ViewBtn->setToggleState(isViewed, notify);
}

bool PluginControlComponent::getMode1View()
{
	return mode1ViewBtn->getToggleState();
}

void PluginControlComponent::setMode2View(bool isViewed, NotificationType notify)
{
	mode2ViewBtn->setToggleState(isViewed, notify);
}

bool PluginControlComponent::getMode2View()
{
	return mode2ViewBtn->getToggleState();
}

int PluginControlComponent::getModeSelectorViewed()
{
	return mode2ViewBtn->getToggleState();
}

int PluginControlComponent::getMappingMode()
{
    return mapModeBox->getSelectedId();
}

void PluginControlComponent::setMappingMode(int mappingModeId, NotificationType notify)
{
	mapModeBox->setSelectedId(mappingModeId, notify);

    inMappingMode = mappingModeId > 1;

	Array<Component*> mappingComponents = {
		mapStyleLbl.get(),
		mapStyleBox.get(),
		mapOrderEditBtn.get(),
		mode1RootLbl.get(),
		mode1RootSld.get(),
		mode1Box.get(),
		mode1ViewBtn.get(),
		mode2ViewBtn.get()
	};
	
	for (auto c : mappingComponents)
	{
		c->setVisible(inMappingMode);
		c->setEnabled(inMappingMode);
	}

    if (mappingModeId == 3 || mapStyleBox->getSelectedId() == 3)
    {
        mapOrderEditBtn->setVisible(true);
        mapApplyBtn->setVisible(true);
    }
    else
    {
        mapOrderEditBtn->setVisible(false);
        mapApplyBtn->setVisible(false);
    }

    if (inMappingMode)
        mode2ViewBtn->setToggleState(true, sendNotification);

    resized();
}

int PluginControlComponent::getMappingStyle()
{
	return mapStyleBox->getSelectedId();
}

void PluginControlComponent::setMappingStyleId(int idIn, NotificationType notify)
{
	mapStyleBox->setSelectedId(idIn, notify);

    if (inMappingMode && (idIn == 3 || mapModeBox->getSelectedId() == 3))
    {
        mapOrderEditBtn->setVisible(true);
        mapApplyBtn->setVisible(true);
    }
    else
    {
        mapOrderEditBtn->setVisible(false);
        mapApplyBtn->setVisible(false);
    }
}

int PluginControlComponent::getMode1Root()
{
	return mode1RootSld->getValue();
}

void PluginControlComponent::setMode1Root(int rootIn, NotificationType notify)
{
	mode1RootSld->setValue(rootIn, notify);
	mode1RootLbl->setText(MidiMessage::getMidiNoteName(rootIn, true, true, 4), dontSendNotification);
}

int PluginControlComponent::getMode2Root()
{
	return mode2RootSld->getValue();
}

void PluginControlComponent::setMode2Root(int rootIn, NotificationType notify)
{
	mode2RootSld->setValue(rootIn, notify);
	mode2RootLbl->setText(MidiMessage::getMidiNoteName(rootIn, true, true, 4), dontSendNotification);
}

int PluginControlComponent::getPeriodShift()
{
	return periodShiftSld->getValue();

}
void PluginControlComponent::setPeriodShift(int periodsIn, NotificationType notify)
{
	periodShiftSld->setValue(periodsIn);
}

int PluginControlComponent::getMidiChannel()
{
	return midiChannelSld->getValue();
}

void PluginControlComponent::setMidiChannel(int channelIn, NotificationType notify)
{
	midiChannelSld->setValue(channelIn, notify);
}
