/*
  ==============================================================================

    MappingHelper.h
    Created: 21 Aug 2019 4:38:52pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "NoteMap.h"

class MappingHelper
{
    
public:
    
    MappingHelper(NoteMap mappingIn = NoteMap());
    ~MappingHelper();

    NoteMap getCurrentNoteMapping() const;
    
    bool isWaitingForKeyInput();
    int getVirtualKeyToMap();
    bool isMappingAllPeriods();
    
    void prepareKeyToMap(int virtualKeyClicked, bool allPeriodsIn);
    void mapKeysToMidiNotes(int midiNoteTriggered, bool mapAllPeriods, Mode* modeFrom = nullptr, Mode* modeTo = nullptr);
    void cancelKeyMap();
    
    Array<int> getKeyInAllPeriods(int keyNumber, Mode* modeToReference);

public:

    class Listener
    {
    public:

        virtual void keyMappingStatusChanged(int keyNumber, bool preparedToMap) = 0;

        virtual void keyMapConfirmed(int keyNumber, int midiNote) = 0;
    };

    void addListener(Listener* listenerIn) { listeners.add(listenerIn); }

    void removeListener(Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

    ListenerList<Listener> listeners;
    
private:

    NoteMap noteMapping;
    
    int virtualKeyToMap = -1;
    bool allPeriods = false;
    bool waitingForKeyInput = false;
};
