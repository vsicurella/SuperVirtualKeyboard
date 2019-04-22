/*
  ==============================================================================

    VirtualKeyboardGrid.cpp
    Created: 17 Apr 2019 2:12:23pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "VirtualKeyboardGrid.h"

using namespace VirtualKeyboard;

KeyboardGrid::KeyboardGrid(Mode* modeIn)
: FractionalGrid(modeIn->getKeyboardOrdersSize(0), 1), mode(modeIn)
{
    setPlacementType(KeyPlacementType::nestedRight);
}

void KeyboardGrid::setPlacementType(KeyPlacementType placementType)
{
    orderedKeyRatios.clear();
    
    switch (placementType)
    {
        case 1:
            break;
        case 2:
            break;
        default: // aka nestedRight
            
            float localOrder;
            float height;
            float heightMod;
            float xp = 0.618;
            
            for (int i = 0; i < mode->getModeSize(); i++)
            {
                localOrder = mode->getSteps()[i];
                
                if (localOrder < 2)
                    orderedKeyRatios.push_back(1);
                else
                {
                    for (int j = 0; j < localOrder; j++)
                    {
                        if (j == 0)
                            height = 1;
                        else
                            height = (0.5 + 0.2 * localOrder / 6.0) * (pow(localOrder - j, xp) / pow(localOrder-1, xp));
                        
                        orderedKeyRatios.push_back(height);
                    }
                }
            }
			jassert(orderedKeyRatios.size() == mode->getScaleSize());
			break;
    }
}

void KeyboardGrid::resizeKey(Key* key)
{
    key->orderHeightRatio = orderedKeyRatios[key->keyNumber % mode->getScaleSize()];
    key->orderWidthRatio = 1.0f - (key->order > 0) * 1.25f * key->order / 8.0f;
}

void KeyboardGrid::placeKey(Key* key)
{
    Point<int> pt;
    
    float colToPlace = ceil(key->modeDegree);
    int offset = (key->order > 0) * (int)(getColumnSize() / 2.0);
    offset = (int)(offset * 1.2); // not sure why i have to do this to center the ordered keys
    
    pt = Point<int>((int)((colToPlace + 1) * (getColumnSize() + getColumnGap()) - offset), 0 + getRowGap());
    
    key->setTopRightPosition(pt.x, pt.y);
}
