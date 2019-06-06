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
	Mode* mode;
	Array<Mode>* modeChain;
    
    Array<int> degreeMap;
    
public:

	ModeMapper(Mode* modeIn);
	ModeMapper(Array<Mode>* modesIn);
	~ModeMapper() {}

	NoteMap mapTo(const Mode& modeMapped, Array<int> degreeMapIn=Array<int>());
    
    void setAndMapDegrees(Array<int> degreeMapIn);

	static NoteMap map(const Mode& mapFrom, const Mode& mapTom, Array<int> degreeMapIn=Array<int>());
    
    static NoteMap stdMidiToMode(const Mode& modeMapped, int rootNoteStd = 60, Array<int> degreeMapIn=Array<int>());
};
