/*
  ==============================================================================

    FractionalGrid.h
    Created: 24 Mar 2019 1:58:51pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#pragma once

/* Will probably heavily edit this later once new views are designed
	but right now it's mostly designed to handle a piano key grid. 
	It sets items in place by mutating their bounds. */

class FractionalGrid
{
	// Parameters
	float columns = 1;
	float rows = 1;

	Rectangle<int> bounds;

	// Properties

	double colFraction;
	double rowFraction;

	float colSize;
	float rowSize;

protected:

	bool needsToUpdate = true;

public:

	FractionalGrid() {}

	~FractionalGrid() {}

	int columnGap = 1;
	int rowGap = 1;

	float set_columns(float colIn)
	{
		columns = colIn;
		needsToUpdate = true;
		return columns;
	}

	float set_rows(float rowIn)
	{
		rows = rowIn;
		needsToUpdate = true;
		return rows;
	}

	Point<float> set_grid(float colIn, float rowIn)
	{
		columns = colIn;
		rows = rowIn;
		needsToUpdate = true;

		return Point<float>(columns, rows);
	}

	Rectangle<int> setBounds(int x, int y, int w, int h)
	{
		bounds = Rectangle<int>(x, y, w, h);
		needsToUpdate = true;
		return bounds;
	}

	Rectangle<int> setBounds(Rectangle<int> rectIn)
	{
		bounds = rectIn;
		needsToUpdate = true;
		return bounds;
	}

	bool needs_to_update()
	{
		return needsToUpdate;
	}

	float column_size()
	{
		return colSize;
	}

	float row_size()
	{
		return rowSize;
	}

	void update_grid()
	{
		if (bounds == Rectangle<int>(0, 0, 0, 0))
			throw "Error: Bounds not set.";

		colFraction = 1.0 / columns;
		rowFraction = 1.0 / rows;

		colSize = (bounds.getWidth() + columns * columnGap) / columns;
		rowSize = (bounds.getHeight() + rows * rowGap) / rows;

		needsToUpdate = false;
	}

	/* WARNING: mutates Component's position*/
	Point<int> place_component(Component* item, int col, int row)//, Justification justificationType)
	{
		if (needsToUpdate)
			update_grid();

		Point<int> pt = Point<int>((int)(col * (colSize + columnGap)), (int)(row * (rowSize + rowGap)));
		item->setTopLeftPosition(pt);

		return pt;
	}
};