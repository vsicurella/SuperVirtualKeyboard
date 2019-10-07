/*
  ==============================================================================

    VirtualKeyboardGrid.cpp
    Created: 17 Apr 2019 2:12:23pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "VirtualKeyboardGrid.h"

using namespace VirtualKeyboard;

KeyboardGrid::KeyboardGrid(Mode* modeIn, int keyPlacementType)
: FractionalGrid(modeIn->getKeyboardOrdersSize(0), 1), mode(modeIn), keyPlacement(keyPlacementType)
{
}

void KeyboardGrid::setKeyPlacement(int keyPlacementTypeIn)
{
	keyPlacement = keyPlacementTypeIn;
}

void KeyboardGrid::setDefaultKeySize(int widthIn, int heightIn)
{
	keyWidth = widthIn;
	keyHeight = heightIn;
}



void KeyboardGrid::resizeKey(Key& key)
{
	float spread = 4;
	float height, width;
	
	if (key.order == 0)
	{
		height = 1;
		width = 1;
	}
	else
	{
		switch (keyPlacement)
		{
			case(KeyPlacementType::nestedCenter):
			{

				break;
			}

			case(KeyPlacementType::flat):
			{
				float stepHeight = 0.55 + (key.step - 2) / 100.0f * spread;
				height = stepHeight - stepHeight * (key.order - 1) / key.step;
				width = 0.8;

				break;
			}

			case(KeyPlacementType::adjacent):
			{

				break;
			}

			default: // 0 & 1, key nested right
			{
				float stepHeight = 0.55 + (key.step - 2) / 100.0f * spread;
				height = stepHeight - stepHeight * (key.order - 1) / key.step;
				width = 1.0f - 5 * key.order / 32.0f;
				//width = 0.8 - (key.order - 1) * 0.1;
			}
		}
	}

	key.area.setWidth(width * keyWidth);
    key.area.setHeight(height * keyHeight);
}

void KeyboardGrid::placeKey(Key& key)
{
	Point<int> pt;

	switch (keyPlacement)
	{
		case(KeyPlacementType::nestedCenter):
		{
			
			break;
		}

		case(KeyPlacementType::adjacent):
		{
			
			break;
		}

		default: // 0 & 1, key nested right & flat
		{
			float colToPlace = ceil(key.modeDegree);
			float offset = (key.order > 0) * (getColumnSize() / 2.0);

			//offset = (int)(offset * 1.2); // not sure why i have to do this to center the ordered keys

			pt = Point<int>((int)((colToPlace + 1) * (getColumnSize() + getColumnGap()) - offset), 0 + getRowGap());

			break;
		}
	}
    
    key.area.setPosition(pt);
}
