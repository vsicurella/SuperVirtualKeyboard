/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "UI/PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SvkAudioProcessor::SvkAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    svkUndo(new UndoManager()),
    svkCmdMgr(new ApplicationCommandManager()),
    svkValueTree(*this, svkUndo.get(), IDs::svkParentNode, createParameters())
{
    DBG("Parameters are initialized");
	pluginState.reset(new SvkPluginState(svkValueTree));
}

SvkAudioProcessor::~SvkAudioProcessor()
{
	
}

//==============================================================================
const String SvkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SvkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SvkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SvkAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SvkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SvkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SvkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SvkAudioProcessor::setCurrentProgram (int index)
{
}

const String SvkAudioProcessor::getProgramName (int index)
{
    return {};
}

void SvkAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SvkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	pluginState->getMidiProcessor()->reset(sampleRate);
    sendChangeMessage();
}

void SvkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SvkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SvkAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data
    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    pluginState->getMidiProcessor()->processMidi(midiMessages);
}

//==============================================================================
bool SvkAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SvkAudioProcessor::createEditor()
{
    return new SvkPluginEditor(*this);
}

//==============================================================================
void SvkAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	
	MemoryOutputStream memOut(destData, true);
    pluginState->commitParametersToPreset();
	pluginState->pluginStateNode.writeToStream(memOut);
    DBG("Saving Plugin State node to internal memory:" + pluginState->pluginStateNode.toXmlString());
}

void SvkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	
	MemoryInputStream memIn(data, sizeInBytes, false);
    ValueTree presetRecall = ValueTree::readFromStream(memIn);
	DBG("Found this in memory:\n" + presetRecall.toXmlString());

    //presetRecall = ValueTree(); // uncomment this line to test new instantiation

	//pluginState->recallState(presetRecall);
}

//==============================================================================

UndoManager* SvkAudioProcessor::getUndoManager()
{
    return svkUndo.get();
}

ApplicationCommandManager* SvkAudioProcessor::getAppCmdMgr()
{
    return svkCmdMgr.get();
}

SvkPluginState* SvkAudioProcessor::getPluginState()
{
	return pluginState.get();
}

Array<String>* SvkAudioProcessor::getParamIDs()
{
    return &paramIDs;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SvkAudioProcessor();
}

//==============================================================================

AudioProcessorValueTreeState::ParameterLayout SvkAudioProcessor::createParameters()
{
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::presetSlotViewed.toString(),"Preset Slot Viewed", 0, 1, 0));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::modeSlotNumViewed.toString(), "Mode Slot Viewed", 0, 1, 0));
    
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode1SlotNum.toString(), "Mode 1 Slot Number", 0, 128, 0));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode2SlotNum.toString(), "Mode 2 Slot Number", 0, 128, 1));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode1RootNote.toString(), "Mode 1 Root Note", 0, 127, 60));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode2RootNote.toString(), "Mode 2 Root Note", 0, 127, 60));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode1RootChannel.toString(), "Mode 1 Root Channel", 1, 16, 1));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mode2RootChannel.toString(), "Mode 2 Root Channel", 1, 16, 1));
    
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::mappingMode.toString(), "Mapping Mode", 1, 3, 1));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::modeMappingStyle.toString(), "Mapping Style", 1, 3, 1));
    
    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::mpeOn.toString(), "MPE On", false));
    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::mpeLegacyOn.toString(), "MPE Legacy Mode", false));
    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::mpeTuningPreserveMidiNote.toString(), "Preserve Midi Notes", true));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pitchBendGlobalMax.toString(), "Global Pitch Bend", 0, 8192, 2));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pitchBendNoteMax.toString(), "Note Pitch Bend", 0, 8192, 48));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::maxVoices.toString(), "Max Polyphony", 1, 16, 15));

    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::retuneOn.toString(), "Retune On", false));
    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::retuneAuto.toString(), "Retune Auto", false));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::tuningRootNote.toString(), "Tuning Reference Note", 0, 127, 69));
    paramsInit.push_back(std::make_unique<AudioParameterFloat>(IDs::tuningRootFreq.toString(), "Tuning Reference Frequency", 0.01f, 24000.0f, 440.0f));

    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::periodShift.toString(), "Period Shift", -10, 10, 0));
    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::periodShiftModeSize.toString(), "Shift By Mode Size", 0));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::transposeAmt.toString(), "Transpose", -127, 127, 0));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardMidiChannel.toString(), "Midi Channel Out", 1, 16, 1));
    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::pianoKeysShowNoteNumbers.toString(), "Show Note Numbers", false));
    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::pianoKeysShowFilteredNotes.toString(), "Show Filtered Numbers", false));
    paramsInit.push_back(std::make_unique<AudioParameterBool>(IDs::pianoKeyShowNoteLabel.toString(), "Show Pitch Names", false));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardScrollingMode.toString(), "Scrolling Mode", 0, 3, 1));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardScrollingStyle.toString(), "Scrolling Style", 0, 3, 1));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::numKeysInWidth.toString(), "Num Keys in Width", 1, 128, 16));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardNumRows.toString(), "Keyboard Rows", 1, 16, 1));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardOrientation.toString(), "Keyboard Orientation", 0, 3, 0));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardKeysStyle.toString(), "Key Style", 1, 4, 1));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyboardHighlightStyle.toString(), "Highlight Style", 1, 4, 1));
    
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pianoVelocityBehavior.toString(), "Keyboard Velocity Mode", 0, 2, 0));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pianoVelocityValue.toString(), "Default Velocity", 0, 127, 100));
    paramsInit.push_back(std::make_unique<AudioParameterFloat>(IDs::pianoWHRatio.toString(), "Key Size Ratio", 0.01f, 1.62f, 0.25f));
    
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::modeSlotToEdit.toString(), "Mode Slot Debug", 0, 1, 0));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::modeLibraryIndex.toString(), "Mode Debug Index", 0, 1, 0));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::modeRootNote.toString(), "Mode Debug Root", 0, 127, 60));
    
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::keyNumberToEdit.toString(), "Key To Debug", 0, 127, 60));
    paramsInit.push_back(std::make_unique<AudioParameterFloat>(IDs::pianoKeyWidthMod.toString(), "Key Debug Width Mod", 0.001f, 10.0f, 1.0f));
    paramsInit.push_back(std::make_unique<AudioParameterFloat>(IDs::pianoKeyHeightMod.toString(), "Key Debug Height Mod", 0.001f, 10.0f, 1.0f));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pianoKeyXOffset.toString(), "Key Debug X Offset", -1000, 1000, 0));
    paramsInit.push_back(std::make_unique<AudioParameterInt>(IDs::pianoKeyYOffset.toString(), "Key Debug Y Offset", -1000, 1000, 0));
    
    for (int i = 0; i < paramsInit.size(); i++)
    {
        paramIDs.add(paramsInit.at(i)->paramID);
    }
    
    return {paramsInit.begin(), paramsInit.end()};
}
