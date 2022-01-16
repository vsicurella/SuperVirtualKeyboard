/*
  ==============================================================================

    PresetManager.h
    Created: 27 May 2019 10:57:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "CommonFunctions.h"
#include "PluginIDs.h"
#include "Structures/Preset.h"
#include "Structures/Mode.h"
#include "Structures/NoteMap.h"

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

    PopupMenu modeMenu;
    
    std::unique_ptr<FileChooser> chooser;

private:
    void createFactoryModes();
    void resortModeLibrary();

    void initializeModePresets();
    void loadModeDirectory();

    int addModeToLibrary(ValueTree modeNodeIn);
    void addModeToSort(ValueTree modeNodeIn);
    int addAndSortMode(ValueTree modeNodeIn);

    void updateModeMenu();

public:

    ValueTree presetLibraryNode;
    ValueTree modeLibraryNode;
    ValueTree pluginSettingsNode;

    SvkPresetManager(ValueTree pluginSettingsNodeIn);
    ~SvkPresetManager();

    SvkPreset& getPreset();
    int getNumMenuItems(bool withFactoryMenu=true, bool withUserMenu=true, bool withFavMenu=true, bool withSlots=true);
    
    Array<int> getModeSlotsInUse() const;

    int getModeSlotOfSelector(int modeSelectorNumIn) const;

    bool isPresetEdited() const;

    ValueTree getModeInLibrary(int indexIn);

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
    bool resetToSavedPreset(bool sendChangeMessage);

    static ValueTree parseNodeFile(File fileIn);
    static ValueTree parseModeFile(File fileIn);
    static ValueTree parsePresetFile(File fileIn);
   
    class Listener
    {
    public:

        virtual ~Listener() {};

        virtual void modeLibraryUpdated(const PopupMenu& menu) {};
    };

    void addListener(SvkPresetManager::Listener* listenerIn) { listeners.add(listenerIn); }

    void removeListener(SvkPresetManager::Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

    ListenerList<Listener> listeners;

private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SvkPresetManager)
};
