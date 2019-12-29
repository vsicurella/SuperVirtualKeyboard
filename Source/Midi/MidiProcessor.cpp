/*
  ==============================================================================

    MidiProcessor.cpp
    Created: 21 May 2019 6:37:57pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "MidiProcessor.h"

SvkMidiProcessor::SvkMidiProcessor(AudioProcessorValueTreeState& svkTreeIn)
    : svkTree(svkTreeIn)
{
    midiSettingsNode = ValueTree(IDs::midiSettingsNode);
    midiMapNode = ValueTree(IDs::midiMapNode);

	midiSettingsNode.addChild(midiMapNode, -1, nullptr);
    
    midiInputFilter.reset(new MidiFilter());
    midiInputRemap.reset(new MidiFilter());
    midiOutputFilter.reset(new MidiFilter());

    midiInput = nullptr;
    midiOutput = nullptr;
    
    originalKeyboardState.reset(new MidiKeyboardState());
    remappedKeyboardState.reset(new MidiKeyboardState());
        
	retuner.reset(new TunedNoteInterpreter());
    retuner->setPitchBendMax(48);
    
    mpeInst.reset(new MPEInstrument());
    channelAssigner.reset(new SvkMpeChannelAssigner(mpeInst.get()));
    channelAssigner->setIgnorePitchbend(true); // temporary
    
    svkTree.addParameterListener(IDs::periodShift, this);
    svkTree.addParameterListener(IDs::periodShiftModeSize, this);
    svkTree.addParameterListener(IDs::transposeAmt, this);
    svkTree.addParameterListener(IDs::keyboardMidiChannel, this);
    svkTree.addParameterListener(IDs::mpeOn, this);

    // default sample rate
    reset(41000);
    
    mpeZone.setLowerZone(maxNumVoices, pitchBendNoteMax, pitchBendGlobalMax);
    mpeInst->setZoneLayout(mpeZone);
    updateMPEMode();
}

SvkMidiProcessor::~SvkMidiProcessor()
{
    
}

void SvkMidiProcessor::updateNode()
{
    //midiSettingsNode.setProperty(IDs::mpeThru, mpeThru, nullptr);
    midiSettingsNode.setProperty(IDs::mpeZone, 0 /*do something here*/, nullptr);
    midiSettingsNode.setProperty(IDs::mpeLegacyOn, mpeInst->isLegacyModeEnabled(), nullptr);
    midiSettingsNode.setProperty(IDs::pitchBendNoteMax, pitchBendNoteMax, nullptr);
    midiSettingsNode.setProperty(IDs::pitchBendGlobalMax, mpeInst->getLegacyModePitchbendRange(), nullptr);
    midiSettingsNode.setProperty(IDs::mpePitchTrackingMode, mpePitchbendTrackingMode, nullptr);
    midiSettingsNode.setProperty(IDs::mpePressureTrackingMode, mpePressureTrackingMode, nullptr);
    midiSettingsNode.setProperty(IDs::mpeTimbreTrackingMode, mpeTimbreTrackingMode, nullptr);
    midiSettingsNode.setProperty(IDs::mpeTuningPreserveMidiNote, mpeTuningPreserveMidiNote, nullptr);
    
    Array<int> inputFilter = midiInputFilter->getRemappedNotes();
    midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputFilter), nullptr);
    add_array_to_node(midiMapNode, inputFilter, IDs::midiInputFilter, "Note");
    
    Array<int> inputRemap = midiInputRemap->getRemappedNotes();
    midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputRemap), nullptr);
    add_array_to_node(midiMapNode, inputRemap, IDs::midiInputRemap, "Note");
    
    Array<int> outputFilter = midiOutputFilter->getRemappedNotes();
    midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiOutputFilter), nullptr);
    add_array_to_node(midiMapNode, outputFilter, IDs::midiOutputFilter, "Note");
}

