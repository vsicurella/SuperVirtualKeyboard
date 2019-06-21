/*
  ==============================================================================

    NoteMap.h
    Created: 25 May 2019 10:34:43pm
    Author:  Vincenzo

	May turn this into a template later.

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class NoteMap
{
	int size;

	Array<int> keys; // Midi Note In, Key Number Out
	Array<int> values; // Key Number In, Midi Note Out

	int nullVal;

public:
    
    NoteMap();
	NoteMap(int sizeIn, bool useIdentity, int nullValIn = -1);
	NoteMap(Array<int> valuesIn, int nullValIn = -1);
    NoteMap(const NoteMap& mapToCopy);
    ~NoteMap() {}
	
	void setValue(int keyNum, int valIn);
	void setValues(Array<int> valuesIn);
	
	int* setNullVal(int nullValIn);

	int getSize();

	int getKey(int valIn);
	int getValue(int keyNum);

	const Array<int>& getKeys() const;
	const Array<int>& getValues() const;

	int getNullVal();
    
    String toString();
};
