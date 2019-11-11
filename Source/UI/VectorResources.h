/*
  ==============================================================================

    VectorResources.h
    Created: 10 Nov 2019 10:50:29pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

static void drawSaveIcon(Graphics& g, Rectangle<int> bounds, Colour backgroundFill=Colour(),
                         Colour outLineColor=Colours::lightgrey)
{
    g.setColour(backgroundFill);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
    
    g.setColour(outLineColor);
    g.drawRoundedRectangle(bounds.toFloat(), 3, 1);
    
    g.setColour(Colours::whitesmoke);

    int boundsSize = bounds.getWidth();
    float imgSize = boundsSize * 7.0f / 8.0f;
    float thickness = imgSize / 32.0f;
    Rectangle<float> boundsReduced = bounds.reduced(bounds.getWidth() - imgSize).toFloat();
    
    Path outline = Path();

    outline.addLineSegment(Line<float>(boundsReduced.getTopLeft().toFloat(), boundsReduced.getBottomLeft().toFloat()), thickness);
    outline.addLineSegment(Line<float>(boundsReduced.getBottomLeft().toFloat(), boundsReduced.getBottomRight().toFloat()), thickness);
    
    float corner = imgSize / 6.0f;
    Point<float> cornerBottom = boundsReduced.getTopRight().withY(  boundsReduced.getTopRight().getY() + corner);
    Point<float> cornerLeft = boundsReduced.getTopRight().withX(  boundsReduced.getTopRight().getX() - corner);
    
    outline.addLineSegment(Line<float>(boundsReduced.getBottomRight().toFloat(), cornerBottom), thickness);
    outline.addLineSegment(Line<float>(cornerBottom, cornerLeft), thickness);
    outline.addLineSegment(Line<float>(cornerLeft, boundsReduced.getTopLeft().toFloat()), thickness);
    
    outline = outline.createPathWithRoundedCorners(2.0f);
    g.fillPath(outline);
    
    g.drawRect(boundsReduced.reduced(corner, 0).withTrimmedBottom(boundsReduced.getHeight() * 2.0f/3.0f));
    
    float lineWidth = bounds.getWidth() * 1.0f / 8.0f;
    float lineX1 = boundsReduced.getTopLeft().getX() + lineWidth;
    float lineX2 = boundsReduced.getTopRight().getX() - lineWidth;
    float height = boundsReduced.getHeight();
    
    g.drawHorizontalLine(boundsReduced.getTopLeft().getY() + height * 0.6f, lineX1, lineX2);
    g.drawHorizontalLine(boundsReduced.getTopLeft().getY() + height * 0.7f, lineX1, lineX2);
    g.drawHorizontalLine(boundsReduced.getTopLeft().getY() + height * 0.8f, lineX1, lineX2);
}

static void drawLoadIcon(Graphics& g, Rectangle<int> bounds, Colour backgroundFill=Colour(),
                         Colour outLineColor=Colours::whitesmoke)
{
    g.setColour(backgroundFill);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
    
    g.setColour(outLineColor);
    g.drawRoundedRectangle(bounds.toFloat(), 3, 1);
    
    g.setColour(Colours::whitesmoke);

    int boundsSize = bounds.getWidth();
    float imgSize = boundsSize * 7.0f / 8.0f;
    float thickness = imgSize / 32.0f;
    float tabHeight = imgSize / 9.0f;
    float tabWidth = imgSize / 3.0f;
    Rectangle<float> boundsReduced = bounds.reduced(bounds.getWidth() - imgSize).toFloat();
    boundsReduced.reduce(0, imgSize/20);
    
    Path outline = Path();
    outline.addLineSegment(Line<float>(boundsReduced.getTopLeft(), boundsReduced.getBottomLeft()), thickness);
    outline.addLineSegment(Line<float>(boundsReduced.getBottomLeft(), boundsReduced.getBottomRight()), thickness);
    
    Point<float> tabCornerBottomRight = boundsReduced.getTopRight().translated(0, tabHeight);
    Point<float> tabCornerBottomLeft = boundsReduced.getTopLeft().translated(tabWidth*1.15, tabHeight);
    Point<float> tabCornerTop = boundsReduced.getTopLeft().translated(tabWidth, 0);

    outline.addLineSegment(Line<float>(boundsReduced.getBottomRight(), tabCornerBottomRight), thickness);
    outline.addLineSegment(Line<float>(tabCornerBottomRight, tabCornerBottomLeft), thickness);
    outline.addLineSegment(Line<float>(tabCornerBottomLeft, tabCornerTop), thickness);
    outline.addLineSegment(Line<float>(tabCornerTop, boundsReduced.getTopLeft()), thickness);
    
    outline = outline.createPathWithRoundedCorners(2.0f);
    g.fillPath(outline);
}

static void drawSettingsIcon(Graphics& g, Rectangle<int> bounds, Colour backgroundFill=Colour(),
                             Colour outLineColor=Colours::whitesmoke)
{
    g.setColour(backgroundFill);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
    
    g.setColour(outLineColor);
    g.drawRoundedRectangle(bounds.toFloat(), 3, 1);
    
    g.setColour(Colours::whitesmoke);

    int boundsSize = bounds.getWidth();
    float imgSize = boundsSize * 7.0f / 8.0f;
    float thickness = imgSize / 36.0f;
    
    Rectangle<float> boundsReduced = bounds.reduced(bounds.getWidth() - imgSize).toFloat();
    
    Path gear = Path();
    
    int n = 7;
    float radiusBody = imgSize * 3.0f / 11.0f;
    float radiusSpoke = radiusBody * 12.0f/8.0f;
    
    Point<float> bodyRight;
    Point<float> bodyLeft;
    Point<float> spokeRight;
    Point<float> spokeLeft;
    Point<float> nextSpoke;
    Point<float> center = bounds.getCentre().toFloat();
    
    float r = 0;
    float rStep = 6.28318530718f / n;
    float spokeDif = rStep * 4.0f/19.0f;
    float bodDif = spokeDif * 5.0f / 3.0f;
    float rStepNext;
    
    for (int i = 0; i < n; i++)
    {
        r = rStep * i;
        
        bodyRight = Point<float>(cosf(r-bodDif) * radiusBody + center.getX(), sinf(r-bodDif) * radiusBody + center.getY());
        spokeRight = Point<float>(cosf(r-spokeDif) * radiusSpoke + center.getX(), sinf(r-spokeDif) * radiusSpoke + center.getY());

        bodyLeft = Point<float>(cosf(r+bodDif) * radiusBody + center.getX(), sinf(r+bodDif) * radiusBody + center.getY());
        spokeLeft = Point<float>(cosf(r+spokeDif) * radiusSpoke + center.getX(), sinf(r+spokeDif) * radiusSpoke + center.getY());
        
        rStepNext = rStep * (i + 1);
        nextSpoke = Point<float>(cosf(rStepNext-bodDif) * radiusBody + center.getX(), sinf(rStepNext-bodDif) * radiusBody + center.getY());
        
        gear.addLineSegment(Line<float>(bodyRight, spokeRight), thickness);
        gear.addLineSegment(Line<float>(spokeRight, spokeLeft), thickness);
        gear.addLineSegment(Line<float>(spokeLeft, bodyLeft), thickness);
        gear.addLineSegment(Line<float>(bodyLeft, nextSpoke), thickness);
    }
    
    gear = gear.createPathWithRoundedCorners(thickness);
    g.fillPath(gear);
    float cR = radiusBody / 2.0f;
    
    g.drawEllipse(center.getX() - cR, center.getY() - cR, cR * 2, cR * 2, thickness);
}
