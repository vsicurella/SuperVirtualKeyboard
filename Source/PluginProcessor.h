/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "UI/PluginEditor.h"
#include "PluginState.h"

//==============================================================================
/**
*/
class SvkAudioProcessor  : public AudioProcessor,
                            public ChangeBroadcaster
{
public:
    //==============================================================================
    SvkAudioProcessor();
    ~SvkAudioProcessor();
    
    std::unique_ptr<UndoManager> svkUndo;
    std::unique_ptr<ApplicationCommandManager> svkCmdMgr;
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
    ApplicationCommandManager* getAppCmdMgr();
	SvkPluginState* getPluginState();
    Array<String>* getParamIDs();
    
	//==============================================================================

private:
    
    AudioProcessorValueTreeState::ParameterLayout createParameters();
    std::vector<std::unique_ptr<RangedAudioParameter>> paramsInit = {};
    Array<String> paramIDs;

	std::unique_ptr<SvkPluginState> pluginState;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SvkAudioProcessor)
};
