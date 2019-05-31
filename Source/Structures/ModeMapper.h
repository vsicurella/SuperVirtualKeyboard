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

public:

	ModeMapper(Mode* modeIn);
	ModeMapper(Array<Mode>* modesIn);
	~ModeMapper() {}

	NoteMap mapTo(Mode* modeMapped);
	NoteMap mapTo(Mode* modeMapped, int rootNoteTo);

	static NoteMap map(Mode* mapFrom, Mode* mapTo);
	static NoteMap map(Mode* mapFrom, Mode* mapTo, int rootNoteFrom, int rootNoteTo);

	static NoteMap stdMidiToMode(Mode* modeMapped, int rootNoteStd = 60, int rootNoteTo = 60);
};