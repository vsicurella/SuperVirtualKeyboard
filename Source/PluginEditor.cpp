/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

using namespace VirtualKeyboard;

namespace
{
    // A non-interactive, bold popup-menu section header with an icon. Behaves like
    // PopupMenu::addSectionHeader (CustomComponent(false) -> can't be triggered or
    // highlighted). The text is drawn to match the normal menu items exactly (same
    // row height, effective text size and left indent, just bolded), and the icon
    // sits in the tick/icon gutter items reserve, so text stays aligned/item-sized.
    class MenuSectionHeader : public PopupMenu::CustomComponent
    {
    public:
        MenuSectionHeader(String labelIn, std::function<void(Graphics&, Rectangle<int>)> drawIconIn)
            : PopupMenu::CustomComponent(false),
              label(std::move(labelIn)),
              drawIcon(std::move(drawIconIn)) {}

        void getIdealSize(int& idealWidth, int& idealHeight) override
        {
            auto f = getLookAndFeel().getPopupMenuFont();
            idealHeight = roundToInt(f.getHeight() * 1.3f); // == item row height
            idealWidth = roundToInt(f.getHeight() * 1.3f) + f.boldened().getStringWidth(label) + 12;
        }

        void paint(Graphics& g) override
        {
            // Item text sizing/indent, copied from LookAndFeel_V4::drawPopupMenuItem:
            // the row is reduced by 1px, then the font is capped to that height / 1.3
            // and a gutter the width of that height is reserved for the tick/icon.
            auto r = getLocalBounds().reduced(1);
            auto maxFontHeight = (float) r.getHeight() / 1.3f;

            auto f = getLookAndFeel().getPopupMenuFont();
            if (f.getHeight() > maxFontHeight)
                f.setHeight(maxFontHeight);

            r.reduce(jmin(5, getWidth() / 20), 0);

            // Icon stays in the normal tick/icon gutter at the left; the margin is
            // added AFTER it (before the label). Widening the gutter instead would
            // recentre the icon and shove the whole row to the right.
            auto gutter = r.removeFromLeft(roundToInt(maxFontHeight));
            if (drawIcon)
                drawIcon(g, gutter.withSizeKeepingCentre(roundToInt(maxFontHeight), roundToInt(maxFontHeight)));

            r.removeFromLeft(roundToInt(maxFontHeight * 0.5f)); // gap between icon and label
            r.removeFromRight(3);
            g.setColour(Colours::whitesmoke);
            g.setFont(f.boldened());
            g.drawFittedText(label, r, Justification::centredLeft, 1);
        }

    private:
        String label;
        std::function<void(Graphics&, Rectangle<int>)> drawIcon;
    };

    // The minimal-view exit button: same as a normal TextButton but with an almost
    // square corner (the default LookAndFeel_V4 radius is 6px). Mirrors
    // LookAndFeel_V4::drawButtonBackground with a smaller cornerSize.
    class MinimalViewExitButton : public TextButton
    {
    public:
        using TextButton::TextButton;

        void paintButton(Graphics& g, bool shouldDrawHighlighted, bool shouldDrawDown) override
        {
            auto cornerSize = 1.5f;
            auto bounds = getLocalBounds().toFloat().reduced(0.5f, 0.5f);

            auto baseColour = findColour(TextButton::buttonColourId)
                                  .withMultipliedSaturation(hasKeyboardFocus(true) ? 1.3f : 0.9f)
                                  .withMultipliedAlpha(isEnabled() ? 1.0f : 0.5f);

            if (shouldDrawDown || shouldDrawHighlighted)
                baseColour = baseColour.contrasting(shouldDrawDown ? 0.2f : 0.05f);

            g.setColour(baseColour);
            g.fillRoundedRectangle(bounds, cornerSize);

            g.setColour(findColour(ComboBox::outlineColourId));
            g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
        }
    };
}

