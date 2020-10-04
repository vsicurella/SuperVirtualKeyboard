/*
  ==============================================================================

    VirtualKeyboardGrid.cpp
    Created: 17 Apr 2019 2:12:23pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "VirtualKeyboardGrid.h"

using namespace VirtualKeyboard;

KeyboardPositioner::KeyboardPositioner(Component* parentIn, int numModeKeysIn)
    : parent(parentIn)
{
    setNumModeKeys(numModeKeysIn);
}

void KeyboardPositioner::parentResized()
{
    keyWidth = (parent->getWidth() - keyGap) / numModeKeys;
}

float KeyboardPositioner::getKeyWidth() const
{
    return keyWidth;
}

int KeyboardPositioner::getKeyGap() const
{
    return keyGap;
}

void KeyboardPositioner::setNumModeKeys(int numKeysIn)
{
    numModeKeys = numKeysIn;
    parentResized();
}

void KeyboardPositioner::setKeyGap(int keyGapIn)
{
    keyGap = keyGapIn;
    parentResized();
}

void KeyboardPositioner::setKeyPlacement(int keyPlacementTypeIn)
{
    keyPlacement = keyPlacementTypeIn;
}

void KeyboardPositioner::setLayerKeysWidthRatio(float widthRatioIn)
{
    baseWidthRatio = widthRatioIn;
}

void KeyboardPositioner::setLayerKeysHeightRatio(float heightRatioIn)
{
    baseHeightRatio = heightRatioIn;
}

void KeyboardPositioner::resizeKey(Key& key)
{
    float spread = 4; // TODO make setter
    float height, width;
    
    if (key.order == 0)
    {
        height = 1;
        width = 1;
    }
    else
    {
        float stepHeight = baseHeightRatio + (key.step - 2) / 100.0f * spread;
        width = baseWidthRatio;

        switch (keyPlacement)
        {
            case(KeyPlacementType::flat):
            {
                height = stepHeight - stepHeight * (key.order - 1) / (float)key.step * 1.125f;
                break;
            }

            case(KeyPlacementType::adjacent):
            {
                height = stepHeight;
                width /= (key.step - 1) * 0.9f; // mode keys of step 1 will not be in this else condition

                if (key.step == 2)
                    width *= 0.75f;

                break;
            }

            default: // nested keys
            {
                height = stepHeight - stepHeight * (key.order - 1) / (key.step * 1.08f);
                width -= (key.order - 1) * 0.1f;
                break;
            }
        }
    }
    
    key.setSize(width * keyWidth, height * parent->getHeight());
}

void KeyboardPositioner::placeKey(Key& key)
{

}
