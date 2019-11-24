/*
  ==============================================================================

    VirtualKeyboardGrid.cpp
    Created: 17 Apr 2019 2:12:23pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "VirtualKeyboardGrid.h"

using namespace VirtualKeyboard;

KeyboardGrid::KeyboardGrid(Mode* modeIn, int numRows, int keyPlacementType)
: FractionalGrid(modeIn->getKeyboardOrdersSize(0), numRows), mode(modeIn), keyPlacement(keyPlacementType)
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
                height = stepHeight - stepHeight * (key.order - 1) / (key.step * 1.08f);
                width = 0.75f - (key.order - 1) * 0.1f;
                break;
            }

            case(KeyPlacementType::flat):
            {
                float stepHeight = 0.55 + (key.step - 2) / 100.0f * spread;
                height = stepHeight - stepHeight * (key.order - 1) / key.step;
                width = 0.8f;
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
    
    key.setSize(width * colSize, height * rowSize);
}

void KeyboardGrid::placeKey(Key& key)
{
    int row = rowSize + rowGap;
    int rowKeyLimit = round(columns / (mode->getModeSize() * rows)) * mode->getModeSize();
    
    float column = colSize + columnGap;
    float halfColumn = column / 1.75f * (key.order > 0);
    int colToPlace = (int) ceil(key.modeDegree);
    int rowToPlace = 0;
    
    if (colToPlace >= rowKeyLimit)
    {
        rowToPlace = colToPlace / rowKeyLimit;

        // let last row be longer to preserve period
        if (128 - key.keyNumber - 1 <= mode->getModeSize())
            rowToPlace -= 1;
        
        colToPlace -= rowKeyLimit * rowToPlace;
    }
            
    int x;
    int y = rowToPlace * row;

    //DBG("halfcolumn="+String(halfColumn));
    
	switch (keyPlacement)
	{
		case(KeyPlacementType::nestedCenter):
		{
            x = colToPlace * column - halfColumn + (column - key.getWidth() / 2);
            
            key.setTopLeftPosition(x, y);
            break;
		}

		case(KeyPlacementType::adjacent):
		{
            int keyCol = (int) key.modeDegree;
            x = keyCol * column + halfColumn;
            float stepOff = (key.order > 0 && key.step > 2) * (float)(key.order)/key.step;
//            DBG("KeyMD=" + String(key.modeDegree) + "\tStepOff=" + String(stepOff));
            x += stepOff * (key.getWidth() * column);
            key.setTopLeftPosition(x, y);

			break;
		}

		default: // 0 & 1, key nested right & flat
		{
            x = colToPlace * column - halfColumn + (column - key.getWidth());
            key.setTopLeftPosition(x, y);
			break;
		}
	}
}
