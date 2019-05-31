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

	keys.reset(new Array<int>());
	values.reset(new Array<int>());

	nullVal = nullValIn;
	int defaultVal = nullVal;

	for (int i = 0; i < size; i++)
	{
		if (useIdentity)
			defaultVal = i;

		keys->add(defaultVal);
		values->add(defaultVal);
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

void NoteMap::setValue(int keyNum, int valIn)
{
	int oldVal = keys->getUnchecked(keyNum);
	values->set(oldVal, nullVal);

	keys.get()[keyNum] = valIn;
	values.get()[valIn] = keyNum;
}

void NoteMap::setValues(int* valuesIn, int sizeIn)
{
	size = sizeIn;

	keys.reset(new Array<int>());
	values.reset(new Array<int>());

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
	return values->getUnchecked(valIn);
}
int NoteMap::getValue(int keyNum)
{
	return keys->getUnchecked(keyNum);
}

Array<int>* NoteMap::getKeys()
{
	return keys.get();
}

Array<int>* NoteMap::getValues()
{
	return values.get();
}

int NoteMap::getNullVal()
{
	return nullVal;
}