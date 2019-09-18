/*
 ==============================================================================
 
 VirtualKeyboardGrid.h
 Created: 17 Apr 2019 2:12:23pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#pragma once

#include "../../../Structures/FractionalGrid.h"
#include "../../../Structures/Mode.h"
#include "VirtualKeyboardKey.h"

namespace VirtualKeyboard
{
    class KeyboardGrid : public FractionalGrid
    {
        Mode* mode;
        std::vector<float> orderedKeyRatios;
        
    public:
                
        KeyboardGrid(Mode* modeIn);
        ~KeyboardGrid() {};
                
        void resizeKey(Key& key);
        void placeKey(Key& key);
    };
}
