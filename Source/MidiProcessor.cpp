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
       
    midiInputFilter.reset(new MidiFilter());
    midiOutputFilter.reset(new MidiFilter());
    
    //setMidiInput(MidiInput::getDefaultDeviceIndex());
    midiInput = nullptr;
    midiOutput = nullptr;
    
    keyboardState.reset(new MidiKeyboardState());
    // default sample rate
    reset(41000);
}

SvkMidiProcessor::~SvkMidiProcessor()
{
    
}

void SvkMidiProcessor::updateNode()
{
    midiSettingsNode.setProperty(IDs::rootMidiNote, rootMidiNote, nullptr);

    Array<int> inputMap = midiInputFilter->getRemappedNotes();
    midiSettingsNode.removeChild(midiSettingsNode.getChildWithName(IDs::midiInputMap), nullptr);
    add_array_to_node(midiSettingsNode, inputMap, IDs::midiInputMap, "Note");
    
    Array<int> outputMap = midiOutputFilter->getRemappedNotes();
    midiSettingsNode.removeChild(midiSettingsNode.getChildWithName(IDs::midiOutputMap), nullptr);
    add_array_to_node(midiSettingsNode,outputMap, IDs::midiOutputMap, "Note");
}

bool SvkMidiProcessor::restoreFromNode(ValueTree midiSettingsNodeIn)
{
    if (midiSettingsNodeIn.hasType(IDs::midiSettingsNode))
    {
        midiSettingsNode.copyPropertiesFrom(midiSettingsNodeIn, nullptr);
        
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
        
        // Root note
        rootMidiNote = midiSettingsNode.getProperty(IDs::rootMidiNote);
        
        // Note maps
		Array<int> map;

        get_array_from_node(midiSettingsNode, map, IDs::midiInputMap);
		setMidiInputMap(map, false);
        
		map.clear();
        
		get_array_from_node(midiSettingsNode, map, IDs::midiOutputMap);
		setMidiOutputMap(map, false);

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

bool SvkMidiProcessor::isAutoRemapping()
{
    return autoRemap;
}


//==============================================================================

String SvkMidiProcessor::setMidiInput(int deviceIndex)
{
    midiInput->stop();
    midiInput = MidiInput::openDevice(deviceIndex, this);
    midiInput->start();
    inputSelected = deviceIndex;
    midiSettingsNode.setProperty(IDs::midiInputName, midiInput->getName(), nullptr);
    return midiInput->getName();
}

String SvkMidiProcessor::setMidiOutput(int deviceIndex)
{
    midiOutput->stopBackgroundThread();
    midiOutput = MidiOutput::openDevice(deviceIndex);
    midiOutput->startBackgroundThread();
    outputSelected = deviceIndex;
    midiSettingsNode.setProperty(IDs::midiOutputName, midiOutput->getName(), nullptr);
    return midiOutput->getName();
}

void SvkMidiProcessor::setRootNote(int rootNoteIn)
{	
    midiSettingsNode.setProperty(IDs::rootMidiNote, rootNoteIn, nullptr);
}

void SvkMidiProcessor::setInputToRemap(bool doRemap)
{
    isInputRemapped = doRemap;
}

void SvkMidiProcessor::setOutputToRemap(bool doRemap)
{
    isOutputRemapped = doRemap;
}

void SvkMidiProcessor::setMidiInputMap(Array<int> mapIn, bool updateNode)
{
	midiInputFilter->setNoteMap(mapIn);

	if (updateNode)
	{
		midiSettingsNode.removeChild(midiSettingsNode.getChildWithName(IDs::midiInputMap), nullptr);
		add_array_to_node(midiSettingsNode, midiInputFilter->getNoteMap()->getValues(), IDs::midiInputMap, "Note");
	}
}

void SvkMidiProcessor::setMidiInputMap(NoteMap mapIn, bool updateNode)
{
	midiInputFilter->setNoteMap(mapIn);

	if (updateNode)
	{
		midiSettingsNode.removeChild(midiSettingsNode.getChildWithName(IDs::midiInputMap), nullptr);
		add_array_to_node(midiSettingsNode, midiInputFilter->getNoteMap()->getValues(), IDs::midiInputMap, "Note");
	}
}

void SvkMidiProcessor::setMidiOutputMap(Array<int> mapIn, bool updateNode)
{
	midiOutputFilter->setNoteMap(mapIn);

	if (updateNode)
	{
		midiSettingsNode.removeChild(midiSettingsNode.getChildWithName(IDs::midiOutputMap), nullptr);
		add_array_to_node(midiSettingsNode, midiInputFilter->getNoteMap()->getValues(), IDs::midiOutputMap, "Note");
	}
}

void SvkMidiProcessor::setMidiOutputMap(NoteMap mapIn, bool updateNode)
{
	midiOutputFilter->setNoteMap(mapIn.getValues());

	if (updateNode)
	{
		midiSettingsNode.removeChild(midiSettingsNode.getChildWithName(IDs::midiOutputMap), nullptr);
		add_array_to_node(midiSettingsNode, midiInputFilter->getNoteMap()->getValues(), IDs::midiOutputMap, "Note");
	}
}

void SvkMidiProcessor::setAutoRemapOn(bool remapIn)
{
    autoRemap = remapIn;
    midiSettingsNode.setProperty(IDs::autoRemapOn, autoRemap, nullptr);
}


void SvkMidiProcessor::mapInputNote(int noteIn, int noteOut, bool updateNode)
{
    midiInputFilter->setNote(noteIn, noteOut);

	if (updateNode)
		set_value_in_array(midiSettingsNode, IDs::midiInputMap, noteIn, noteOut);
}

void SvkMidiProcessor::mapOutputNode(int noteIn, int noteOut, bool updateNode)
{
    midiOutputFilter->setNote(noteIn, noteOut);

	if (updateNode)
		set_value_in_array(midiSettingsNode, IDs::midiInputMap, noteIn, noteOut);
}

//==============================================================================


void SvkMidiProcessor::processMidi(MidiBuffer& midiMessages)
{
    // Input filtering
    auto midiEvent = MidiBuffer::Iterator(midiMessages);
    MidiMessage msg;
    int smpl;
    
    if (midiInput == nullptr)
    {
        while (midiEvent.getNextEvent(msg, smpl))
        {
			msg.setNoteNumber(midiInputFilter->getNoteRemapped(msg.getNoteNumber()));
			if (msg.getNoteNumber() >= 0 && msg.getNoteNumber() < 128)
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