bool SvkMidiProcessor::restoreFromNode(ValueTree midiSettingsNodeIn)
{
    if (midiSettingsNodeIn.hasType(IDs::midiSettingsNode))
    {
        //midiSettingsNode.copyPropertiesAndChildrenFrom(midiSettingsNodeIn, nullptr);
        
        // Root note
        //rootMidiNote = midiSettingsNode.getProperty(IDs::rootMidiNote);
        
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

int SvkMidiProcessor::getPitchbendNoteMax() const
{
    return pitchBendNoteMax;
}

int SvkMidiProcessor::getPitchTrackingMode() const
{
    return mpePitchbendTrackingMode;
}

int SvkMidiProcessor::getPressureTrackingMode() const
{
    return mpePressureTrackingMode;
}

int SvkMidiProcessor::getTimbreTrackingMode() const
{
    return mpeTimbreTrackingMode;
}

int SvkMidiProcessor::getVoiceLimit() const
{
    return maxNumVoices;
}

bool SvkMidiProcessor::isRetuning() const
{
    return doRetuning;
}

int SvkMidiProcessor::isTuningPreservesMidiNote() const
{
    return mpeTuningPreserveMidiNote;
}

NoteMap* SvkMidiProcessor::getInputNoteRemap()
{
	return midiInputRemap->getNoteMap();
}

NoteMap* SvkMidiProcessor::getOutputNoteFilter()
{
	return midiOutputFilter->getNoteMap();
}

MidiFilter* SvkMidiProcessor::getInputRemapMidiFilter()
{
	return midiInputRemap.get();
}

MidiFilter* SvkMidiProcessor::getOutputMidiFilter()
{
	return midiOutputFilter.get();
}

MPEInstrument* SvkMidiProcessor::getMPEInstrument()
{
    return mpeInst.get();
}

int SvkMidiProcessor::getInputNote(int midiNoteIn)
{
    return midiInputRemap->getNoteRemapped(midiNoteIn);
}

int SvkMidiProcessor::getOutputNote(int midiNoteIn)
{
    return midiOutputFilter->getNoteRemapped(midiNoteIn);
}

//==============================================================================

String SvkMidiProcessor::setMidiInput(String deviceID)
{
    //midiInput = MidiInput::openDevice(deviceID, this);
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

void SvkMidiProcessor::setTuning(Tuning* tuningIn)
{
	tuning = tuningIn;
	retuner->resetTuning(tuning);
}

void SvkMidiProcessor::updateMPEMode()
{
    if (isMPEOn())
    {
        mpeInst->setZoneLayout(mpeZone);
        channelAssigner->allNotesOff();
        // TODO: actually turn notes off
    }
    else
    {
        mpeInst->setLegacyModeChannelRange(Range<int>(1, 2));
        channelAssigner->allNotesOff();
    }
}

void SvkMidiProcessor::setPitchBendNoteMax(int bendAmtIn)
{
    pitchBendNoteMax = bendAmtIn;
    retuner->setPitchBendMax(pitchBendNoteMax);
}

void SvkMidiProcessor::setPitchBendGlobalMax(int bendAmtIn)
{
    pitchBendGlobalMax = bendAmtIn;
    //mpeInst->setLegacyModePitchbendRange(bendAmtIn);
}

void SvkMidiProcessor::setTuningPreservesMidiNote(bool preserveMidiNote)
{
    mpeTuningPreserveMidiNote = preserveMidiNote;
}

void SvkMidiProcessor::setPitchbendTrackingMode(int modeIn)
{
    mpePitchbendTrackingMode = modeIn;
    mpeInst->setPitchbendTrackingMode(MPEInstrument::TrackingMode(mpePitchbendTrackingMode));
}

void SvkMidiProcessor::setPressureTrackingMode(int modeIn)
{
    mpePressureTrackingMode = modeIn;
    mpeInst->setPressureTrackingMode(MPEInstrument::TrackingMode(mpePressureTrackingMode));
}

void SvkMidiProcessor::setTimbreTrackingMode(int modeIn)
{
    mpeTimbreTrackingMode = modeIn;
    mpeInst->setTimbreTrackingMode(MPEInstrument::TrackingMode(mpeTimbreTrackingMode));
}

void SvkMidiProcessor::setVoiceLimit(int maxVoicesIn)
{
    maxNumVoices = maxVoicesIn;
    mpeZone.setLowerZone(maxVoicesIn, pitchBendNoteMax, pitchBendGlobalMax);
    mpeInst->setZoneLayout(mpeZone);
}

void SvkMidiProcessor::setRetuneOn(bool retuneOn)
{
    doRetuning = retuneOn;
}

void SvkMidiProcessor::setInputToFilter(bool doRemap)
{
    isInputFiltered = doRemap;
}

void SvkMidiProcessor::setInputToRemap(bool doRemap)
{
    isInputRemapped = doRemap;
}

void SvkMidiProcessor::setOutputToFilter(bool doFilter)
{
    isOutputFiltered = doFilter;
}

void SvkMidiProcessor::setMidiMaps(ValueTree midiMapIn)
{
    if (midiMapIn.hasType(IDs::midiMapNode))
    {
        Array<int> map;
        
        get_array_from_node(midiMapIn, map, IDs::midiInputFilter);
        
        if (map.size() > 0)
            setInputFilter(map);
        else
            resetInputFilter();
        
        map.clear();
        get_array_from_node(midiMapIn, map, IDs::midiInputRemap);
        
        if (map.size() > 0)
            setInputRemap(map);
        else
            resetInputMap();
        
        map.clear();
        get_array_from_node(midiMapIn, map, IDs::midiOutputFilter);
        
        if (map.size() > 0)
            setOutputFilter(map);
        else
            resetOutputFilter();
        
        midiMapNode.copyPropertiesAndChildrenFrom(midiMapIn, nullptr);
    }
}

void SvkMidiProcessor::setInputFilter(Array<int> mapIn, bool updateNode)
{
    setInputFilter(NoteMap(mapIn));
}

void SvkMidiProcessor::setInputFilter(NoteMap mapIn, bool updateNode)
{
    midiInputFilter->setNoteMap(mapIn);

    if (updateNode)
    {
        midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputFilter), nullptr);
        add_array_to_node(midiMapNode, midiInputFilter->getNoteMap()->getValues(), IDs::midiInputFilter, "Note");
    }
}

void SvkMidiProcessor::setInputRemap(Array<int> mapIn, bool updateNode)
{
    setInputRemap(NoteMap(mapIn));
}

void SvkMidiProcessor::setInputRemap(NoteMap mapIn, bool updateNode)
{
	midiInputRemap->setNoteMap(mapIn);

	if (updateNode)
	{
		midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputRemap), nullptr);
		add_array_to_node(midiMapNode, midiInputRemap->getNoteMap()->getValues(), IDs::midiInputRemap, "Note");
	}
}

