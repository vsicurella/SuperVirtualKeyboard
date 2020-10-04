/*
  ==============================================================================

    ColourLibraryComponent.h
    Created: 18 Sep 2020 9:56:16pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FocussedComponent.h"

//==============================================================================
/*
*/


class PaintSwatch : public Component, public FocusableComponent
{
public:

    PaintSwatch(Array<Colour>& parentPaletteIn, int indexIn, String swatchLabelIn = "", bool showLabelIn = false, std::function<void(var)> callback = {})
        : parentPalette(parentPaletteIn), index(indexIn), swatchLabel(swatchLabelIn), showLabel(showLabelIn), FocusableComponent(nullptr, callback)
    {
        setName("Swatch_" + swatchLabelIn);
    };

    ~PaintSwatch() {};

    void paint(Graphics& g) override
    {
        g.fillAll(getCurrentColour());

        if (getCurrentColour().isTransparent())
        {
            g.setColour(Colours::darkgrey.darker());
            g.drawRect(getLocalBounds());
        }

        g.setColour(getCurrentColour().contrasting());

        if (showLabel)
        {
            g.drawFittedText(swatchLabel, getLocalBounds(), Justification::centred, 1, 0.1f);
        }

        if (isFocussedOn())
        {
            g.drawRect(getLocalBounds(), 2.0f);
        }
    }

    void setNewFocusser(FocussedComponent* focusserIn) override
    {
        FocusableComponent::setNewFocusser(focusserIn);
        repaint();
    }

    void setLabelText(String labelTextIn)
    {
        swatchLabel = labelTextIn;
    }

    void performFocusFunction(var dataIn) override
    {
        parentPalette.set(index, Colour::fromString(dataIn.toString()));
        repaint();
    }

    Colour getCurrentColour() const
    {
        return parentPalette[index];
    }

private:

    Array<Colour>& parentPalette;
    const int index;

    String swatchLabel;

    bool showLabel;
};

//==============================================================================
/*
*/

class ColourLibraryComponent : public juce::Component
{

public:

    enum ColourIds
    {
        backgroundColourId        = 0x00010100,
        rowBackgroundColourId    = 0x00010101,
        outLineColourId            = 0x00010200,
        rowOutlineColourId        = 0x00010201
    };

public:
    ColourLibraryComponent(StringArray rowNamesIn, Array<Array<Colour>*> arraysToControl = {}, bool showLabelsIn = false, bool onlyOpaqueIn = false);
    ~ColourLibraryComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void showSwatchLabels(bool toShowLabels);

    void onlyShowOpaque(bool onlyOpaque);

    void setNewRows(Array<Array<Colour>*> arraysToControl);

    void refreshSwatches();

    void forceSwatchColumns(int maxNumColumns);

private:

    void setupFlex();

private:

    Array<Array<Colour>*> rowColours;

    FlexBox flexBox;
    Array<FlexBox> rowBoxes;

    OwnedArray<Label> rowLabels;
    OwnedArray<PaintSwatch> swatches;

    bool showLabels;
    bool onlyOpaque;

    int maxSwatchColumns = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColourLibraryComponent)
};