//==============================================================================
SvkPluginEditor::SvkPluginEditor(SvkAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p), svkState(processor.getState())
{
    setName("SuperVirtualKeyboard");
    setResizable(true, true);
    setBroughtToFrontOnMouseClick(true);

    setMouseClickGrabsKeyboardFocus(true);
    addMouseListener(this, true);
    
    //auto pluginEditorNode = processor.buildStateValueTree().getChildWithName(SvkProperty::pluginEditorNode);
    auto pluginEditorNode = svkState.getPresetNode().getChildWithName(SvkProperty::pluginEditorNode);
    // TODO probably should be more of a state node than preset node

    // Intialization
    if (!pluginEditorNode.isValid())
    {
        pluginEditorNode = ValueTree(SvkProperty::pluginEditorNode);
        pluginEditorNode.setProperty(SvkProperty::windowBoundsW, 1000, nullptr);
        pluginEditorNode.setProperty(SvkProperty::windowBoundsH, 210, nullptr);
        pluginEditorNode.setProperty(SvkProperty::viewportPosition, 60, nullptr);
        pluginEditorNode.setProperty(SvkProperty::settingsOpen, false, nullptr);
    }

    scaleTextBox.reset (new TextEditor ("Scale Text Box"));
    addAndMakeVisible (scaleTextBox.get());
    scaleTextBox->setMultiLine (false);
    scaleTextBox->setReturnKeyStartsNewLine (false);
    scaleTextBox->setReadOnly (false);
    scaleTextBox->setScrollbarsShown (true);
    scaleTextBox->setCaretVisible (true);
    scaleTextBox->setPopupMenuEnabled (true);
    scaleTextBox->setText (String());

    mode1Box.reset (new ComboBox ("Mode1 Box"));
    addAndMakeVisible (mode1Box.get());
    mode1Box->setEditableText (false);
    mode1Box->setJustificationType (Justification::centredLeft);
    mode1Box->setTextWhenNothingSelected (String());
    mode1Box->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode1Box->setTooltip(inputModeTooltip);
    mode1Box->addListener (this);

    mode2Box.reset (new ComboBox ("Mode2 Box"));
    addAndMakeVisible (mode2Box.get());
    mode2Box->setEditableText (false);
    mode2Box->setJustificationType (Justification::centredLeft);
    mode2Box->setTextWhenNothingSelected (String());
    mode2Box->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    mode2Box->setTooltip(outputModeTooltip);
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

    mode1ViewBtn.reset (new ToggleButton ("Mode1 View Button"));
    addChildComponent (mode1ViewBtn.get());
    mode1ViewBtn->setButtonText (String());
    mode1ViewBtn->setRadioGroupId (10);
    mode1ViewBtn->addListener (this);
    mode1ViewBtn->setToggleState (true, dontSendNotification);

    mode2ViewBtn.reset (new ToggleButton ("Mode2 View Button"));
    addChildComponent (mode2ViewBtn.get());
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
    mapStyleBox->addItem (TRANS("By Key Layers"), 3);
    mapStyleBox->setTooltip(mapStyleTooltip);
    mapStyleBox->addListener (this);

    mapStyleLbl.reset (new Label ("Mapping Style Label",
                                  TRANS("Mapping Style:")));
    addAndMakeVisible (mapStyleLbl.get());
    mapStyleLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    mapStyleLbl->setJustificationType (Justification::centredLeft);
    mapStyleLbl->setEditable (false, false, false);
    mapStyleLbl->setColour (TextEditor::textColourId, Colours::black);
    mapStyleLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    mapOrderEditBtn.reset (new TextButton ("Map Order Edit Button", TRANS("Set layer mappings")));
    addChildComponent(mapOrderEditBtn.get());
    mapOrderEditBtn->setButtonText (TRANS("Layers"));
    mapOrderEditBtn->addListener (this);

    mapModeBox.reset (new ComboBox ("Mapping Mode Box"));
    addAndMakeVisible(mapModeBox.get());
    mapModeBox->setEditableText (false);
    mapModeBox->setJustificationType (Justification::centredLeft);
    mapModeBox->setTextWhenNothingSelected (TRANS("Mapping Off"));
    mapModeBox->setTextWhenNoChoicesAvailable (TRANS("Mapping Off"));
    mapModeBox->addItem (TRANS("Mapping Off"), 1);
    mapModeBox->addItem (TRANS("Auto Map"), 2);
    mapModeBox->addItem (TRANS("Manual Map"), 3);
    mapModeBox->setTooltip(mapModeTooltip);
    mapModeBox->addListener (this);

    mapApplyBtn.reset (new TextButton ("Apply Layer Map Button"));
    addChildComponent (mapApplyBtn.get());
    mapApplyBtn->setButtonText (TRANS("Apply"));
    mapApplyBtn->addListener (this);

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
    keyboard->addListener(processor.getMidiProcessor());

    keyboardViewport.reset (new KeyboardViewport(keyboard.get(), "Keyboard Viewport"));
    addAndMakeVisible (keyboardViewport.get());    
    viewportScrollBar = &keyboardViewport->getHorizontalScrollBar();

    menuButton.reset (new ImageButton ("Menu Button"));
    addAndMakeVisible (menuButton.get());
    menuButton->setButtonText (TRANS("Menu"));
    menuButton->addListener (this);
    menuButton->setImages (false, true, true,
                           Image(), 1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000));
    menuButton->setSize(28, 28);

    settingsButton.reset (new ImageButton ("Settings Button"));
    addAndMakeVisible (settingsButton.get());
    settingsButton->setButtonText (TRANS("Settings"));
    settingsButton->addListener (this);
    settingsButton->setImages (false, true, true,
                               Image(), 1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000));
    settingsButton->setSize(28, 28);

    // Thin, empty button shown only in minimal view, sitting where the menu
    // button normally is; clicking it leaves minimal view.
    minimalViewExitBtn.reset (new MinimalViewExitButton ("Minimal View Exit Button"));
    addChildComponent (minimalViewExitBtn.get());
    minimalViewExitBtn->setButtonText (String());
    minimalViewExitBtn->setTooltip (TRANS("Exit Minimal View"));
    minimalViewExitBtn->addListener (this);

    menuIcon.reset(new Image(Image::PixelFormat::RGB, menuButton->getWidth(), menuButton->getHeight(), true));
    menuButton->setImages(true, true, true, *menuIcon.get(), 0.0f, Colour(), *menuIcon.get(), 0.0f, Colours::white.withAlpha(0.25f), *menuIcon.get(), 0.0f, Colours::white.withAlpha(0.5f));

    
    scaleTextBox->addListener(this);
    //scaleTextBox->setInputFilter(&txtFilter, false);

    //keyboardViewport->setScrollingMode(3);

    // allows for implementing mouseDown() to update the menus
    //mode1Box->setInterceptsMouseClicks(false, false);
    //mode2Box->setInterceptsMouseClicks(false, false);


    int recallWidth = pluginEditorNode[SvkProperty::windowBoundsW];
    int recallHeight = pluginEditorNode[SvkProperty::windowBoundsH];
    centerKeyPos = pluginEditorNode[SvkProperty::viewportPosition];

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

    svkState.addPresetListener(this);
    processor.addPresetManagerListener(this);
    modeLibraryUpdated(processor.getPresetManager()->getModeMenu());

    loadPreset(svkState);

    viewportScrollBar->addListener(this);

    // Update keyboard to show external midi input
#if (!JUCE_ANDROID && !JUCE_IOS)
    startTimerHz(30);
#endif

    if (pluginEditorNode[SvkProperty::settingsOpen])
        showSettingsDialog();

    int width = (int)pluginEditorNode[SvkProperty::windowBoundsW];
    int height = (int)pluginEditorNode[SvkProperty::windowBoundsH];

    setSize(width, height);
    setResizeLimits(defaultMinWidth, defaultMinHeight, defaultMaxWidth, defaultMaxHeight);
}

