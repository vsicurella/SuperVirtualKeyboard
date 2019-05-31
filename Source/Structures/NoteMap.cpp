/*
  ==============================================================================

    NoteMap.cpp
    Created: 25 May 2019 10:34:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "NoteMap.h"

NoteMap::NoteMap()
    :  NoteMap(128, true, -1)
{
}

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

NoteMap::NoteMap(Array<int> valuesIn, int nullValIn)
{
	nullVal = nullValIn;
	setValues(valuesIn);
}

NoteMap::NoteMap(const NoteMap& mapToCopy)
{
    size = mapToCopy.size;
    nullVal = mapToCopy.nullVal;
    
    keys.reset(new Array<int>());
    values.reset(new Array<int>());
    
    for (int i = 0; i < size; i++)
    {
        keys->add(mapToCopy.keys->getUnchecked(i));
        values->add(mapToCopy.values->getUnchecked(i));
    }

}


NoteMap::~NoteMap()
{
	keys.release();
	values.release();
}

void NoteMap::setValue(int keyNum, int valIn)
{
	int oldVal = keys->getUnchecked(keyNum);
    if (oldVal >= 0)
        values->set(oldVal, nullVal);

	keys->set(keyNum, valIn);
    if (valIn >= 0)
        values->set(valIn, keyNum);
}

void NoteMap::setValues(Array<int> valuesIn)
{
	size = valuesIn.size();

	keys.reset(new Array<int>(valuesIn));
	values.reset(new Array<int>());
    values->resize(size);

	for (int i = 0; i < size; i++)
	{
        setValue(i, valuesIn[i]);
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

Array<int> NoteMap::getKeys()
{
	return *keys.get();
}

Array<int> NoteMap::getValues()
{
	return *values.get();
}

int NoteMap::getNullVal()
{
	return nullVal;
}
