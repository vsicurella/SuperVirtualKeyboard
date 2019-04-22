/*
  ==============================================================================

    FractionalGrid.h
    Created: 24 Mar 2019 1:58:51pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#pragma once

class FractionalGrid
{
	// Parameters
	float columns;
	float rows;

	Rectangle<int> bounds;

	// Properties
	double colFraction;
	double rowFraction;

	float colSize;
	float rowSize;
    
    int columnGap = 1;
    int rowGap = 1;

    // Methods
    void updateGrid();

public:

    FractionalGrid(float columsIn, float rowsIn);

    ~FractionalGrid();

    Rectangle<int> setBounds(int x, int y, int w, int h);

    Rectangle<int> setBounds(Rectangle<int> rectIn);
    
    float getColumns();
    
    float getRows();

    float getColumnSize();

    float getRowSize();
    
    int getColumnGap();
    
    int getRowGap();
    
    void setColumnGap(int colGapIn);
    
    void setRowGap(int rowGapIn);

	/* WARNING: mutates Component's position*/
    Point<int> place_component(Component* item, int col, int row);
};
