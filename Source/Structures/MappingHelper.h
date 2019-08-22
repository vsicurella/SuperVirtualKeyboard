/*
  ==============================================================================

    MappingHelper.h
    Created: 21 Aug 2019 4:38:52pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../PluginState.h"
#include "../MidiProcessor.h"
#include "NoteMap.h"

class MappingHelper : public MidiKeyboardStateListener
{
    
public:
    
    MappingHelper(SvkPluginState* pluginStateIn);
    ~MappingHelper();
    
    bool isWaitingForKeyInput();
    int getVirtualKeyToMap();
    bool isMappingAllPeriods();
    
    void prepareKeyToMap(int virtualKeyClicked, bool allPeriodsIn);
    void mapKeysToMidiNotes(int midiNoteTriggered, bool mapAllPeriods);
    void cancelKeyMap();
    
    Array<int> getKeyInAllPeriods(int keyNumber, Mode* modeToReference);
    
private:
    
    // Will receive the actual MIDI note and connect it to the Virtual Keys waiting to be mapped.
    void handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;

    SvkPluginState* pluginState;
    
    int virtualKeyToMap = -1;
    bool allPeriods = false;
    bool waitingForKeyInput = false;
};
