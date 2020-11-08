/*
  ==============================================================================

    PluginControlComponent.cpp
    Created: 8 Jul 2019
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "PluginControlComponent.h"

PluginControlComponent::PluginControlComponent(SvkPluginState* pluginStateIn)
    : pluginState(pluginStateIn)
{
    pluginState->addListener(this);
    presetManager = pluginState->getPresetManager();

    scaleTextBox.reset(new TextEditor("Scale Text Box"));
    addAndMakeVisible(scaleTextBox.get());
    scaleTextBox->setMultiLine(false);
    scaleTextBox->setReturnKeyStartsNewLine(false);
    scaleTextBox->setReadOnly(false);
    scaleTextBox->setScrollbarsShown(true);
    scaleTextBox->setCaretVisible(true);
    scaleTextBox->setPopupMenuEnabled(true);
    scaleTextBox->setText(String());

    mode1Box.reset(new ReferencedComboBox("Mode1 Box"));
    addAndMakeVisible(mode1Box.get());
    mode1Box->setEditableText(false);
    mode1Box->setJustificationType(Justification::centredLeft);
    mode1Box->setTextWhenNothingSelected(String());
    mode1Box->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    mode1Box->setTooltip(inputModeTooltip);
    mode1Box->addListener(this);

    mode2Box.reset(new ReferencedComboBox("Mode2 Box"));
    addAndMakeVisible(mode2Box.get());
    mode2Box->setEditableText(false);
    mode2Box->setJustificationType(Justification::centredLeft);
    mode2Box->setTextWhenNothingSelected(String());
    mode2Box->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    mode2Box->setTooltip(outputModeTooltip);
    mode2Box->addListener(this);

    mode1RootSld.reset(new Slider("Mode1 Root Slider"));
    addAndMakeVisible(mode1RootSld.get());
    mode1RootSld->setRange(0, 127, 1);
    mode1RootSld->setSliderStyle(Slider::IncDecButtons);
    mode1RootSld->setTextBoxStyle(Slider::TextBoxLeft, false, 40, 20);
    mode1RootSld->addListener(this);

    mode2RootSld.reset(new Slider("mode2RootSld"));
    addAndMakeVisible(mode2RootSld.get());
    mode2RootSld->setRange(0, 127, 1);
    mode2RootSld->setSliderStyle(Slider::IncDecButtons);
    mode2RootSld->setTextBoxStyle(Slider::TextBoxLeft, false, 40, 20);
    mode2RootSld->addListener(this);

    scaleEntryBtn.reset(new TextButton("Scale Entry Button"));
    addAndMakeVisible(scaleEntryBtn.get());
    scaleEntryBtn->setButtonText(TRANS("OK"));
    scaleEntryBtn->addListener(this);

    modeInfoButton.reset(new TextButton("Mode Info Button"));
    addAndMakeVisible(modeInfoButton.get());
    modeInfoButton->setButtonText(TRANS("i"));
    modeInfoButton->addListener(this);

    mode1ViewBtn.reset(new ToggleButton("Mode1 View Button"));
    addChildComponent(mode1ViewBtn.get());
    mode1ViewBtn->setButtonText(String());
    mode1ViewBtn->setRadioGroupId(10);
    mode1ViewBtn->addListener(this);
    mode1ViewBtn->setToggleState(true, dontSendNotification);

    mode2ViewBtn.reset(new ToggleButton("Mode2 View Button"));
    addChildComponent(mode2ViewBtn.get());
    mode2ViewBtn->setButtonText(String());
    mode2ViewBtn->setRadioGroupId(10);
    mode2ViewBtn->addListener(this);

    mode1RootLbl.reset(new Label("Mode1 Root Label",
        TRANS("C4")));
    addAndMakeVisible(mode1RootLbl.get());
    mode1RootLbl->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
    mode1RootLbl->setJustificationType(Justification::centredLeft);
    mode1RootLbl->setEditable(false, false, false);
    mode1RootLbl->setColour(TextEditor::textColourId, Colours::black);
    mode1RootLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    mode2RootLbl.reset(new Label("Mode 2 Root Label",
        TRANS("F5")));
    addAndMakeVisible(mode2RootLbl.get());
    mode2RootLbl->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
    mode2RootLbl->setJustificationType(Justification::centredLeft);
    mode2RootLbl->setEditable(false, false, false);
    mode2RootLbl->setColour(TextEditor::textColourId, Colours::black);
    mode2RootLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    mapStyleBox.reset(new ComboBox("Mapping Style Box"));
    addAndMakeVisible(mapStyleBox.get());
    mapStyleBox->setEditableText(false);
    mapStyleBox->setJustificationType(Justification::centredLeft);
    mapStyleBox->setTextWhenNothingSelected(TRANS("Mode To Mode"));
    mapStyleBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    mapStyleBox->addItem(TRANS("Mode To Mode"), 1);
    mapStyleBox->addItem(TRANS("Scale To Mode"), 2);
    mapStyleBox->addItem(TRANS("By Key Layers"), 3);
    mapStyleBox->setTooltip(mapStyleTooltip);
    mapStyleBox->addListener(this);

    mapStyleLbl.reset(new Label("Mapping Style Label",
        TRANS("Mapping Style:")));
    addAndMakeVisible(mapStyleLbl.get());
    mapStyleLbl->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
    mapStyleLbl->setJustificationType(Justification::centredLeft);
    mapStyleLbl->setEditable(false, false, false);
    mapStyleLbl->setColour(TextEditor::textColourId, Colours::black);
    mapStyleLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    mapOrderEditBtn.reset(new TextButton("Map Order Edit Button", TRANS("Set layer mappings")));
    addChildComponent(mapOrderEditBtn.get());
    mapOrderEditBtn->setButtonText(TRANS("Layers"));
    mapOrderEditBtn->addListener(this);

    mapModeBox.reset(new ComboBox("Mapping Mode Box"));
    addAndMakeVisible(mapModeBox.get());
    mapModeBox->setEditableText(false);
    mapModeBox->setJustificationType(Justification::centredLeft);
    mapModeBox->setTextWhenNothingSelected(TRANS("Mapping Off"));
    mapModeBox->setTextWhenNoChoicesAvailable(TRANS("Mapping Off"));
    mapModeBox->addItem(TRANS("Mapping Off"), 1);
    mapModeBox->addItem(TRANS("Auto Map"), 2);
    mapModeBox->addItem(TRANS("Manual Map"), 3);
    mapModeBox->setTooltip(mapModeTooltip);
    mapModeBox->addListener(this);

    mapApplyBtn.reset(new TextButton("Apply Layer Map Button"));
    addChildComponent(mapApplyBtn.get());
    mapApplyBtn->setButtonText(TRANS("Apply"));
    mapApplyBtn->addListener(this);

    mapCopyToManualBtn.reset(new TextButton("Copy Mapping to Manual Button", TRANS("Edit this mapping in Manual mode.")));
    addChildComponent(mapCopyToManualBtn.get());
    mapCopyToManualBtn->setButtonText(TRANS("Edit"));
    mapCopyToManualBtn->setSize(40, barHeight);
    mapCopyToManualBtn->addListener(this);

    mapManualTip.reset(new Label("Manual Mapping Tip", TRANS("Right-click output key, then trigger input MIDI note.")));
    addChildComponent(mapManualTip.get());

    mapManualStatus.reset(new Label("Manual Mapping Status", noKeySelectedTrans));
    addChildComponent(mapManualStatus.get());

    mapManualCancel.reset(new TextButton("Manual Mapping Cancel", TRANS("Cancel key mapping.")));
    addChildComponent(mapManualCancel.get());
    mapManualCancel->setButtonText(juce::CharPointer_UTF8("\xc3\x97"));
    mapManualCancel->addListener(this);
    mapManualCancel->setSize(barHeight * 0.75f, barHeight * 0.75f);

    mapManualResetBtn.reset(new TextButton("Reset Manual Mapping Button", TRANS("Reset mapping.")));
    addChildComponent(mapManualResetBtn.get());
    mapManualResetBtn->setButtonText(TRANS("Reset"));
    mapManualResetBtn->setSize(48, barHeight);
    mapManualResetBtn->addListener(this);

    keyboard.reset(new VirtualKeyboard::Keyboard());
    keyboard->addListener(pluginState->getMidiProcessor());

    keyboardViewport.reset(new KeyboardViewport(keyboard.get(), "Keyboard Viewport"));
    addAndMakeVisible(keyboardViewport.get());
    viewportScrollBar = &keyboardViewport->getHorizontalScrollBar();

    saveButton.reset(new ImageButton("Save Button"));
    addAndMakeVisible(saveButton.get());
    saveButton->setButtonText(TRANS("Save"));
    saveButton->addListener(this);
    saveButton->setImages(false, true, true,
        Image(), 1.000f, Colour(0x00000000),
        Image(), 1.000f, Colour(0x00000000),
        Image(), 1.000f, Colour(0x00000000));
    saveButton->setSize(28, 28);

    openButton.reset(new ImageButton("Open Button"));
    addAndMakeVisible(openButton.get());
    openButton->setButtonText(TRANS("Open"));
    openButton->addListener(this);
    openButton->setImages(false, true, true,
        Image(), 1.000f, Colour(0x00000000),
        Image(), 1.000f, Colour(0x00000000),
        Image(), 1.000f, Colour(0x00000000));
    openButton->setSize(28, 28);

    settingsButton.reset(new ImageButton("Settings Button"));
    addAndMakeVisible(settingsButton.get());
    settingsButton->setButtonText(TRANS("Settings"));
    settingsButton->addListener(this);
    settingsButton->setImages(false, true, true,
        Image(), 1.000f, Colour(0x00000000),
        Image(), 1.000f, Colour(0x00000000),
        Image(), 1.000f, Colour(0x00000000));
    settingsButton->setSize(28, 28);

    saveIcon.reset(new Image(Image::PixelFormat::RGB, saveButton->getWidth(), saveButton->getHeight(), true));
    openIcon.reset(new Image(Image::PixelFormat::RGB, openButton->getWidth(), openButton->getHeight(), true));

    saveButton->setImages(true, true, true, *saveIcon.get(), 0.0f, Colour(), *saveIcon.get(), 0.0f, Colours::white.withAlpha(0.25f), *saveIcon.get(), 0.0f, Colours::white.withAlpha(0.5f));
    openButton->setImages(true, true, true, *openIcon.get(), 0.0f, Colour(), *openIcon.get(), 0.0f, Colours::white.withAlpha(0.25f), *openIcon.get(), 0.0f, Colours::white.withAlpha(0.5f));


    scaleTextBox->addListener(this);
    scaleTextBox->setInputRestrictions(0, " 0123456789");

    //keyboardViewport->setScrollingMode(3);

    // allows for implementing mouseDown() to update the menus
    mode1Box->setInterceptsMouseClicks(false, false);
    mode2Box->setInterceptsMouseClicks(false, false);


    saveMenu.reset(new PopupMenu());
    saveMenuItems =
    {
        PopupMenu::Item("Save Mode").setID(SaveMenuOptions::SaveMode).setAction([&]() { pluginState->saveModeViewedToFile(); }),
        PopupMenu::Item("Save Preset").setID(SaveMenuOptions::SavePreset).setAction([&]() { pluginState->savePresetToFile(); }),
        PopupMenu::Item("Export Mapping as KBM").setID(SaveMenuOptions::ExportKBM).setAction([&]() { exportKbmMapping(); }),
        PopupMenu::Item("Export for Reaper Note Names").setID(SaveMenuOptions::ExportReaper).setAction([&]() { exportModeViewedForReaper(); }),
        PopupMenu::Item("Export for Ableton Folding").setID(SaveMenuOptions::ExportAbleton).setAction([&]() { exportModeViewedForAbleton(); })
    };
    for (auto item : saveMenuItems)
        saveMenu->addItem(item);
    
    loadMenu.reset(new PopupMenu());
    loadMenuItems =
    {
        PopupMenu::Item("Load Mode").setID(LoadMenuOptions::LoadMode).setAction([&]() { browseForModeToOpen(); }),
        PopupMenu::Item("Load Preset").setID(LoadMenuOptions::LoadPreset).setAction([&]() { browseForPresetToOpen(); })
    };
    for (auto item : loadMenuItems)
        loadMenu->addItem(item);


    ValueTree pluginWindowNode = pluginState->getPluginEditorNode();
    int recallWidth = pluginWindowNode[IDs::windowBoundsW];
    int recallHeight = pluginWindowNode[IDs::windowBoundsH];
    centerKeyPos = pluginWindowNode[IDs::viewportPosition];

    setSize(
        recallWidth > 0 ? recallWidth : 1000,
        recallHeight > 0 ? recallHeight : defaultHeight
    );

    settingsIcon.reset(new Image(Image::PixelFormat::RGB, settingsButton->getWidth(), settingsButton->getHeight(), true));
    settingsButton->setImages(true, true, true, *settingsIcon.get(), 0.0f, Colour(), *settingsIcon.get(), 0.0f, Colours::white.withAlpha(0.25f), *settingsIcon.get(), 0.0f, Colours::white.withAlpha(0.5f));
    settingsButton->setClickingTogglesState(true);

    mappingComponents = 
    {
        mapStyleLbl.get(),
        mapStyleBox.get(),
        mapOrderEditBtn.get(),
        mode1RootLbl.get(),
        mode1RootSld.get(),
        mode1Box.get(),
        mode1ViewBtn.get(),
        mode2ViewBtn.get(),
        mapCopyToManualBtn.get()
    };

    loadPresetNode(pluginState->getPresetNode());

    viewportScrollBar->addListener(this);

    // Update keyboard to show external midi input
#if (!JUCE_ANDROID && !JUCE_IOS)
    startTimerHz(30);
#endif
}

PluginControlComponent::~PluginControlComponent()
{
    pluginState->removeListener(this);

    if (settingsPanelOpen)
    {
        pluginState->removeListener(settingsContainer.get());
        settingsContainer = nullptr;
    }

    scaleTextBox = nullptr;
    mode1Box = nullptr;
    mode2Box = nullptr;
    mode1RootSld = nullptr;
    mode2RootSld = nullptr;
    scaleEntryBtn = nullptr;
    modeInfoButton = nullptr;
    mode1ViewBtn = nullptr;
    mode2ViewBtn = nullptr;
    mode1RootLbl = nullptr;
    mode2RootLbl = nullptr;
    mapStyleBox = nullptr;
    mapStyleLbl = nullptr;
    mapManualTip = nullptr;
    mapManualStatus = nullptr;
    mapOrderEditBtn = nullptr;
    mapManualCancel = nullptr;
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
        ValueTree presetNode = presetNodeIn;

        ValueTree properties = presetNode.getChildWithName(IDs::presetProperties);
        if (properties.isValid())
        {
            mode2ViewBtn->setToggleState((int)properties[IDs::modeSelectorViewed], dontSendNotification);
        }

        ValueTree keyboardSettings = presetNode.getChildWithName(IDs::pianoNode);
        if (keyboardSettings.isValid())
        {
            keyboard->restoreNode(keyboardSettings);
        }

        ValueTree mapping = presetNode.getChildWithName(IDs::midiMapNode);
        if (mapping.isValid())
        {
            setMapModeBoxId(mapping[IDs::mappingMode]);
            setMappingStyleId(mapping[IDs::autoMappingStyle]);
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
                    mode1RootSld->setValue(selector[IDs::modeSelectorRootNote], dontSendNotification);
                    mode1Box->setText(mode->getName(), dontSendNotification);
                }
                else if (num == 1)
                {
                    mode2RootSld->setValue(selector[IDs::modeSelectorRootNote], dontSendNotification);
                    mode2Box->setText(mode->getName(), dontSendNotification);
                }

                updateRootNoteLabels();

                if (num == (int)presetNode.getChildWithName(IDs::presetProperties)[IDs::modeSelectorViewed])
                {
                    modeViewedChanged(mode, num, 0 /*unused*/);
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
    int basicHeight = getHeight();

    if (settingsPanelOpen)
    {
        basicHeight -= defaultHeight;
        settingsContainer->setBounds(0, basicHeight, getWidth(), defaultHeight);
        settingsContainer->resized();
    }

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

        mapManualTip->setBounds(mapStyleLbl->getX(), mapStyleLbl->getY(), mapManualTip->getFont().getStringWidth(mapManualTip->getText()), barHeight);
        mapManualStatus->setBounds(mapManualTip->getRight(), mapManualTip->getY(), mapManualStatus->getFont().getStringWidth(mapManualStatus->getText()) + 8, barHeight);

        mapOrderEditBtn->setBounds(mapStyleBox->getRight() + gap, mapStyleBox->getY(), 55, barHeight);
        mapApplyBtn->setBounds(mapOrderEditBtn->getRight() + gap, mapOrderEditBtn->getY(), 55, barHeight);

        if (mapApplyBtn->isVisible())
            mapCopyToManualBtn->setTopLeftPosition(mapApplyBtn->getRight() + gap, mapApplyBtn->getY());
        else
            mapCopyToManualBtn->setTopLeftPosition(mapOrderEditBtn->getPosition());

        mapManualCancel->setTopLeftPosition(mapManualStatus->getRight() + gap / 2, mapManualStatus->getY() + mapManualCancel->getHeight() / 4);

        if (mapModeBox->getSelectedId() == 3)
        {
            if (mapManualCancel->isVisible())
                mapManualResetBtn->setTopLeftPosition(mapManualCancel->getRight() + gap, mapManualStatus->getY());
            else
                mapManualResetBtn->setTopLeftPosition(mapManualStatus->getRight() + gap, mapManualStatus->getY());
        }

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
    
    viewportScrollBar->removeListener(this);

    int scrollBarThickness = basicHeight / 28;
    int keyboardViewWidth = getWidth() - gap * 2;
    int keyboardViewHeight = jmax(basicHeight - keyboardY - gap, 1);

    keyboardViewport->setBounds(gap, keyboardY, keyboardViewWidth, keyboardViewHeight);
    keyboardViewport->setScrollBarThickness(scrollBarThickness);
    keyboardViewport->resizeKeyboard();
    keyboardViewport->centerOnKey((int)centerKeyPos);

    if (keyboard->getWidth() < keyboardViewWidth)
    {
        keyboardViewport->setSize(keyboard->getWidth(), keyboardViewHeight);
        keyboardViewport->setTopLeftPosition((getWidth() - keyboard->getWidth()) / 2, keyboardY);
    }

    viewportScrollBar->addListener(this);

    pluginState->getPluginEditorNode().setProperty(IDs::windowBoundsW, getWidth(), nullptr);
    pluginState->getPluginEditorNode().setProperty(IDs::windowBoundsH, basicHeight, nullptr);
}

