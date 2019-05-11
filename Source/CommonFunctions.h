/*
  ==============================================================================

    CommonFunctions.h
    Created: 6 May 2019 7:31:20pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Structures\Mode.h"

struct ModeScaleSorter
{
	// Scale first, then mode, then family

	static int compareElements(const Mode* t1, const Mode* t2)
	{
		int sz1 = t1->getScaleSize();
		int sz2 = t2->getScaleSize();

		int m1 = t1->getModeSize();
		int m2 = t2->getModeSize();

		String f1 = t1->getFamily();
		String f2 = t2->getFamily();

		if (sz1 < sz2) return -1;
		else if (sz1 > sz2) return 1;
		else
		{
			if (m1 < m2) return -1;
			else if (m1 > m2) return 1;
			else
			{
				if (f1 < f2) return  -1;
				else if (f1 > f2) return 1;
				else return 0;
			}
		}
	}
};

struct ModeModeSorter
{
	// Mode first, then scale, then family

	static int compareElements(const Mode* t1, const Mode* t2)
	{
		int sz1 = t1->getScaleSize();
		int sz2 = t2->getScaleSize();

		int m1 = t1->getModeSize();
		int m2 = t2->getModeSize();

		String f1 = t1->getFamily();
		String f2 = t2->getFamily();

		if (m1 < m2) return -1;
		else if (m1 > m2) return 1;
		else
		{
			if (sz1 < sz2) return -1;
			else if (sz1 > sz2) return 1;
			else
			{
				if (f1 < f2) return  -1;
				else if (f1 > f2) return 1;
				else return 0;
			}
		}
	}
};
struct ModeFamilySorter
{
	// Family first, then scale, then mode

	static int compareElements(const Mode* t1, const Mode* t2)
	{
		int sz1 = t1->getScaleSize();
		int sz2 = t2->getScaleSize();

		int m1 = t1->getModeSize();
		int m2 = t2->getModeSize();

		String f1 = t1->getFamily();
		String f2 = t2->getFamily();

		if (f1 < f2) return -1;
		else if (f1 > f2) return 1;
		else
		{
			if (sz1 < sz2) return -1;
			else if (sz1 > sz2) return 1;
			else
			{
				if (m1 < m2) return  -1;
				else if (m1 > m2) return 1;
				else return 0;
			}
		}
	}
};

static Point<int> pointFromString(const Point<int>& p, String pointToStringIn)
{
	String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
	String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
	return Point<int>(val1.getIntValue(), val2.getIntValue());
}

static Point<float> pointFromString(const Point<float>& p, String pointToStringIn)
{
	String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
	String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
	return Point<float>(val1.getFloatValue(), val2.getFloatValue());
}

static Point<double> pointFromString(const Point<double>& p, String pointToStringIn)
{
	String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
	String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
	return Point<double>(val1.getDoubleValue(), val2.getDoubleValue());
}

template <class T>
static void add_array_to_node(ValueTree nodeIn, const Array<T>& arrayIn, Identifier arrayID, Identifier itemId)
{
	ValueTree arrayTree = ValueTree(arrayID);
	ValueTree item;

	for (int i = 0; i < arrayIn.size(); i++)
	{
		item = ValueTree(itemId);
		item.setProperty("Value", arrayIn[i], nullptr);
		arrayTree.addChild(item, i, nullptr);
	}

	nodeIn.addChild(arrayTree, -1, nullptr);
}

template <class T>
static void get_array_from_node(const ValueTree nodeIn, Array<T>& arrayIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
	{
		for (int i = 0; i < childArray.getNumChildren(); i++)
		{
			arrayIn.add(childArray.getChild(i).getProperty("Value"));
		}
	}
}

static void add_array_to_node(ValueTree nodeIn, const Array<Colour>& arrayIn, Identifier arrayID, Identifier itemId)
{
	ValueTree arrayTree = ValueTree(arrayID);
	ValueTree item;

	for (int i = 0; i < arrayIn.size(); i++)
	{
		item = ValueTree(itemId);
		item.setProperty("Value", arrayIn[i].toString(), nullptr);
		arrayTree.addChild(item, i, nullptr);
	}

	nodeIn.addChild(arrayTree, -1, nullptr);
}

static void get_array_from_node(const ValueTree nodeIn, Array<Colour>& arrayIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
	{
		for (int i = 0; i < childArray.getNumChildren(); i++)
		{
			arrayIn.add(Colour::fromString(childArray.getChild(i).getProperty("Value").toString()));
		}
	}
}

template <class T>
static void add_array_to_node(ValueTree nodeIn, const Array<Point<T>>& arrayIn, Identifier arrayID, Identifier itemId)
{
	ValueTree arrayTree = ValueTree(arrayID);
	ValueTree item;
	for (int i = 0; i < arrayIn.size(); i++)
	{
		item = ValueTree(itemId);
		item.setProperty("Value", arrayIn[i].toString(), nullptr);
		arrayTree.addChild(item, i, nullptr);
	}

	nodeIn.addChild(arrayTree, -1, nullptr);
}

static void get_array_from_node(const ValueTree nodeIn, Array<Point<int>>& arrayIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
		for (int i = 0; i < childArray.getNumChildren(); i++)
			arrayIn.add(pointFromString(arrayIn[0], childArray.getChild(i).getProperty("Value").toString()));
}

static void get_array_from_node(const ValueTree nodeIn, Array<Point<float>>& arrayIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
		for (int i = 0; i < childArray.getNumChildren(); i++)
			arrayIn.add(pointFromString(arrayIn[0], childArray.getChild(i).getProperty("Value")));
}

static void get_array_from_node(const ValueTree nodeIn, Array<Point<double>>& arrayIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
		for (int i = 0; i < childArray.getNumChildren(); i++)
			arrayIn.add(pointFromString(arrayIn[0], childArray.getChild(i).getProperty("Value")));
}

static int totalModulus(int numIn, int mod)
{
	return ((numIn % mod) + mod) % mod;
}