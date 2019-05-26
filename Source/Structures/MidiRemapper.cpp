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
}

MidiRemapper::MidiRemapper(Array<int> mapIn)
    : noteRange(0, 127)
{
	setNoteMap(mapIn);
}

InjectiveMap* MidiRemapper::setNoteMap(Array<int> mapToCopy)
{
    midiNoteMapping.reset(new InjectiveMap(mapToCopy.getRawDataPointer(), mapToCopy.size()));
	
	return midiNoteMapping.get();
}

int MidiRemapper::getNoteRemapped(int midiNoteIn)
{
    int midiNoteOut = jlimit(noteRange.x, noteRange.y, midiNoteIn);
    
    if (midiNoteMapping.get())
        midiNoteOut = midiNoteMapping->getValue(midiNoteOut);
    
	return jlimit(noteRange.x, noteRange.y, midiNoteOut);
}

int MidiRemapper::getNoteMidi(int remappedNoteIn)
{
	int remappedNote = jlimit(noteRange.x, noteRange.y, remappedNoteIn);
	
	int noteOut = -1;

	if (midiNoteMapping.get())
		noteOut = midiNoteMapping->getKey(remappedNote);

	return jlimit(noteRange.x, noteRange.y, noteOut);
}


Point<int> MidiRemapper::getNoteRange()
{
    return noteRange;
}

int MidiRemapper::setNote(int noteIn, int noteOut)
{
    if (midiNoteMapping.get())
        midiNoteMapping->setValue(noteIn, noteOut);
    
    return noteOut;
}

void MidiRemapper::setNoteRange(int lowestMidiNote, int highestMidiNote)
{
    noteRange = Point<int>(lowestMidiNote, highestMidiNote);
}

InjectiveMap* MidiRemapper::getInjectiveMap()
{
	return midiNoteMapping.get();
}

int* MidiRemapper::getNoteMap()
{
	return midiNoteMapping->getValues();
}

Array<int> MidiRemapper::getStandardMap()
{
    Array <int> mapOut;
    
    mapOut.resize(128);
    
    for (int i = 0; i < 128; i++)
        mapOut.set(i, i);
    
    return mapOut;
}
