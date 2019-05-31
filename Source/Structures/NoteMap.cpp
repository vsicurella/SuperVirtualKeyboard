/*
  ==============================================================================

    NoteMap.cpp
    Created: 25 May 2019 10:34:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "NoteMap.h"

NoteMap::NoteMap(int sizeIn, bool useIdentity, int nullValIn)
{
	size = sizeIn;

	keys.reset(new int[size]);
	values.reset(new int[size]);

	nullVal = nullValIn;
	int defaultVal = nullVal;

	for (int i = 0; i < size; i++)
	{
		if (useIdentity)
			defaultVal = i;

		keys.get()[i] = defaultVal;
		values.get()[i] = defaultVal;
	}
}

NoteMap::NoteMap(int* valuesIn, int sizeIn, int nullValIn)
{
	nullVal = nullValIn;
	setValues(valuesIn, sizeIn);
}


NoteMap::NoteMap(NoteMap& mapToCopy)
{
	keys.reset(mapToCopy.getKeys());
	values.reset(mapToCopy.getValues());

	nullVal = mapToCopy.getNullVal();
}

NoteMap::~NoteMap()
{
	keys.release();
	values.release();
}

int* NoteMap::setValue(int keyNum, int valIn)
{

	int oldVal = keys.get()[keyNum];
	values.get()[oldVal] = nullVal;

	keys.get()[keyNum] = valIn;
	values.get()[valIn] = keyNum;

	return keys.get() + keyNum;
}

void NoteMap::setValues(int* valuesIn, int sizeIn)
{
	size = sizeIn;

	keys.reset(new int[size]);
	values.reset(new int[size]);

	for (int i = 0; i < size; i++)
	{
		values.get()[i] = nullVal;
	}

	for (int i = 0; i < size; i++)
	{
		int val = valuesIn[i];
		keys.get()[i] = val;
		
		if (val != nullVal)
		{
			values.get()[valuesIn[i]] = i;
		}
	}
}

int* NoteMap::setNullVal(int nullValIn)
{
	nullVal = nullValIn;
	return &nullVal;
}

int NoteMap::getSize()
{
	return size;
}

int NoteMap::getKey(int valIn)
{
	return values.get()[valIn];
}
int NoteMap::getValue(int keyNum)
{
	return keys.get()[keyNum];
}

int* NoteMap::getKeys()
{
	return keys.get();
}
int* NoteMap::getValues()
{
	return values.get();
}

int NoteMap::getNullVal()
{
	return nullVal;
}