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
    
    midiInputFilter.reset(new MidiFilter());
    midiInputRemap.reset(new MidiFilter());
    midiOutputFilter.reset(new MidiFilter());

    midiInput = nullptr;
    midiOutput = nullptr;
    
    originalKeyboardState.reset(new MidiKeyboardState());
    remappedKeyboardState.reset(new MidiKeyboardState());
        
	//retuner.reset(new TunedNoteInterpreter());
 //   retuner->setPitchBendMax(48);
 //   
 //   mpeInst.reset(new MPEInstrument());
 //   channelAssigner.reset(new SvkMpeChannelAssigner(mpeInst.get()));
 //   channelAssigner->setIgnorePitchbend(true); // temporary

    // default sample rate
    reset(41000);
    
    //mpeZone.setLowerZone(maxNumVoices, pitchBendNoteMax, pitchBendGlobalMax);
    //mpeInst->setZoneLayout(mpeZone);
    //updateMPEMode();
}

SvkMidiProcessor::~SvkMidiProcessor()
{
    
}

void SvkMidiProcessor::connectToParameters()
{
	Array<Identifier> params(
		{
			IDs::periodShift,
			IDs::periodShiftModeSize,
			IDs::transposeAmt,
			IDs::keyboardMidiChannel,
			IDs::mpeOn,
			IDs::mappingMode
		});

	for (auto param : params)
	{
		svkTree.addParameterListener(param, this);
	}
	DBG("MidiProcessor connected to parameters");
}

void SvkMidiProcessor::updateNode()
{
    //midiSettingsNode.setProperty(IDs::mpeThru, mpeThru, nullptr);
    //midiSettingsNode.setProperty(IDs::mpeZone, 0 /*do something here*/, nullptr);
    //midiSettingsNode.setProperty(IDs::mpeLegacyOn, mpeInst->isLegacyModeEnabled(), nullptr);
    //midiSettingsNode.setProperty(IDs::pitchBendNoteMax, pitchBendNoteMax, nullptr);
    //midiSettingsNode.setProperty(IDs::pitchBendGlobalMax, mpeInst->getLegacyModePitchbendRange(), nullptr);
    //midiSettingsNode.setProperty(IDs::mpePitchTrackingMode, mpePitchbendTrackingMode, nullptr);
    //midiSettingsNode.setProperty(IDs::mpePressureTrackingMode, mpePressureTrackingMode, nullptr);
    //midiSettingsNode.setProperty(IDs::mpeTimbreTrackingMode, mpeTimbreTrackingMode, nullptr);
    //midiSettingsNode.setProperty(IDs::mpeTuningPreserveMidiNote, mpeTuningPreserveMidiNote, nullptr);
    
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
		midiSettingsNode = midiSettingsNodeIn;
      
		periodShift = midiSettingsNode[IDs::periodShift];
		midiChannelOut = jlimit(1, 16, (int) midiSettingsNode[IDs::keyboardMidiChannel]);

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

int SvkMidiProcessor::getVoiceLimit() const
{
    return maxNumVoices;
}

bool SvkMidiProcessor::isRetuning() const
{
    return doRetuning;
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
//
//MPEInstrument* SvkMidiProcessor::getMPEInstrument()
//{
//    return mpeInst.get();
//}

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
	updateNoteTransposition();
}

void SvkMidiProcessor::setPeriodShift(int periodsToShift)
{
	periodShift = periodsToShift;
	midiSettingsNode.setProperty(IDs::periodShift, periodShift, nullptr);
	updateNoteTransposition();
}

void SvkMidiProcessor::setMidiChannelOut(int virtualKeyboardMidiChannelOut)
{
	midiChannelOut = virtualKeyboardMidiChannelOut;
	midiSettingsNode.setProperty(IDs::keyboardMidiChannel, midiChannelOut, nullptr);

	// TODO: remove notes or better yet...use a note queue for the proper note off
}

void SvkMidiProcessor::updateNoteTransposition()
{
	currentNoteShift = transposeAmt;
	if (periodShiftModeSize)
		currentNoteShift += periodShift * mode2->getModeSize();
	else
		currentNoteShift += periodShift * mode2->getScaleSize();

	// TODO: handle sustained notes
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
    
	// Add external input into queue
    while (inputEvents.getNextEvent(msg, smpl))
    {
		if (msg.isNoteOnOrOff())
		{
			midiNote = msg.getNoteNumber();

			if (isInputFiltered)
			{
				midiNote = midiInputFilter->getNoteRemapped(midiNote);
			}

			originalKeyboardState->processNextMidiEvent(msg);

			if (isInputRemapped)
			{
				midiNote = midiInputRemap->getNoteRemapped(midiNote);
			}

			if (midiNote < 0 || midiNote > 127)
				continue;

			msg.setNoteNumber(midiNote);
			remappedKeyboardState->processNextMidiEvent(msg);
		}

        addMessageToQueue(msg);
    }

	// Combine all MIDI input
    MidiBuffer preBuffer;
    removeNextBlockOfMessages(preBuffer, 4096);
	midiMessages.clear();

	// Output Filtering on all MIDI events
    auto allMidiEvents = MidiBuffer::Iterator(preBuffer);
    while (allMidiEvents.getNextEvent(msg, smpl))
    {
        // Process transpositions
		if (msg.isNoteOnOrOff())
		{
			midiNote = msg.getNoteNumber() + currentNoteShift;

			if (isOutputFiltered)
			{
				midiNote = midiOutputFilter->getNoteRemapped(midiNote);
			}

			if (midiNote < 0 || midiNote > 127)
				continue;

			msg.setNoteNumber(midiNote);
		}

		midiMessages.addEvent(msg, smpl);
    }

    sendBufferToOutputs(midiMessages);
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
	if (paramID == IDs::mappingMode.toString())
	{
		isInputRemapped = newValue > 1;
	}
    else if (paramID == IDs::periodShift.toString())
    {
        periodShift = newValue;
		updateNoteTransposition();
    }
    else if (paramID == IDs::periodShiftModeSize.toString())
    {
        periodShiftModeSize = newValue;
    }
    else if (paramID == IDs::transposeAmt.toString())
    {
        transposeAmt = newValue;
		updateNoteTransposition();
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
    
    DBG("Midi processor updated parameter " + paramID + " to " + String(newValue));
}

void SvkMidiProcessor::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage msg = MidiMessage::noteOn(midiChannelOut, midiNoteNumber, velocity);
    msg.setTimeStamp(++msgCount);
    addMessageToQueue(msg);
}

void SvkMidiProcessor::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage msg = MidiMessage::noteOn(midiChannelOut, midiNoteNumber, velocity);
    msg.setTimeStamp(++msgCount);
    addMessageToQueue(msg);
}

void SvkMidiProcessor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg)
{
    MidiMessage myMsg = MidiMessage(msg);
    ++msgCount;
    addMessageToQueue(myMsg);
}