/*
  ==============================================================================

    SvkSettingsPanel.h
    Created: 5 Sep 2020 4:08:42pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "../../../PluginState.h"
#include "../DirectoryBrowserComponent.h"

class SvkSettingsPanel : public Component,
	private Slider::Listener,
	private Button::Listener,
	private ComboBox::Listener,
	private TextEditor::Listener
{

public:

	enum ControlTypeNames
	{
		SliderControl = 0,
		ToggleControl,
		MenuControl,
		DirectoryControl
	};

	struct SvkControlProperties
	{
		int controlType;
		String controlName;
		var defaultValue;
		var minValue;
		var maxValue;
		var increment;

		SvkControlProperties(int typeIn = 1, String nameIn = "", var defaultIn = var(), var minIn = 0, var maxIn = 1, var incrementIn = 1)
			: controlType(typeIn), controlName(nameIn), defaultValue(defaultIn), minValue(minIn), maxValue(maxIn), increment(incrementIn)
		{};
	};

public:

	SvkSettingsPanel(SvkPluginState* pluginStateIn, Array<Identifier> controlIdsIn, Array<SvkControlProperties> controlTypesIn)
		: pluginState(pluginStateIn), controlIdentifiers(controlIdsIn), controlTypes(controlTypesIn)
	{
		flexBox = FlexBox(
			FlexBox::Direction::row,
			FlexBox::Wrap::wrap,
			FlexBox::AlignContent::flexStart,
			FlexBox::AlignItems::stretch,
			FlexBox::JustifyContent::flexStart);

		setupControls();
	}

	virtual ~SvkSettingsPanel()
	{

	}

	void resized() override
	{
		flexBox.performLayout(getLocalBounds());
	}

	//=============================================================================================================

	virtual void sliderValueChanged(Slider* sliderThatChanged) override {};

	virtual void buttonClicked(Button* buttonThatWasClicked) override {};

	virtual void comboBoxChanged(ComboBox* comboBoxThatChanged) override {};

private:

	void setupControls()
	{
		int numControls = jmin(controlIdentifiers.size(), controlTypes.size());

		for (int i = 0; i < numControls; i++)
		{
			Identifier id = controlIdentifiers[i];
			SvkControlProperties properties = controlTypes[i];

			Component* control;
			Label* controlLabel;

			switch (properties.controlType)
			{
			case ControlTypeNames::ToggleControl:
				control = new ToggleButton(properties.controlName);
				break;
			
			case ControlTypeNames::MenuControl:
				control = new ComboBox(properties.controlName);
				controlLabel = (Label*)controls.add(new Label(properties.controlName));
				controlLabels.set(control, controlLabel);
				break;
			
			case ControlTypeNames::DirectoryControl:
				control = new DirectoryBrowserComponent(properties.controlName);
				controlLabel = (Label*)controls.add(new Label(properties.controlName));
				controlLabels.set(control, controlLabel);
				break;

			default:
				control = new Slider(properties.controlName);
				controlLabel = (Label*)controls.add(new Label(properties.controlName));
				controlLabels.set(control, controlLabel);
			}

			if (controlLabel)
			{
				controlLabel->setText(controlLabel->getName() + ":", dontSendNotification);
				controlLabel->setName(controlLabel->getName() + "_Label");
				flexBox.items.add(FlexItem(*controlLabel)
					.withMinWidth(controlMinWidth)
					.withMinHeight(controlMinHeight)
					.withMargin(controlMargin)
					.withFlex(controlFlex));
				addAndMakeVisible(controlLabel);
			}

			if (control)
			{
				controls.add(control);
				flexBox.items.add(FlexItem(*control)
					.withMinWidth(controlMinWidth)
					.withMinHeight(controlMinHeight)
					.withMargin(controlMargin)
					.withFlex(controlFlex));
				addAndMakeVisible(control);
				idToControl.set(id, control);
			}
		}
	}

protected:

	SvkPluginState* pluginState;

	Array<Identifier> controlIdentifiers;
	Array<SvkControlProperties> controlTypes;

	FlexBox flexBox;

	OwnedArray<Component> controls;
	HashMap<Component*, Label*> controlLabels;
	HashMap<Identifier, Component*, IDasStringHash> idToControl;

	int controlMinWidth = 250;
	int controlMinHeight = 50;
	FlexItem::Margin controlMargin = FlexItem::Margin(10, 5, 10, 10);
	float controlFlex = 1.0f;
};