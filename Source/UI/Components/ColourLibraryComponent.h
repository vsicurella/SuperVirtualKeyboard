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

	PaintSwatch(Colour initialColourIn, String swatchLabelIn = "", bool showLabelIn = false, std::function<void(var)> callback = {})
		: currentColour(initialColourIn), swatchLabel(swatchLabelIn), showLabel(showLabelIn), FocusableComponent(nullptr, callback)
	{
		setName("Swatch_" + swatchLabelIn);
	};

	~PaintSwatch() {};

	void paint(Graphics& g) override
	{
		g.fillAll(currentColour);

		g.setColour(currentColour.contrasting());

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
		currentColour = Colour::fromString(dataIn.toString());
		repaint();
	}

	Colour getCurrentColour() const
	{
		return currentColour;
	}

private:

	Colour currentColour;
	String swatchLabel;

	bool showLabel;
};

//==============================================================================
/*
*/

class ColourLibraryComponent : public juce::Component
{
public:
	ColourLibraryComponent(StringArray rowNamesIn, Array<Array<Colour>*> arraysToControl = {}, bool showLabelsIn = false, bool onlyOpaqueIn = false);
	~ColourLibraryComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void showSwatchLabels(bool toShowLabels);

	void onlyShowOpaque(bool onlyOpaque);

	void setNewRows(Array<Array<Colour>*> arraysToControl);

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

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColourLibraryComponent)
};