SvkPluginEditor::~SvkPluginEditor()
{
    svkState.removePresetListener(this);
    processor.removePresetManagerListener(this);
    minimalViewExitBtn = nullptr;
    settingsButton = nullptr;
    menuButton = nullptr;
    keyboardViewport = nullptr;
    keyboard = nullptr;
    mapCopyToManualBtn = nullptr;
    mapManualResetBtn = nullptr;
    mapManualCancel = nullptr;
    mapManualStatus = nullptr;
    mapManualTip = nullptr;
    mapApplyBtn = nullptr;
    mapModeBox = nullptr;
    mapOrderEditBtn = nullptr;
    mapStyleLbl = nullptr;
    mapStyleBox = nullptr;
    mode2RootLbl = nullptr;
    mode1RootLbl = nullptr;
    mode2ViewBtn = nullptr;
    mode1ViewBtn = nullptr;
    modeInfoButton = nullptr;
    scaleEntryBtn = nullptr;
    mode2RootSld = nullptr;
    mode1RootSld = nullptr;
    mode2Box = nullptr;
    mode1Box = nullptr;
    scaleTextBox = nullptr;

    settingsContainer = nullptr;
    mappingHelper = nullptr;
    settingsIcon = nullptr;
    menuIcon = nullptr;
}

//==============================================================================

void SvkPluginEditor::valueTreePropertyChanged(ValueTree& parent, const Identifier& property)
{
    // if (parent == pluginEditorNode && property == SvkProperty::windowBoundsH) ***************
    // {
    //     VirtualKeyboard::Keyboard* keyboard = controlComponent->getKeyboard();
    //     Viewport* viewport = controlComponent->getViewport();

    //     setResizeLimits(
    //         defaultMinWidth,
    //         getHeight() - keyboard->getHeight() + 100 + viewport->getScrollBarThickness() + 1,
    //         controlComponent->getViewport()->getX() * 2 + keyboard->getWidth(),
    //         defaultMaxHeight
    //     );
    // }
}


// Would like to turn these command descriptions into tooltips...or something

//void SvkPluginEditor::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
//{
//    switch (commandID)
//    {
//    case SvkProperty::CommandSvkProperty::savePresetToFile:
//        result.setInfo("Save Preset", "Save your custom layout to a file.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::saveMode:
//        result.setInfo("Save Mode", "Save the currently viewed mode.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::showSaveMenu:
//        result.setInfo("Show Saving Options", "Save current mode or whole preset.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::loadPreset:
//        result.setInfo("Load Preset", "Load a custom layout from a file.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::loadMode:
//        result.setInfo("Load Mode", "Load only the mode of a preset.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::showLoadMenu:
//        result.setInfo("Show Loading Options", "Load a mode or whole preset.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::exportReaperMap:
//        result.setInfo("Export for Reaper", "Exports the current preset to a MIDI Note Name text file for use in Reaper's piano roll.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::exportAbletonMap:
//        result.setInfo("Export for Ableton", "Exports the mode mapping to a MIDI file for to use in Ableton's piano roll for folding.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::showExportMenu:
//        result.setInfo("Show Export Options", "Shows different ways you can export a mode or preset.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::showSettingsDialog:
//        result.setInfo("Show Settings Dialog", "Change default directories", "Settings", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::commitCustomScale:
//        result.setInfo("Commit custom scale", "Registers the entered scale steps as the current custom scale.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::setMode1:
//        result.setInfo("Set Mode 1", "Loads the mode into the Mode 1 slot.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::setMode2:
//        result.setInfo("Set Mode 2", "Loads the mode into the Mode 2 slot.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::setMode1RootNote:
//        result.setInfo("Set Mode 1 Root", "Applies the selected root note for Mode 1.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::setMode2RootNote:
//        result.setInfo("Set Mode 2 Root", "Applies the selected root note for Mode 2.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::setModeViewed:
//        result.setInfo("Set Mode Viewed", "Shows the mode slot on the keyboard.", "Keyboard", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::showModeInfo:
//        result.setInfo("Show Mode Info", "Shows information regarding the selected Mode.", "Mode", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::setMappingStyle:
//        result.setInfo("Mapping Style", "Choose a mapping style for remapping MIDI notes.", "Midi", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::showMapOrderEdit:
//        result.setInfo("Edit Mappings by Order", "Choose how to map modes with the order mapping method.", "Preset", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::applyMapping:
//        result.setInfo("Apply Mapping", "Map incoming MIDI notes to Mode 2 with the selected mapping style.", "Midi", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::setMappingMode:
//        result.setInfo("Auto Map to Scale", "Remap Midi notes when scale changes", "Midi", 0);
//        break;
//    case SvkProperty::CommandSvkProperty::beginColorEditing:
//        result.setInfo("Change Keyboard Colors", "Allows you to change the default colors for the rows of keys.", "Keyboard", 0);
//        break;
    //case SvkProperty::CommandSvkProperty::setPeriodShift:
    //    result.setInfo("Shift by Mode 2 Period", "Shift the outgoing MIDI notes by the selected number of Mode 2 periods.", "Midi", 0);
    //    break;
    //case SvkProperty::CommandSvkProperty::setMidiChannelOut:
    //    result.setInfo("Set MIDI Channel Out", "Set the outgoing MIDI Notes to the selected MIDI Channel.", "Midi", 0);
    //    break;
    //case SvkProperty::CommandSvkProperty::showMidiNoteNumbers:
    //    result.setInfo("Show Midi Note Numbers", "Shows incoming MIDI notes on Mode 1 and outgoing MIDI Notes on Mode 2.", "Keyboard", 0);
    //    break;
    //case SvkProperty::CommandSvkProperty::setKeyStyle:
    //    result.setInfo("Set Key Style", "Sets the selected style for drawing overlapping degrees between mode degrees.", "Keyboard", 0);
    //    break;
    //case SvkProperty::CommandSvkProperty::setHighlightStyle:
    //    result.setInfo("Set Highlight Style", "Sets the selected style for drawing triggered notes.", "Keyboard", 0);
    //    break;
