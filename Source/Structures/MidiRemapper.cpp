/*
  ==============================================================================

    MidiInputFilter.cpp
    Created: 14 May 2019 10:25:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "MidiRemapper.h"

MidiRemapper::MidiRemapper()
    : noteRange(0, 127)
{
    midiNoteMapping = nullptr;
}

MidiRemapper::MidiRemapper(Array<int>* mapIn)
    : noteRange(0, 127)
{
    midiNoteMapping = mapIn;
}

Array<int>* MidiRemapper::setNoteMap(Array<int>* noteMapIn)
{
    midiNoteMapping = noteMapIn;
    return midiNoteMapping;
}

Array<int> MidiRemapper::removeDuplicates(int noteToKeep)
{
	int val = midiNoteMapping->getUnchecked(noteToKeep);
	Array<int> indiciesOut;

	for (int i = 0; i < midiNoteMapping->size(); i++)
	{
		if (i != noteToKeep && midiNoteMapping->getUnchecked(i) == val)
		{
			midiNoteMapping->set(i, -1);
			indiciesOut.add(i);
		}
	}

	return indiciesOut;
}

int MidiRemapper::getNote(int midiNoteIn)
{
    int midiNoteOut = jlimit(noteRange.x, noteRange.y, midiNoteIn);
    
    if (midiNoteMapping)
        midiNoteOut = midiNoteMapping->getUnchecked(midiNoteOut);
    
    return midiNoteOut;
}

Point<int> MidiRemapper::getNoteRange()
{
    return noteRange;
}

int MidiRemapper::setNote(int noteIn, int noteOut)
{
    if (midiNoteMapping)
        midiNoteMapping->set(noteIn, noteOut);
    
    return noteOut;
}

void MidiRemapper::setNoteRange(int lowestMidiNote, int highestMidiNote)
{
    noteRange = Point<int>(lowestMidiNote, highestMidiNote);
}

Array<int>* MidiRemapper::getNoteMap()
{
    return midiNoteMapping;
}


Array<int> MidiRemapper::getStandardMap()
{
    Array <int> mapOut;
    
    mapOut.resize(128);
    
    for (int i = 0; i < 128; i++)
        mapOut.set(i, i);
    
    return mapOut;
}
