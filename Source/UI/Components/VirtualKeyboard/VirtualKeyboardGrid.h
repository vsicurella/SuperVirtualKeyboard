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
	enum KeyPlacementType
	{
		nestedRight = 1,
		nestedCenter,
		flat,
		adjacent
	};

    class KeyboardGrid : public FractionalGrid
    {
        Mode* mode;
        std::vector<float> orderedKeyRatios;

		int keyPlacement = 1;
        
    public:
                
		KeyboardGrid(Mode* modeIn, int numRows=1, int keyPlacementType=1);
        ~KeyboardGrid() {};
                
		void setKeyPlacement(int keyPlacementTypeIn);

        void resizeKey(Key& key);
        void placeKey(Key& key);
    };
}
