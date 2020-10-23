/*
  ==============================================================================

    MappingHelper.cpp
    Created: 21 Aug 2019 4:38:52pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "MappingHelper.h"

MappingHelper::MappingHelper(NoteMap noteMapIn)
    : noteMapping(noteMapIn)
{
    virtualKeyToMap = -1;
    waitingForKeyInput = false;
    allPeriods = false;
}

MappingHelper::~MappingHelper()
{

}

//================================================================

NoteMap MappingHelper::getCurrentNoteMapping() const
{
    return noteMapping;
}

void MappingHelper::mapMidiNoteToKey(int midiNoteIn, int keyNumberOut)
{
    noteMapping.setValue(midiNoteIn, keyNumberOut);
    listeners.call(&Listener::mappingEditorChanged, noteMapping);
}

void MappingHelper::resetMapping(NoteMap mappingIn, bool sendMessage)
{
    noteMapping = mappingIn;

    if (sendMessage)
        listeners.call(&Listener::mappingEditorChanged, noteMapping);
}

//================================================================

bool MappingHelper::isWaitingForKeyInput()
{
    return waitingForKeyInput;
}

int MappingHelper::getVirtualKeyToMap()
{
    if (waitingForKeyInput)
        return virtualKeyToMap;
    else
        return -1;
}

bool MappingHelper::isMappingAllPeriods()
{
    return allPeriods;
}

void MappingHelper::prepareKeyToMap(int virtualKeyClicked, bool allPeriodsIn)
{
    virtualKeyToMap = virtualKeyClicked;
    allPeriods = allPeriodsIn;
    waitingForKeyInput = true;
    listeners.call(&Listener::keyMappingStatusChanged, virtualKeyToMap, true);
}

void MappingHelper::mapPreparedKeyToNote(int midiNoteTriggered, bool allPeriods, Mode* modeFrom, Mode* modeTo)
{
    if (waitingForKeyInput)
    {
        if (allPeriods && modeFrom != nullptr && modeTo != nullptr)
        {
            Array<int> triggeredPeriods = getKeyInAllPeriods(midiNoteTriggered, modeFrom);
            Array<int> virtualKeyPeriods = getKeyInAllPeriods(virtualKeyToMap, modeTo);

            // TODO
        }
        
        else
        {
            mapMidiNoteToKey(midiNoteTriggered, virtualKeyToMap);
        }
        
        DBG("Midi Note " + String(midiNoteTriggered) + " is now mapped to " + String(noteMapping.getValue(midiNoteTriggered)));
        waitingForKeyInput = false;
        listeners.call(&Listener::keyMapConfirmed, virtualKeyToMap, midiNoteTriggered);
    }
}

void MappingHelper::cancelKeyMap()
{
    waitingForKeyInput = false;
    listeners.call(&Listener::keyMappingStatusChanged, virtualKeyToMap, false);
    virtualKeyToMap = -1;
    DBG("Key map cancelled.");
}

Array<int> MappingHelper::getKeyInAllPeriods(int keyNumber, Mode* modeToReference)
{
    Array<int> keyNumsOut;
    
    int scaleSize = modeToReference->getScaleSize();
    int totalNumPeriods = 128 / scaleSize;
    int keyPeriod = keyNumber / scaleSize;
    int keyNum;
    
    for (int i = 0; i < totalNumPeriods; i++)
    {
        keyNum = (i - keyPeriod) * scaleSize + keyNumber;
        
        if (keyNum >= 0 && keyNum < 128)
            keyNumsOut.add(keyNum);
    }
    
    return keyNumsOut;
}