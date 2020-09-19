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
		//label->setColour(Label::ColourIds::outlineColourId, Colours::green);
		rowLabels.add(label);
		addAndMakeVisible(label);
	}

	if (rowColoursIn.size() > 0)
		setupFlex();
}

ColourLibraryComponent::~ColourLibraryComponent()
{
}

void ColourLibraryComponent::paint (juce::Graphics& g)
{
	//g.setColour(Colours::white);
	//g.drawRect(getLocalBounds(), 2.0f);
}

void ColourLibraryComponent::resized()
{	
	for (int i = 0; i < flexBox.items.size(); i++)
	{
		flexBox.items.getReference(i).width = getWidth();
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
	setupFlex();
}

Label* ColourLibraryComponent::getNewOutliner()
{
	Random r;
	Label* c = outlines.add(new Label());
	c->setColour(Label::outlineColourId, Colour(r.nextFloat(), r.nextFloat(), r.nextFloat() / 3 + (2 / 3.0f), 1.0f));
	addAndMakeVisible(c);
	return c;
}

void ColourLibraryComponent::setupFlex()
{
	flexBox = FlexBox(
		FlexBox::Direction::column,
		FlexBox::Wrap::noWrap,
		FlexBox::AlignContent::spaceAround,
		FlexBox::AlignItems::stretch,
		FlexBox::JustifyContent::spaceAround
	);

	rowBoxes.clear();
	swatches.clear();
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
		//boxAsItem.associatedComponent = getNewOutliner();
		boxAsItem.alignSelf = FlexItem::AlignSelf::stretch;
		boxAsItem.margin = FlexItem::Margin(2, 0, 2, 8);
		flexBox.items.add(boxAsItem);

		Array<Colour>* colorRow = rowColours.getReference(r);

		for (int i = 0; i < colorRow->size(); i++)
		{
			Colour& c = colorRow->getReference(i);

			if (onlyOpaque && !c.isOpaque())
				continue;

			PaintSwatch* swatch = new PaintSwatch(c, String(i + 1), showLabels, 
				[&] (var dataIn) { c = Colour::fromString(dataIn.toString()); }
			);
			addAndMakeVisible(swatch);

			swatches.add(swatch);

			FlexItem swatchItem(20, 20, *swatch);
			swatchItem.margin = FlexItem::Margin(2);
			rowBoxes.getReference(r).items.add(swatchItem);
		}
	}
}