void PluginControlComponent::mouseDown(const MouseEvent& e)
{
    if (mode1Box->isVisible() && mode1Box->getBounds().contains(e.getPosition()))
    {
        String display = mode1Box->getText();
        presetManager->requestModeMenu(mode1Box->getRootMenu());
        mode1Box->setText(display, dontSendNotification);
        mode1Box->showPopup();
    }

    if (mode2Box->isVisible() && mode2Box->getBounds().contains(e.getPosition()))
    {
        String display = mode2Box->getText();
        presetManager->requestModeMenu(mode2Box->getRootMenu());
        mode2Box->setText(display, dontSendNotification);
        mode2Box->showPopup();
    }
}

void PluginControlComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == mode1Box.get())
    {
        pluginState->handleModeSelection(0, mode1Box->getSelectedId());
    }
    else if (comboBoxThatHasChanged == mode2Box.get())
    {
        pluginState->handleModeSelection(1, mode2Box->getSelectedId());
    }
    else if (comboBoxThatHasChanged == mapModeBox.get())
    {
        setMapModeBoxId(mapModeBox->getSelectedId());
    }
    else if (comboBoxThatHasChanged == mapStyleBox.get())
    {
        setMappingStyleId(mapStyleBox->getSelectedId());
    }
}

void PluginControlComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == mode1RootSld.get())
    {
        pluginState->setModeSelectorRoot(0, mode1RootSld->getValue());
        updateRootNoteLabels();
    }
    else if (sliderThatWasMoved == mode2RootSld.get())
    {
        pluginState->setModeSelectorRoot(1, mode2RootSld->getValue());
        updateRootNoteLabels();
    }
}

