/*
  ==============================================================================

    MidiInputFilter.cpp
    Created: 14 May 2019 10:25:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "MidiFilter.h"

MidiRemapper::MidiRemapper()
{
	noteRange = Point<int>(-1, 127);
	midiNoteMapping.reset(new NoteMap(128, true));
}

MidiRemapper::MidiRemapper(Array<int> mapIn)
{
	noteRange = Point<int>(-1, 127);
	setNoteMap(mapIn);
}

NoteMap* MidiRemapper::setNoteMap(Array<int> mapToCopy)
{
    midiNoteMapping.reset(new NoteMap(mapToCopy.getRawDataPointer(), mapToCopy.size()));
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

NoteMap* MidiRemapper::getNoteMap()
{
	return midiNoteMapping.get();
}

int* MidiRemapper::getRemappedNotes()
{
	return midiNoteMapping->getKeys();
}

int MidiRemapper::size()
{
	int sizeOut = 0;
	if (midiNoteMapping.get())
		sizeOut = midiNoteMapping->getSize();

	return sizeOut;
}

String MidiRemapper::toString()
{
	String out;

	for (int i = 0; i < midiNoteMapping->getSize(); i++)
	{
		out << "Midi Note In " << i << " --> " << midiNoteMapping->getValue(i) << "\t Midi Note Out " << i << " <-- " << midiNoteMapping->getKey(i) << "\n";
	}

	return out;
}

Array<int> MidiRemapper::getStandardMap()
{
    Array <int> mapOut;
    
    mapOut.resize(128);
    
    for (int i = 0; i < 128; i++)
        mapOut.set(i, i);
    
    return mapOut;
}
