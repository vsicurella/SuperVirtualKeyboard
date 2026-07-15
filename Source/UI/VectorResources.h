/*
  ==============================================================================

    VectorResources.h
    Created: 10 Nov 2019 10:50:29pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

static void drawSaveIcon(Graphics& g, Rectangle<int> bounds, Colour backgroundFill=Colour(),
                         Colour outLineColor=Colours::lightgrey)
{
    g.setColour(backgroundFill);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
    
    g.setColour(outLineColor);
    g.drawRoundedRectangle(bounds.toFloat(), 3, 1);
    
    g.setColour(Colours::whitesmoke);

    // Float geometry (no int truncation) + a floored, proportional stroke so the
    // icon stays crisp at small sizes and fractional DPI scaling.
    int size = jmin(bounds.getWidth(), bounds.getHeight());
    float imgSize = size * 7.0f / 8.0f;
    float thickness = jmax(1.0f, imgSize / 14.0f);
    Rectangle<float> boundsReduced = bounds.toFloat().reduced(size / 8.0f);

    Path outline = Path();

    outline.addLineSegment(Line<float>(boundsReduced.getTopLeft(), boundsReduced.getBottomLeft()), thickness);
    outline.addLineSegment(Line<float>(boundsReduced.getBottomLeft(), boundsReduced.getBottomRight()), thickness);

    float corner = imgSize / 6.0f;
    Point<float> cornerBottom = boundsReduced.getTopRight().withY(boundsReduced.getTopRight().getY() + corner);
    Point<float> cornerLeft = boundsReduced.getTopRight().withX(boundsReduced.getTopRight().getX() - corner);

    outline.addLineSegment(Line<float>(boundsReduced.getBottomRight(), cornerBottom), thickness);
    outline.addLineSegment(Line<float>(cornerBottom, cornerLeft), thickness);
    outline.addLineSegment(Line<float>(cornerLeft, boundsReduced.getTopLeft()), thickness);

    outline = outline.createPathWithRoundedCorners(2.0f);
    g.fillPath(outline);

    // Shutter (outline scales with the stroke instead of a fixed 1px line).
    g.drawRect(boundsReduced.reduced(corner, 0).withTrimmedBottom(boundsReduced.getHeight() * 2.0f/3.0f), thickness);

    // Label lines drawn as thickness-aware filled rects (crisp at any DPI).
    float lineWidth = size / 8.0f;
    float lineX1 = boundsReduced.getX() + lineWidth;
    float lineX2 = boundsReduced.getRight() - lineWidth;
    float height = boundsReduced.getHeight();

    for (float p : { 0.6f, 0.72f, 0.84f })
    {
        float y = boundsReduced.getY() + height * p;
        g.fillRect(Rectangle<float>(lineX1, y - thickness / 2.0f, lineX2 - lineX1, thickness));
    }
}

static void drawLoadIcon(Graphics& g, Rectangle<int> bounds, Colour backgroundFill=Colour(),
                         Colour outLineColor=Colours::whitesmoke)
{
    g.setColour(backgroundFill);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
    
    g.setColour(outLineColor);
    g.drawRoundedRectangle(bounds.toFloat(), 3, 1);
    
    g.setColour(Colours::whitesmoke);

    // Float geometry + a floored, proportional stroke for DPI/size robustness.
    int size = jmin(bounds.getWidth(), bounds.getHeight());
    float imgSize = size * 7.0f / 8.0f;
    float thickness = jmax(1.0f, imgSize / 14.0f);
    float tabHeight = imgSize / 9.0f;
    float tabWidth = imgSize / 3.0f;
    Rectangle<float> boundsReduced = bounds.toFloat().reduced(size / 8.0f);
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

static void drawMenuIcon(Graphics& g, Rectangle<int> bounds, Colour backgroundFill=Colour(),
                         Colour outLineColor=Colours::whitesmoke)
{
    g.setColour(backgroundFill);
    g.fillRoundedRectangle(bounds.toFloat(), 3);

    g.setColour(outLineColor);
    g.drawRoundedRectangle(bounds.toFloat(), 3, 1);

    g.setColour(Colours::whitesmoke);

    int boundsSize = bounds.getWidth();
    float imgSize = boundsSize * 7.0f / 8.0f;
    float thickness = jmax(1.0f, imgSize / 32.0f);
    Rectangle<float> boundsReduced = bounds.reduced(bounds.getWidth() - imgSize).toFloat();

    float x1 = boundsReduced.getX();
    float x2 = boundsReduced.getRight();

    // three evenly spaced horizontal lines
    for (int i = 1; i <= 3; ++i)
    {
        float y = boundsReduced.getY() + boundsReduced.getHeight() * i / 4.0f;
        g.fillRect(Rectangle<float>(x1, y - thickness / 2.0f, x2 - x1, thickness));
    }
}

static void drawExportIcon(Graphics& g, Rectangle<int> bounds, Colour backgroundFill=Colour(),
                           Colour outLineColor=Colours::whitesmoke)
{
    g.setColour(backgroundFill);
    g.fillRoundedRectangle(bounds.toFloat(), 3);

    g.setColour(outLineColor);
    g.drawRoundedRectangle(bounds.toFloat(), 3, 1);

    g.setColour(Colours::whitesmoke);

    // Float geometry + a floored, proportional stroke for DPI/size robustness.
    int size = jmin(bounds.getWidth(), bounds.getHeight());
    float imgSize = size * 7.0f / 8.0f;
    float thickness = jmax(1.0f, imgSize / 14.0f);
    Rectangle<float> r = bounds.toFloat().reduced(size / 8.0f);

    // A square on the left, with a gap in its right edge where the arrow exits.
    float boxSize = r.getHeight() * 0.62f;
    Rectangle<float> box(r.getX(), r.getCentreY() - boxSize / 2.0f, boxSize, boxSize);
    float gapHalf = boxSize * 0.20f;

    Path boxPath;
    boxPath.addLineSegment(Line<float>(box.getTopLeft(), box.getBottomLeft()), thickness);
    boxPath.addLineSegment(Line<float>(box.getTopLeft(), box.getTopRight()), thickness);
    boxPath.addLineSegment(Line<float>(box.getBottomLeft(), box.getBottomRight()), thickness);
    boxPath.addLineSegment(Line<float>(box.getTopRight(), Point<float>(box.getRight(), box.getCentreY() - gapHalf)), thickness);
    boxPath.addLineSegment(Line<float>(Point<float>(box.getRight(), box.getCentreY() + gapHalf), box.getBottomRight()), thickness);
    g.fillPath(boxPath);

    // An arrow that starts inside the box and extends out past its right edge.
    float arrowY = r.getCentreY();
    float shaftStart = box.getX() + boxSize * 0.4f;
    float shaftEnd = r.getRight();
    float head = imgSize * 0.2f;

    Path arrow;
    arrow.addLineSegment(Line<float>(Point<float>(shaftStart, arrowY), Point<float>(shaftEnd, arrowY)), thickness);
    arrow.addLineSegment(Line<float>(Point<float>(shaftEnd, arrowY), Point<float>(shaftEnd - head, arrowY - head)), thickness);
    arrow.addLineSegment(Line<float>(Point<float>(shaftEnd, arrowY), Point<float>(shaftEnd - head, arrowY + head)), thickness);
    arrow = arrow.createPathWithRoundedCorners(thickness);
    g.fillPath(arrow);
}