//    default:
//        break;
//    }
//}

void SvkPluginEditor::loadPreset(SvkState& stateIn)
{
    if (stateIn.getPresetVersion() == SVK_PRESET_VERSION)
    {
        ValueTree presetNode = stateIn.getPresetNode();

        ValueTree properties = presetNode.getChildWithName(SvkProperty::presetProperties);
        if (properties.isValid())
        {
            mode2ViewBtn->setToggleState((int)properties[SvkProperty::modeSelectorViewed], dontSendNotification);
        }

        ValueTree keyboardSettings = presetNode.getChildWithName(SvkProperty::pianoNode);
        if (keyboardSettings.isValid())
            keyboard->restoreNode(keyboardSettings);

        ValueTree mapping = presetNode.getChildWithName(SvkProperty::midiMapNode);
        if (mapping.isValid())
        {
            setMappingMode((MappingMode)(int)mapping[SvkProperty::mappingMode]);
            setMappingStyleId((MappingStyle)(int)mapping[SvkProperty::autoMappingStyle]);
        }

        ValueTree modeSelectors = presetNode.getChildWithName(SvkProperty::modeSelectorsNode);
        if (modeSelectors.isValid())
        {
            for (int num = 0; num < modeSelectors.getNumChildren(); num++)
            {
                ValueTree selector = modeSelectors.getChild(num);
                Mode* mode = svkState.getModeBySelector(num);

                // TODO: generalize
                if (num == 0)
                {
                    mode1RootSld->setValue(selector[SvkProperty::modeSelectorRootNote], dontSendNotification);
                    mode1Box->setText(mode->getName(), dontSendNotification);
                }
                else if (num == 1)
                {
                    mode2RootSld->setValue(selector[SvkProperty::modeSelectorRootNote], dontSendNotification);
                    mode2Box->setText(mode->getName(), dontSendNotification);
                }

                updateRootNoteLabels();

                if (num == (int)presetNode.getChildWithName(SvkProperty::presetProperties)[SvkProperty::modeSelectorViewed])
                {
                    modeViewedChanged(mode, num, 0 /*unused*/);
                }
            }
        }
    }
}

void SvkPluginEditor::onModeViewedChange(const Mode* modeViewed)
{
    setScaleEntryText(modeViewed->getStepsString());
    keyboard->applyMode(modeViewed, true);
    resized();
}

void SvkPluginEditor::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));

    if (minimalView)
        return;

    Colour buttonFill = getLookAndFeel().findColour(TextButton::ColourIds::buttonColourId);
    Colour buttonOutline = getLookAndFeel().findColour(TextEditor::ColourIds::outlineColourId);

    drawMenuIcon(g, menuButton->getBoundsInParent(), buttonFill, buttonOutline);
    drawSettingsIcon(g, settingsButton->getBoundsInParent(), buttonFill, buttonOutline);
}

