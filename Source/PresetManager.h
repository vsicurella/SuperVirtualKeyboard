/*
  ==============================================================================

    PresetManager.h
    Created: 27 May 2019 10:57:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "CommonFunctions.h"
#include "PluginIDs.h"
#include "Structures/Preset.h"
#include "Structures/Mode.h"

class SvkPresetManager : public ChangeBroadcaster
{
    Array<ValueTree> loadedFactoryModes;
    Array<ValueTree> loadedUserModes;
    Array<ValueTree> favoriteModes;

    Array<Array<ValueTree>> modesSorted;
    int numSortTypes = 4;

    ScaleSizeSorter scaleSizeSort;
    ModeSizeSorter modeSizeSort;
    FamilyNameSorter familyNameSort;

    SvkPreset svkPresetSaved;
    SvkPreset svkPresetWorking;
    
    OwnedArray<Mode> modeSlots;
    std::unique_ptr<Mode> modeCustom;

    // Methods
    void createFactoryModes();
    void resortModeLibrary();

    void initializeModePresets();
    void loadModeDirectory();

    int addModeToLibrary(ValueTree modeNodeIn);
    void addModeToSort(ValueTree modeNodeIn);
    int addAndSortMode(ValueTree modeNodeIn);

public:

    ValueTree presetLibraryNode;
    ValueTree modeLibraryNode;
    ValueTree pluginSettingsNode;

    SvkPresetManager(ValueTree pluginSettingsNodeIn);
    ~SvkPresetManager();

    SvkPreset& getPreset();
    int getNumMenuItems(bool withFactoryMenu=true, bool withUserMenu=true, bool withFavMenu=true, bool withSlots=true);

    int getNumModeSlots() const;
    
    Array<int> getModeSlotsInUse() const;

    int getModeSlotOfSelector(int modeSelectorNumIn) const;

    ValueTree getModeInLibrary(int indexIn);

    /*
        Returns the mode in the given mode slot number
    */
    Mode* getModeInSlot(int modeSlotNumIn);

    /*
        Returns the mode used by given selector
        If the selector is set to an invalid slot, this will return nullptr
    */
    Mode* getModeBySelector(int selectorNumber);

    Mode* getModeCustom();
    
    Mode* setModeCustom(ValueTree modeNodeIn);
    Mode* setModeCustom(String stepsIn, String familyIn = "undefined", String nameIn = "", String infoIn = "", int rootNoteIn = 60);

    int setSlotToMode(int modeSlotNum, ValueTree modeNode);
    int addSlot(ValueTree modeNode);
    int setSlotAndSelection(int modeSlotNum, int modeSelectorNum, ValueTree modeNode);
    int addSlotAndSetSelection(int modeSelectorNumber, ValueTree modeNode);

    void removeMode(int modeSlotNum);
    void resetModeSlots();

    /*
        Replaces the mode slot the selector is pointed to with a new mode
        - See note above definition -
    */
    void handleModeSelection(int selectorNumber, int idIn);

    bool loadPreset(ValueTree presetNodeIn, bool sendChangeSignal=true);

    bool saveNodeToFile(ValueTree nodeToSave, String saveMsg, String fileEnding, String absolutePath = "");
    bool savePresetToFile(String absolutePath="");
    bool saveModeToFile(int modeSlotNumber, String absolutePath = "");
    
    /*
        Saves changes made since the last preset recall
        Returns true if successful
    */
    bool commitPreset();

    /*
        Reverts the changes made since the last recall
        Returns true if successful
    */
    bool resetToSavedPreset();

    static ValueTree nodeFromFile(String openMsg, String fileEnding, String absoluteFilePath = "");
    static ValueTree modeFromFile(String absoluteFilePath = "");
    static ValueTree presetFromFile(String absoluteFilePath = "");
    
    void requestModeMenu(PopupMenu* comboBoxToUse);

private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvkPresetManager)
};
