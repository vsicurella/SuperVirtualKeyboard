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
            //if (key.order > 0)
            //{
                float stepHeight = 0.55 + (key.step - 2) / 100.0f * spread;
                height = stepHeight - stepHeight * (key.order - 1) / (key.step * 1.08f);
                width = 0.75f - (key.order - 1) * 0.1f;
            //}
            break;
        }

        case(KeyPlacementType::flat):
        {
//            if (key.order > 0)
//            {
                float stepHeight = 0.55 + (key.step - 2) / 100.0f * spread;
                height = stepHeight - stepHeight * (key.order - 1) / key.step;
                width = 0.8f;
//            }
            break;
        }

        case(KeyPlacementType::adjacent):
        {
            //if (key.order > 0)
            //{
                height = 0.55;
                width = 0.8f / (key.step-1); // shouldn't ever be 1 here
//            }
//            else
//            {
//                width += key.step > 1 * (key.step - 2) * (0.02f);
//            }
            break;
        }

        default: // 0 & 1, key nested right
        {
//            if (key.order > 0)
//            {
                float stepHeight = 0.55f + (key.step - 2) / 100.0f * spread;
                height = stepHeight - stepHeight * (key.order - 1) / key.step;
                width = 0.8f - (key.order - 1) * 0.1f;
//            }
            break;
        }
	}
    }
    key.setSize(width*getColumnWidth(), height*getRowHeight());
}

void KeyboardGrid::placeKey(Key& key)
{
	int xPosition = 0;
    float column = getColumnWidth() + getColumnGap();
    float halfColumn = column / 2.0f * (key.order > 0);
    int colToPlace = ceil(key.modeDegree);
    //DBG("halfcolumn="+String(halfColumn));
    
	switch (keyPlacement)
	{
		case(KeyPlacementType::nestedCenter):
		{
            xPosition = colToPlace * column - halfColumn;
            xPosition *= 1.0f + (0.001 * 10 * key.order > 1);
            key.setCentrePosition(xPosition, key.getHeight()/2.0f);
            break;
		}

		case(KeyPlacementType::adjacent):
		{
            int keyCol = (int) key.modeDegree;
            xPosition = keyCol * column + halfColumn;
            float stepOff = (key.order > 0 && key.step > 2) * (float)(key.order)/key.step;
            DBG("KeyMD=" + String(key.modeDegree) + "\tStepOff=" + String(stepOff));
            xPosition += stepOff * (key.getWidth() * column);
            key.setTopLeftPosition(xPosition, 0);

			break;
		}

		default: // 0 & 1, key nested right & flat
		{
            xPosition = colToPlace * column - halfColumn;

			//DBG("keyNumber=" + String(key.keyNumber) + " modeDegree=" + String(key.modeDegree) + " colToPlace=" + String(colToPlace) + " offset=" + String(offset) + " x=" + String(xPosition));

            key.setTopRightPosition(xPosition, 0);
			break;
		}
	}
}
