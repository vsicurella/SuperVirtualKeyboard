/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginEditor.h"
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
	undoManager(new UndoManager()), pluginState(new SuperVirtualKeyboardPluginState(undoManager.get()))
#endif
{
	createPresets();
	// Create default piano layout and initialize ModeLayout ValueTree
	presetSelected = &presets->getUnchecked(7);
	presetSelected->set_valuetree_node(pluginState->modeLayoutNode);
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

void SuperVirtualKeyboardAudioProcessor::set_midi_input_state(MidiKeyboardState* stateIn)
{
	externalKeyboardState = stateIn;
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
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	if (externalKeyboardState)
	{
		auto midiEvent = MidiBuffer::Iterator(midiMessages);
		MidiMessage msg;
		int smpl;

		while (midiEvent.getNextEvent(msg, smpl))
		{
			externalKeyboardState->processNextMidiEvent(msg);
		}
	}

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }

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
	return new SuperVirtualKeyboardAudioProcessorEditor(*this);
}

//==============================================================================
void SuperVirtualKeyboardAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	
	/*
	MemoryOutputStream memOut;
	processorNode.writeToStream(memOut);
	destData.append(memOut.getData(), memOut.getDataSize());
	*/
}

void SuperVirtualKeyboardAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	
	/*
	MemoryInputStream memIn(data, sizeInBytes, false);
	processorNode.readFromStream(memIn);
	*/
}

//==============================================================================
void SuperVirtualKeyboardAudioProcessor::setViewportPositionProportions(Point<int> pointIn)
{
	viewportPos = pointIn;
}

Point<int> SuperVirtualKeyboardAudioProcessor::getViewportPositionProportions()
{
	return viewportPos;
}

void SuperVirtualKeyboardAudioProcessor::setModeLayout(ModeLayout* modeLayoutIn)
{
	presetSelected = modeLayoutIn;
}

ModeLayout* SuperVirtualKeyboardAudioProcessor::getModeLayout()
{
	return presetSelected;
}

struct ModeScaleSorter
{
	static int compareElements(const ValueTree& t1, const ValueTree& t2)
	{
		int sz1 = t1.getProperty(IDs::scaleSize);
		int sz2 = t2.getProperty(IDs::scaleSize);

		if (sz1 < sz2)
			return -1;
		else if (sz1 > sz2)
			return 1;
		else
			return 0;
	}
};

struct ModeModeSorter
{
	static int compareElements(const ValueTree& t1, const ValueTree& t2)
	{
		int sz1 = t1.getProperty(IDs::modeSize);
		int sz2 = t2.getProperty(IDs::modeSize);

		if (sz1 < sz2)
			return -1;
		else if (sz1 > sz2)
			return 1;
		else
			return 0;
	}
};
struct ModeFamilySorter
{
	static int compareElements(const ValueTree& t1, const ValueTree& t2)
	{
		String sz1 = t1.getProperty(IDs::temperamentFamily);
		String sz2 = t2.getProperty(IDs::temperamentFamily);

		if (sz1 < sz2)
			return -1;
		else if (sz1 > sz2)
			return 1;
		else
			return 0;
	}
};

Array<ModeLayout>* SuperVirtualKeyboardAudioProcessor::get_presets()
{
	return presets.get();
}

ValueTree SuperVirtualKeyboardAudioProcessor::get_presets_sorted()
{
	return presetsSorted;
}



