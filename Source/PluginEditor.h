/*  
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "./PluginProcessor.h"
#include "./Structures/MappingHelper.h"

#include "./UI/VectorResources.h"

#include "./UI/Components/VirtualKeyboard/KeyboardComponent.h"
#include "./UI/Components/VirtualKeyboard/KeyboardViewport.h"

#include "./UI/Components/Settings/SettingsContainer.h"

#include "./UI/Dialogs/ModeInfoDialog.h"
#include "./UI/Dialogs/MapByOrderDialog.h"
#include "./UI/Dialogs/AboutDialog.h"
#include "./UI/Dialogs/ExportKbmDialog.h"

#include "./IO/ReaperWriter.h"
#include "./IO/AbletonMidiWriter.h"

class SvkPluginEditor : public AudioProcessorEditor, 
                        protected ValueTree::Listener,
                        public TextEditor::Listener,
                        public ComboBox::Listener,
                        public Slider::Listener,
                        public Button::Listener,
                        public ScrollBar::Listener,
                        public ChangeListener,
                        private SvkPresetManager::Listener,
                        private SvkState::Listener,
                        private SettingsContainer::Listener,
                        private MappingHelper::Listener,
                        private Timer
{
public:
    SvkPluginEditor (SvkAudioProcessor&);
    ~SvkPluginEditor();

    void paint (Graphics&) override;
    void resized() override;

    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged(Slider* sliderThatWasMoved) override;
    void buttonClicked(Button* buttonThatWasClicked) override;

    void textEditorTextChanged(TextEditor& textEditor) override {};
    void textEditorEscapeKeyPressed(TextEditor& textEditor) override {};
    void textEditorReturnKeyPressed(TextEditor& textEditor) override;
    void textEditorFocusLost(TextEditor& textEditor) override {};

    void scrollBarMoved(ScrollBar* bar, double newRangeStart) override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void timerCallback() override;

    //==============================================================================
    // SvkPresetManager::Listener implementation
    
    void modeLibraryUpdated(const PopupMenu& menu) override;

    //==============================================================================
    // SvkPreset::Listener implementation
    
    void presetReloaded(SvkState& stateIn) override;

    void modeViewedChanged(const Mode* modeIn, int selectorNumber, int slotNumber) override;

    void keyboardKeyPlacementTypeChanged(VirtualKeyboard::KeyPlacementType placementType) override;
    void keyboardHighlightStyleChanged(VirtualKeyboard::HighlightStyle highlightStyle) override;
    void keyboardKeyRatioChanged(float keyRatio) override;
    void keyParametersShown(int keyParameters) override;

    void inputMappingChanged(const NoteMap* inputNoteMap) override;

    void customModeChanged(const Mode* newCustomMode) override;

    //void modeInfoChanged(const Mode* modeEdited) override;

    void settingsTabChanged(int tabIndex, const String& tabName, SvkSettingsPanel* panelChangedTo) override;

    void keyMappingStatusChanged(int keyNumber, bool preparedToMap) override;

    void keyMapConfirmed(int keyNumber, int midiNote) override;

    //void mappingChanged(NoteMap&) override;

    //==============================================================================

    String getScaleEntryText();
    void setScaleEntryText(String textIn, NotificationType notify = NotificationType::dontSendNotification);

    VirtualKeyboard::Keyboard* getKeyboard();

    TextEditor* getScaleTextEditor();
    Viewport* getViewport();
    ComboBox* getMappingStyleBox();

    ComboBox* getMode1Box();
    ComboBox* getMode2Box();

    ImageButton* getSettingsButton();
    TextButton* getModeInfoButton();

    //==============================================================================

    /*
        Sets controls to plugin state
    */
    void loadPreset(SvkState& stateIn);

    /*
        Updates UI to the new mode
    */
    void onModeViewedChange(const Mode* modeViewed);

    int getModeSelectorViewed();

    void setMappingMode(MappingMode mapModeIn, NotificationType notify = NotificationType::dontSendNotification);

    void setMappingStyleId(MappingStyle idIn, NotificationType notify = NotificationType::dontSendNotification);

    void setMode1Root(int rootIn, NotificationType notify = NotificationType::dontSendNotification);

    void setMode2Root(int rootIn, NotificationType notify = NotificationType::dontSendNotification);

    void submitCustomScale();

    void updateRootNoteLabels();

    //==============================================================================

    void showModeInfo();

    void showMapOrderEditDialog();

    void showExportKbmDialog();

    void showMainMenu();

    void showAboutDialog();

    void showSettingsDialog();

    void hideSettings();

    /*
        Collapses the header bar into a thin top margin (matching the bottom margin)
        and hides the resize grip, maximising the keyboard area. Leave via the thin
        button placed where the menu button normally sits.
    */
    void setMinimalView(bool shouldBeMinimal);

    void beginColorEditing(ColourSettingsPanel* panel = nullptr);

    void endColorEditing();

    void beginManualMapping();

    //==============================================================================

    bool browseForModeToOpen();

    bool browseForPresetToOpen();

    bool exportModeViewedForReaper();

    bool exportModeViewedForAbleton();