void PluginControlComponent::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == scaleEntryBtn.get())
    {
        submitCustomScale();
    }
    else if (buttonThatWasClicked == modeInfoButton.get())
    {
        showModeInfo();
    }
    else if (buttonThatWasClicked == mode1ViewBtn.get() || buttonThatWasClicked == mode2ViewBtn.get())
    {
        pluginState->setModeSelectorViewed(mode2ViewBtn->getToggleState());
        scaleTextBox->setText(pluginState->getModeViewed()->getStepsString(), false);
    }
    else if (buttonThatWasClicked == mapCopyToManualBtn.get())
    {
        NoteMap autoMap = *pluginState->getMidiInputFilterMap();
        mapModeBox->setSelectedId(3, sendNotificationSync);
        manualMappingHelper->resetMapping(autoMap);
    }
    else if (buttonThatWasClicked == mapOrderEditBtn.get())
    {
        showMapOrderEditDialog();
    }
    else if (buttonThatWasClicked == mapApplyBtn.get())
    {
        pluginState->doAutoMapping();
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
        showSettingsDialog();
    }
    else if (buttonThatWasClicked == mapManualCancel.get())
    {
        manualMappingHelper->cancelKeyMap();
    }
    else if (buttonThatWasClicked == mapManualResetBtn.get())
    {
        manualMappingHelper->resetMapping();
        keyMappingStatusChanged(-1, false);
    }
}

