/*
  ==============================================================================

    ModeMapper.cpp
    Created: 30 May 2019 8:02:42pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ModeMapper.h"

ModeMapper::ModeMapper(Mode* modeIn)
{
	mode = modeIn;
}

ModeMapper::ModeMapper(Array<Mode>* modesIn)
{
	modeChain = modesIn;
}

NoteMap ModeMapper::mapTo(Mode* modeMapped)
{
	return mapTo(modeMapped, modeMapped->getRootNote());
}

NoteMap ModeMapper::mapTo(Mode* modeMapped, int rootNoteTo)
{
	return map(mode, modeMapped, mode->getRootNote(), rootNoteTo);
}

NoteMap ModeMapper::map(Mode* mapFrom, Mode* mapTo)
{
	return map(mapFrom, mapTo, mapFrom->getRootNote(), mapTo->getRootNote());
}

NoteMap ModeMapper::map(Mode* mapFrom, Mode* mapTo, int rootNoteFrom, int rootNoteTo)
{
	Array<int> midiNotesFrom = mapFrom->getModalMidiNotes(0, rootNoteFrom);
	Array<int> midiNotesTo = mapTo->getModalMidiNotes(0, rootNoteTo);

	NoteMap mappingOut(midiNotesFrom.size());
	int degreeCount = 0;

	for (int i = 0; i < midiNotesFrom.size(); i++)
	{
		mappingOut.setValue(midiNotesFrom[i], midiNotesTo[i]);
	}

	return mappingOut;
}

NoteMap ModeMapper::stdMidiToMode(Mode* modeMapped, int rootNoteStd, int rootNoteMap)
{
	Mode meantone7_12 = Mode("2 2 1 2 2 2 1", "Meantone", rootNoteStd);
	return map(&meantone7_12, modeMapped, rootNoteStd, rootNoteMap);
}

