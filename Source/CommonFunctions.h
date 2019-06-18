/*
  ==============================================================================

    CommonFunctions.h
    Created: 6 May 2019 7:31:20pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginIDs.h"

struct ScaleSizeSorter
{
	// Scale first, then mode, then family

	static int compareElements(const ValueTree t1, const ValueTree t2)
	{
		if ((t1.hasType(IDs::modePresetNode) || t1.hasType(IDs::presetNode)) &&
		   (t2.hasType(IDs::modePresetNode) || t2.hasType(IDs::presetNode)))
		{
			int sz1 = (int) t1[IDs::scaleSize];
			int sz2 = (int) t2[IDs::scaleSize];

			int m1 = (int) t1[IDs::modeSize];
			int m2 = (int) t2[IDs::modeSize];

			String f1 = t1[IDs::family];
			String f2 = t2[IDs::family];

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
		else
			return 0;
	}
};

struct ModeSizeSorter
{
	// Mode first, then scale, then family

	static int compareElements(const ValueTree t1, const ValueTree t2)
	{
		if ((t1.hasType(IDs::modePresetNode) || t1.hasType(IDs::presetNode)) &&
			(t2.hasType(IDs::modePresetNode) || t2.hasType(IDs::presetNode)))
		{
			int sz1 = (int) t1[IDs::scaleSize];
			int sz2 = (int) t2[IDs::scaleSize];

			int m1 = (int) t1[IDs::modeSize];
			int m2 = (int) t2[IDs::modeSize];

			String f1 = t1[IDs::family];
			String f2 = t2[IDs::family];

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
		return 0;
	}
};
struct FamilyNameSorter
{
	// Family first, then scale, then mode

	static int compareElements(const ValueTree t1, const ValueTree t2)
	{
		if ((t1.hasType(IDs::modePresetNode) || t1.hasType(IDs::presetNode)) &&
			(t2.hasType(IDs::modePresetNode) || t2.hasType(IDs::presetNode)))
		{
			int sz1 = (int) t1[IDs::scaleSize];
			int sz2 = (int) t2[IDs::scaleSize];

			int m1 = (int) t1[IDs::modeSize];
			int m2 = (int) t2[IDs::modeSize];

			String f1 = t1[IDs::family];
			String f2 = t2[IDs::family];

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
		return 0;
	}
};

class TextFilterIntOrSpace : public TextEditor::InputFilter
{
	String filterNewText(TextEditor&, const String& newInput) override
	{
		String out;

		for (int c = 0; c < newInput.length(); c++)
		{
			if (newInput[c] >= 48 && newInput[c] < 58)
			{
				out += newInput[c];
				continue;
			}

			out += " ";
		}

		return out;
	}
};

class TextFilterInt : public TextEditor::InputFilter
{
	String filterNewText(TextEditor&, const String& newInput) override
	{
		String out;

		for (int c = 0; c < newInput.length(); c++)
		{
			if (newInput[c] == 45 || (newInput[c] >= 48 && newInput[c] < 58))
			{
				out += newInput[c];
				continue;
			}
		}

		return out;
	}
};

static Point<int> pointFromString(Point<int>& p, String pointToStringIn)
{
	String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
	String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
	p = Point<int>(val1.getIntValue(), val2.getIntValue());
	return p;
}

static Point<float> pointFromString(Point<float>& p, String pointToStringIn)
{
	String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
	String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
	p = Point<float>(val1.getFloatValue(), val2.getFloatValue());
	return p;
}

static Point<double> pointFromString(Point<double>& p, String pointToStringIn)
{
	String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
	String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
	p = Point<double>(val1.getDoubleValue(), val2.getDoubleValue());
	return p;
}

static ValueTree extractNode(ValueTree nodeOrigin, Identifier nodeType)
{
    ValueTree nodeOut;
    
    if (nodeOrigin.hasType(nodeType))
        nodeOut = nodeOrigin;
    else
        nodeOut = nodeOrigin.getChildWithName(nodeType);
        
    return nodeOut;
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
			arrayIn.add(pointFromString(arrayIn.getReference(0), childArray.getChild(i).getProperty("Value").toString()));
}

static void get_array_from_node(const ValueTree nodeIn, Array<Point<float>>& arrayIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
		for (int i = 0; i < childArray.getNumChildren(); i++)
			arrayIn.add(pointFromString(arrayIn.getReference(0), childArray.getChild(i).getProperty("Value")));
}

static void get_array_from_node(const ValueTree nodeIn, Array<Point<double>>& arrayIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
		for (int i = 0; i < childArray.getNumChildren(); i++)
			arrayIn.add(pointFromString(arrayIn.getReference(0), childArray.getChild(i).getProperty("Value")));
}

template <class T>
static void set_value_in_array(ValueTree nodeIn, Identifier arrayID, int indexToChange, T valueToSet)
{
    ValueTree arrayNode = nodeIn.getChildWithName(arrayID);
    
    if (arrayNode.isValid() && indexToChange >= 0 && indexToChange < arrayNode.getNumChildren())
    {
        arrayNode.getChild(indexToChange).setProperty("Value", valueToSet, nullptr);
    }
}

static int totalModulus(int numIn, int mod)
{
	return ((numIn % mod) + mod) % mod;
}

template <class T>
static T sumUpToIndex(Array<T> arrayIn, int indexIn)
{
	indexIn = totalModulus(indexIn, arrayIn.size());
	T sum = 0;

	for (int i = 0; i < indexIn; i++)
	{
		sum += arrayIn[i];
	}

	return sum;
}

template <class T>
static T sumUpToRingIndex(Array<T> arrayIn, int indexIn)
{
	T sum = 0;

	for (int i = 0; i < indexIn; i++)
	{
		sum += arrayIn[i % arrayIn.size()];
	}

	return sum;
}

template <class T>
static void DBGArray(Array<T>& arrayIn, String arrayName="Array")
{
	DBG(arrayName + ":");

	for (int i = 0; i < arrayIn.size(); i++)
	{
		DBG(String(i) + ": " + String(arrayIn[i]));
	}
}
