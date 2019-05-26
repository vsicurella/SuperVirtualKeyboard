/*
  ==============================================================================

    InjectiveMap.cpp
    Created: 25 May 2019 10:34:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "InjectiveMap.h"

InjectiveMap::InjectiveMap(int sizeIn, bool useIdentity, int nullValIn)
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

InjectiveMap::InjectiveMap(int* valuesIn, int sizeIn, int nullValIn)
{
	nullVal = nullValIn;
	setValues(valuesIn, sizeIn);
}


InjectiveMap::InjectiveMap(InjectiveMap& mapToCopy)
{
	keys.reset(mapToCopy.getKeys());
	values.reset(mapToCopy.getValues());

	nullVal = mapToCopy.getNullVal();
}

InjectiveMap::~InjectiveMap()
{
	keys.release();
	values.release();
}

int* InjectiveMap::setValue(int keyNum, int valIn)
{
	int oldVal = keys.get()[keyNum];
	values.get()[oldVal] = nullVal;

	keys.get()[keyNum] = valIn;
	values.get()[valIn] = keyNum;

	return keys.get() + keyNum;
}

void InjectiveMap::setValues(int* valuesIn, int sizeIn)
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

int* InjectiveMap::setNullVal(int nullValIn)
{
	nullVal = nullValIn;
	return &nullVal;
}

int InjectiveMap::getSize()
{
	return size;
}

int InjectiveMap::getKey(int valIn)
{
	return values.get()[valIn];
}
int InjectiveMap::getValue(int keyNum)
{
	return keys.get()[keyNum];
}

int* InjectiveMap::getKeys()
{
	return keys.get();
}
int* InjectiveMap::getValues()
{
	return values.get();
}

int InjectiveMap::getNullVal()
{
	return nullVal;
}