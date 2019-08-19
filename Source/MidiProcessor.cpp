/*
  ==============================================================================

    MidiProcessor.cpp
    Created: 21 May 2019 6:37:57pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "MidiProcessor.h"

SvkMidiProcessor::SvkMidiProcessor()
{
    midiSettingsNode = ValueTree(IDs::midiSettingsNode);
    midiMapNode = ValueTree(IDs::midiMapNode);

	midiSettingsNode.addChild(midiMapNode, -1, nullptr);
           
    midiInputFilter = std::make_unique<MidiFilter>();
    midiOutputFilter = std::make_unique<MidiFilter>();

    //setMidiInput(MidiInput::getDefaultDeviceIndex());
    midiInput = nullptr;
    midiOutput = nullptr;
    
    keyboardState.reset(new MidiKeyboardState());
    // default sample rate
    reset(41000);
    
    setRootNote(60);
}

SvkMidiProcessor::~SvkMidiProcessor()
{
    
}

void SvkMidiProcessor::updateNode()
{
    midiSettingsNode.setProperty(IDs::rootMidiNote, rootMidiNote, nullptr);

    Array<int> inputMap = midiInputFilter->getRemappedNotes();
    midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputMap), nullptr);
    add_array_to_node(midiMapNode, inputMap, IDs::midiInputMap, "Note");
    
    Array<int> outputMap = midiOutputFilter->getRemappedNotes();
    midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiOutputMap), nullptr);
    add_array_to_node(midiMapNode, outputMap, IDs::midiOutputMap, "Note");
}

bool SvkMidiProcessor::restoreFromNode(ValueTree midiSettingsNodeIn)
{
    if (midiSettingsNodeIn.hasType(IDs::midiSettingsNode))
    {
        midiSettingsNode.copyPropertiesAndChildrenFrom(midiSettingsNodeIn, nullptr);

        /*
        
        // Open Input Device
        int deviceIndex = MidiInput::getDefaultDeviceIndex();
        String deviceName = midiSettingsNode.getProperty(IDs::midiInputName);
        StringArray deviceList = MidiInput::getDevices();
        
        for (int i = 0; i < deviceList.size(); i++)
        {
            if (deviceList[i] == deviceName)
            {
                deviceIndex = i;
                break;
            }
        }
    
        setMidiInput(deviceIndex);
        
        // Open Output Device
        deviceIndex = MidiOutput::getDefaultDeviceIndex();
        deviceName = midiSettingsNode.getProperty(IDs::midiOutputName);
        deviceList = MidiInput::getDevices();
        
        for (int i = 0; i < deviceList.size(); i++)
        {
            if (deviceList[i] == deviceName)
            {
                deviceIndex = i;
                break;
            }
        }
        
        setMidiOutput(deviceIndex);
         
        */
        
        // Root note
        rootMidiNote = midiSettingsNode.getProperty(IDs::rootMidiNote);
        
		// Set Note Maps
		if (!midiSettingsNode.getChildWithName(IDs::midiMapNode).isValid())
		{
			midiMapNode = ValueTree(IDs::midiMapNode);
			midiSettingsNode.addChild(midiMapNode, -1, nullptr);
		}

		setMidiMaps(midiSettingsNode.getChildWithName(IDs::midiMapNode));

        return true;
    }
    
    return false;
}

//==============================================================================

void SvkMidiProcessor::resetWithRate(double sampleRateIn)
{
	reset(sampleRateIn);
}

StringArray SvkMidiProcessor::getAvailableInputs()
{
    return MidiInput::getDevices();
}

StringArray SvkMidiProcessor::getAvailableOutputs()
{
    return MidiOutput::getDevices();
}

MidiInput* SvkMidiProcessor::getInputDevice()
{
    return midiInput;
}

MidiOutput*  SvkMidiProcessor::getOutputDevice()
{
    return midiOutput;
}

MidiKeyboardState* SvkMidiProcessor::getKeyboardState()
{
    return keyboardState.get();
}

int SvkMidiProcessor::getRootNote()
{
    return rootMidiNote;
}

int SvkMidiProcessor::getPeriodShift()
{
    return periodShift;
}

int SvkMidiProcessor::getMidiChannelOut()
{
    return midiChannelOut;
}

