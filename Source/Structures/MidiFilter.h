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

	int getNoteRemapped(int midiNoteIn);
	int getNoteMidi(int remappedNoteIn);

	Point<int> getNoteRange();

	int setNote(int noteIn, int noteOut);
	void setNoteRange(int lowestMidiNote, int highestMidiNote);

	NoteMap* setNoteMap(Array<int> mapToCopy);
	NoteMap* getNoteMap();
	int* getRemappedNotes();

	int size();

	String toString();

	static Array<int> getStandardMap();
};