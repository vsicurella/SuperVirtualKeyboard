/*
  ==============================================================================

    MappingHelper.h
    Created: 21 Aug 2019 4:38:52pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once
#include "MappingEditorBase.h"

class MappingHelper : public MappingEditor
{
    
public:
    
    MappingHelper(NoteMap mappingIn = NoteMap());
    ~MappingHelper();

    //================================================================

    // MappingEditor implementation

    NoteMap getCurrentNoteMapping() const override;

    void mapMidiNoteToKey(int midiNoteIn, int keyNumberOut) override;

    void resetMapping(NoteMap mappingIn = NoteMap(), bool sendMessage = true) override;

    //================================================================

    bool isWaitingForKeyInput();
    int getVirtualKeyToMap();
    bool isMappingAllPeriods();
    
    void prepareKeyToMap(int virtualKeyClicked, bool allPeriodsIn);
    void mapPreparedKeyToNote(int midiNoteTriggered, bool mapAllPeriods, Mode* modeFrom = nullptr, Mode* modeTo = nullptr);
    void cancelKeyMap();

    
    Array<int> getKeyInAllPeriods(int keyNumber, Mode* modeToReference);
    
private:

    NoteMap noteMapping;
    
    int virtualKeyToMap = -1;
    bool allPeriods = false;
    bool waitingForKeyInput = false;
};