void SvkPluginEditor::resized()
{
    int basicHeight = getHeight();

    if (settingsPanelOpen)
    {
        basicHeight -= defaultHeight;
        settingsContainer->setBounds(0, basicHeight, getWidth(), defaultHeight);
        settingsContainer->resized();
    }

    scaleTextBox->setSize(scaleBoxWidth, barHeight);
    scaleTextBox->setCentrePosition(proportionOfWidth(0.5f), barHeight / 2 + gap);
    
    scaleEntryBtn->setBounds(scaleTextBox->getRight() + 8, scaleTextBox->getY(), 31, 24);
    modeInfoButton->setBounds(scaleTextBox->getX() - 32, scaleTextBox->getY(), 24, 24);

    menuButton->setSize(barHeight, barHeight);
    menuButton->setTopLeftPosition(gap, scaleTextBox->getY());

    settingsButton->setSize(barHeight, barHeight);
    settingsButton->setTopLeftPosition(menuButton->getRight() + gap, menuButton->getY());

    mapModeBox->setTopLeftPosition(settingsButton->getRight() + gap, gap);
    mapModeBox->setSize(jmin(mapModeBoxWidth, modeInfoButton->getX() - mapModeBox->getX() - gap), barHeight);

    mode1ViewBtn->setBounds(getWidth() - 32, gap, 32, barHeight);
    mode2ViewBtn->setBounds(getWidth() - 32, mode1ViewBtn->getBottom() + gap, 32, barHeight);

    int keyboardY = barHeight + gap * 2;

    if (inMappingMode)
    {
        int modeBoxW = jlimit(modeBoxWidthMin, modeBoxWidthMax, proportionOfWidth(0.15f));

        // Stop the transpose slider/label group from sliding under the OK button:
        // shrink the box (right edge fixed) so the group clears the button.
        int transposeGroupW = gap + 80 + 32; // gap + slider + note-name label
        int minBoxX = scaleEntryBtn->getRight() + gap + transposeGroupW;
        int boxRight = mode1ViewBtn->getX() - gap / 2;
        if (boxRight - modeBoxW < minBoxX)
            modeBoxW = jmax(60, boxRight - minBoxX);

        mode1Box->setSize(modeBoxW, barHeight);
        mode1Box->setTopLeftPosition(boxRight - modeBoxW, gap);

        mode2Box->setSize(modeBoxW, barHeight);
        mode2Box->setTopLeftPosition(mode1Box->getX(), mode2ViewBtn->getY());

        mapStyleLbl->setTopLeftPosition(gap / 2, scaleTextBox->getBottom() + gap);
        mapStyleLbl->setSize(mapModeBox->getX() - mapStyleLbl->getX(), barHeight);
        mapStyleBox->setBounds(mapStyleLbl->getRight(), mapStyleLbl->getY(), mapStyleBoxWidth, barHeight);

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
        int modeBoxW = jlimit(modeBoxWidthMin, modeBoxWidthMax, proportionOfWidth(0.15f) + mode2ViewBtn->getWidth());

        // Stop the transpose slider/label group from sliding under the OK button:
        // shrink the box (right edge fixed) so the group clears the button.
        int transposeGroupW = gap + 80 + 32; // gap + slider + note-name label
        int minBoxX = scaleEntryBtn->getRight() + gap + transposeGroupW;
        int boxRight = getWidth() - gap;
        if (boxRight - modeBoxW < minBoxX)
            modeBoxW = jmax(60, boxRight - minBoxX);

        mode2Box->setSize(modeBoxW, barHeight);
        mode2Box->setTopLeftPosition(boxRight - modeBoxW, gap);
    }

    mode1RootSld->setBounds(mode1Box->getX() - 80 - gap, mode1Box->getY(), 80, barHeight);
    mode2RootSld->setBounds(mode2Box->getX() - 80 - gap, mode2Box->getY(), 80, barHeight);

    mode1RootLbl->setBounds(mode1RootSld->getX() - 32, mode1Box->getY(), 32, barHeight);
    mode2RootLbl->setBounds(mode2RootSld->getX() - 32, mode2Box->getY(), 32, barHeight);

    if (minimalView)
    {
        // Collapse the header into a top margin equal to the bottom margin (gap),
        // and drop the thin exit button into it with its left edge flush with the
        // keyboard's left edge and the menu button's width.
        keyboardY = gap;
        minimalViewExitBtn->setBounds(gap / 3, 0, barHeight, keyboardY);
    }

    viewportScrollBar->removeListener(this);

    // Minimal mode uses thinner horizontal margins and drops the bottom margin so
    // the scroll bar sits flush against the window's bottom edge; normal mode keeps
    // the full margins.
    int keyboardMarginX = minimalView ? gap / 3 : gap;
    int scrollBarThickness = basicHeight / 28;
    int keyboardViewWidth = getWidth() - keyboardMarginX * 2;
    int keyboardMarginBottom = minimalView ? 0 : gap;
    int keyboardViewHeight = jmax(basicHeight - keyboardY - keyboardMarginBottom, 1);

    keyboardViewport->setBounds(keyboardMarginX, keyboardY, keyboardViewWidth, keyboardViewHeight);
    keyboardViewport->setScrollBarThickness(scrollBarThickness);
    keyboardViewport->resizeKeyboard();
    keyboardViewport->centerOnKey((int)centerKeyPos);
    viewportScrollBar->addListener(this);

    // processor.getPluginEditorNode().setProperty(SvkProperty::windowBoundsW, getWidth(), nullptr); ***********
    // processor.getPluginEditorNode().setProperty(SvkProperty::windowBoundsH, basicHeight, nullptr);
}

void SvkPluginEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == mode1Box.get())
    {
        processor.handleModeSelection(0, mode1Box->getSelectedId());
    }
    else if (comboBoxThatHasChanged == mode2Box.get())
    {
        processor.handleModeSelection(1, mode2Box->getSelectedId());
    }
    else if (comboBoxThatHasChanged == mapModeBox.get())
    {
        auto mode = (MappingMode)(mapModeBox->getSelectedId() - 1);
        setMappingMode(mode);
        processor.setMapMode(mode);
    }
    else if (comboBoxThatHasChanged == mapStyleBox.get())
    {
        auto style = (MappingStyle)mapStyleBox->getSelectedId();
        setMappingStyleId(style);
        processor.setAutoMapStyle(style);
    }
}

void SvkPluginEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == mode1RootSld.get())
    {
        processor.setModeSelectorRoot(0, mode1RootSld->getValue());
        updateRootNoteLabels();
    }
    else if (sliderThatWasMoved == mode2RootSld.get())
    {
        processor.setModeSelectorRoot(1, mode2RootSld->getValue());
        updateRootNoteLabels();
    }
}

void SvkPluginEditor::buttonClicked (Button* buttonThatWasClicked)
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
        processor.setModeSelectorViewed(mode2ViewBtn->getToggleState());
        scaleTextBox->setText(processor.getState().getModeViewed()->getStepsString(), false);
    }
    else if (buttonThatWasClicked == mapCopyToManualBtn.get())
    {
        NoteMap autoMap = *processor.getMidiInputFilterMap();
        mapModeBox->setSelectedId(3, sendNotificationSync);
        mappingHelper->resetMapping(autoMap);
    }
    else if (buttonThatWasClicked == mapOrderEditBtn.get())
    {
        showMapOrderEditDialog();
    }
    else if (buttonThatWasClicked == mapApplyBtn.get())
    {
        processor.doAutoMapping();
    }
    else if (buttonThatWasClicked == menuButton.get())
    {
        showMainMenu();
    }
    else if (buttonThatWasClicked == settingsButton.get())
    {
        showSettingsDialog();
    }
    else if (buttonThatWasClicked == minimalViewExitBtn.get())
    {
        setMinimalView(false);
    }
    else if (buttonThatWasClicked == mapManualCancel.get())
    {
        mappingHelper->cancelKeyMap();
    }
    else if (buttonThatWasClicked == mapManualResetBtn.get())
    {
        mappingHelper->resetMapping();
        keyMappingStatusChanged(-1, false);
    }
}

void SvkPluginEditor::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    submitCustomScale();
}

void SvkPluginEditor::scrollBarMoved(ScrollBar* bar, double newRangeStart)
{
    centerKeyPos = keyboardViewport->getCenterKeyProportion();
    //processor.getPluginEditorNode().setProperty(SvkProperty::viewportPosition, centerKeyPos, nullptr);
}

