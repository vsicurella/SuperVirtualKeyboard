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

#define defaultSectionFlexBox FlexBox(FlexBox::Direction::column, FlexBox::Wrap::wrap, FlexBox::AlignContent::stretch, FlexBox::AlignItems::flexStart, FlexBox::JustifyContent::spaceAround)
#define defaultSectionFlexItem FlexItem(250, 24).withFlex(1.0f).withMargin(FlexItem::Margin(5, 0, 5, 5))
#define defaultSectionAsFlexItem FlexItem(100, 100).withFlex(1.0f);

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
        DirectoryControl,
        GenericControl
    };

    struct SvkControlProperties
    {
        int controlType;
        String controlName;
        bool controlLabelled;
        int sectionNum;
        var defaultValue;
        var minValue;
        var maxValue;
        var increment;

        SvkControlProperties(
            int typeIn = 1, String nameIn = "", bool labelled = false, int sectionNumIn = 0,
            var defaultIn = var(), var minIn = 0, var maxIn = 1, var incrementIn = 1
        ) : 
            controlType(typeIn), controlName(nameIn), controlLabelled(labelled), sectionNum(sectionNumIn),
            defaultValue(defaultIn), minValue(minIn), maxValue(maxIn), increment(incrementIn)
        {};
    };

public:
    
    SvkSettingsPanel(
        String                       panelName,
        SvkPluginState*              pluginStateIn,
        StringArray                  sectionNamesIn,
        Array<Identifier>            controlIdsIn,
        Array<SvkControlProperties>  controlTypesIn,
        FlexBox                      flexParentStyle = FlexBox(),
        Array<FlexBox>               sectionBoxStyle = Array<FlexBox>(),
        Array<FlexItem>              sectionItemsStyle = Array<FlexItem>()
    ) :
        pluginState(pluginStateIn),
        sectionNames(sectionNamesIn),
        controlIdentifiers(controlIdsIn),
        controlTypes(controlTypesIn),
        flexParent(flexParentStyle)
    {
        setName(panelName);
        numSections = sectionNames.size();

        flexParent.items.resize(numSections);
        FlexItem sectionDefault = defaultSectionAsFlexItem;
        flexParent.items.fill(sectionDefault);

        // Combine given and default FlexBox styles
        flexSections.addArray(sectionBoxStyle);
        while (flexSections.size() < numSections)
            flexSections.add(defaultSectionFlexBox);

        // Combine given and default FlexItem styles
        sectionsDefaultFlexItems.addArray(sectionItemsStyle);
        while (sectionsDefaultFlexItems.size() < numSections)
            sectionsDefaultFlexItems.add(defaultSectionFlexItem);

        setupSections();
        setupControls();
    }

    virtual ~SvkSettingsPanel()
    {

    }

    virtual void paint(Graphics& g) override
    {
        //g.fillAll(Colours::darkgrey);
        //g.setColour(Colours::blueviolet);

        //float x = 0;
        //for (int i = 0; i < flexParent.items.size(); i++)
        //{
        //    FlexItem& item = flexParent.items.getReference(i);
        //    Rectangle<float> rect = { x, 0, item.width, item.height };
        //    x += item.width;

        //    g.fillRoundedRectangle(rect.reduced(3, 3), 3.0f);
        //}

        g.setColour(Colour(0xff323e44).darker());

        for (auto label : sectionHeaderLabels)
        {
            g.fillRoundedRectangle(label->getBounds().reduced(2.0f, 0).toFloat(), 5.0f);
        }
    }

    void resized() override
    {
        flexParent.performLayout(getLocalBounds());
    }

    virtual void setKeyboardPointer(VirtualKeyboard::Keyboard* keyboardPointer)
    {
        virtualKeyboard = keyboardPointer;
    }

    virtual void refreshPanel() {};

    //=============================================================================================================

    virtual void sliderValueChanged(Slider*) override {};

    virtual void buttonClicked(Button*) override {};

    virtual void comboBoxChanged(ComboBox*) override {};

    virtual void directoryChanged(DirectoryBrowserComponent*, File) override {};

protected:

    FlexBox* getSectionFlexBox(int sectionNum)
    {
        return flexParent.items.getReference(jmin(sectionNum, numSections - 1)).associatedFlexBox;
    }

    FlexItem* getControlFlexItem(int controlNum)
    {
        FlexBox* sectionBox = getSectionFlexBox(controlTypes[controlNum].sectionNum);
        for (int i = 0; i < sectionBox->items.size(); i++)
        {
            FlexItem* item = &sectionBox->items.getReference(i);
            if (item->associatedComponent == controls[controlNum])
                return item;
        }

        return nullptr;
    }

    bool setControlFlexItem(int controlNum, FlexItem flexItemIn)
    {
        FlexBox* sectionBox = getSectionFlexBox(controlTypes[controlNum].sectionNum);
        for (int i = 0; i < sectionBox->items.size(); i++)
        {
            if (sectionBox->items.getReference(i).associatedComponent == controls[controlNum])
            {
                sectionBox->items.set(i, flexItemIn);
                return true;
            }
        }
        return false;
    }

private:

    void setupSections()
    {
        // Associate section flexboxes with parent FlexItems and add section headers
        for (int i = 0; i < numSections; i++)
        {
            FlexBox& section = flexSections.getReference(i);
            flexParent.items.getReference(i).associatedFlexBox = &section;

            String header = sectionNames[i];
            if (header.length() > 0)
            {
                Label* l = sectionHeaderLabels.add(new Label(getName() + "_Header" + String(i), header));

                addAndMakeVisible(l);
                section.items.add(FlexItem(225, 24, *l));
            }
        }
    }

    void setupControls()
    {
        int numControls = jmin(controlIdentifiers.size(), controlTypes.size());

        for (int i = 0; i < numControls; i++)
        {
            Identifier id = controlIdentifiers[i];
            SvkControlProperties properties = controlTypes[i];

            Slider* slider = nullptr;
            Component* control = nullptr;

            switch (properties.controlType)
            {
            case ControlTypeNames::SliderControl:
                slider = new Slider(properties.controlName);
                slider->setRange(properties.minValue, properties.maxValue, properties.increment);
                slider->setValue(properties.defaultValue, dontSendNotification);
                control = slider;
                break;

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
                control = new Component(properties.controlName);
            }

            if (control)
            {
                if (properties.controlLabelled)
                {
                    control = new LabelledComponent(*control, properties.controlName + ":");
                }

                controls.add(control);

                int sectionNum = jmin(properties.sectionNum, numSections - 1);
                FlexBox& section = flexSections.getReference(sectionNum);
                FlexItem item = sectionsDefaultFlexItems[sectionNum];
                item.associatedComponent = control;

                section.items.add(item);
                addAndMakeVisible(control);
                idToControl.set(id, control);
            }
        }
    }

protected:
    
    SvkPluginState* pluginState;

    StringArray sectionNames;
    int numSections;

    Array<Identifier> controlIdentifiers;
    Array<SvkControlProperties> controlTypes;

    OwnedArray<Label> sectionHeaderLabels;
    Array<FlexBox> flexSections;
    FlexBox flexParent;

    Array<FlexItem> sectionsDefaultFlexItems;

    OwnedArray<Component> controls;
    HashMap<Component*, Label*> controlLabels;
    HashMap<Identifier, Component*, IDasStringHash> idToControl;

    int controlMinWidth = 250;
    int controlMinHeight = 24;

    VirtualKeyboard::Keyboard* virtualKeyboard = nullptr;
};
