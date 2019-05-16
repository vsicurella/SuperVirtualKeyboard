/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "UI/PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SuperVirtualKeyboardAudioProcessor::SuperVirtualKeyboardAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	pluginState(new SuperVirtualKeyboardPluginState())
#endif
{
    midiStateInput = pluginState->midiStateIn.get();
    midiStateOutput = pluginState->midiStateOut.get();
}

SuperVirtualKeyboardAudioProcessor::~SuperVirtualKeyboardAudioProcessor()
{
	
}

//==============================================================================
const String SuperVirtualKeyboardAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SuperVirtualKeyboardAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SuperVirtualKeyboardAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SuperVirtualKeyboardAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SuperVirtualKeyboardAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SuperVirtualKeyboardAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SuperVirtualKeyboardAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SuperVirtualKeyboardAudioProcessor::setCurrentProgram (int index)
{
}

const String SuperVirtualKeyboardAudioProcessor::getProgramName (int index)
{
    return {};
}

void SuperVirtualKeyboardAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================


MidiBuffer* SuperVirtualKeyboardAudioProcessor::get_midi_buffer()
{
	return &midiBuffer;
}

//==============================================================================
void SuperVirtualKeyboardAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SuperVirtualKeyboardAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SuperVirtualKeyboardAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SuperVirtualKeyboardAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

    auto midiEvent = MidiBuffer::Iterator(midiMessages);
    MidiMessage msg;
    int smpl;

    while (midiEvent.getNextEvent(msg, smpl))
    {
        midiStateInput->processNextMidiEvent(msg);
    }

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data
    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	midiMessages.addEvents(midiBuffer, 0, -1, 0);
	midiBuffer.clear();
}

//==============================================================================
bool SuperVirtualKeyboardAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SuperVirtualKeyboardAudioProcessor::createEditor()
{
    return new SuperVirtualKeyboardAudioProcessorEditor(*this, pluginState->appCmdMgr.get());
}

//==============================================================================
void SuperVirtualKeyboardAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	
	MemoryOutputStream memOut;
	pluginState->pluginStateNode.writeToStream(memOut);
	destData.append(memOut.getData(), memOut.getDataSize());
}

void SuperVirtualKeyboardAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	
	MemoryInputStream memIn(data, sizeInBytes, false);
	pluginState->pluginStateNode.readFromStream(memIn);
}

//==============================================================================

SuperVirtualKeyboardPluginState * SuperVirtualKeyboardAudioProcessor::get_plugin_state()
{
	return pluginState.get();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SuperVirtualKeyboardAudioProcessor();
}