void SvkPluginEditor::changeListenerCallback(ChangeBroadcaster* source)
{
    // Mode Info Changed
    if (source == modeInfo)
    {
        processor.commitModeInfo();
        onModeViewedChange(processor.getState().getModeViewed());
    }
}

void SvkPluginEditor::timerCallback()
{
    keyboard->repaint();
}

//==============================================================================

void SvkPluginEditor::modeLibraryUpdated(const PopupMenu& menu)
{
    // TODO keep selected id

    mode1Box->getRootMenu()->operator=(menu);
    mode2Box->getRootMenu()->operator=(menu);
}

void SvkPluginEditor::presetReloaded(SvkState& stateIn)
{
    DBG("UI LOADING PRESET");
    loadPreset(stateIn);
}

void SvkPluginEditor::keyboardKeyPlacementTypeChanged(VirtualKeyboard::KeyPlacementType placementType)
{
    keyboard->setKeyStyle(placementType);
    keyboard->resized();
}

void SvkPluginEditor::keyboardHighlightStyleChanged(VirtualKeyboard::HighlightStyle highlightStyle)
{
    keyboard->setHighlightStyle(highlightStyle);
}

void SvkPluginEditor::keyboardKeyRatioChanged(float keyRatio)
{
    keyboard->setKeySizeRatio(keyRatio, true);
}

void SvkPluginEditor::keyParametersShown(int keyParameters)
{
    keyboard->setShowNoteNumbers(keyParameters != 0);
    keyboard->repaint();
}

void SvkPluginEditor::modeViewedChanged(const Mode* modeIn, int selectorNumber, int slotNumber)
{
    MidiKeyboardState* displayState = selectorNumber == 0
        ? processor.getMidiProcessor()->getOriginalKeyboardState()
        : processor.getMidiProcessor()->getRemappedKeyboardState();

    keyboard->displayKeyboardState(displayState);
    onModeViewedChange(modeIn);
}

void SvkPluginEditor::inputMappingChanged(const NoteMap* inputNoteMap)
{
    keyboard->setInputNoteMap(inputNoteMap);
}

void SvkPluginEditor::customModeChanged(const Mode* newCustomMode)
{
    // This is a hack and should be handled better

    String customModeName = newCustomMode->getName();

    if (getModeSelectorViewed() == 1)
        mode2Box->setText(customModeName, dontSendNotification);
    else
        mode1Box->setText(customModeName, dontSendNotification);

}

//void SvkPluginEditor::modeInfoChanged(const Mode* modeEdited)
//{
//    if (getModeSelectorViewed() == 1)
//        mode2Box->setText(modeEdited->getName(), dontSendNotification);
//    else
//        mode1Box->setText(modeEdited->getName(), dontSendNotification);
//}

void SvkPluginEditor::settingsTabChanged(int tabIndex, const String& tabName, SvkSettingsPanel* panelChangedTo)
{
    DBG("Settings changed to " + tabName + ", tab: " + String(tabIndex));

    if (panelChangedTo->getName() == "ColourSettingsPanel")
    {
        beginColorEditing(static_cast<ColourSettingsPanel*>(panelChangedTo));
    }
    else if (isColorEditing)
    {
        endColorEditing();
    }
    
    if (tabName == "Mapping")
    {
        mappingSettingsOpen = true;

        MappingSettingsPanel* msp = static_cast<MappingSettingsPanel*>(panelChangedTo);
        if (mapModeBox->getSelectedId() == 3)
        {
            //msp->setEditorToListenTo(mappingHelper.get());
        }

        //msp->listenToEditor(this);
    }

    else if (mappingSettingsOpen)
    {
        mappingSettingsOpen = false;

        // MappingSettingsPanel removes listeners from NoteMapEditor
    }

    //processor.getPluginEditorNode().setProperty(SvkProperty::settingsTabName, tabName, nullptr);
}

void SvkPluginEditor::keyMappingStatusChanged(int keyNumber, bool preparedToMap)
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

void SvkPluginEditor::keyMapConfirmed(int keyNumber, int midiNote)
{
    mapManualStatus->setText("Key " + String(keyNumber) + " mapped to MIDI Note " + String(midiNote), sendNotification);
    mapManualCancel->setVisible(false);
    resized();
}

//void SvkPluginEditor::mappingChanged(NoteMap& newMapping)
//{
//    DBG("MAPPING CHANGED");
//    processor.setMidiInputMap(newMapping, true);
//}

//==============================================================================

VirtualKeyboard::Keyboard* SvkPluginEditor::getKeyboard()
{
    return keyboard.get();
}

Viewport* SvkPluginEditor::getViewport()
{
    return  keyboardViewport.get();
}

TextEditor* SvkPluginEditor::getScaleTextEditor()
{
    return scaleTextBox.get();
}

ComboBox* SvkPluginEditor::getMappingStyleBox()
{
    return mapStyleBox.get();
}

String SvkPluginEditor::getScaleEntryText()
{
    return scaleTextBox->getText();
}

void SvkPluginEditor::setScaleEntryText(String textIn, NotificationType notify)
{
    scaleTextBox->setText(textIn, notify == sendNotification);
}

ComboBox* SvkPluginEditor::getMode1Box()
{
    return mode1Box.get();
}

ComboBox* SvkPluginEditor::getMode2Box()
{
    return mode2Box.get();
}

ImageButton* SvkPluginEditor::getSettingsButton()
{
    return settingsButton.get();
}

TextButton* SvkPluginEditor::getModeInfoButton()
{
    return modeInfoButton.get();
}

int SvkPluginEditor::getModeSelectorViewed()
{
    return mode2ViewBtn->getToggleState();
}

