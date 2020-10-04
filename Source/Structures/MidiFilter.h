/*
==============================================================================
MidiFilter.h
Created: 14 May 2019 10:25:01pm
Author:  Vincenzo
==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "NoteMap.h"

class MidiFilter
{
    std::unique_ptr<NoteMap> midiNoteMapping;
    Point<int> noteRange;

public:

    MidiFilter();
    MidiFilter(Array<int> mapIn);
    MidiFilter(NoteMap mapIn);

    int getNoteRemapped(int midiNoteIn) const;
    int getNoteMidi(int remappedNoteIn) const;
    Array<int> getRemappedNotes();

    Point<int> getNoteRange() const;
    NoteMap* getNoteMap();

    int setNote(int noteIn, int noteOut);
    void setNoteRange(int lowestMidiNote, int highestMidiNote);

    NoteMap* setNoteMap(Array<int> mapToCopy);
    NoteMap* setNoteMap(NoteMap mapToCopy);

    int size() const;

    String toString();

    static Array<int> getStandardMap();
};
