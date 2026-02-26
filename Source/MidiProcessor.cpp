/*
  ==============================================================================

    MidiProcessor.cpp
    Created: 21 May 2019 6:37:57pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "MidiProcessor.h"

SvkMidiProcessor::SvkMidiProcessor(SvkState& stateIn)
    : state(stateIn)
{
    midiSettingsNode = ValueTree(SvkProperty::midiSettingsNode);
    midiMapNode = ValueTree(SvkProperty::midiMapNode);
    midiDeviceNode = ValueTree(SvkProperty::midiDeviceSettingsNode);
    
    midiInputFilter.reset(new MidiFilter());
    midiInputRemap.reset(new MidiFilter());
    midiOutputFilter.reset(new MidiFilter());

    midiInput = nullptr;
    midiOutput = nullptr;
    
    originalKeyboardState.reset(new MidiKeyboardState());
    remappedKeyboardState.reset(new MidiKeyboardState());

    startTime = Time::getMillisecondCounterHiRes();

    // Create all-notes-off buffer
    for (int i = 0; i < 128; i++)
        for (int ch = 1; ch <= 16; ch++)
            allNotesOffBuffer.addEvent(MidiMessage::noteOff(ch, i), ch * 16 + i);
}

SvkMidiProcessor::~SvkMidiProcessor()
{
    
}

//==============================================================================

ValueTree SvkMidiProcessor::getNode() const
{
    ValueTree midiNode(SvkProperty::midiSettingsNode);
    midiNode.addChild(midiMapNode, 0, nullptr);
    midiNode.addChild(midiDeviceNode, 1, nullptr);
    return midiNode;
}

void SvkMidiProcessor::updateNodes()
{
    //midiSettingsNode.setProperty(SvkProperty::mpeThru, mpeThru, nullptr);
    //midiSettingsNode.setProperty(SvkProperty::mpeZone, 0 /*do something here*/, nullptr);
    //midiSettingsNode.setProperty(SvkProperty::mpeLegacyOn, mpeInst->isLegacyModeEnabled(), nullptr);
    //midiSettingsNode.setProperty(SvkProperty::pitchBendNoteMax, pitchBendNoteMax, nullptr);
    //midiSettingsNode.setProperty(SvkProperty::pitchBendGlobalMax, mpeInst->getLegacyModePitchbendRange(), nullptr);
    //midiSettingsNode.setProperty(SvkProperty::mpePitchTrackingMode, mpePitchbendTrackingMode, nullptr);
    //midiSettingsNode.setProperty(SvkProperty::mpePressureTrackingMode, mpePressureTrackingMode, nullptr);
    //midiSettingsNode.setProperty(SvkProperty::mpeTimbreTrackingMode, mpeTimbreTrackingMode, nullptr);
    //midiSettingsNode.setProperty(SvkProperty::mpeTuningPreserveMidiNote, mpeTuningPreserveMidiNote, nullptr);
    
    NoteMap standardNoteMap;

    NoteMap* inputFilter = midiInputFilter->getNoteMap();
    midiMapNode.removeChild(midiMapNode.getChildWithName(SvkProperty::midiInputFilter), nullptr);
    if (*inputFilter != standardNoteMap)
        midiMapNode.appendChild(inputFilter->getAsValueTree(SvkProperty::midiInputFilter), nullptr);
    
    NoteMap* inputRemap = midiInputRemap->getNoteMap();
    midiMapNode.removeChild(midiMapNode.getChildWithName(SvkProperty::midiInputRemap), nullptr);
    if (*inputRemap != standardNoteMap)
        midiMapNode.appendChild(inputRemap->getAsValueTree(SvkProperty::midiInputRemap), nullptr);

    NoteMap* outputFilter = midiOutputFilter->getNoteMap();
    midiMapNode.removeChild(midiMapNode.getChildWithName(SvkProperty::midiOutputFilter), nullptr);
    if (*outputFilter != standardNoteMap)
        midiMapNode.appendChild(outputFilter->getAsValueTree(SvkProperty::midiOutputFilter), nullptr);
}


bool SvkMidiProcessor::restoreSettingsNode(ValueTree midiSettingsNodeIn)
{
    if (midiSettingsNodeIn.hasType(SvkProperty::midiSettingsNode))
    {
        midiSettingsNode = midiSettingsNodeIn;
      
        periodShift = midiSettingsNode[SvkProperty::periodShift];
        transposeAmt = midiSettingsNode[SvkProperty::transposeAmt];
        midiChannelOut = jlimit(1, 16, (int) midiSettingsNode[SvkProperty::keyboardMidiChannel]);
        // Note transposition is updated when new mode is applied

        return true;
    }
    
    return false;
}