void PluginControlComponent::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    submitCustomScale();
}

void PluginControlComponent::scrollBarMoved(ScrollBar* bar, double newRangeStart)
{
    centerKeyPos = keyboardViewport->getCenterKeyProportion();
    pluginState->getPluginEditorNode().setProperty(IDs::viewportPosition, centerKeyPos, nullptr);
}

void PluginControlComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    // Mode Info Changed
    if (source == modeInfo)
    {
        pluginState->commitModeInfo();
        onModeViewedChange(pluginState->getModeViewed());
    }
}

void PluginControlComponent::timerCallback()
{
    keyboard->repaint();
}

//==============================================================================

void PluginControlComponent::resyncPresetNode(ValueTree presetNode)
{
    keyboard->restoreNode(presetNode.getChildWithName(IDs::pianoNode), false);
}

void PluginControlComponent::modeViewedChanged(Mode* modeIn, int selectorNumber, int slotNumber)
{
    MidiKeyboardState* displayState = selectorNumber == 0
        ? pluginState->getMidiProcessor()->getOriginalKeyboardState()
        : pluginState->getMidiProcessor()->getRemappedKeyboardState();

    keyboard->displayKeyboardState(displayState);
    onModeViewedChange(modeIn);
}

void PluginControlComponent::mappingModeChanged(int mappingModeId)
{
    inMappingMode = mappingModeId > 1;

    for (auto c : mappingComponents)
    {
        c->setVisible(inMappingMode);
        c->setEnabled(inMappingMode);
    }

    if (inMappingMode)
    {
        // this is a fallback
        if (mappingModeId == 2)
            mapStyleBox->setSelectedId(pluginState->getMappingNode()[IDs::autoMappingStyle], dontSendNotification);

        if (mappingModeId == 2 && mapStyleBox->getSelectedId() == 3)
        {
            mapOrderEditBtn->setVisible(true);
            mapApplyBtn->setVisible(true);
        }
        else
        {
            mapOrderEditBtn->setVisible(false);
            mapApplyBtn->setVisible(false);
        }

        mode2ViewBtn->setToggleState(true, sendNotification);
        toggleMenuItemEnablement(*saveMenu, SaveMenuOptions::ExportKBM, true);
    }
    else
    {
        toggleMenuItemEnablement(*saveMenu, SaveMenuOptions::ExportKBM, false);
    }

    if (mappingModeId == 3)
    {
        beginManualMapping();
    }
    else
    {
        mapManualTip->setVisible(false);
        mapManualStatus->setVisible(false);
        mapManualCancel->setVisible(false);
        mapManualResetBtn->setVisible(false);

        keyboard->setUIMode(VirtualKeyboard::UIMode::playMode);
        mapManualStatus->setText(noKeySelectedTrans, sendNotification);
    }

    resized();
}

