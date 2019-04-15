/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "UI/PluginEditor.h"
#include "Structures/ModeLayout.h"
#include "PluginState.h"

//==============================================================================
/**
*/
class SuperVirtualKeyboardAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SuperVirtualKeyboardAudioProcessor();
    ~SuperVirtualKeyboardAudioProcessor();
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

	MidiBuffer* get_midi_buffer();
	void set_midi_input_state(MidiKeyboardState* stateIn);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	//==============================================================================

	OwnedArray<ModeLayout>* get_presets();
	Array<Array<ModeLayout*>>* get_presets_sorted();
	void createPresets();

	ValueTree get_current_preset();
	int get_current_preset_index();
	ModeLayout* get_current_preset_mode();

	void set_current_preset(int presetIndexIn);
	void set_current_preset(ModeLayout* modeIn);
	void set_current_preset(ValueTree nodeIn);

	//==============================================================================

	void connect_editor_node(ValueTree pluginEditorNodeIn);
	SuperVirtualKeyboardPluginState* get_plugin_state();
	ApplicationCommandManager* get_app_cmd_mgr();
    
private:
    //==============================================================================

	std::unique_ptr<SuperVirtualKeyboardPluginState> pluginState;
	std::unique_ptr<UndoManager> undoManager;
	ApplicationCommandManager appCmdMgr;
	ValueTree processorNode;
	ValueTree presetCurrentNode;

	MidiBuffer midiBuffer;
	MidiKeyboardState* externalKeyboardState;

	OwnedArray<ModeLayout> presets;
	Array<Array<ModeLayout*>> presetsSorted;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperVirtualKeyboardAudioProcessor)
};
