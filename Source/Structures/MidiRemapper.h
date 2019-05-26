/*
  ==============================================================================

    MidiInputFilter.h
    Created: 14 May 2019 10:25:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "InjectiveMap.h"

class MidiRemapper
{
	std::unique_ptr<InjectiveMap> midiNoteMapping;
	Point<int> noteRange;

public:

	MidiRemapper();
    MidiRemapper(Array<int> mapIn);
    
    int getNoteRemapped(int midiNoteIn);
	int getNoteMidi(int remappedNoteIn);

    Point<int> getNoteRange();
    
    int setNote(int noteIn, int noteOut);
    void setNoteRange(int lowestMidiNote, int highestMidiNote);
    
	InjectiveMap* setNoteMap(Array<int> mapToCopy);
	InjectiveMap* getInjectiveMap();
	int* getNoteMap();
    
    static Array<int> getStandardMap();
};
