/*
  ==============================================================================

    CommonFunctions.h
    Created: 6 May 2019 7:31:20pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginIDs.h"
#include "Structures/OwnedHashMap.h"

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

struct IDasStringHash
{
    static int generateHash (const Identifier& key, int upperLimit) noexcept
    {
        return DefaultHashFunctions::generateHash(key.toString(), upperLimit);
    }
};

//typedef OwnedHashMap<Identifier, RangedAudioParameter, IDasStringHash> SvkParameters;

template <class T, class U>
struct Pair
{
    T key;
    U value;

    Pair<T, U>(T keyIn, U valueIn)
    {
        key = keyIn;
        value = valueIn;
    }
};

static bool toggleMenuItemEnablement(PopupMenu& menuIn, int itemIdIn, bool shouldBeEnabled)
{
    PopupMenu::MenuItemIterator menuItems(menuIn);
    while (menuItems.next())
    {
        PopupMenu::Item& item = menuItems.getItem();
        if (item.itemID == itemIdIn)
        {
            item.isEnabled = shouldBeEnabled;
            return true;
        }
    }

    return false;
}

//struct MidiPitch : Pair<int, int>
//{
//    MidiPitch(int midiNoteIn=-1, int pitchbendIn=0)
//    : Pair<int, int>(midiNoteIn, pitchbendIn) {}
//};

//static Point<int> pointFromString(Point<int>& p, String pointToStringIn)
//{
//    String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
//    String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
//    p = Point<int>(val1.getIntValue(), val2.getIntValue());
//    return p;
//}
//
//static Point<float> pointFromString(Point<float>& p, String pointToStringIn)
//{
//    String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
//    String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
//    p = Point<float>(val1.getFloatValue(), val2.getFloatValue());
//    return p;
//}
//
//static Point<double> pointFromString(Point<double>& p, String pointToStringIn)
//{
//    String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
//    String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
//    p = Point<double>(val1.getDoubleValue(), val2.getDoubleValue());
//    return p;
//}

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

///* Adds a Colour array reduced to nontrivial items to a node represented in a ValueTree structure */

//static void add_array_to_node(ValueTree nodeIn, const Array<Colour>& arrayIn, Identifier arrayID, Identifier itemId)
//{
//    ValueTree arrayTree = ValueTree(arrayID);
//    ValueTree item;
//
//    for (int i = 0; i < arrayIn.size(); i++)
//    {
//        if (arrayIn[i].isOpaque())
//        {
//            item = ValueTree(itemId);
//            item.setProperty("Key", i, nullptr);
//            item.setProperty("Value", arrayIn[i].toString(), nullptr);
//            arrayTree.addChild(item, i, nullptr);
//        }
//    }
//
//    nodeIn.addChild(arrayTree, -1, nullptr);
//}
//
//static void add_array_as_property(ValueTree& nodeIn, const Array<Colour>& arrayIn, Identifier itemId)
//{    
//    String array = "";
//
//    for (int i = 0; i < arrayIn.size(); i++)
//    {
//        array += arrayIn[i].toString();
//        array += '\n';
//    }
//
//    nodeIn.setProperty(itemId, array, nullptr);
//}

//static void get_array_from_node(const ValueTree& nodeIn, Array<Colour>& arrayIn, Identifier arrayID)
//{
//    String array = nodeIn[arrayID];
//    String value = "";
//    int charsRead = 0;
//
//    Colour c = Colours::transparentWhite;
//
//    while (charsRead < array.length())
//    {
//        value += array[charsRead];
//
//        if (charsRead + 1 < array.length())
//        {
//            if (array[charsRead + 1] == '\n')
//                c = Colour::fromString(value);
//        }
//
//        if (c != Colours::transparentWhite)
//        {
//            arrayIn.add(c);
//            c = Colours::transparentWhite;
//        }
//    }
//}

///* Creates a Colour array from a node, and populates trivial and nontrivial keys */

//static void get_array_from_node(const ValueTree nodeIn, Array<Colour>& arrayIn, Identifier arrayID, int arraySizeOut)
//{
//    ValueTree arrayNode;
//    ValueTree item;
//
//    arrayIn.resize(arraySizeOut);
//
//    arrayNode = nodeIn.getChildWithName(arrayID);
//
//    for (int i = 0; i < arrayNode.getNumChildren(); i++)
//    {
//        item = arrayNode.getChild(i);
//        arrayIn.set(item["Key"], Colour::fromString(item["Value"].toString()));
//    }
//}

//template <class T>
//static void add_array_to_node(ValueTree nodeIn, const Array<Point<T>>& arrayIn, Identifier arrayID, Identifier itemId)
//{
//    ValueTree arrayTree = ValueTree(arrayID);
//    ValueTree item;
//    for (int i = 0; i < arrayIn.size(); i++)
//    {
//        item = ValueTree(itemId);
//        item.setProperty("Value", arrayIn[i].toString(), nullptr);
//        arrayTree.addChild(item, i, nullptr);
//    }
//
//    nodeIn.addChild(arrayTree, -1, nullptr);
//}

//static void get_array_from_node(const ValueTree nodeIn, Array<Point<int>>& arrayIn, Identifier arrayID)
//{
//    ValueTree childArray = nodeIn.getChildWithName(arrayID);
//
//    if (childArray.isValid())
//        for (int i = 0; i < childArray.getNumChildren(); i++)
//            arrayIn.add(pointFromString(arrayIn.getReference(0), childArray.getChild(i).getProperty("Value").toString()));
//}
//
//static void get_array_from_node(const ValueTree nodeIn, Array<Point<float>>& arrayIn, Identifier arrayID)
//{
//    ValueTree childArray = nodeIn.getChildWithName(arrayID);
//
//    if (childArray.isValid())
//        for (int i = 0; i < childArray.getNumChildren(); i++)
//            arrayIn.add(pointFromString(arrayIn.getReference(0), childArray.getChild(i).getProperty("Value")));
//}
//
//static void get_array_from_node(const ValueTree nodeIn, Array<Point<double>>& arrayIn, Identifier arrayID)
//{
//    ValueTree childArray = nodeIn.getChildWithName(arrayID);
//
//    if (childArray.isValid())
//        for (int i = 0; i < childArray.getNumChildren(); i++)
//            arrayIn.add(pointFromString(arrayIn.getReference(0), childArray.getChild(i).getProperty("Value")));
//}

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
    int val = 0;

    if (mod != 0)
        val = ((numIn % mod) + mod) % mod;

    return val;
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
Array<T> reverseArray(const Array<T>& arrayIn)
{
    Array<T> outArray;
    outArray.resize(arrayIn.size());

    for (int i = 0; i < arrayIn.size(); i++)
    {
        outArray.set(i, arrayIn.getUnchecked(arrayIn.size() - i - 1));
    }

    return outArray;
}

template <class T>
String arrayToString(const Array<T>& arrayIn)
{
    String arrayString = "";
    
    for (int i = 0; i < arrayIn.size(); i++)
    {
        arrayString+= String(arrayIn[i]);
        
        if (i + 1 < arrayIn.size())
            arrayString += ", ";
    }
    
    return arrayString;
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

static double getStdFrequency(int midiNoteIn)
{
    return 440.0 * pow(2, (midiNoteIn - 69) / 12.0);
}