bool SvkMidiProcessor::restoreMappingNode(ValueTree midiMapIn)
{
    bool loadedMap = false;

    if (midiMapIn.hasType(SvkProperty::midiMapNode))
    {
        midiMapNode = midiMapIn;

        Array<int> map;

        NoteMap noteMap;

        //get_array_from_node(midiMapIn, map, SvkProperty::midiInputFilter);
        //
        //if (map.size() > 0)
        //    setInputFilter(map);
        //else
        //    resetInputFilter();
        //
        //map.clear();

        ValueTree remapNode = midiMapIn.getChildWithName(SvkProperty::midiInputRemap);
        noteMap = NoteMap(midiMapIn.getChildWithName(SvkProperty::midiInputRemap));

        setInputRemap(noteMap);

        if (remapNode.isValid() && remapNode.getNumChildren() > 0)
            loadedMap = true;

        //map.clear();
        //get_array_from_node(midiMapIn, map, SvkProperty::midiOutputFilter);
        //
        //if (map.size() > 0)
        //    setOutputFilter(map);
        //else
        //    resetOutputFilter();
    }

    return loadedMap;
}

bool SvkMidiProcessor::restoreDevicesNode(ValueTree midiDevicesNodeIn)
{
    // TODO: check if devices exist, give a name to imply if it doesn't
    if (JUCEApplication::isStandaloneApp && midiDevicesNodeIn.hasType(SvkProperty::midiDeviceSettingsNode))
    {
        midiDeviceNode = midiDevicesNodeIn;

        setMidiInput(midiDeviceNode[SvkProperty::midiInputName]);
        setMidiOutput(midiDeviceNode[SvkProperty::midiOutputName]);

        return true;
    }

    return false;
}

