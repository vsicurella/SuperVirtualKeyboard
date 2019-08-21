/*
  ==============================================================================

    MappingHelper.cpp
    Created: 21 Aug 2019 4:38:52pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "MappingHelper.h"

MappingHelper::MappingHelper(NoteMap& noteMapToEdit, Mode* mode1In, Mode* mode2In)
: noteMap(noteMapToEdit)
{
    mode1 = mode1In;
    mode2 = mode2In;
}

MappingHelper::~MappingHelper()
{
    mode1 = nullptr;
    mode2 = nullptr;
}

Array<int> MappingHelper::setKeysToMap(int keyClicked, bool mapAllPeriods)
{
    if (mapAllPeriods)
    {
        keysToMap = getKeyInAllPeriods(keyClicked, mode1);
    }
    else
    {
        keysToMap = Array<int>(keyClicked);
    }
    
    waitingForKeyInput = true;
    
    return keysToMap;
}

Array<int> MappingHelper::getKeyInAllPeriods(int keyClicked, Mode* modeToReference)
{
    Array<int> keyNumsOut;
    
    int scaleSize = modeToReference->getScaleSize();
    int totalNumPeriods = 128 / scaleSize;
    int keyPeriod = keyClicked / scaleSize;
    int keyNum;
    
    for (int i = 0; i < totalNumPeriods; i++)
    {
        keyNum = (i - keyPeriod) * scaleSize + keyClicked;
        
        if (keyNum >= 0 && keyNum < 128)
            keyNumsOut.add(keyNum);
    }
    
    return keyNumsOut;
}

// For assigning the selected keys
void MappingHelper::handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    int key = 0;
    int mappedMidiNote;
    
    if (waitingForKeyInput)
    {
        int notePeriod = midiNoteNumber / mode1->getScaleSize();
        
        for (int i = 0; i < keysToMap.size(); i++)
        {
            key = keysToMap[i];
            mappedMidiNote = midiNoteNumber + (i - notePeriod) * mode1->getScaleSize();
            
            noteMap.setValue(key, mappedMidiNote);
        }

        waitingForKeyInput = false;
    }
}

void MappingHelper::handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{

}