protected:

    void valueTreePropertyChanged(ValueTree&, const Identifier&) override;


private:
    SvkAudioProcessor& processor;
    SvkState& svkState;

    std::unique_ptr<Image> menuIcon;
    std::unique_ptr<Image> settingsIcon;

    std::unique_ptr<MappingHelper> mappingHelper;

    std::unique_ptr<SettingsContainer> settingsContainer;

    ScrollBar* viewportScrollBar;

    ModeInfoDialog* modeInfo;
    MapByOrderDialog* mapByOrderDialog;
    ExportKbmDialog* exportKbmDialog;

    // Held as members so they outlive the async file choosers they launch.
    std::unique_ptr<ReaperWriter> reaperWriter;
    std::unique_ptr<AbletonMidiWriter> abletonWriter;

    Array<Component*> mappingComponents;

    //TextFilterIntOrSpace txtFilter;

    bool settingsPanelOpen = false;
    bool isColorEditing = false;
    ColourSettingsPanel* colourPanel = nullptr;
    bool inMappingMode = false;
    bool mappingSettingsOpen = false;

    bool minimalView = false;
    // Visibility of the collapsed header controls at the moment minimal view was
    // entered, so leaving it restores exactly the prior state (mapping controls
    // depend on the current mapping mode).
    Array<bool> minimalViewRestoreVisible;

    String noKeySelectedTrans = TRANS("No key selected.");
    String waitingForTrans = TRANS("Waiting for input to map to key ");

    String inputModeTooltip = TRANS("The input scale that represents your physical keyboard or DAW piano roll.");
    String outputModeTooltip = TRANS("The output scale that you want to play or reference.");

    String mapModeTooltip = TRANS(
        "Maps MIDI note input to notes in the selected scale (Mode 2).\n"
        "Auto Map: automatically maps notes based on Mode 1 and Mode 2.\n"
        "Manual: create a custom mapping by clicking notes on the virtual keyboard."
    );

    String mapStyleTooltip = TRANS(
        "The way notes are mapped from the input to output.\n"
        "Mode To Mode: natural notes from Mode 1 are mapped to natural Notes in Mode 2.\n"
        "Scale To Mode: each note from Mode 1 is mapped to natural notes in Mode 2.\n"
        "by Layer: Notes from a layer in Mode 1 are mapped to a layer in Mode 2."
    );

    float centerKeyPos = 60.5f;

    int defaultHeight = 210;
    int barHeight = 24;
    int gap = 8;

    //==============================================================================
    std::unique_ptr<TextEditor> scaleTextBox;
    std::unique_ptr<ComboBox> mode1Box;
    std::unique_ptr<ComboBox> mode2Box;
    std::unique_ptr<Slider> mode1RootSld;
    std::unique_ptr<Slider> mode2RootSld;
    std::unique_ptr<TextButton> scaleEntryBtn;
    std::unique_ptr<TextButton> modeInfoButton;
    std::unique_ptr<ToggleButton> mode1ViewBtn;
    std::unique_ptr<ToggleButton> mode2ViewBtn;
    std::unique_ptr<Label> mode1RootLbl;
    std::unique_ptr<Label> mode2RootLbl;
    std::unique_ptr<ComboBox> mapStyleBox;
    std::unique_ptr<Label> mapStyleLbl;
    std::unique_ptr<TextButton> mapOrderEditBtn;
    std::unique_ptr<ComboBox> mapModeBox;
    std::unique_ptr<TextButton> mapApplyBtn;
    std::unique_ptr<Label> mapManualTip;
    std::unique_ptr<Label> mapManualStatus;
    std::unique_ptr<TextButton> mapManualCancel;
    std::unique_ptr<TextButton> mapManualResetBtn;
    std::unique_ptr<TextButton> mapCopyToManualBtn;
    std::unique_ptr<VirtualKeyboard::Keyboard> keyboard;
    std::unique_ptr<KeyboardViewport> keyboardViewport;
    std::unique_ptr<ImageButton> menuButton;
    std::unique_ptr<ImageButton> settingsButton;
    std::unique_ptr<TextButton> minimalViewExitBtn;


    //==============================================================================
    // = 2*modeBoxWidthMin + scaleBoxWidth + 350, so the transpose slider always
    // clears the centered OK button at the minimum window width.
    int defaultMinWidth = 880;
    int defaultMinHeight = 172;
    int defaultMaxWidth = 10e4;
    int defaultMaxHeight = 10e4;

    // Fixed toolbar control widths so these controls size to their content instead
    // of scaling with the window. Values in px, tuned for the default ~15-16pt UI
    // font plus a ~35px combo arrow; adjust here if the font changes.
    int scaleBoxWidth = 260;    // fits >= 19 single-digit steps plus separators
    int mapModeBoxWidth = 125;  // fits "Mapping Off"
    int mapStyleBoxWidth = 145; // fits "Scale To Mode"
    int modeBoxWidthMin = 135;  // ~ an average-length mode title
    int modeBoxWidthMax = 240;  // ~ 1.5x the longest default mode title

    TooltipWindow tooltip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SvkPluginEditor)
};
