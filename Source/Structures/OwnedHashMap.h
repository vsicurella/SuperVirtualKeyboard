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
    
    TheArray& getOwnedArray()
    {
        return dynamic_cast<TheArray&>(this);
    }
    
    TheHash& getHashMap()
    {
        return dynamic_cast<TheHash&>(this);
    }
    
    ValueType* stash(KeyType keyIn, ValueType* valueIn)
    {
        ValueType* valueOut = valueIn;
        
        
        this->add(valueIn);
        this->TheHash::set(keyIn, valueOut);
        
        return valueOut;
    }
    
    ValueType* grab(KeyType keyIn)
    {
        return this->TheHash::operator[](keyIn);
    }
    
    void toss(KeyType keyIn)
    {
        this->TheHash::remove(keyIn);
        this->TheArray::remove(keyIn);
    }
};

