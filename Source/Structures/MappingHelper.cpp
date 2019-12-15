/*
  ==============================================================================

    MappingHelper.cpp
    Created: 21 Aug 2019 4:38:52pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "MappingHelper.h"

MappingHelper::MappingHelper(SvkPluginState* pluginStateIn)
{
    virtualKeyToMap = -1;
    waitingForKeyInput = false;
    allPeriods = false;
}

MappingHelper::~MappingHelper()
{

}

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
}

void MappingHelper::mapKeysToMidiNotes(int midiNoteTriggered, bool allPeriods)
{
    if (waitingForKeyInput)
    {
        if (allPeriods)
        {
            Array<int> triggeredPeriods = getKeyInAllPeriods(midiNoteTriggered, pluginState->getMode1());
            Array<int> virtualKeyPeriods = getKeyInAllPeriods(virtualKeyToMap, pluginState->getMode2());
        }
        
        else
        {
            pluginState->getMidiInputFilterMap()->setValue(midiNoteTriggered, virtualKeyToMap);
        }
        
        DBG("Midi Note " + String(midiNoteTriggered) + " is now mapped to " + String(pluginState->getMidiInputFilterMap()->getValue(midiNoteTriggered)));
        waitingForKeyInput = false;
    }
}

void MappingHelper::cancelKeyMap()
{
    virtualKeyToMap = -1;
    waitingForKeyInput = false;
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

void MappingHelper::handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    mapKeysToMidiNotes(midiNoteNumber, allPeriods);
}

void MappingHelper::handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{

}
