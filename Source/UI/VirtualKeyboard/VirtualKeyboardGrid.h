/*
 ==============================================================================
 
 VirtualKeyboardGrid.h
 Created: 17 Apr 2019 2:12:23pm
 Author:  Soundtoys Quality Assurance
 
 ==============================================================================
 */

#pragma once

#include "../../Structures/FractionalGrid.h"
#include "../../Structures/Mode.h"
#include "VirtualKeyboardKey.h"

namespace VirtualKeyboard
{
    struct KeyboardGrid : public FractionalGrid
    {
        int modeSize;
        
        Mode* mode;
        
        std::vector<float> orderedKeyRatios;
                
        KeyboardGrid(Mode* modeIn);
        
        ~KeyboardGrid() {};
        
        void set_ordered_key_view(int placementType);
        
        void resize_ordered_key(Key* key);
        
        void resize_ordered_keys(OwnedArray<Key>* keys);
        
        void place_key(Key* key);
        
        void place_key_layout(OwnedArray<Key>* keys);
        
    };
    
}
