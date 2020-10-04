/*
  ==============================================================================

    MidiInputFilter.cpp
    Created: 14 May 2019 10:25:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "MidiFilter.h"

MidiFilter::MidiFilter()
{
    noteRange = Point<int>(-1, 127);
    midiNoteMapping = std::make_unique<NoteMap>(128, true);
}

MidiFilter::MidiFilter(Array<int> mapIn)
{
    noteRange = Point<int>(-1, 127);
    setNoteMap(mapIn);
}

MidiFilter::MidiFilter(NoteMap mapIn)
{
    noteRange = Point<int>(-1, 127);
    setNoteMap(mapIn.getValues());
}

NoteMap* MidiFilter::setNoteMap(Array<int> mapToCopy)
{
    midiNoteMapping = std::make_unique<NoteMap>(mapToCopy);
    return midiNoteMapping.get();
}

NoteMap* MidiFilter::setNoteMap(NoteMap mapToCopy)
{
    midiNoteMapping = std::make_unique<NoteMap>(mapToCopy);
    return midiNoteMapping.get();
}

int MidiFilter::getNoteRemapped(int midiNoteIn) const
{
    int midiNoteOut = jlimit(noteRange.x, noteRange.y, midiNoteIn);
    
    if (midiNoteMapping.get())
        midiNoteOut = midiNoteMapping->getValue(midiNoteOut);
    
    return midiNoteOut;//jlimit(noteRange.x, noteRange.y, midiNoteOut);
}

int MidiFilter::getNoteMidi(int remappedNoteIn) const
{
    int remappedNote = jlimit(noteRange.x, noteRange.y, remappedNoteIn);
    
    int noteOut = -1;

    if (midiNoteMapping.get())
        noteOut = midiNoteMapping->getKey(remappedNote);

    return jlimit(noteRange.x, noteRange.y, noteOut);
}


Point<int> MidiFilter::getNoteRange()  const
{
    return noteRange;
}

int MidiFilter::setNote(int noteIn, int noteOut)
{
    if (midiNoteMapping.get())
        midiNoteMapping->setValue(noteIn, noteOut);
    
    return noteOut;
}

void MidiFilter::setNoteRange(int lowestMidiNote, int highestMidiNote)
{
    noteRange = Point<int>(lowestMidiNote, highestMidiNote);
}

NoteMap* MidiFilter::getNoteMap()
{
    return midiNoteMapping.get();
}

Array<int> MidiFilter::getRemappedNotes()
{
    return midiNoteMapping->getValues();
}

int MidiFilter::size() const
{
    int sizeOut = 0;
    if (midiNoteMapping.get())
        sizeOut = midiNoteMapping->getSize();

    return sizeOut;
}

String MidiFilter::toString()
{
    String out;

    for (int i = 0; i < midiNoteMapping->getSize(); i++)
    {
        out << "Midi Note In " << i << " --> " << midiNoteMapping->getValue(i) << "\t Midi Note Out " << i << " <-- " << midiNoteMapping->getKey(i) << "\n";
    }

    return out;
}

Array<int> MidiFilter::getStandardMap()
{
    Array <int> mapOut;
    
    mapOut.resize(128);
    
    for (int i = 0; i < 128; i++)
        mapOut.set(i, i);
    
    return mapOut;
}
