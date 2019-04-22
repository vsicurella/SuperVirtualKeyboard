/*
 ==============================================================================
 
 VirtualKeyboardGrid.h
 Created: 17 Apr 2019 2:12:23pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#pragma once

#include "../../Structures/FractionalGrid.h"
#include "../../Structures/Mode.h"
#include "VirtualKeyboardKey.h"

namespace VirtualKeyboard
{
    enum KeyPlacementType
    {
        nestedRight = 0,
        nestedCenter,
        adjacent
    };
    
    class KeyboardGrid : public FractionalGrid
    {
        Mode* mode;
        std::vector<float> orderedKeyRatios;
        
    public:
                
        KeyboardGrid(Mode* modeIn);
        ~KeyboardGrid() {};
        
        void setPlacementType(KeyPlacementType placementType);
        
        void resizeKey(Key* key);
        void placeKey(Key* key);
    };
}
