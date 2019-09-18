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
}

void KeyboardGrid::resizeKey(Key& key)
{
	
    key.area.setHeight(orderedKeyRatios[key.keyNumber % mode->getScaleSize()]);
    key.area.setWidth(1.0f - (key.order > 0) * 1.25f * key.order / 8.0f);
	
}

void KeyboardGrid::placeKey(Key& key)
{
	
    Point<int> pt;
    
    float colToPlace = ceil(key.modeDegree);
    int offset = (key.order > 0) * (int)(getColumnSize() / 2.0);
    
    offset = (int)(offset * 1.2); // not sure why i have to do this to center the ordered keys
    
    pt = Point<int>((int)((colToPlace + 1) * (getColumnSize() + getColumnGap()) - offset), 0 + getRowGap());
    
    key.area.setPosition(pt);
	
}