NoteMap* SvkMidiProcessor::getInputNoteMap()
{
	return midiInputFilter->getNoteMap();
}

NoteMap* SvkMidiProcessor::getOutputNoteMap()
{
	return midiOutputFilter->getNoteMap();
}

MidiFilter* SvkMidiProcessor::getMidiInputFilter()
{
	return midiInputFilter.get();
}

MidiFilter* SvkMidiProcessor::getMidiOutputFilter()
{
	return midiOutputFilter.get();
}

int SvkMidiProcessor::getInputNote(int midiNoteIn)
{
    return midiInputFilter->getNoteRemapped(midiNoteIn);
}

int SvkMidiProcessor::getOutputNote(int midiNoteIn)
{
    return midiOutputFilter->getNoteRemapped(midiNoteIn);
}

//==============================================================================

String SvkMidiProcessor::setMidiInput(int deviceIndex)
{
    midiInput->stop();
    midiInput = MidiInput::openDevice(deviceIndex, this).get();
    midiInput->start();
    inputSelected = deviceIndex;
    midiSettingsNode.setProperty(IDs::midiInputName, midiInput->getName(), nullptr);
    return midiInput->getName();
}

String SvkMidiProcessor::setMidiOutput(int deviceIndex)
{
    midiOutput->stopBackgroundThread();
    midiOutput = MidiOutput::openDevice(deviceIndex).get();
    midiOutput->startBackgroundThread();
    outputSelected = deviceIndex;
    midiSettingsNode.setProperty(IDs::midiOutputName, midiOutput->getName(), nullptr);
    return midiOutput->getName();
}

void SvkMidiProcessor::setModeViewed(Mode* modeViewedIn)
{
	modeViewed = modeViewedIn;
}

void SvkMidiProcessor::setMode1(Mode* mode1In)
{
	mode1 = mode1In;
}

void SvkMidiProcessor::setMode2(Mode* mode2In)
{
	mode2 = mode2In;
}

void SvkMidiProcessor::setRootNote(int rootNoteIn)
{	
    midiSettingsNode.setProperty(IDs::rootMidiNote, rootNoteIn, nullptr);
    rootMidiNote = rootNoteIn;
}

void SvkMidiProcessor::setPeriodShift(int shiftIn)
{
    midiSettingsNode.setProperty(IDs::periodShift, periodShift, nullptr);
    periodShift = shiftIn;
}

void SvkMidiProcessor::periodUsesModeSize(bool useModeSizeIn)
{
	useModeSize = useModeSizeIn;
}

void SvkMidiProcessor::setMidiChannelOut(int channelOut)
{
    if (channelOut < 1)
        channelOut = 1;
    
    channelOut = channelOut % 16;
    
    midiSettingsNode.setProperty(IDs::pianoMidiChannel, channelOut, nullptr);
    midiChannelOut = channelOut;
}

void SvkMidiProcessor::setInputToRemap(bool doRemap)
{
    isInputRemapped = doRemap;
}

void SvkMidiProcessor::setOutputToRemap(bool doRemap)
{
    isOutputRemapped = doRemap;
}

void SvkMidiProcessor::setMidiMaps(ValueTree midiMapIn)
{
    if (midiMapIn.hasType(IDs::midiMapNode))
    {
        Array<int> map;
        
        get_array_from_node(midiMapIn, map, IDs::midiInputMap);
        
        if (map.size() > 0)
            setMidiInputMap(map);
        else
            setMidiInputMap(NoteMap());
        
        map.clear();
        get_array_from_node(midiMapIn, map, IDs::midiOutputMap);
        
        if (map.size() > 0)
            setMidiOutputMap(map);
        else
            setMidiOutputMap(NoteMap());
        
        midiMapNode.copyPropertiesAndChildrenFrom(midiMapIn, nullptr);
    }
}

void SvkMidiProcessor::setMidiInputMap(Array<int> mapIn, bool updateNode)
{
	midiInputFilter->setNoteMap(mapIn);

	if (updateNode)
	{
		midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputMap), nullptr);
		add_array_to_node(midiMapNode, midiInputFilter->getNoteMap()->getValues(), IDs::midiInputMap, "Note");
	}
}

