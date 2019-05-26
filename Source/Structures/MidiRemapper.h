/*
  ==============================================================================

    MidiInputFilter.h
    Created: 14 May 2019 10:25:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

struct MidiRemapper
{
	MidiRemapper();
    MidiRemapper(Array<int>* mapIn);
    
    int getNote(int midiNoteIn);
    Point<int> getNoteRange();
    
    int setNote(int noteIn, int noteOut);
    void setNoteRange(int lowestMidiNote, int highestMidiNote);

	Array<int> removeDuplicates(int noteToKeep);
    
    Array<int>* setNoteMap(Array<int>* noteMapIn);
    Array<int>* getNoteMap();
    
    static Array<int> getStandardMap();
    
private:
    
    Array<int>* midiNoteMapping;
    Point<int> noteRange;


};
