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

    midiInput = nullptr;
    midiOutput = nullptr;
    
    mpeInst.reset(new MPEInstrument());
    
    originalKeyboardState.reset(new MidiKeyboardState());
    remappedKeyboardState.reset(new MidiKeyboardState());

    setRootNote(60);
}

SvkMidiProcessor::~SvkMidiProcessor()
{
    
}

void SvkMidiProcessor::updateNode()
{
    midiSettingsNode.setProperty(IDs::rootMidiNote, rootMidiNote, nullptr);
    midiSettingsNode.setProperty(IDs::pianoMidiChannel, midiChannelOut, nullptr);
    midiSettingsNode.setProperty(IDs::periodShift, periodShift, nullptr);
    midiSettingsNode.setProperty(IDs::periodShiftModeSize, useModePeriod, nullptr);
    midiSettingsNode.setProperty(IDs::transposeAmt, transposeAmt, nullptr);
    midiSettingsNode.setProperty(IDs::mpeOn, mpeOn, nullptr);
    midiSettingsNode.setProperty(IDs::mpeThru, mpeThru, nullptr);
    midiSettingsNode.setProperty(IDs::mpeZone, 0 /*do something here*/, nullptr);
    midiSettingsNode.setProperty(IDs::mpeLegacyOn, mpeLegacyOn, nullptr);
    midiSettingsNode.setProperty(IDs::pitchBendNoteMax, pitchBendNoteMax, nullptr);
    midiSettingsNode.setProperty(IDs::pitchBendGlobalMax, pitchBendGlobalMax, nullptr);
    midiSettingsNode.setProperty(IDs::mpePitchTrackingMode, mpePitchTrackingMode, nullptr);
    midiSettingsNode.setProperty(IDs::mpePressureTrackingMode, mpePressureTrackingMode, nullptr);
    midiSettingsNode.setProperty(IDs::mpeTimbreTrackingMode, mpeTimbreTrackingMode, nullptr);

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

StringArray SvkMidiProcessor::getAvailableInputs() const
{
    return MidiInput::getDevices();
}

Array<MidiDeviceInfo> SvkMidiProcessor::getAvailableOutputs() const
{
    return MidiOutput::getAvailableDevices();
}

MidiInput* SvkMidiProcessor::getInputDevice()
{
    return midiInput.get();
}

MidiOutput*  SvkMidiProcessor::getOutputDevice()
{
    return midiOutput.get();
}

String SvkMidiProcessor::getInputName() const
{
    return midiInputName;
}

String SvkMidiProcessor::getOutputName() const
{
    return midiOutputName;
}

MidiKeyboardState* SvkMidiProcessor::getOriginalKeyboardState()
{
    return originalKeyboardState.get();
}


MidiKeyboardState* SvkMidiProcessor::getRemappedKeyboardState()
{
    return remappedKeyboardState.get();
}

int SvkMidiProcessor::getRootNote() const
{
    return rootMidiNote;
}

int SvkMidiProcessor::getPeriodShift() const
{
    return periodShift;
}

int SvkMidiProcessor::getMidiChannelOut() const
{
    return midiChannelOut;
}

int SvkMidiProcessor::getTransposeAmt() const
{
    return transposeAmt;
}

bool SvkMidiProcessor::isMPEOn() const
{
    return mpeOn;
}

bool SvkMidiProcessor::isMPELegacyMode() const
{
    return mpeLegacyOn;
}

bool SvkMidiProcessor::isMPEThru() const
{
    return mpeThru;
}

MPEZoneLayout SvkMidiProcessor::getMPEZone() const
{
    return mpeZone;
}

int SvkMidiProcessor::getPitchBendNoteMax() const
{
    return pitchBendNoteMax;
}

int SvkMidiProcessor::getPitchBendGlobalMax() const
{
    return pitchBendGlobalMax;
}

int SvkMidiProcessor::getPitchTrackingMode() const
{
    return mpePitchTrackingMode;
}

int SvkMidiProcessor::getPressureTrackingMode() const
{
    return mpePressureTrackingMode;
}

int SvkMidiProcessor::getTimbreTrackingMode() const
{
    return mpeTimbreTrackingMode;
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

String SvkMidiProcessor::setMidiInput(String deviceID)
{
    midiInput = MidiInput::openDevice(deviceID, this);
    midiInputName = midiInput->getName();
    if (midiInputName.isNotEmpty())
    {
        inputSelected = deviceID;
        midiSettingsNode.setProperty(IDs::midiInputName, midiInput->getIdentifier(), nullptr);
    }
    else
    {
        inputSelected = "";
        midiInput = nullptr;
    }
    
    return midiInputName;
}

String SvkMidiProcessor::setMidiOutput(String deviceID)
{
    midiOutput = MidiOutput::openDevice(deviceID);
    
    if (midiOutput.get())
    {
        midiOutputName = midiOutput->getName();
        outputSelected = deviceID;
        midiSettingsNode.setProperty(IDs::midiOutputName, midiOutput->getIdentifier(), nullptr);
    }
    else
    {
        midiOutputName = "";
        outputSelected = "";
        midiOutput = nullptr;
    }
    
    return midiOutputName;
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
    useModePeriod = useModeSizeIn;
}

void SvkMidiProcessor::setMidiChannelOut(int channelOut)
{
    if (channelOut < 1)
        channelOut = 1;
    
    channelOut = channelOut % 16;
    
    midiSettingsNode.setProperty(IDs::pianoMidiChannel, channelOut, nullptr);
    midiChannelOut = channelOut;
}

void SvkMidiProcessor::setTransposeAmt(int notesToTranspose)
{
    transposeAmt = notesToTranspose;
}

void SvkMidiProcessor::setMPEOn(bool turnOnMPE)
{
    mpeOn = turnOnMPE;
}

void SvkMidiProcessor::setMPEThru(bool mpeOnThru)
{
    mpeThru = mpeOnThru;
}

void SvkMidiProcessor::setMPELegacy(bool turnOnLegacy)
{
    mpeLegacyOn = turnOnLegacy;
}

void SvkMidiProcessor::setMPEZone(MPEZoneLayout zoneIn)
{
    mpeZone = zoneIn;
}

void SvkMidiProcessor::setPitchBendNoteMax(int bendAmtIn)
{
    pitchBendNoteMax = bendAmtIn;
}

void SvkMidiProcessor::setPitchBendGlobalMax(int bendAmtIn)
{
    pitchBendGlobalMax = bendAmtIn;
}

void SvkMidiProcessor::setPitchTrackingMode(int modeIn)
{
    mpePitchTrackingMode = modeIn;
}

void SvkMidiProcessor::setPressureTrackingMode(int modeIn)
{
    mpePressureTrackingMode = modeIn;
}

void SvkMidiProcessor::setTimbreTrackingMode(int modeIn)
{
    mpeTimbreTrackingMode = modeIn;
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
            originalKeyboardState->processNextMidiEvent(msg);
            midiNote = msg.getNoteNumber();
            
            if (isInputRemapped)
                midiNote = midiInputFilter->getNoteRemapped(msg.getNoteNumber());
                        
            msg.setNoteNumber(midiNote);
            
            if (midiNote >= 0 && midiNote < 128)
            {
                msg.setTimeStamp(++msgCount);
                remappedKeyboardState->processNextMidiEvent(msg);
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
            int midiNote = msg.getNoteNumber();
            midiNote = midiOutputFilter->getNoteRemapped(msg.getNoteNumber());
            midiNote += periodShift * mode2->getScaleSize();
            msg.setNoteNumber(midiNote);
            msg.setTimeStamp(++msgCount);
            
            
            
            midiMessages.addEvent(msg, smpl);
            sendMsgToOutputs(msg);
        }
    }
    
    midiBuffer.clear();
    msgCount = 0;
}

void SvkMidiProcessor::sendMsgToOutputs(const MidiMessage& msg)
{
    if (midiOutput)
        midiOutput->sendMessageNow(msg);
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
    
    addMessageToQueue(myMsg);
}