void SvkMidiProcessor::setOutputFilter(Array<int> mapIn, bool updateNode)
{
    setOutputFilter(NoteMap(mapIn));
}

void SvkMidiProcessor::setOutputFilter(NoteMap mapIn, bool updateNode)
{
	midiOutputFilter->setNoteMap(mapIn.getValues());

	if (updateNode)
	{
		midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiOutputFilter), nullptr);
		add_array_to_node(midiMapNode, midiInputRemap->getNoteMap()->getValues(), IDs::midiOutputFilter, "Note");
	}
}

void SvkMidiProcessor::resetInputFilter(bool updateNode)
{
    setInputFilter(NoteMap(), updateNode);
}

void SvkMidiProcessor::resetInputMap(bool updateNode)
{
    setInputRemap(NoteMap(), updateNode);
}

void SvkMidiProcessor::resetOutputFilter(bool updateNode)
{
    setOutputFilter(NoteMap(), updateNode);
}

void SvkMidiProcessor::mapNoteForInputFilter(int noteIn, int noteOut, bool updateNode)
{
    midiInputFilter->setNote(noteIn, noteOut);
    
    if (updateNode)
        set_value_in_array(midiMapNode, IDs::midiInputFilter, noteIn, noteOut);
}

void SvkMidiProcessor::mapNoteForInputRemap(int noteIn, int noteOut, bool updateNode)
{
    midiInputRemap->setNote(noteIn, noteOut);

	if (updateNode)
		set_value_in_array(midiMapNode, IDs::midiInputRemap, noteIn, noteOut);
}

