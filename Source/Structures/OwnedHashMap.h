/*
  ==============================================================================

    OwnedHashMap.h
    Created: 17 Nov 2019 4:50:00pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

template <typename KeyType,
          typename ValueType,
          class HFunctionType = DefaultHashFunctions,
          class CriticalSectionToUse = DummyCriticalSection>

class OwnedHashMap : public OwnedArray<ValueType, CriticalSectionToUse>,
                    public HashMap<KeyType, ValueType*, HFunctionType, CriticalSectionToUse>
{

public:
    
    typedef OwnedArray<ValueType> TheArray;
    typedef HashMap<KeyType, ValueType*, HFunctionType, CriticalSectionToUse> TheHash;
        
    OwnedHashMap(int numberOfSlots=101, HFunctionType hashFunction=HFunctionType())
        : TheArray(), TheHash(numberOfSlots, hashFunction)
    {
    }
    ~OwnedHashMap()
    {
        this->TheHash::clear();
        this->TheArray::clear();
    }
    
    TheArray& getOwnedArray()
    {
        return dynamic_cast<TheArray&>(*this);
    }
    
    TheHash& getHashMap()
    {
        return dynamic_cast<TheHash&>(*this);
    }
    
    int getSize()
    {
        return this->TheArray::size();
    }
    
    ValueType* stash(KeyType keyIn, ValueType* valueIn)
    {
        ValueType* valueOut = valueIn;
        
        this->add(valueIn);
        this->TheHash::set(keyIn, valueOut);
        
        return valueOut;
    }
    
    ValueType* grab(const KeyType& keyIn) const
    {
        return this->TheHash::operator[](keyIn);
    }
    
    void toss(KeyType keyIn)
    {
        this->TheArray::removeObject(grab(keyIn));
        this->TheHash::remove(keyIn);
    }

    static void test()
    {
        // keep the "fun" in "function"

        OwnedHashMap<String, RangedAudioParameter> parameters;

        Identifier normal("NormalcyAmount");
        parameters.stash(normal.toString(), new AudioParameterFloat(normal.toString(), "Sounds good", 0.0f, 100.0f, 50.0f));

        Identifier weird("WeirdnessAmount");
        parameters.stash(weird.toString(), new AudioParameterFloat(weird.toString(), "What??", 0.0f, 100.0f, 50.0f));

        parameters.grab(weird.toString())->setValue(100.0f);
        parameters.grab(normal.toString())->setValue(0.0f);
        parameters.toss(normal.toString());
    }
};

