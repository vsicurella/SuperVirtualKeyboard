/*
  ==============================================================================

    ModeMapper.h
    Created: 30 May 2019 8:02:42pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Mode.h"
#include "NoteMap.h"

class ModeMapper
{
    
public:

	static NoteMap mapByOrder(const Mode& mode1, const Mode& mode2, int mode1Order=0, int mode2Order=0, int mode1Offset=0, int mode2Offset=0);

	static NoteMap mapToMode1Period(const Mode& mapFrom, const Mode& mapTo, Array<int> degreeMapIn);
    
    static NoteMap stdMidiToMode(const Mode& modeMapped, int rootNoteStd = 60, Array<int> degreeMapIn=Array<int>());
    
    static Array<int> autoDegreeMapPeriod(const Mode& mode1, const Mode& mode2);
    
    static Array<int> autoDegreeMapFull(const Mode& mode1, const Mode& mode2);

};
