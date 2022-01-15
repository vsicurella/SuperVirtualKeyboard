/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PresetManager.h"
#include "Constants.h"
#include "PluginModes.h"
#include "PluginSettings.h"
#include "MidiProcessor.h"
#include "Structures/ModeMapper.h"

typedef AudioProcessorValueTreeState::Parameter Parameter;

//==============================================================================
/**
*/

class SvkAudioProcessor  : public AudioProcessor,
                           public ChangeBroadcaster,
                           public SvkPresetManager::Listener
{
public:
    //==============================================================================
    SvkAudioProcessor();
    ~SvkAudioProcessor();
    
    std::unique_ptr<UndoManager> svkUndo;
    AudioProcessorValueTreeState svkValueTree;
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    UndoManager* getUndoManager();
    //ApplicationCommandManager* getAppCmdMgr();
    //const Array<String>& getParamIDs() const;
    

    //==============================================================================
    // Data
    
    void recallState(ValueTree nodeIn, bool fallbackToDefaultSettings = false);

    ValueTree buildStateValueTree();

    static int isValidStateNode(ValueTree pluginStateNodeIn);


    //==============================================================================
    // Object getters

    //ValueTree getPluginEditorNode() const;
    
    SvkPresetManager* getPresetManager() const;
    SvkMidiProcessor* getMidiProcessor() const;
    SvkPluginSettings* getPluginSettings() const;
    
    ModeMapper* getModeMapper() const;
    
    NoteMap* getMidiInputFilterMap() const;
    NoteMap* getMidiOutputFilterMap() const;

    ValueTree getPresetNode() const;

    Mode* getModeInSlot(int slotNumIn) const;
    Mode* getModeViewed() const;
    Mode* getMode1() const;
    Mode* getMode2() const;
    Mode* getModeCustom() const;

    //==============================================================================

    void addPresetManagerListener(SvkPresetManager::Listener* listenerIn);
    void removePresetManagerListener(SvkPresetManager::Listener* listenerIn);
    
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

    SvkPreset& getPreset() const;
    bool isPresetEdited();

    //==============================================================================

    void setParameterValue(Identifier paramIdIn, float valueIn);

    void setMapMode(int mapModeSelectionIn);
    void setAutoMapStyle(int mapStyleIn);

    void setModeSelectorRoot(int modeSlotIn, int rootNoteIn, bool updateParameter=false);

    /*
        Should be called whenever a mode is initialized in the PresetManager
    */
    void onModeUpdate(bool sendModeViewedChangeMessage = true, bool sendMappingChangeMessage = true);
    
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

    void revertToSavedPreset(bool fallbackToDefaultSettings = false, bool sendChange = true);

    bool savePresetToFile();
    bool loadPresetFromFile();

    bool saveModeViewedToFile();
    bool loadModeFromFile();

    void commitModeInfo(bool sendChangeMessage = true);

    //==============================================================================

    //void parameterChanged(const String& paramID, float newValue) override;

    // void changeListenerCallback(ChangeBroadcaster* source) override;

    //==============================================================================
    // Internal Functionality

    void buildFactoryDefaultState();
    void buildUserDefaultState();

    void updateModeViewed(bool sendChange=true);

private:

    std::unique_ptr<SvkPluginSettings> pluginSettings;
    std::unique_ptr<SvkPresetManager> presetManager;
    std::unique_ptr<SvkMidiProcessor> midiProcessor;

    std::unique_ptr<ModeMapper> modeMapper;
    
    std::unique_ptr<FileChooser> chooser;

    ValueTree factoryDefaultPluginStateNode;
    ValueTree defaultPluginStateNode;

    SvkPreset& workingPreset;
    
    Mode* modeViewed; // What is currently on screen
    int modeSelectorViewedNum = 1;
    
    //void recordParamIDs();
    //
    AudioProcessorValueTreeState::ParameterLayout createParameters();
    //Array<String> paramIDs;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SvkAudioProcessor)
};
