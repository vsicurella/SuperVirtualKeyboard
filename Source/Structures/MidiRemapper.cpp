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

Array<int>* MidiRemapper::setNoteMap(Array<int>& noteMapIn)
{
    midiNoteMapping = &noteMapIn;
    return midiNoteMapping;
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
	{
		midiNoteMapping->set(noteIn, noteOut);
	}
    
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
