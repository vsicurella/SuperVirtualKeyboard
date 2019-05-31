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

	std::unique_ptr<Array<int>> keys;
	std::unique_ptr<Array<int>> values;

	int nullVal;
    
public:
    
    NoteMap();
	NoteMap(int sizeIn, bool useIdentity, int nullValIn = -1);
	NoteMap(Array<int> valuesIn, int nullValIn = -1);
    NoteMap(const NoteMap& mapToCopy);
	~NoteMap();
	
	void setValue(int keyNum, int valIn);
	void setValues(Array<int> valuesIn);
	
	int* setNullVal(int nullValIn);

	int getSize();

	int getKey(int valIn);
	int getValue(int keyNum);

	Array<int> getKeys();
	Array<int> getValues();

	int getNullVal();
};
