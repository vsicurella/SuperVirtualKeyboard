/*
  ==============================================================================

    InjectiveMap.h
    Created: 25 May 2019 10:34:43pm
    Author:  Vincenzo

	May turn this into a template later.

  ==============================================================================
*/

#pragma once
#include <memory>

class InjectiveMap
{
	int size;

	std::unique_ptr<int> keys;
	std::unique_ptr<int> values;

	int nullVal;

public:

	InjectiveMap(int sizeIn, bool useIdentity = false, int nullValIn = -1);
	InjectiveMap(int* valuesIn, int sizeIn, int nullValIn = -1);
	InjectiveMap(InjectiveMap& mapToCopy);
	~InjectiveMap();
	
	int* setValue(int keyNum, int valIn);
	void setValues(int* valuesIn, int sizeIn);
	
	int* setNullVal(int nullValIn);

	int getSize();

	int getKey(int valIn);
	int getValue(int keyNum);

	int* getKeys();
	int* getValues();

	int getNullVal();
};