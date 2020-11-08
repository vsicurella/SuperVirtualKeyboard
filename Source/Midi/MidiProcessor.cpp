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
    midiInputFilter.reset(new MidiFilter());
    midiInputRemap.reset(new MidiFilter());
    midiOutputFilter.reset(new MidiFilter());

    midiInput = nullptr;
    midiOutput = nullptr;
    
    originalKeyboardState.reset(new MidiKeyboardState());
    remappedKeyboardState.reset(new MidiKeyboardState());

    notesOnPerChannel.resize(16);

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

void SvkMidiProcessor::updateNodes()
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
    
    // Unused so far / TODO
    //NoteMap* inputFilter = midiInputFilter->getNoteMap();
    //midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputFilter), nullptr);
    //if (*inputFilter != standardNoteMap)
    //    midiMapNode.appendChild(inputFilter->getAsValueTree(IDs::midiInputFilter), nullptr);
    
    midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputRemap), nullptr);
    if (inManualMappingMode || (manualRemap != currentRemap))
        midiMapNode.appendChild(manualRemap.getAsValueTree(IDs::midiInputRemap), nullptr);

    // Unused so far / TODO
    //NoteMap* outputFilter = midiOutputFilter->getNoteMap();
    //midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiOutputFilter), nullptr);
    //if (*outputFilter != standardNoteMap)
    //    midiMapNode.appendChild(outputFilter->getAsValueTree(IDs::midiOutputFilter), nullptr);
}


bool SvkMidiProcessor::restoreSettingsNode(ValueTree midiSettingsNodeIn)
{
    if (midiSettingsNodeIn.hasType(IDs::midiSettingsNode))
    {
        midiSettingsNode = midiSettingsNodeIn;
      
        periodShift = midiSettingsNode[IDs::periodShift];
        transposeAmt = midiSettingsNode[IDs::transposeAmt];
        midiChannelOut = jlimit(1, 16, (int) midiSettingsNode[IDs::keyboardMidiChannel]);
        // Note transposition is updated when new mode is applied

        return true;
    }
    
    return false;
}

