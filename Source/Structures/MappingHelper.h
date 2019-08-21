/*
  ==============================================================================

    MappingHelper.h
    Created: 21 Aug 2019 4:38:52pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../MidiProcessor.h"
#include "NoteMap.h"

class MappingHelper : public MidiKeyboardStateListener
{
    
public:
    
    MappingHelper();
    MappingHelper(NoteMap& noteMapToEdit, Mode* mode1In, Mode* mode2In);
    ~MappingHelper();
    
    Array<int> setKeysToMap(int keyClicked, bool mapAllPeriods);
    
    Array<int> getKeyInAllPeriods(int keyClicked, Mode* modeToReference);
    
private:
    
    void handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;

    NoteMap& noteMap;
    Mode* mode1;
    Mode* mode2;
    
    Array<int> keysToMap;
    bool waitingForKeyInput = false;
}