void PluginControlComponent::inputMappingChanged(NoteMap& inputNoteMap)
{
    keyboard->setInputNoteMap(inputNoteMap);
}

void PluginControlComponent::customModeChanged(Mode* newCustomMode)
{
    // This is a hack and should be handled better

    String customModeName = newCustomMode->getName();

    if (getModeSelectorViewed() == 1)
        mode2Box->setText(customModeName, dontSendNotification);
    else
        mode1Box->setText(customModeName, dontSendNotification);

}

void PluginControlComponent::modeInfoChanged(Mode* modeEdited)
{
    if (getModeSelectorViewed() == 1)
        mode2Box->setText(modeEdited->getName(), dontSendNotification);
    else
        mode1Box->setText(modeEdited->getName(), dontSendNotification);
}

void PluginControlComponent::settingsTabChanged(int tabIndex, const String& tabName, SvkSettingsPanel* panelChangedTo)
{
    DBG("Settings changed to " + tabName + ", tab: " + String(tabIndex));

    if (panelChangedTo->getName() == "ColourSettingsPanel")
    {
        // TODO clean up
        beginColorEditing();
    }
    else if (isColorEditing)
    {
        endColorEditing();
    }
    
    if (tabName == "Mapping")
    {
        mappingSettingsOpen = true;
        static_cast<MappingSettingsPanel*>(panelChangedTo)->registerEditorListener(this);
    }

    else if (mappingSettingsOpen)
    {
        mappingSettingsOpen = false;

        // MappingSettingsPanel removes listeners from NoteMapEditor
    }

    pluginState->getPluginEditorNode().setProperty(IDs::settingsTabName, tabName, nullptr);
}

