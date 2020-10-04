/*
 ==============================================================================
 
 VirtualKeyboardGrid.h
 Created: 17 Apr 2019 2:12:23pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#pragma once

#include "../../../PluginModes.h"
#include "../../../Structures/FractionalGrid.h"
#include "../../../Structures/Mode.h"
#include "VirtualKeyboardKey.h"

namespace VirtualKeyboard
{
    class KeyboardPositioner
    {
        Component* parent;
        
        int numModeKeys;
        int keyGap = 0;
        int keyPlacement = 1;
        float baseWidthRatio = 0.8f;
        float baseHeightRatio = 0.55f;

        float keyWidth = 0;
        
    public:

        KeyboardPositioner(Component* parentIn, int numModeKeysIn = 128);
        ~KeyboardPositioner() {};

        void parentResized();

        float getKeyWidth() const;

        int getKeyGap() const;

        void setNumModeKeys(int numKeysIn);

        void setKeyGap(int keyGapIn);
        
        void setKeyPlacement(int keyPlacementTypeIn);

        void setLayerKeysWidthRatio(float widthRatioIn);
        void setLayerKeysHeightRatio(float widthRatioIn);

        void resizeKey(Key& key);
        void placeKey(Key& key);
    };
}
