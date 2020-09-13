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
#include "../LabelledComponent.h"
#include "../VirtualKeyboard/KeyboardComponent.h"

class SvkSettingsPanel : public Component,
	protected Slider::Listener,
	protected Button::Listener,
	protected ComboBox::Listener,
	protected DirectoryBrowserComponent::Listener
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
		bool controlLabelled;
		var defaultValue;
		var minValue;
		var maxValue;
		var increment;

		SvkControlProperties(
			int typeIn = 1, String nameIn = "", bool labelled = false,
			var defaultIn = var(), var minIn = 0, var maxIn = 1, var incrementIn = 1
		) : 
			controlType(typeIn), controlName(nameIn), controlLabelled(labelled), 
			defaultValue(defaultIn), minValue(minIn), maxValue(maxIn), increment(incrementIn)
		{};
	};

public:

	SvkSettingsPanel(SvkPluginState* pluginStateIn, Array<Identifier> controlIdsIn, Array<SvkControlProperties> controlTypesIn)
		: pluginState(pluginStateIn), controlIdentifiers(controlIdsIn), controlTypes(controlTypesIn)
	{
		flexBox = FlexBox(
			FlexBox::Direction::column,
			FlexBox::Wrap::wrap,
			FlexBox::AlignContent::flexStart,
			FlexBox::AlignItems::flexStart,
			FlexBox::JustifyContent::center);

		setupControls();
	}

	virtual ~SvkSettingsPanel()
	{

	}

	void resized() override
	{
		flexBox.performLayout(getLocalBounds());
	}

	virtual void setKeyboardPointer(VirtualKeyboard::Keyboard* keyboardPointer)
	{
		virtualKeyboard = keyboardPointer;
	}

	//=============================================================================================================

	virtual void sliderValueChanged(Slider*) override {};

	virtual void buttonClicked(Button*) override {};

	virtual void comboBoxChanged(ComboBox*) override {};

	virtual void directoryChanged(DirectoryBrowserComponent*, File) override {};

private:

	void setupControls()
	{
		int numControls = jmin(controlIdentifiers.size(), controlTypes.size());

		for (int i = 0; i < numControls; i++)
		{
			Identifier id = controlIdentifiers[i];
			SvkControlProperties properties = controlTypes[i];

			Component* control = nullptr;


			switch (properties.controlType)
			{
			case ControlTypeNames::ToggleControl:
				control = new ToggleButton(properties.controlName);
				break;
			
			case ControlTypeNames::MenuControl:
				control = new ComboBox(properties.controlName);
				break;
			
			case ControlTypeNames::DirectoryControl:
				control = new DirectoryBrowserComponent(properties.controlName);
				break;

			default:
				control = new Slider(properties.controlName);
			}

			if (control)
			{
				if (properties.controlLabelled)
				{
					control = new LabelledComponent(*control, properties.controlName + ":");
				}

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
	int controlMinHeight = 24;
	FlexItem::Margin controlMargin = FlexItem::Margin(10, 5, 10, 10);
	float controlFlex = 1.0f;

	VirtualKeyboard::Keyboard* virtualKeyboard = nullptr;
};