void SvkMidiProcessor::mapNoteForOutputFilter(int noteIn, int noteOut, bool updateNode)
{
    midiOutputFilter->setNote(noteIn, noteOut);

	if (updateNode)
		set_value_in_array(midiMapNode, IDs::midiOutputFilter, noteIn, noteOut);
}

//==============================================================================

void SvkMidiProcessor::processMidi(MidiBuffer &midiMessages)
{
    // TODO: handle note offs if period/transpose is changed before note offs
    
    // Process external input
    auto inputEvents = MidiBuffer::Iterator(midiMessages);
    MidiMessage msg;
    int smpl;
    int midiNote;
    msgCount = 0;
    
    while (inputEvents.getNextEvent(msg, smpl))
    {
        if (isInputFiltered)
        {
            midiNote = midiInputFilter->getNoteRemapped(msg.getNoteNumber());
            msg.setNoteNumber(midiNote);
        }
        
        // check for out of range notes?
        originalKeyboardState->processNextMidiEvent(msg);
    
        if (isInputRemapped)
        {
            midiNote = midiInputRemap->getNoteRemapped(msg.getNoteNumber());
        }
        
        remappedKeyboardState->processNextMidiEvent(msg);
        msg.setTimeStamp(smpl + ++msgCount);
        addMessageToQueue(msg);
    }
    
    MidiBuffer preBuffer;
    removeNextBlockOfMessages(preBuffer, 4096);
    
    auto allMidiEvents = MidiBuffer::Iterator(preBuffer);
    msgCount = 0;
    
    // Output Filtering
    while (allMidiEvents.getNextEvent(msg, smpl))
    {
        // Process transpositions
        
        midiNote = msg.getNoteNumber() + transposeAmt;
        midiNote += (periodShift * mode2->getScaleSize());
        msg.setNoteNumber(midiNote);
        
        if (isOutputFiltered)
        {
            midiNote = midiOutputFilter->getNoteRemapped(msg.getNoteNumber());
            msg.setNoteNumber(midiNote);
        }
    }
    
    if (isMPEOn())
    {
        MidiBuffer mpeBuffer;
        mpeBuffer = convertToMPE(preBuffer);
        preBuffer = mpeBuffer;
    }
    
    midiMessages = preBuffer;
    sendBufferToOutputs(midiMessages);
    msgCount = 0;
}