bool SvkMidiProcessor::restoreMappingNode(ValueTree midiMapIn)
{
    bool loadedMap = false;

    if (midiMapIn.hasType(IDs::midiMapNode))
    {
        midiMapNode = midiMapIn;

        Array<int> map;

        NoteMap noteMap;

        //get_array_from_node(midiMapIn, map, IDs::midiInputFilter);
        //
        //if (map.size() > 0)
        //    setInputFilter(map);
        //else
        //    resetInputFilter();
        //
        //map.clear();

        ValueTree remapNode = midiMapNode.getChildWithName(IDs::midiInputRemap);
        noteMap = NoteMap(midiMapNode.getChildWithName(IDs::midiInputRemap));

        if (remapNode.isValid() && remapNode.getNumChildren() > 0)
        {
            loadedMap = true;
            setInputRemap(noteMap);
        }

        //map.clear();
        //get_array_from_node(midiMapIn, map, IDs::midiOutputFilter);
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
    if (JUCEApplication::isStandaloneApp && midiDevicesNodeIn.hasType(IDs::midiDeviceSettingsNode))
    {
        midiDeviceNode = midiDevicesNodeIn;

        setMidiInput(midiDeviceNode[IDs::midiInputName]);
        setMidiOutput(midiDeviceNode[IDs::midiOutputName]);

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

NoteMap SvkMidiProcessor::getManualNoteMap() const
{
    return manualRemap;
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
        midiDeviceNode.setProperty(IDs::midiInputName, inputSelected, nullptr);
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
        midiDeviceNode.setProperty(IDs::midiOutputName, outputSelected, nullptr);
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

void SvkMidiProcessor::setTransposeAmt(int transposeAmtIn)
{
    transposeAmt = transposeAmtIn;
    midiSettingsNode.setProperty(IDs::transposeAmt, transposeAmt, nullptr);
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

    // handle sustained notes | TODO: more thorough solution
    for (int i = 0; i < 16; i++)
    {
        Array<int>& channel = notesOnPerChannel.getReference(i);

        for (auto note : channel)
        {
            directOutBuffer.addEvent(MidiMessage::noteOff(i + 1, note), directMsgs++);
        }
    }
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
    // not using inManualMappingMode because the processMidi call depends on it,
    // and setting that to "true" before initialization is complete causes a crash in some hosts
    if ((int)midiMapNode[IDs::mappingMode] == 3 || updateNode)
        manualRemap = mapIn;
    
    currentRemap = mapIn;
    midiInputRemap->setNoteMap(currentRemap);

    if (updateNode)
    {
        midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputRemap), nullptr);
        midiMapNode.appendChild(currentRemap.getAsValueTree(IDs::midiInputRemap), nullptr);
        //add_array_to_node(midiMapNode, midiInputRemap->getNoteMap()->getValues(), IDs::midiInputRemap, "Note");
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
        midiMapNode.appendChild(mapIn.getAsValueTree(IDs::midiOutputFilter), nullptr);
        //add_array_to_node(midiMapNode, midiInputRemap->getNoteMap()->getValues(), IDs::midiOutputFilter, "Note");
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
    manualMappingHelper = helperIn;
}

void SvkMidiProcessor::setInManualMappingMode(bool manualModeOn)
{
    inManualMappingMode = manualModeOn;
}

//==============================================================================

void SvkMidiProcessor::processMidi(MidiBuffer &midiMessages)
{
    // TODO: handle note offs if period/transpose is changed before note offs
    
    // Combine external inputs
    midiMessages.addEvents(externalInputBuffer, 0, numInputMsgs, 0);
    externalInputBuffer.clear();
    numInputMsgs = 0;

    // Process input
    MidiBuffer combinedMessages;
    MidiMessage msg;
    int midiNote;
    
    for (auto msgData : midiMessages)
    {
        msg = msgData.getMessage();

        if (msg.isNoteOnOrOff())
        {
            midiNote = msg.getNoteNumber();

            if (inManualMappingMode)
            {
                if (manualMappingHelper->isWaitingForKeyInput())
                {
                    MessageManagerLock lock;
                    manualMappingHelper->mapPreparedKeyToNote(midiNote, false);
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

    combinedMessages.addEvents(svkBuffer, 0, numSvkMsgs, 0);
    MidiBuffer finalOutputBuffer;

    // Output Filtering on all MIDI events
    for (auto msgData : combinedMessages)
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

            // TODO: better method for handling this (when transpositions change before note off)
            if (msg.isNoteOn())
                notesOnPerChannel.getReference(msg.getChannel() - 1).addIfNotAlreadyThere(msg.getNoteNumber());
            else if (msg.isNoteOff())
                notesOnPerChannel.getReference(msg.getChannel() - 1).removeAllInstancesOf(msg.getNoteNumber());
        }

       finalOutputBuffer.addEvent(msg, msgData.samplePosition);
    }

    svkBuffer.clear();
    numSvkMsgs = 0;

    finalOutputBuffer.addEvents(directOutBuffer, 0, directMsgs, 0);
    directOutBuffer.clear();
    directMsgs = 0;

    midiMessages.swapWith(finalOutputBuffer);
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
//
//void SvkMidiProcessor::parameterChanged(const String& paramID, float newValue)
//{
//    if (paramID == IDs::mappingMode.toString())
//    {
//        isInputRemapped = newValue > 1;
//    }
//    else if (paramID == IDs::periodShift.toString())
//    {
//        periodShift = newValue;
//        updateNoteTransposition();
//    }
//    else if (paramID == IDs::periodShiftModeSize.toString())
//    {
//        periodShiftModeSize = newValue;
//    }
//    else if (paramID == IDs::transposeAmt.toString())
//    {
//        transposeAmt = newValue;
//        updateNoteTransposition();
//    }
//    else if (paramID == IDs::keyboardMidiChannel.toString())
//    {
//        //allNotesOff(midiChannelOut);
//        midiChannelOut = newValue;
//    }
//    else if (paramID == IDs::mpeOn.toString())
//    {
//        mpeOn = newValue;
//    }
//    
//    DBG("Midi processor updated parameter " + paramID + " to " + String(newValue));
//}

void SvkMidiProcessor::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage msg = MidiMessage::noteOn(midiChannelOut, midiNoteNumber, velocity);
    msg.setTimeStamp(Time::getMillisecondCounterHiRes() - startTime);
    svkBuffer.addEvent(msg, numSvkMsgs++);
}

void SvkMidiProcessor::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage msg = MidiMessage::noteOn(midiChannelOut, midiNoteNumber, velocity);
    msg.setTimeStamp(Time::getMillisecondCounterHiRes() - startTime);
    svkBuffer.addEvent(msg, numSvkMsgs++);
}

void SvkMidiProcessor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg)
{
    MidiMessage myMsg = MidiMessage(msg);
    externalInputBuffer.addEvent(myMsg, numInputMsgs++);
}