void PluginControlComponent::keyMappingStatusChanged(int keyNumber, bool preparedToMap)
{
    if (preparedToMap)
    {
        mapManualStatus->setText(waitingForTrans + String(keyNumber), sendNotification);
        mapManualCancel->setVisible(true);
    }
    else
    {
        mapManualStatus->setText(noKeySelectedTrans, sendNotification);
        mapManualCancel->setVisible(false);
    }

    mapManualStatus->setSize(mapManualStatus->getFont().getStringWidth(mapManualStatus->getText()) + 8, mapManualStatus->getHeight());
    resized();
}

void PluginControlComponent::keyMapConfirmed(int keyNumber, int midiNote)
{
    mapManualStatus->setText("Key " + String(keyNumber) + " mapped to MIDI Note " + String(midiNote), sendNotification);
    mapManualCancel->setVisible(false);
    resized();
}

void PluginControlComponent::mappingEditorChanged(NoteMap& newMapping)
{
    DBG("MAPPING CHANGED");
    pluginState->setMidiInputMap(newMapping, mapModeBox->getSelectedId() == 3);
}

//==============================================================================

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

int PluginControlComponent::getModeSelectorViewed()
{
    return mode2ViewBtn->getToggleState();
}

void PluginControlComponent::setMapModeBoxId(int mappingModeId, NotificationType notify)
{
    mapModeBox->setSelectedId(mappingModeId, dontSendNotification);

    if (notify != dontSendNotification)
        pluginState->setMapMode(mapModeBox->getSelectedId());
}

