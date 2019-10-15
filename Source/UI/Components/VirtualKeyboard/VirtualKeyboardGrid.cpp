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
                float stepHeight = 0.55 + (key.step - 2) / 100.0f * spread;
                height = stepHeight - stepHeight * (key.order - 1) / (key.step * 1.08);
                width = 0.75 - (key.order - 1) * 0.1;
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
                height = 0.55;
                width = 1.0f / (key.step);
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
    
    key.setSize(width*getColumnWidth(), height*getRowHeight());
}

void KeyboardGrid::placeKey(Key& key)
{
	int xPosition = 0;

	switch (keyPlacement)
	{
		case(KeyPlacementType::nestedCenter):
		{
            int colToPlace = ceil(key.modeDegree);
            float offset = (key.order > 0) * (getColumnWidth() / 2.0f);
            offset = (int)(offset * 1.1);
            xPosition = colToPlace * (getColumnWidth() + getColumnGap()) - offset;
            xPosition *= 1.0f + (0.001 * 10 * key.order > 1);
            key.setCentrePosition(xPosition, key.getHeight()/2.0f);
            break;
		}

		case(KeyPlacementType::adjacent):
		{
            int colToPlace = (int)(key.modeDegree);
            float offset = (key.modeDegree - colToPlace) * key.step > 2 * (key.step-1) * (key.getWidth() * 2);
            xPosition = colToPlace * (getColumnWidth() + getColumnGap()) + offset;
            key.setTopLeftPosition(xPosition, 0);

			break;
		}

		default: // 0 & 1, key nested right & flat
		{
			int colToPlace = ceil(key.modeDegree);
			float offset = (key.order > 0) * (getColumnWidth() / 2.0f);

			offset = (int)(offset * 1.2); // not sure why i have to do this to center the ordered keys

			xPosition = colToPlace * (getColumnWidth() + getColumnGap()) - offset;

			//DBG("keyNumber=" + String(key.keyNumber) + " modeDegree=" + String(key.modeDegree) + " colToPlace=" + String(colToPlace) + " offset=" + String(offset) + " x=" + String(xPosition));

            key.setTopRightPosition(xPosition, 0);
			break;
		}
	}
    
    
}