//==============================================================================

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
    std::unique_ptr<MidiInput> inputToOpen = MidiInput::openDevice(deviceID, this);

    if (inputToOpen.get())
    {
        if (midiInput.get())
            midiInput->stop();

        midiInput.swap(inputToOpen);
        midiInput->start();

        midiInputName = midiInput->getName();
        inputSelected = deviceID;
        midiDeviceNode.setProperty(SvkProperty::midiInputName, inputSelected, nullptr);
        DBG("Successfully opened input " + midiInputName);
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
    std::unique_ptr<MidiOutput> outputToOpen = MidiOutput::openDevice(deviceID);

    if (outputToOpen.get())
    {
        midiOutput.swap(outputToOpen);
        midiOutputName = midiOutput->getName();
        outputSelected = deviceID;
        midiDeviceNode.setProperty(SvkProperty::midiOutputName, outputSelected, nullptr);
        DBG("Successfully opened output " + midiOutputName);
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

//void SvkMidiProcessor::setMode1(Mode* mode1In)
//{
//    mode1 = mode1In;
//}
//
//void SvkMidiProcessor::setMode2(Mode* getMode2(modeIn)
//{
//    if (modeIn == nullptr)
//        modeIn = getMode1();
//    mode2 = modeIn;
//    updateNoteTransposition();
//}

void SvkMidiProcessor::setPeriodShift(int periodsToShift)
{
    periodShift = periodsToShift;
    midiSettingsNode.setProperty(SvkProperty::periodShift, periodShift, nullptr);
    updateNoteTransposition();
}

void SvkMidiProcessor::setTransposeAmt(int transposeAmtIn)
{
    transposeAmt = transposeAmtIn;
    midiSettingsNode.setProperty(SvkProperty::transposeAmt, transposeAmt, nullptr);
    updateNoteTransposition();
}

void SvkMidiProcessor::setMidiChannelOut(int virtualKeyboardMidiChannelOut)
{
    midiChannelOut = virtualKeyboardMidiChannelOut;
    midiSettingsNode.setProperty(SvkProperty::keyboardMidiChannel, midiChannelOut, nullptr);

    // TODO: remove notes or better yet...use a note queue for the proper note off
}

void SvkMidiProcessor::updateNoteTransposition()
{
    currentNoteShift = transposeAmt;
    auto* mode2 = state.getMode2();
    if (mode2 != nullptr)
    {
        if (periodShiftModeSize)
            currentNoteShift += periodShift * mode2->getModeSize();
        else
            currentNoteShift += periodShift * mode2->getScaleSize();
    }
    // TODO: handle sustained notes
}

//void SvkMidiProcessor::setRetuneOn(bool retuneOn)
//{
//    doRetuning = retuneOn;
//}

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

void SvkMidiProcessor::setInputFilter(Array<int> mapIn, bool updateNode)
{
    setInputFilter(NoteMap(mapIn));
}

void SvkMidiProcessor::setInputFilter(NoteMap mapIn, bool updateNode)
{
    midiInputFilter->setNoteMap(mapIn);

    if (updateNode)
    {
        midiMapNode.removeChild(midiMapNode.getChildWithName(SvkProperty::midiInputFilter), nullptr);
        add_array_to_node(midiMapNode, midiInputFilter->getNoteMap()->getValues(), SvkProperty::midiInputFilter, "Note");
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
        midiMapNode.removeChild(midiMapNode.getChildWithName(SvkProperty::midiInputRemap), nullptr);
        midiMapNode.appendChild(mapIn.getAsValueTree(SvkProperty::midiInputRemap), nullptr);
        //add_array_to_node(midiMapNode, midiInputRemap->getNoteMap()->getValues(), SvkProperty::midiInputRemap, "Note");
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
        midiMapNode.removeChild(midiMapNode.getChildWithName(SvkProperty::midiOutputFilter), nullptr);
        midiMapNode.appendChild(mapIn.getAsValueTree(SvkProperty::midiOutputFilter), nullptr);
        //add_array_to_node(midiMapNode, midiInputRemap->getNoteMap()->getValues(), SvkProperty::midiOutputFilter, "Note");
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

void SvkMidiProcessor::setMappingHelper(MappingHelper* helperIn)
{
    mappingHelper = helperIn;
}

//==============================================================================

void SvkMidiProcessor::processMidi(MidiBuffer &midiMessages)
{
    // TODO: handle note offs if period/transpose is changed before note offs
    
    // Combine external inputs
    midiMessages.addEvents(externalInputBuffer, 0, numInputMsgs, 0);
    externalInputBuffer.clear();
    numInputMsgs = 0;

    // Process external input
    MidiBuffer combinedMessages;
    MidiMessage msg;
    int midiNote;
    
    // Add external input into queue
    for (auto msgData : midiMessages)
    {
        msg = msgData.getMessage();

        if (msg.isNoteOnOrOff())
        {
            midiNote = msg.getNoteNumber();

            if ((int)midiMapNode[SvkProperty::mappingMode] == 3)
            {
                if (mappingHelper->isWaitingForKeyInput())
                {
                    MessageManagerLock lock;
                    mappingHelper->mapPreparedKeyToNote(midiNote, false);
                    originalKeyboardState->processNextMidiEvent(msg);
                    continue;
                }
            }

            if (isInputFiltered)
            {
                midiNote = midiInputFilter->getNoteRemapped(midiNote);
            }

            originalKeyboardState->processNextMidiEvent(msg);

            if (isInputRemapped)
            {
                midiNote = midiInputRemap->getNoteRemapped(midiNote);
            }

            msg.setNoteNumber(midiNote);
            remappedKeyboardState->processNextMidiEvent(msg);
        }

        combinedMessages.addEvent(msg, msgData.samplePosition);
    }

    // Output Filtering on all MIDI events
    for (auto msgData : svkBuffer)
    {
        if (!msgData.numBytes)
            continue;

        msg = msgData.getMessage();

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

        combinedMessages.addEvent(msg, msgData.samplePosition);
    }

    svkBuffer.clear();
    numSvkMsgs = 0;

    midiMessages.swapWith(combinedMessages);
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
    auto events = MidiBuffer::Iterator(allNotesOffBuffer);
    MidiMessage msg;
    int smplNum;
    while (events.getNextEvent(msg, smplNum))
    {
        externalInputBuffer.addEvent(msg, numInputMsgs++);
    }
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
    if (paramID == SvkProperty::mappingMode.toString())
    {
        isInputRemapped = newValue > 1;
    }
    else if (paramID == SvkProperty::periodShift.toString())
    {
        periodShift = newValue;
        updateNoteTransposition();
    }
    else if (paramID == SvkProperty::periodShiftModeSize.toString())
    {
        periodShiftModeSize = newValue;
    }
    else if (paramID == SvkProperty::transposeAmt.toString())
    {
        transposeAmt = newValue;
        updateNoteTransposition();
    }
    else if (paramID == SvkProperty::keyboardMidiChannel.toString())
    {
        //allNotesOff(midiChannelOut);
        midiChannelOut = newValue;
    }
    else if (paramID == SvkProperty::mpeOn.toString())
    {
        mpeOn = newValue;
    }
    
    DBG("Midi processor updated parameter " + paramID + " to " + String(newValue));
}

void SvkMidiProcessor::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage msg = MidiMessage::noteOn(midiChannelOut, midiNoteNumber, velocity);
    msg.setTimeStamp(Time::getMillisecondCounterHiRes() - startTime);
    svkBuffer.addEvent(msg, numSvkMsgs++);
}

void SvkMidiProcessor::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage msg = MidiMessage::noteOff(midiChannelOut, midiNoteNumber, velocity);
    msg.setTimeStamp(Time::getMillisecondCounterHiRes() - startTime);
    svkBuffer.addEvent(msg, numSvkMsgs++);
}

void SvkMidiProcessor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg)
{
    MidiMessage myMsg = MidiMessage(msg);
    externalInputBuffer.addEvent(myMsg, numInputMsgs++);
}