void PluginControlComponent::setMappingStyleId(int idIn, NotificationType notify)
{
    mapStyleBox->setSelectedId(idIn, dontSendNotification);

    if (idIn == 3 && mapModeBox->getSelectedId() == 2)
    {
        mapOrderEditBtn->setVisible(true);
        mapApplyBtn->setVisible(true);
    }
    else
    {
        mapOrderEditBtn->setVisible(false);
        mapApplyBtn->setVisible(false);
    }

    if (notify != dontSendNotification)
        pluginState->setAutoMapStyle(mapStyleBox->getSelectedId());

    resized();
}

void PluginControlComponent::submitCustomScale()
{
    pluginState->setModeCustom(scaleTextBox->getText());
}

void PluginControlComponent::updateRootNoteLabels()
{
    mode1RootLbl->setText(MidiMessage::getMidiNoteName(mode1RootSld->getValue(), true, true, 4), dontSendNotification);
    mode2RootLbl->setText(MidiMessage::getMidiNoteName(mode2RootSld->getValue(), true, true, 4), dontSendNotification);
}

//==============================================================================

void PluginControlComponent::showModeInfo()
{
    modeInfo  = new ModeInfoDialog(pluginState->getModeViewed());
    modeInfo->addChangeListener(this);
    CallOutBox::launchAsynchronously(std::unique_ptr<Component>(modeInfo), scaleTextBox->getScreenBounds(), nullptr);
}

void PluginControlComponent::showMapOrderEditDialog()
{
    mapByOrderDialog = new MapByOrderDialog(pluginState->getModeMapper(), pluginState->getMode1(), pluginState->getMode2());
    CallOutBox::launchAsynchronously(std::unique_ptr<Component>(mapByOrderDialog), mapStyleBox->getScreenBounds(), nullptr);
}

void PluginControlComponent::showSettingsDialog()
{
    if (!settingsPanelOpen)
    {
        settingsContainer.reset(new SettingsContainer(pluginState));
        settingsContainer->setKeyboardPointer(keyboard.get());
        settingsContainer->addListener(this);
        pluginState->addListener(settingsContainer.get());

        settingsPanelOpen = true;
        pluginState->getPluginEditorNode().setProperty(IDs::settingsOpen, true, nullptr);
        settingsButton->setToggleState(true, dontSendNotification);

        addAndMakeVisible(settingsContainer.get());
        getParentComponent()->setSize(getWidth(), getHeight() + defaultHeight);
        
        int setToTab = 0;
        String lastTab = pluginState->getPluginEditorNode()[IDs::settingsTabName].toString();
        DBG("Last tab: " + lastTab);
        if (lastTab.length() > 0)
            setToTab = settingsContainer->getTabNames().indexOf(lastTab);

        settingsContainer->setCurrentTabIndex(setToTab, true);
    }
    else
    {
        hideSettings();
    }
}

