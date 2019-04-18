/*
  ==============================================================================

    VirtualKeyboardGrid.cpp
    Created: 17 Apr 2019 2:12:23pm
    Author:  Soundtoys Quality Assurance

  ==============================================================================
*/

#include "VirtualKeyboardGrid.h"

using namespace VirtualKeyboard;

KeyboardGrid::KeyboardGrid()
{
    order = 1;
    modeSize = 1;
    layout;
}

KeyboardGrid::KeyboardGrid(ModeLayout* layoutIn)
{
    layout = layoutIn;
    order = layout->get_highest_order();
    modeSize = layout->modeSize;
    
    set_grid(layout->get_num_modal_notes(), 1);
}

/*
	Circular dependencies issues....
	0 = nestedRight
	1 = nestedCenter
	2 = adjacent
*/

void KeyboardGrid::set_ordered_key_view(int placementType)
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
            
            for (int i = 0; i < layout->modeSize; i++)
            {
                localOrder = layout->get_steps()[i];
                
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
			jassert(orderedKeyRatios.size() == layout->scaleSize);
			break;
    }
}

void KeyboardGrid::resize_ordered_key(Key* key)
{
    if (orderedKeyRatios.size() < 1)
        set_ordered_key_view(0);
    
    key->orderHeightRatio = orderedKeyRatios[key->keyNumber % layout->scaleSize];
    key->orderWidthRatio = 1.0f - (key->order > 0) * 1.25f * key->order / 8.0f;
    
}

void KeyboardGrid::resize_ordered_keys(OwnedArray<Key>* keys)
{
    Key* key;
    
    for (int i = 0; i < keys->size(); i++)
    {
        key = keys->getUnchecked(i);
        resize_ordered_key(key);
    }
}


void KeyboardGrid::place_key(Key* key)
{
    if (needs_to_update())
        update_grid();
    
    Point<int> pt;
    
    float colToPlace = ceil(key->modeDegree);
    int offset = (key->order > 0) * (int)(column_size() / 2.0);
    offset = (int)(offset * 1.2); // not sure why i have to do this to center the ordered keys
    
    if (key->order > 0)
    {
        colToPlace = ceil(colToPlace);
    }
    
    pt = Point<int>((int)((colToPlace + 1) * (column_size() + columnGap) - offset), 0 + rowGap);
    
    key->setTopRightPosition(pt.x, pt.y);
}

void KeyboardGrid::place_key_layout(OwnedArray<Key>* keys)
{
    if (needs_to_update())
        update_grid();
    
    for (int i = 0; i < keys->size(); i++)
    {
        place_key(keys->getUnchecked(i));
    }
}
