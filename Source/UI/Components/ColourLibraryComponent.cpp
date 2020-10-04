/*
  ==============================================================================

    ColourLibraryComponent.cpp
    Created: 18 Sep 2020 9:56:16pm
    Author:  Vincenzo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ColourLibraryComponent.h"

//==============================================================================
ColourLibraryComponent::ColourLibraryComponent(StringArray rowNamesIn, Array<Array<Colour>*> rowColoursIn, bool showLabelsIn, bool onlyOpaqueIn)
    : rowColours(rowColoursIn), showLabels(showLabelsIn), onlyOpaque(onlyOpaqueIn)
{
    for (auto rowName : rowNamesIn)
    {
        Label* label = new Label(rowName + "_Label", rowName);
        rowLabels.add(label);
        addAndMakeVisible(label);
    }

    if (rowColoursIn.size() > 0)
        setupFlex();

    setColour(ColourLibraryComponent::ColourIds::backgroundColourId, Colour());
    setColour(ColourLibraryComponent::ColourIds::outLineColourId, Colour());
    setColour(ColourLibraryComponent::ColourIds::rowBackgroundColourId, Colour());
    setColour(ColourLibraryComponent::ColourIds::rowOutlineColourId, Colour());
}

ColourLibraryComponent::~ColourLibraryComponent()
{
}

void ColourLibraryComponent::paint (juce::Graphics& g)
{
    g.fillAll(findColour(ColourLibraryComponent::ColourIds::backgroundColourId));

    g.setColour(findColour(ColourLibraryComponent::ColourIds::outLineColourId));
    g.drawRect(getLocalBounds());

    for (int i = 0; i < flexBox.items.size() / 2; i++)
    {
        Component* label = flexBox.items.getReference(i * 2).associatedComponent;
        FlexItem& item = flexBox.items.getReference(i * 2 + 1);
        Rectangle<int> rowRect = { label->getX(), label->getY(), jmax(label->getWidth(), (int)item.width), label->getHeight() + (int)item.height };

        g.setColour(findColour(ColourLibraryComponent::ColourIds::rowBackgroundColourId));
        g.fillRect(rowRect);

        g.setColour(findColour(ColourLibraryComponent::ColourIds::rowOutlineColourId));
        g.drawRect(rowRect);
    }
}

void ColourLibraryComponent::resized()
{    
    for (int i = 0; i < flexBox.items.size(); i++)
    {
        int maxWidth = getWidth();

        if (maxSwatchColumns > 0)
            maxWidth = 25 * maxSwatchColumns + 8; // default width plus margins

        flexBox.items.getReference(i).width = maxWidth;
        flexBox.items.getReference(i).height = 24;
    }

    flexBox.performLayout(getLocalBounds());
}

void ColourLibraryComponent::showSwatchLabels(bool showLabelsIn)
{
    showLabels = showLabelsIn;
}

void ColourLibraryComponent::onlyShowOpaque(bool onlyOpaqueIn)
{
    if ((int)onlyOpaque + (int)onlyOpaqueIn == 1)
    {
        onlyOpaque = onlyOpaqueIn;
        setupFlex();
        return;
    }
    
    onlyOpaque = onlyOpaqueIn;
}

void ColourLibraryComponent::setNewRows(Array<Array<Colour>*> arraysToControl)
{
    rowColours = arraysToControl;
    refreshSwatches();
}

void ColourLibraryComponent::refreshSwatches()
{
    swatches.clear();
    for (int r = 0; r < rowBoxes.size(); r++)
    {
        FlexBox& rowBox = rowBoxes.getReference(r);
        Array<Colour>* colorRow = rowColours.getReference(r);

        rowBox.items.clear();

        for (int i = 0; i < colorRow->size(); i++)
        {
            Colour& c = colorRow->getReference(i);

            if (onlyOpaque && c.getAlpha() < 0xff)
                continue;

            PaintSwatch* swatch = new PaintSwatch(*colorRow, i, String(i + 1), showLabels);
            addAndMakeVisible(swatch);
            swatches.add(swatch);

            FlexItem swatchItem(20, 20, *swatch);
            swatchItem.margin = FlexItem::Margin(2);
            rowBox.items.add(swatchItem);
        }
    }

    resized();
}

void ColourLibraryComponent::forceSwatchColumns(int maxNumColumns)
{
    maxSwatchColumns = maxNumColumns;
}

void ColourLibraryComponent::setupFlex()
{
    flexBox = FlexBox(
        FlexBox::Direction::column,
        FlexBox::Wrap::noWrap,
        FlexBox::AlignContent::flexStart,
        FlexBox::AlignItems::stretch,
        FlexBox::JustifyContent::flexStart
    );

    rowBoxes.clear();
    for (int r = 0; r < rowLabels.size(); r++)
    {
        rowBoxes.add(FlexBox(
            FlexBox::Direction::row,
            FlexBox::Wrap::wrap,
            FlexBox::AlignContent::spaceAround,
            FlexBox::AlignItems::stretch,
            FlexBox::JustifyContent::flexStart
        ));

        Label* row = rowLabels[r];

        FlexItem labelAsItem;
        labelAsItem.associatedComponent = row;
        labelAsItem.minHeight = 24;
        labelAsItem.minWidth = Font().getStringWidth(row->getText());
        flexBox.items.add(labelAsItem);

        FlexItem boxAsItem;
        boxAsItem.associatedFlexBox = &rowBoxes.getReference(r);
        boxAsItem.alignSelf = FlexItem::AlignSelf::stretch;
        boxAsItem.margin = FlexItem::Margin(2, 0, 2, 8);
        flexBox.items.add(boxAsItem);
    }

    refreshSwatches();
}