void SvkMidiProcessor::setMidiInputMap(NoteMap mapIn, bool updateNode)
{
	midiInputFilter->setNoteMap(mapIn);

	if (updateNode)
	{
		midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputMap), nullptr);
		add_array_to_node(midiMapNode, midiInputFilter->getNoteMap()->getValues(), IDs::midiInputMap, "Note");
	}
}

void SvkMidiProcessor::setMidiOutputMap(Array<int> mapIn, bool updateNode)
{
	midiOutputFilter->setNoteMap(mapIn);

	if (updateNode)
	{
		midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiOutputMap), nullptr);
		add_array_to_node(midiMapNode, midiInputFilter->getNoteMap()->getValues(), IDs::midiOutputMap, "Note");
	}
}

void SvkMidiProcessor::setMidiOutputMap(NoteMap mapIn, bool updateNode)
{
	midiOutputFilter->setNoteMap(mapIn.getValues());

	if (updateNode)
	{
		midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiOutputMap), nullptr);
		add_array_to_node(midiMapNode, midiInputFilter->getNoteMap()->getValues(), IDs::midiOutputMap, "Note");
	}
}

void SvkMidiProcessor::resetInputMap(bool updateNode)
{
    setMidiInputMap(NoteMap(), updateNode);
}

void SvkMidiProcessor::resetOutputMap(bool updateNode)
{
    setMidiOutputMap(NoteMap(), updateNode);
}

void SvkMidiProcessor::mapInputNote(int noteIn, int noteOut, bool updateNode)
{
    midiInputFilter->setNote(noteIn, noteOut);

	if (updateNode)
		set_value_in_array(midiMapNode, IDs::midiInputMap, noteIn, noteOut);
}

void SvkMidiProcessor::mapOutputNode(int noteIn, int noteOut, bool updateNode)
{
    midiOutputFilter->setNote(noteIn, noteOut);

	if (updateNode)
		set_value_in_array(midiMapNode, IDs::midiInputMap, noteIn, noteOut);
}

//==============================================================================


void SvkMidiProcessor::processMidi(MidiBuffer& midiMessages)
{
    // Input filtering
    auto midiEvent = MidiBuffer::Iterator(midiMessages);
    MidiMessage msg;
    int smpl;
    int midiNote;
    
    if (midiInput == nullptr)
    {
        while (midiEvent.getNextEvent(msg, smpl))
        {
            midiNote = msg.getNoteNumber();
            
            if (isInputRemapped)
                midiNote = midiInputFilter->getNoteRemapped(msg.getNoteNumber());
            
			midiNote += periodShift * mode2->getScaleSize();
            
            msg.setNoteNumber(midiNote);
            
			if (midiNote >= 0 && midiNote < 128)
			{
				msg.setTimeStamp(++msgCount);
				keyboardState->processNextMidiEvent(msg);
				addMessageToQueue(msg);
			}
        }
    }
    
    // Midi Output filtering
    msgCount = 0;
    midiMessages.clear();
    removeNextBlockOfMessages(midiBuffer, 4096);
    
    if (!midiInputPaused)
    {
        auto midiEventOut = MidiBuffer::Iterator(midiBuffer);
        while (midiEventOut.getNextEvent(msg, smpl))
        {
            msg.setNoteNumber(midiOutputFilter->getNoteRemapped(msg.getNoteNumber()));
            msg.setTimeStamp(++msgCount);
            midiMessages.addEvent(msg, smpl);
        }
    }
    
    midiBuffer.clear();
    msgCount = 0;
}


//==============================================================================


void SvkMidiProcessor::pauseMidiInput(bool setPaused)
{
    midiInputPaused = setPaused;
}

bool SvkMidiProcessor::isMidiPaused()
{
    return midiInputPaused;
}

//==============================================================================

void SvkMidiProcessor::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage msg = MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
    msg.setTimeStamp(++msgCount);
    addMessageToQueue(msg);
}

void SvkMidiProcessor::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage msg = MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
    msg.setTimeStamp(++msgCount);
    addMessageToQueue(msg);
}

void SvkMidiProcessor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg)
{
    MidiMessage myMsg = MidiMessage(msg);
    
    if (isInputRemapped && !midiInputPaused)
        myMsg.setNoteNumber(midiInputFilter->getNoteRemapped(myMsg.getNoteNumber()));
    
    //myMsg.addToTimeStamp(++msgCount);
    addMessageToQueue(myMsg);
}
