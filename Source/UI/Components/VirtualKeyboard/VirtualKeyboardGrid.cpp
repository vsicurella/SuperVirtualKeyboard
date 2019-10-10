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
				width = 0.8 - (key.order - 1) * 0.1;
			}
		}
	}

	key.area.setWidth(width * getColumnWidth());
	key.area.setHeight(height * getRowHeight());
}

void KeyboardGrid::placeKey(Key& key)
{
	int xPosition = 0;

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
			int colToPlace = ceil(key.modeDegree);
			float offset = (key.order > 0) * (getColumnWidth() / 2.0f);

			offset = (int)(offset * 0.8); // not sure why i have to do this to center the ordered keys

			xPosition = colToPlace * (getColumnWidth() + getColumnGap()) - offset;

			DBG("keyNumber=" + String(key.keyNumber) + " modeDegree=" + String(key.modeDegree) + " colToPlace=" + String(colToPlace) + " offset=" + String(offset) + " x=" + String(xPosition));

			break;
		}
	}
    
    key.area.setPosition(xPosition, 0);
}
