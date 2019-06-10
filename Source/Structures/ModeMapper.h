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

	static NoteMap map(const Mode& mapFrom, const Mode& mapTo);

	static NoteMap map(const Mode& mapFrom, const Mode& mapTo, Array<int> degreeMapIn);
    
    static NoteMap stdMidiToMode(const Mode& modeMapped, int rootNoteStd = 60, Array<int> degreeMapIn=Array<int>());
    
    static Array<int> autoDegreeMap(const Mode& mode1, const Mode& mode2);
};
