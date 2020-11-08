/*
  ==============================================================================

    PluginState.h
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#include "CommonFunctions.h"
#include "Constants.h"
#include "PluginIDs.h"
#include "PluginModes.h"
#include "PluginSettings.h"
#include "Midi/MidiProcessor.h"
#include "PresetManager.h"
#include "Structures/Preset.h"
#include "Structures/ModeMapper.h"

class SvkPluginState : public ChangeListener, public ValueTree::Listener
                        //private AudioProcessorValueTreeState::Listener
{
public:
    AudioProcessorValueTreeState& svkTree;
    ValueTree pluginStateNode;
    //ValueTree pluginSettingsNode;
    //ValueTree midiSettingsNode;
    //ValueTree modeLibraryNode;
    //ValueTree pianoNode;
    
    SvkPluginState(AudioProcessorValueTreeState&);
    ~SvkPluginState() {}

    //==============================================================================
    // Data
    
    void recallState(ValueTree nodeIn, bool fallbackToDefaultSettings = false);

    // TODO: make it so it's not necessary to call this before saving (?)
    void commitStateNode();

    static int isValidStateNode(ValueTree pluginStateNodeIn);
    
    //==============================================================================
    // Object getters

    ValueTree getPluginEditorNode() const;
    ValueTree getMappingNode() const;
    
    SvkPresetManager* getPresetManager();
    SvkMidiProcessor* getMidiProcessor();
    SvkPluginSettings* getPluginSettings();
    
    ModeMapper* getModeMapper();
    MappingHelper* getManualMappingHelper();
    
    NoteMap* getMidiInputFilterMap();
    NoteMap* getMidiOutputFilterMap();

    ValueTree getPresetNode();

    Mode* getModeInSlot(int slotNumIn) const;
    Mode* getModeViewed();
    Mode* getMode1();
    Mode* getMode2();
    Mode* getModeCustom();

    //==============================================================================
    // Parameter Getters

    //float getParameterValue(Identifier paramId);
    //float getParameterMin(Identifier paramId);
    //float getParameterMax(Identifier paramId);
    //float getParameterDefaultValue(Identifier paramId);

    int getNumModesLoaded() const;
    int getModeSelectorViewed() const;
    int getModeViewedSlotNumber() const;

    int getMappingMode() const;
    int getAutoMappingStyle() const;
    bool isAutoMapping() const;

    //bool isAutoRetuning();

    int getModeSlotRoot(int slotNum) const;
    int getMode1Root() const;
    int getMode2Root() const;

    bool isPresetEdited();

    // ValueTree::Listener Implementation
    void valueTreePropertyChanged(ValueTree&, const Identifier&) override;

    void valueTreeChildAdded(ValueTree& parentTree, ValueTree& child) override;

    void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& child, int index) override;

    //==============================================================================

    //void setParameterValue(Identifier paramIdIn, float valueIn);

    void setMapMode(int mapModeSelectionIn, bool sendChangeMessage = true);
    void setAutoMapStyle(int mapStyleIn);

    void setModeSelectorRoot(int modeSlotIn, int rootNoteIn, bool updateParameter=false);

    /*
        Should be called whenever a mode is initialized in the PresetManager
    */
    void onModeUpdate(bool sendModeViewedChangeMessage = true, bool sendMappingChangeMessage = true);

	void sendChangeNotificationToHost();
    
    //==============================================================================
    // User Functionality
    
    void setModeSelectorViewed(int modeViewedIn);
    void handleModeSelection(int modeBoxNum, int idIn);

    void setMidiInputMap(NoteMap noteMapIn, bool updateNode, bool sendChangeMessage = true);
    void setMidiOutputMap(NoteMap noteMapIn, bool updateNode);
    
    void setModeCustom(String stepsIn, bool sendChangeMessage = true);

    void setMapOrder1(int orderIn);
    void setMapOrder2(int orderIn);
    void setMapOrderOffset1(int offsetIn);
    void setMapOrderOffset2(int offsetIn);

    void doAutoMapping(const Mode* mode1, const Mode* mode2, bool sendChangeMessage = true);
    void doAutoMapping(bool sendChangeMessage = true);

    void revertToSavedState(bool fallbackToDefaultSettings = false, bool sendChange = true);

    bool savePresetToFile();
    bool loadPresetFromFile();

    bool saveModeViewedToFile();
    bool loadModeFromFile();

    void commitModeInfo(bool sendChangeMessage = true);

    //==============================================================================

    //void parameterChanged(const String& paramID, float newValue) override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    //==============================================================================

    class Listener
    {
    public:

        virtual ~Listener() {};

        virtual void presetLoaded(ValueTree presetNodeIn) {};

        virtual void resyncPresetNode(ValueTree presetNodeIn) {}; // TODO: make unnecessary

        virtual void modeViewedChanged(Mode* modeIn, int selectorNumber, int slotNumber) {};

        virtual void mappingModeChanged(int mappingModeId) {};

        virtual void inputMappingChanged(NoteMap& inputNoteMap) {};

        virtual void outputMappingChanged(NoteMap& outputNoteMap) {};

        virtual void customModeChanged(Mode* newCustomMode) {};

        virtual void modeInfoChanged(Mode* modeEdited) {};
    };

    void addListener(SvkPluginState::Listener* listenerIn) { listeners.add(listenerIn); }

    void removeListener(SvkPluginState::Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

    ListenerList<Listener> listeners;

private:
    //==============================================================================
    // Internal Functionality

    void buildFactoryDefaultState();
    void buildUserDefaultState();

    void updateModeViewed(bool sendChange=true);

    void syncNodes(bool sendChange=true);

private:

    std::unique_ptr<SvkPresetManager> presetManager;
    std::unique_ptr<SvkMidiProcessor> midiProcessor;
    std::unique_ptr<SvkPluginSettings> pluginSettings;

    std::unique_ptr<ModeMapper> modeMapper;
    std::unique_ptr<MappingHelper> manualMappingHelper;

    AudioParameterBool* notifyHostDummy;
    bool notifyHostValue = true;

    ValueTree factoryDefaultPluginStateNode;
    ValueTree defaultPluginStateNode;
    
    SvkPreset* svkPreset;
    ValueTree presetNode;

    Mode* modeViewed; // What is currently on screen
    int modeSelectorViewedNum = 1;
    
    bool presetEdited = false;
};