MidiBuffer SvkMidiProcessor::convertToMPE(const MidiBuffer& bufferIn)
{
    MidiBuffer mpeOut;

    auto midiEvents = MidiBuffer::Iterator(bufferIn);
    MidiMessage msgIn, msgOut, pitchBendOut;
    int smpl;
    int mpeMsgCount = 0;

    MPENote mpeNote;
    MidiPitch midiPitch(-1, 0);
    int pitchBend;
    int newChannel;

    while (midiEvents.getNextEvent(msgIn, smpl))
    {
        if (msgIn.isNoteOff())
        {
            if (mpeTuningPreserveMidiNote || tuning == nullptr)
            {
                midiPitch = MidiPitch(msgIn.getNoteNumber(), 8192);
            }
            else
            {
                midiPitch = retuner->calculateClosestMidiPitchForSemitone(tuning->getNoteInSemitones(msgIn.getNoteNumber()));
            }
            
            newChannel = channelAssigner->noteOff(midiPitch);
            
            msgOut = MidiMessage::noteOff(newChannel, midiPitch.key, (uint8) msgIn.getVelocity());
            
            mpeOut.addEvent(MidiMessage::pitchWheel(newChannel, midiPitch.value), smpl + mpeMsgCount);
            mpeOut.addEvent(msgOut, smpl + ++mpeMsgCount);
        }
        
        else if (msgIn.isNoteOn())
        {
            
			if (tuning == nullptr)
			{
				pitchBend = 8192;
				midiPitch = MidiPitch(msgIn.getNoteNumber(), pitchBend);
			}
            if (mpeTuningPreserveMidiNote)
            {
                pitchBend = retuner->calculatePitchbendFromStdMidiNote(msgIn.getNoteNumber());
                midiPitch = MidiPitch(msgIn.getNoteNumber(), pitchBend);
            }
            else
            {
                midiPitch = retuner->calculateClosestMidiPitchForSemitone(tuning->getNoteInSemitones(msgIn.getNoteNumber()));
            }

            newChannel = channelAssigner->findMidiChannelForNote(midiPitch);
            
            msgOut = MidiMessage::noteOn(newChannel, midiPitch.key, (uint8) msgIn.getVelocity());
            pitchBendOut = MidiMessage::pitchWheel(newChannel, midiPitch.value);
            
            mpeOut.addEvent(pitchBendOut, smpl + mpeMsgCount);
            mpeOut.addEvent(msgOut, smpl + ++mpeMsgCount);
        }
    }

    return mpeOut;
}

void SvkMidiProcessor::sendMsgToOutputs(const MidiMessage& msg)
{
    if (midiOutput)
        midiOutput->sendMessageNow(msg);
}

void SvkMidiProcessor::sendBufferToOutputs(const MidiBuffer& bufferToSend)
{
    if (midiOutput)
    {
        midiOutput->sendBlockOfMessagesNow(bufferToSend);
    }
}

void SvkMidiProcessor::allNotesOff()
{
    for (int i = 1; i <= 16; i++)
    {
        addMessageToQueue(MidiMessage::allNotesOff(i));
    }
}

void SvkMidiProcessor::allNotesOff(int channelNumber)
{
    addMessageToQueue(MidiMessage::allNotesOff(channelNumber));
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

void SvkMidiProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == IDs::periodShift.toString())
    {
        periodShift = newValue;
    }
    else if (paramID == IDs::periodShiftModeSize.toString())
    {
        periodShiftModeSIze = newValue;
    }
    else if (paramID == IDs::transposeAmt.toString())
    {
        transposeAmt = newValue;
    }
    else if (paramID == IDs::keyboardMidiChannel.toString())
    {
        //allNotesOff(midiChannelOut);
        midiChannelOut = newValue;
    }
    else if (paramID == IDs::mpeOn.toString())
    {
        mpeOn = newValue;
    }
    
    DBG("Midi processor updated");
}

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
    ++msgCount;
    addMessageToQueue(myMsg);
}
//
//
//void SvkMidiProcessor::noteAdded(MPENote newNote)
//{
//    MidiMessage msgTuned = MidiMessage::noteOn(newNote.midiChannel, newNote.initialNote, (uint8) newNote.noteOnVelocity.as7BitInt());
//
//    MidiMessage pitchbend = MidiMessage::pitchWheel(newNote.midiChannel, tuning->getPitchBendAtMidiNote(newNote.initialNote));
//    addMessageToQueue(pitchbend);
//    addMessageToQueue(msgTuned);
//}
//
//void SvkMidiProcessor::noteReleased (MPENote finishedNote)
//{
//    MidiMessage msgOff = MidiMessage::noteOff(finishedNote.midiChannel, finishedNote.initialNote, (uint8) finishedNote.noteOffVelocity.as7BitInt());
//    addMessageToQueue(msgOff);
//}
