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
    
    noteInputMap.reset(new Array<int>(MidiRemapper::getStandardMap()));
    noteOutputMap.reset(new Array<int>(MidiRemapper::getStandardMap()));
    
    midiInputFilter.reset(new MidiRemapper(noteInputMap.get()));
    midiOutputFilter.reset(new MidiRemapper(noteOutputMap.get()));
    
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
    
    midiSettingsNode.removeChild(midiSettingsNode.getChildWithName(IDs::midiInputMap), nullptr);
    add_array_to_node(midiSettingsNode, *noteInputMap.get(), IDs::midiInputMap, "Note");
    
    midiSettingsNode.removeChild(midiSettingsNode.getChildWithName(IDs::midiOutputMap), nullptr);
    add_array_to_node(midiSettingsNode, *noteOutputMap.get(), IDs::midiOutputMap, "Note");
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
        noteInputMap->clear();
        get_array_from_node(midiSettingsNode, *noteInputMap.get(), IDs::midiInputMap);
        
        noteOutputMap->clear();
        get_array_from_node(midiSettingsNode, *noteOutputMap.get(), IDs::midiOutputMap);
        
        return true;
    }
    
    return false;
}

//==============================================================================

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

Array<int>* SvkMidiProcessor::getInputNoteMap()
{
    return noteInputMap.get();
}

Array<int>* SvkMidiProcessor::getOutputNoteMap()
{
    return noteOutputMap.get();
}

int SvkMidiProcessor::getInputNote(int midiNoteIn)
{
    return midiInputFilter->getNote(midiNoteIn);
}

int SvkMidiProcessor::getOutputNote(int midiNoteIn)
{
    return midiOutputFilter->getNote(midiNoteIn);
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
    rootMidiNote = totalModulus(rootNoteIn, 128);
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

void SvkMidiProcessor::setMidiInputMap(Array<int> mapIn)
{
    noteInputMap.reset(new Array<int>(mapIn));
}

void SvkMidiProcessor::setMidiOutputMap(Array<int> mapIn)
{
    noteOutputMap.reset(new Array<int>(mapIn));
}

Array<int> SvkMidiProcessor::mapInputNote(int noteIn, int noteOut)
{
    midiInputFilter->setNote(noteIn, noteOut);
    return midiInputFilter->removeDuplicates(noteIn);
}

void SvkMidiProcessor::mapOutputNode(int noteIn, int noteOut)
{
    midiOutputFilter->setNote(noteIn, noteOut);
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
            msg.setTimeStamp(++msgCount);
            keyboardState->processNextMidiEvent(msg);
            addMessageToQueue(msg);
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
            msg.setNoteNumber(midiOutputFilter->getNote(msg.getNoteNumber()));
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
        myMsg.setNoteNumber(midiInputFilter->getNote(myMsg.getNoteNumber()));
    
    //myMsg.addToTimeStamp(++msgCount);
    addMessageToQueue(myMsg);
}