void PluginControlComponent::hideSettings()
{
    settingsContainer->removeListener(this);
    settingsContainer->setVisible(false);
    pluginState->removeListener(settingsContainer.get());
    settingsContainer = nullptr;

    settingsPanelOpen = false;
    pluginState->getPluginEditorNode().setProperty(IDs::settingsOpen, false, nullptr);

    if (isColorEditing)
        endColorEditing();

    getParentComponent()->setSize(getWidth(), getHeight() - defaultHeight);
}

void PluginControlComponent::beginColorEditing()
{
    keyboard->setUIMode(VirtualKeyboard::UIMode::editMode);
    isColorEditing = true;
    
    keyMappingStatusChanged(-1, false);
    mapModeBox->setEnabled(false);
    mapStyleBox->setEnabled(false);
    mapManualResetBtn->setEnabled(false);
    mapOrderEditBtn->setEnabled(false);
    mapApplyBtn->setEnabled(false);
    mapCopyToManualBtn->setEnabled(false);
}

void PluginControlComponent::endColorEditing()
{
    keyboard->setUIMode(VirtualKeyboard::UIMode::playMode);
    isColorEditing = false;

    mapModeBox->setEnabled(true);
    mapStyleBox->setEnabled(true);
    mapManualResetBtn->setEnabled(true);
    mapOrderEditBtn->setEnabled(true);
    mapApplyBtn->setEnabled(true);
    mapCopyToManualBtn->setEnabled(true);

    // Restore previous UI state
    pluginState->setMapMode(mapModeBox->getSelectedId());
}

void PluginControlComponent::beginManualMapping()
{
    mapManualStatus->setText(noKeySelectedTrans, sendNotification);

    mapStyleBox->setVisible(false);
    mapStyleLbl->setVisible(false);
    mapCopyToManualBtn->setVisible(false);

    mapManualTip->setVisible(true);
    mapManualStatus->setVisible(true);
    mapManualResetBtn->setVisible(true);

    manualMappingHelper = pluginState->getManualMappingHelper();
    manualMappingHelper->addListener(this);

    keyboard->setUIMode(VirtualKeyboard::UIMode::mapMode);

    // Selects the key to map a MIDI note to
    keyboard->setMappingHelper(manualMappingHelper);
}

void PluginControlComponent::setMode1Root(int rootIn, NotificationType notify)
{
    mode1RootSld->setValue(rootIn, notify);
    mode1RootLbl->setText(MidiMessage::getMidiNoteName(rootIn, true, true, 4), dontSendNotification);
}

void PluginControlComponent::setMode2Root(int rootIn, NotificationType notify)
{
    mode2RootSld->setValue(rootIn, notify);
    mode2RootLbl->setText(MidiMessage::getMidiNoteName(rootIn, true, true, 4), dontSendNotification);
}

bool PluginControlComponent::browseForModeToOpen()
{
    ValueTree modeNode = presetManager->nodeFromFile("Open Mode", "*.svk", pluginState->getPluginSettings()->getModePath());

    if (Mode::isValidMode(modeNode))
    {
        presetManager->addSlotAndSetSelection(getModeSelectorViewed(), modeNode);
        onModeViewedChange(pluginState->getModeViewed());
        return true;
    }

    return false;
}

bool PluginControlComponent::browseForPresetToOpen()
{
    ValueTree presetLoaded = presetManager->presetFromFile(pluginState->getPluginSettings()->getPresetPath());
    pluginState->recallState(presetLoaded);
    return true;
}

bool PluginControlComponent::exportModeViewedForReaper()
{
    ReaperWriter rpp = ReaperWriter(pluginState->getModeViewed());
    return rpp.write();
}

bool PluginControlComponent::exportModeViewedForAbleton()
{
    AbletonMidiWriter amw(*pluginState->getModeViewed());
    return amw.write();
}

void PluginControlComponent::exportKbmMapping()
{
    DialogWindow::LaunchOptions options;
    options.dialogTitle = "Export KBM file";
    options.escapeKeyTriggersCloseButton = true;
    options.componentToCentreAround = this;
    options.content = OptionalScopedPointer<Component>(new ExportKbmDialog(
        pluginState->getMappingNode(),
        *pluginState->getMode1(), *pluginState->getMode2(), *pluginState->getModeMapper()), true
        );
    options.content->setSize(316, 112);

    if (JUCEApplication::isStandaloneApp())
        options.runModal();
    else
        options.launchAsync();
}