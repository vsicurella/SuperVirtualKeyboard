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
    int mapType;
    
    int mapByOrderNum1 = 0;
    int mapByOrderNum2 = 0;
    int mapByOrderOffset1 = 0;
    int mapByOrderOffset2 = 0;
    
public:
    
    ModeMapper() {};
    
    void setMapType(int mapTypeIn);
    
    void setMapOrdersParameters(int order1, int order2, int offset1, int offset2);
    
    NoteMap map(const Mode& mode1, const Mode& mode2, int order1=0, int order2=0, int offset1=0, int offset2=0, NoteMap prevMap = NoteMap());

	static NoteMap mapFull(const Mode& mode1, const Mode& mode2, Array<int> degreeMapIn = Array<int>());

	static NoteMap mapByOrder(const Mode& mode1, const Mode& mode2, int mode1Order=0, int mode2Order=0, int mode1Offset=0, int mode2Offset=0, NoteMap prevMap=NoteMap());

	static NoteMap mapToMode1Period(const Mode& mapFrom, const Mode& mapTo, Array<int> degreeMapIn=Array<int>());
    
    static NoteMap mapToMode1Scale(const Mode& mode1, const Mode& mode2, int mode2Order=0);
    
    static NoteMap stdMidiToMode(const Mode& modeMapped, int rootNoteStd = 60);
    
    static Array<int> degreeMapPeriod(const Mode& mode1, const Mode& mode2);
    
    static Array<int> degreeMapFullMode(const Mode& mode1, const Mode& mode2);
    
    enum MapType
    {
        ModeToMode = 0,
        ModeToScale,
        ModeByOrder
    };
};
