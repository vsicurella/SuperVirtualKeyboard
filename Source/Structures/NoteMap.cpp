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
    if (sizeIn > 0)
        size = sizeIn;
    else
        size = 128;

    keys.clear();
    values.clear();

    nullVal = nullValIn;
    int defaultVal = nullVal;

    for (int i = 0; i < size; i++)
    {
        if (useIdentity)
            defaultVal = i;

        keys.add(defaultVal);
        values.add(defaultVal);
    }
}

NoteMap::NoteMap(Array<int> valuesIn, int nullValIn)
{
    nullVal = nullValIn;
    setValues(valuesIn);
}

NoteMap::NoteMap(ValueTree noteMappingNode)
    : NoteMap(noteMappingNode[IDs::mappingSize], true, -1)
{
    for (auto child : noteMappingNode)
    {
        if (child.hasType(IDs::noteMapNode))
        {
            int key = child[IDs::mappingMidiNoteIn];
            if (key < size)
                setValue(key, child[IDs::pianoKeyNumber]);
        }
    }
}

NoteMap::NoteMap(const NoteMap& mapToCopy)
{
    size = mapToCopy.size;
    nullVal = mapToCopy.nullVal;
    
    keys.clear();
    values.clear();
    
    for (int i = 0; i < size; i++)
    {
        keys.add(mapToCopy.keys.getUnchecked(i));
        values.add(mapToCopy.values.getUnchecked(i));
    }

}

ValueTree NoteMap::getAsValueTree(Identifier parentNodeId, bool includeIdentities) const
{
    ValueTree node(parentNodeId);
    node.setProperty(IDs::mappingSize, size, nullptr);

    for (int i = 0; i < size; i++)
    {
        if (includeIdentities || values[i] != i)
        {
            ValueTree value(IDs::noteMapNode);
            value.setProperty(IDs::mappingMidiNoteIn, i, nullptr);
            value.setProperty(IDs::pianoKeyNumber, values[i], nullptr);
            node.appendChild(value, nullptr);
        }
    }

    return node;
}

void NoteMap::setValue(int keyNum, int valIn)
{
    int oldVal = values.getUnchecked(keyNum);
    
    if (oldVal >= 0)
        keys.set(oldVal, nullVal);

    values.set(keyNum, valIn);
    
    if (valIn >= 0)
        keys.set(valIn, keyNum);
}

void NoteMap::setValues(Array<int> valuesIn)
{
    size = valuesIn.size();

    values.clear();
    keys.clear();
    
    values.resize(size);
    keys.resize(size);

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
    return keys.getUnchecked(valIn);
}
int NoteMap::getValue(int keyNum)
{
    return values.getUnchecked(keyNum);
}

const Array<int>& NoteMap::getKeys() const
{
    return keys;
}

const Array<int>& NoteMap::getValues() const
{
    return values;
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
        toStr += String(i) + "\t: " + String(values.getUnchecked(i)) + "\n";
    }
    
    return toStr;
}
