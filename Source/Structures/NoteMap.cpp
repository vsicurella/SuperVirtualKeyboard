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
	int oldVal = values->getUnchecked(keyNum);
    
    if (oldVal >= 0)
        keys->set(oldVal, nullVal);

    values->set(keyNum, valIn);
    
    if (valIn >= 0)
        keys->set(valIn, keyNum);
}

void NoteMap::setValues(Array<int> valuesIn)
{
	size = valuesIn.size();

	values.reset(new Array<int>());
	keys.reset(new Array<int>());
    values->resize(size);
    keys->resize(size);

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
	return keys->getUnchecked(valIn);
}
int NoteMap::getValue(int keyNum)
{
    return values->getUnchecked(keyNum);
}

const Array<int>& NoteMap::getKeys() const
{
	return *keys.get();
}

const Array<int>& NoteMap::getValues() const
{
	return *values.get();
}

int NoteMap::getNullVal()
{
	return nullVal;
}

String NoteMap::toString()
{
    String toStr = "";
    
    for (int i = 0; i < size; i++)
    {
        toStr += String(i) + "\t: " + String(values->getUnchecked(i)) + "\n";
    }
    
    return toStr;
}
