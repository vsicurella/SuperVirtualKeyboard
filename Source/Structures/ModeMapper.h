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

	int roundUpOrders();

public:

	ModeMapper(Mode* modeIn);
	ModeMapper(Array<Mode>* modesIn);
	~ModeMapper();


	NoteMap mapTo(Mode* modeMapped, int rootNoteTo = 60);

	static NoteMap map(Mode* mapFrom, Mode* mapTo, int rootNoteFrom = 60, int rootNoteTo = 60);
	static NoteMap standardMidiToMode(Mode* modeMapped, int rootNoteStd = 60, int rootNoteTo = 60);
};