void SuperVirtualKeyboardAudioProcessor::createPresets()
{
	presets.reset(new Array<ModeLayout>());
	presets.get()->add(ModeLayout( Array<int>({ 1, 2, 1, 2, 1 }), "Mavila"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 1, 2, 2, 1 }), "Father"));
	presets.get()->add(ModeLayout( Array<int>({ 1, 1, 2, 1, 1, 1, 2 }), "Mavila"));
	presets.get()->add(ModeLayout( Array<int>({ 1, 2, 1, 2, 1, 2, 1 }), "Dicot"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 2, 2, 2, 1 }), "Machine"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 1, 2, 1, 2, 1, 2 }), "Orgone"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 1, 3, 1, 3, 1 }), "Augmented"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 1, 2, 2, 2, 1 }), "Meantone"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1 }), "Diminished"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 1, 2, 2, 1, 2, 1 }), "Father"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1, 1 }), "Orwell"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 2, 2, 1, 2, 2, 1 }), "Hedgehog"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 1, 2, 1, 2, 1, 2, 1 }), "Titanium"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 3, 3, 3, 3 }), "Blackwood"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 1, 3, 1, 3, 1, 3 }), "Orgone"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 2, 2, 2, 2, 2, 1 }), "Porcupine"));
	presets.get()->add(ModeLayout( Array<int>({ 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1 }), "Orgone"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 3, 1, 3, 3, 3 }), "Gorgo"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 2, 1, 2, 2, 2, 2, 1 }), "Mavila"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 1, 2, 1, 2, 2, 1, 2, 1, 2 }), "Lemba"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 3, 1, 3, 3, 3, 1 }), "Meantone"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 2, 3, 2, 3, 2, 2 }), "Maqamic"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 1, 2, 2, 1, 2, 2, 2, 1 }), "Maqamic"));
	presets.get()->add(ModeLayout( Array<int>({ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2 }), "Machine"));
	presets.get()->add(ModeLayout( Array<int>({ 4, 4, 1, 4, 4, 1 }), "Bicycle"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 1, 3, 3, 1, 3, 3, 1 }), "Father"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 3, 2, 3, 3, 3, 2 }), "Meantone"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 3, 2, 2, 3, 2, 3, 2 }), "Keemun"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 2, 2, 3, 2, 2, 2, 2 }), "Negri"));
	presets.get()->add(ModeLayout( Array<int>({ 5, 2, 2, 5, 2, 2, 2 }), "Mavila"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 1, 3, 1, 3, 1, 3, 1, 3, 1 }), "Blackwood"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 3, 2, 2, 3, 2, 2, 3, 2 }), "Miracle"));
	presets.get()->add(ModeLayout( Array<int>({ 4, 4, 1, 4, 4, 4, 1 }), "Superpyth"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 3, 3, 3, 2, 3, 3, 2 }), "Hedgehog"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 2, 3, 2, 3, 2, 3, 2, 2 }),"Orwell"));
	presets.get()->add(ModeLayout( Array<int>({ 3, 3, 3, 1, 3, 3, 3, 3, 1 }), "Mavila"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 2, 3, 2, 2, 3, 2, 2, 3 }), "Sephiroth"));
	presets.get()->add(ModeLayout( Array<int>({ 5, 5, 4, 5, 5 }), "Godzilla"));
	presets.get()->add(ModeLayout( Array<int>({ 5, 3, 5, 3, 5, 3 }), "Triforce"));
	presets.get()->add(ModeLayout( Array<int>({ 4, 3, 3, 4, 3, 3, 4 }), "Dastgah-e Sehgah / Maqam Nairuz"));
	presets.get()->add(ModeLayout( Array<int>({ 4, 4, 2, 4, 4, 4, 2 }), "Meantone"));
	presets.get()->add(ModeLayout( Array<int>({ 4, 3, 4, 3, 4, 3, 3 }), "Mohajira"));
	presets.get()->add(ModeLayout( Array<int>({ 4, 4, 3, 4, 4, 4, 3 }), "Meantone"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 3, 2, 2, 3, 2, 3, 2, 2, 3, 2 }), "Orgone"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 3 }),"Injera"));
	presets.get()->add(ModeLayout( Array<int>({ 5, 5, 3, 5, 5, 5, 3 }), "Meantone"));
	presets.get()->add(ModeLayout( Array<int>({ 4, 3, 4, 3, 4, 3, 4, 3, 3, }), "Orwell"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 1, 1, 2, 1, 2, 1, 2, 1 }),"BP Lambda"));
	presets.get()->add(ModeLayout( Array<int>({ 2, 1, 1, 2, 1, 2, 1, 1, 2 }), "BP Dur II"));

	presetsSorted = ValueTree(IDs::presetsSorted);

	// Sort default presets, scale size
	presetsSorted.addChild(ValueTree(IDs::scaleSort), SortType::scaleSize, nullptr);
	presetsSorted.addChild(ValueTree(IDs::modeSort), SortType::modeSize, nullptr);
	presetsSorted.addChild(ValueTree(IDs::modeSort), SortType::family, nullptr);

	ModeScaleSorter scaleSort;
	ModeModeSorter modeSort;
	ModeFamilySorter famSort;

	ValueTree sorting = ValueTree(IDs::presetsSorted);
	for (int i = 0; i < presets.get()->size(); i++)
	{
		sorting.addChild(presets.get()->getUnchecked(i).modeLayoutNode, i, nullptr);
	}

	sorting.sort(scaleSort, nullptr, false);

	for (auto child : sorting)
		presetsSorted.getChild(SortType::scaleSize).addChild(child, -1, nullptr);

	sorting.sort(modeSort, nullptr, false);

	for (auto child : sorting)
		presetsSorted.getChild(SortType::modeSize).addChild(child, -1, nullptr);

	sorting.sort(famSort, nullptr, false);

	for (auto child : sorting)
		presetsSorted.getChild(SortType::family).addChild(child, -1, nullptr);
}



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SuperVirtualKeyboardAudioProcessor();
}