void SvkPluginEditor::setMappingMode(MappingMode mappingModeId, NotificationType notify)
{
    mapModeBox->setSelectedId((int)mappingModeId + 1, notify);
    inMappingMode = mappingModeId > MappingMode::None;
    
    for (auto c : mappingComponents)
    {
        c->setVisible(inMappingMode);
        c->setEnabled(inMappingMode);
    }

    if (inMappingMode)
    {
        if (mappingModeId == MappingMode::Auto && mapStyleBox->getSelectedId() == 3)
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
    }

    if (mappingModeId == MappingMode::Manual)
    {
        beginManualMapping();
    }
    else
    {
        mapManualTip->setVisible(false);
        mapManualStatus->setVisible(false);
        mapManualCancel->setVisible(false);
        mapManualResetBtn->setVisible(false);

        // TODO: make sure this doesn't conflict with color editing
        keyboard->setUIMode(VirtualKeyboard::UIMode::playMode);

        if (mappingHelper.get())
        {
            mappingHelper->removeListener(this);
            mappingHelper = nullptr;
            mapManualStatus->setText(noKeySelectedTrans, sendNotification);
        }
    }

    resized();
}

void SvkPluginEditor::setMappingStyleId(MappingStyle idIn, NotificationType notify)
{
    mapStyleBox->setSelectedId((int)idIn, notify);

    if (idIn == MappingStyle::ModeToMode && mapModeBox->getSelectedId() == (int)MappingMode::Auto + 1)
    {
        mapOrderEditBtn->setVisible(true);
        mapApplyBtn->setVisible(true);
    }
    else
    {
        mapOrderEditBtn->setVisible(false);
        mapApplyBtn->setVisible(false);
    }

    resized();
}

void SvkPluginEditor::submitCustomScale()
{
    processor.setModeCustom(scaleTextBox->getText());
}

void SvkPluginEditor::updateRootNoteLabels()
{
    mode1RootLbl->setText(MidiMessage::getMidiNoteName(mode1RootSld->getValue(), true, true, 4), dontSendNotification);
    mode2RootLbl->setText(MidiMessage::getMidiNoteName(mode2RootSld->getValue(), true, true, 4), dontSendNotification);
}

//==============================================================================

void SvkPluginEditor::showModeInfo()
{
    modeInfo  = new ModeInfoDialog(processor.getState().getModeViewed());
    modeInfo->addChangeListener(this);
    CallOutBox::launchAsynchronously(std::unique_ptr<Component>(modeInfo), scaleTextBox->getScreenBounds(), nullptr);
}

void SvkPluginEditor::showMapOrderEditDialog()
{
    mapByOrderDialog = new MapByOrderDialog(processor.getModeMapper(), processor.getState().getMode1(), processor.getState().getMode2());
    CallOutBox::launchAsynchronously(std::unique_ptr<Component>(mapByOrderDialog), mapStyleBox->getScreenBounds(), nullptr);
}

void SvkPluginEditor::showMainMenu()
{
    PopupMenu menu;

    menu.addCustomItem(-1, std::make_unique<MenuSectionHeader>("Load",
        [](Graphics& g, Rectangle<int> a) { drawLoadIcon(g, a, Colours::transparentBlack, Colours::transparentBlack); }));
    menu.addItem("Mode", [this]() { browseForModeToOpen(); });
    menu.addItem("Preset", [this]() { browseForPresetToOpen(); });

    menu.addCustomItem(-1, std::make_unique<MenuSectionHeader>("Save",
        [](Graphics& g, Rectangle<int> a) { drawSaveIcon(g, a, Colours::transparentBlack, Colours::transparentBlack); }));
    menu.addItem("Mode", [this]() { processor.saveModeViewedToFile(); });
    menu.addItem("Preset", [this]() { processor.savePresetToFile(); });

    menu.addCustomItem(-1, std::make_unique<MenuSectionHeader>("Export",
        [](Graphics& g, Rectangle<int> a) { drawExportIcon(g, a, Colours::transparentBlack, Colours::transparentBlack); }));
    menu.addItem("for Reaper Note Names", [this]() { exportModeViewedForReaper(); });
    menu.addItem("for Ableton Folding", [this]() { exportModeViewedForAbleton(); });

    menu.addSeparator();
    menu.addItem("Minimal View", [this]() { setMinimalView(true); });

    menu.showMenuAsync(PopupMenu::Options().withTargetComponent(menuButton.get()));
}

void SvkPluginEditor::setMinimalView(bool shouldBeMinimal)
{
    if (minimalView == shouldBeMinimal)
        return;

    minimalView = shouldBeMinimal;

    // Every header-bar control except the keyboard viewport and the exit button.
    Array<Component*> headerComps = {
        scaleTextBox.get(), scaleEntryBtn.get(), modeInfoButton.get(),
        menuButton.get(), settingsButton.get(), mapModeBox.get(),
        mode1Box.get(), mode2Box.get(), mode1RootSld.get(), mode2RootSld.get(),
        mode1RootLbl.get(), mode2RootLbl.get(), mode1ViewBtn.get(), mode2ViewBtn.get(),
        mapStyleLbl.get(), mapStyleBox.get(), mapOrderEditBtn.get(), mapApplyBtn.get(),
        mapManualTip.get(), mapManualStatus.get(), mapManualCancel.get(),
        mapManualResetBtn.get(), mapCopyToManualBtn.get()
    };

    if (minimalView)
    {
        minimalViewRestoreVisible.clearQuick();
        for (auto* c : headerComps)
        {
            minimalViewRestoreVisible.add(c->isVisible());
            c->setVisible(false);
        }
        minimalViewExitBtn->setVisible(true);
    }
    else
    {
        for (int i = 0; i < headerComps.size(); ++i)
            headerComps[i]->setVisible(minimalViewRestoreVisible[i]);
        minimalViewExitBtn->setVisible(false);
    }

    // Hide the resize grip in minimal view so it can't overlap the keys.
    if (resizableCorner != nullptr)
        resizableCorner->setVisible(!minimalView);

    resized();
    repaint();
}

