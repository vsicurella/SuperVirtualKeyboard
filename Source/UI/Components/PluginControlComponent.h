/*
  ==============================================================================

    PluginControlComponent.h
    Created: 8 Jul 2019
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "../../PluginState.h"
#include "../VectorResources.h"
#include "ReferencedComboBox.h"
#include "VirtualKeyboard/KeyboardComponent.h"
#include "VirtualKeyboard/KeyboardViewport.h"
#include "../../Structures/MappingHelper.h"

#include "Settings/SettingsContainer.h"

#include "../Dialogs/ModeInfoDialog.h"
#include "../Dialogs/MapByOrderDialog.h"
#include "../Dialogs/ExportKbmDialog.h"

#include "../../File IO/ReaperWriter.h"
#include "../../File IO/AbletonMidiWriter.h"
#include "../../File IO/KbmWriter.h"

class PluginControlComponent  : public Component,
                                public TextEditor::Listener,
                                public ComboBox::Listener,
                                public Slider::Listener,
                                public Button::Listener,
                                public ScrollBar::Listener,
                                public ChangeListener,
                                private SvkPluginState::Listener,
                                private SettingsContainer::Listener,
                                private MappingHelper::Listener,
                                private Timer
{
public:

    enum SaveMenuOptions
    {
        SaveMode = 1,
        SavePreset,
        ExportKBM,
        ExportReaper,
        ExportAbleton
    };

    enum LoadMenuOptions
    {
        LoadMode = 1,
        LoadPreset,
        LoadKBM
    };

public:
    //==============================================================================
    PluginControlComponent (SvkPluginState* pluginStateIn);

    ~PluginControlComponent();

    //==============================================================================

    void paint(Graphics& g) override;
    void resized() override;

    void mouseDown(const MouseEvent& e) override;

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

    void resyncPresetNode(ValueTree presetNode) override;

    void modeViewedChanged(Mode* modeIn, int selectorNumber, int slotNumber) override;

    void mappingModeChanged(int mappingModeId) override;

    void inputMappingChanged(NoteMap& inputNoteMap) override;

    void customModeChanged(Mode* newCustomMode) override;

    void modeInfoChanged(Mode* modeEdited) override;

    void settingsTabChanged(int tabIndex, const String& tabName, SvkSettingsPanel* panelChangedTo) override;

    void keyMappingStatusChanged(int keyNumber, bool preparedToMap) override;

    void keyMapConfirmed(int keyNumber, int midiNote) override;

    void mappingEditorChanged(NoteMap&) override;

    //==============================================================================

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

    //==============================================================================

    /*
        Sets controls to plugin state
    */
    void loadPresetNode(ValueTree presetNodeIn);

    /*
        Updates UI to the new mode
    */
    void onModeViewedChange(Mode* modeViewed);

    int getModeSelectorViewed();

    void setMapModeBoxId(int mappingModeId, NotificationType notify = NotificationType::sendNotification);

    void setMappingStyleId(int idIn, NotificationType notify = NotificationType::sendNotification);

    void setMode1Root(int rootIn, NotificationType notify = NotificationType::dontSendNotification);

    void setMode2Root(int rootIn, NotificationType notify = NotificationType::dontSendNotification);

    void submitCustomScale();

    void updateRootNoteLabels();

    //==============================================================================

    void showModeInfo();

    void showMapOrderEditDialog();

    void showSettingsDialog();

    void hideSettings();

    void beginColorEditing();

    void endColorEditing();

    void beginManualMapping();

    //==============================================================================

    bool browseForModeToOpen();

    bool browseForPresetToOpen();

    bool exportModeViewedForReaper();

    bool exportModeViewedForAbleton();

    void exportKbmMapping();

private:

    SvkPluginState* pluginState;
    SvkPresetManager* presetManager;

    std::unique_ptr<Image> saveIcon;
    std::unique_ptr<Image> openIcon;
    std::unique_ptr<Image> settingsIcon;

    std::unique_ptr<PopupMenu> saveMenu;
    Array<PopupMenu::Item> saveMenuItems;

    std::unique_ptr<PopupMenu> loadMenu;
    Array<PopupMenu::Item> loadMenuItems;
    
    MappingHelper* manualMappingHelper;
    std::unique_ptr<SettingsContainer> settingsContainer;

    ScrollBar* viewportScrollBar;
    ModeInfoDialog* modeInfo;
    MapByOrderDialog* mapByOrderDialog;

    Array<Component*> mappingComponents;

    bool settingsPanelOpen = false;
    bool isColorEditing = false;
    bool inMappingMode = false;
    bool mappingSettingsOpen = false;

    String noKeySelectedTrans = "No key selected.";
    String waitingForTrans = "Waiting for input to map to key ";

    String inputModeTooltip = "The input scale that represents your physical keyboard or DAW piano roll.";
    String outputModeTooltip = "The output scale that you want to play or reference.";

    String mapModeTooltip =
        "Maps MIDI note input to notes in the selected scale (Mode 2).\n"
        "Auto Map: automatically maps notes based on Mode 1 and Mode 2.\n"
        "Manual: create a custom mapping by clicking notes on the virtual keyboard.";

    String mapStyleTooltip =
        "The way notes are mapped from the input to output.\n"
        "Mode To Mode: natural notes from Mode 1 are mapped to natural Notes in Mode 2.\n"
        "Scale To Mode: each note from Mode 1 is mapped to natural notes in Mode 2.\n"
        "by Layer: Notes from a layer in Mode 1 are mapped to a layer in Mode 2.";

    float centerKeyPos = 60.5f;

    int defaultHeight = 210;
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
    std::unique_ptr<ImageButton> saveButton;
    std::unique_ptr<ImageButton> openButton;
    std::unique_ptr<ImageButton> settingsButton;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginControlComponent)
};
