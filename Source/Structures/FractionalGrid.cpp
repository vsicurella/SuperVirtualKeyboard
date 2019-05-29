/*
 ==============================================================================
 
 FractionalGrid.cpp
 Created: 22 Apr 2019 12:05:22pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#include "FractionalGrid.h"

FractionalGrid::FractionalGrid(float columsIn, float rowsIn)
{
    columns = columsIn;
    rows = rowsIn;
}

FractionalGrid::~FractionalGrid() {}

Rectangle<int> FractionalGrid::setBounds(int x, int y, int w, int h)
{
    bounds = Rectangle<int>(x, y, w, h);
    
    updateGrid();
    return bounds;
}

Rectangle<int> FractionalGrid::setBounds(Rectangle<int> rectIn)
{
    bounds = rectIn;
    
    updateGrid();
    return bounds;
}

void FractionalGrid::updateGrid()
{
    colFraction = 1.0 / columns;
    rowFraction = 1.0 / rows;
    
    colSize = (bounds.getWidth() + columns * columnGap) / columns;
    rowSize = (bounds.getHeight() + rows * rowGap) / rows;
}

float FractionalGrid::getColumnSize()
{
    return colSize;
}

float FractionalGrid::getRowSize()
{
    return rowSize;
}

float FractionalGrid::getColumns()
{
    return columns;
}

float FractionalGrid::getRows()
{
    return rows;
}

void FractionalGrid::setColumnGap(int colGapIn)
{
    columnGap = colGapIn;
    updateGrid();
}

void FractionalGrid::setRowGap(int rowGapIn)
{
    rowGap = rowGapIn;
    updateGrid();
}

int FractionalGrid::getColumnGap()
{
    return columnGap;
}

int FractionalGrid::getRowGap()
{
    return rowGap;
}


Point<int> FractionalGrid::place_component(Component* item, int col, int row)
{
    if (bounds.getX() == 0 || bounds.getY() == 0)
        throw "Cannot have 0 size";
    
    Point<int> pt = Point<int>((int)(col * (colSize + columnGap)), (int)(row * (rowSize + rowGap)));
    item->setTopLeftPosition(pt);
    
    return pt;
}