void SvkPluginEditor::showSettingsDialog()
{
    if (!settingsPanelOpen)
    {
        auto stateNode = processor.buildStateValueTree();
        settingsContainer.reset(new SettingsContainer(*processor.getPluginSettings(), svkState));
        //settingsContainer->setKeyboardPointer(keyboard.get());
        settingsContainer->addListener(this);
        svkState.addPresetListener(settingsContainer.get());
        //processor.addPresetManagerListener(settingsContainer.get());

        settingsPanelOpen = true;
        //processor.getPluginEditorNode().setProperty(SvkProperty::settingsOpen, true, nullptr);
        settingsButton->setToggleState(true, dontSendNotification);

        addAndMakeVisible(settingsContainer.get());
        // Resize the editor itself, not the parent. In standalone the parent is
        // the real window, but when hosted the parent is a host-owned wrapper the
        // DAW doesn't track resizing the editor is what notifies the host.
        setSize(getWidth(), getHeight() + defaultHeight);
        // Keep the panel from being squished if the window is shrunk while open.
        setResizeLimits(defaultMinWidth, defaultMinHeight + defaultHeight, defaultMaxWidth, defaultMaxHeight);

        int setToTab = 0;
        String lastTab = stateNode.getChildWithName(SvkProperty::pluginEditorNode)[SvkProperty::settingsTabName].toString();//processor.getPluginEditorNode()[SvkProperty::settingsTabName].toString();
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

void SvkPluginEditor::hideSettings()
{
    settingsContainer->removeListener(this);
    settingsContainer->setVisible(false);
    //preset.removePresetManagerListener(settingsContainer.get());
    svkState.removePresetListener(settingsContainer.get());

    settingsContainer = nullptr;

    settingsPanelOpen = false;
    //processor.getPluginEditorNode().setProperty(SvkProperty::settingsOpen, false, nullptr);

    if (isColorEditing)
        endColorEditing();

    // Restore the min height before shrinking, otherwise the constrainer clamps
    // the editor back up to the settings-open minimum.
    setResizeLimits(defaultMinWidth, defaultMinHeight, defaultMaxWidth, defaultMaxHeight);
    setSize(getWidth(), getHeight() - defaultHeight);
}

void SvkPluginEditor::beginColorEditing(ColourSettingsPanel* panel)
{
    keyboard->setUIMode(VirtualKeyboard::UIMode::editMode);
    isColorEditing = true;
    colourPanel = panel;
    if (colourPanel)
        colourPanel->setKeyboardPointer(keyboard.get());
    
    keyMappingStatusChanged(-1, false);
    mapModeBox->setEnabled(false);
    mapStyleBox->setEnabled(false);
    mapManualResetBtn->setEnabled(false);
    mapOrderEditBtn->setEnabled(false);
    mapApplyBtn->setEnabled(false);
    mapCopyToManualBtn->setEnabled(false);
}

void SvkPluginEditor::endColorEditing()
{
    if (colourPanel)
    {
        colourPanel->setKeyboardPointer(nullptr);
        colourPanel = nullptr;
    }
    keyboard->setUIMode(VirtualKeyboard::UIMode::playMode);
    isColorEditing = false;

    mapModeBox->setEnabled(true);
    mapStyleBox->setEnabled(true);
    mapManualResetBtn->setEnabled(true);
    mapOrderEditBtn->setEnabled(true);
    mapApplyBtn->setEnabled(true);
    mapCopyToManualBtn->setEnabled(true);

    setMappingMode(MappingMode(mapModeBox->getSelectedId() - 1));
}

void SvkPluginEditor::beginManualMapping()
{
    if (mappingHelper.get())
    {
        mappingHelper->removeListener(this);
        mapManualStatus->setText(noKeySelectedTrans, sendNotification);
    }

    mapStyleBox->setVisible(false);
    mapStyleLbl->setVisible(false);
    mapCopyToManualBtn->setVisible(false);

    mapManualTip->setVisible(true);
    mapManualStatus->setVisible(true);
    mapManualResetBtn->setVisible(true);

    mappingHelper.reset(new MappingHelper(*processor.getMidiInputFilterMap()));
    mappingHelper->addListener(this);

    keyboard->setUIMode(VirtualKeyboard::UIMode::mapMode);

    // Selects the key to map a MIDI note to
    keyboard->setMappingHelper(mappingHelper.get());

    // Selects the MIDI note that maps to the selected key
    processor.getMidiProcessor()->setMappingHelper(mappingHelper.get());

    // Updates MappingSettingsPanel with changes
    if (settingsContainer && settingsContainer->getCurrentTabName() == "Mapping")
    {
        //static_cast<MappingSettingsPanel*>(settingsContainer->getCurrentContentComponent())->setEditorToListenTo(mappingHelper.get());
    }
}

void SvkPluginEditor::setMode1Root(int rootIn, NotificationType notify)
{
    mode1RootSld->setValue(rootIn, notify);
    mode1RootLbl->setText(MidiMessage::getMidiNoteName(rootIn, true, true, 4), dontSendNotification);
}

void SvkPluginEditor::setMode2Root(int rootIn, NotificationType notify)
{
    mode2RootSld->setValue(rootIn, notify);
    mode2RootLbl->setText(MidiMessage::getMidiNoteName(rootIn, true, true, 4), dontSendNotification);
}

bool SvkPluginEditor::browseForModeToOpen()
{
    return processor.loadModeFromFile();
}

bool SvkPluginEditor::browseForPresetToOpen()
{
    return processor.loadPresetFromFile();
}

bool SvkPluginEditor::exportModeViewedForReaper()
{
    ReaperWriter rpp(processor.getState().getModeViewed());
    return true;
}

bool SvkPluginEditor::exportModeViewedForAbleton()
{
    AbletonMidiWriter amw(processor.getState().getModeViewed());
    return true;
}
