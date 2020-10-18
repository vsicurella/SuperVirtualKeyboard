/*
  ==============================================================================

    LabelledComponent.h
    Created: 6 Sep 2020 4:43:16pm
    Author:  Vincenzo

    A container for a component that allows a offsetLabel to be attached a certain way.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LabelledComponent : public Component
{

public:

    enum LabelPosition
    {
        LeftOfComponent = 0,
        TopOfComponent,
        RightOfComponent,
        BottomOfComponent
    };

    enum LabelJustification
    {
        LeftJustify = 0,
        CentredJustify,
        RightJustify
    };

public:

    LabelledComponent(
        Component&          componentIn, 
        String              labelTextIn, 
        bool                ownsComponent = true, 
        LabelPosition       positionIn = LabelPosition::LeftOfComponent, 
        LabelJustification  justificationIn = LabelJustification::LeftJustify,
        Font                fontIn = Font()
    ) : 
        componentReference  (componentIn), 
        deleteComponent     (ownsComponent), 
        position            (positionIn), 
        justification       (justificationIn),
        font                (fontIn)
    {
        (Component*)&componentReference;

        if (deleteComponent)
        {
            component.reset(&componentReference);
        }
        
        addAndMakeVisible(&componentReference);

        label.reset(new Label(componentReference.getName() + "_Label"));
        label->setText(labelTextIn, dontSendNotification);
        label->setFont(font);
        label->setJustificationType(Justification::verticallyCentred);
        addAndMakeVisible(label.get());

        setLabelSize(font.getStringWidth(label->getText()) + 8, 24);

        // Default Size
        component->setSize(225, 24);
    }

    ~LabelledComponent()
    {
        if (deleteComponent)
            component = nullptr;

        label = nullptr;
    }

    void resized() override
    {
        //if ((expectedWidth > 0 && expectedHeight > 0) 
        //    && (expectedWidth != getWidth() || expectedHeight != getHeight())
        //   )
        //{
        //    float widthRatio = getWidth() / expectedWidth;
        //    float heightRatio = getHeight() / expectedHeight;

        //    componentReference.setSize(componentReference.getWidth() * widthRatio, componentReference.getHeight() * heightRatio);
        //    offsetLabel->setSize(offsetLabel->getWidth() * widthRatio, offsetLabel->getHeight() * heightRatio);
        //}

        int labelX = 0, labelY = 0, compX = 0, compY = 0;

        if (position % 2 == 0)
        {            
            if (position == LabelPosition::LeftOfComponent)
                compX = label->getWidth();
            
            else
                labelX = componentReference.getWidth();
        }
        else
        {
            if (position == LabelPosition::TopOfComponent)
                compY = label->getHeight();

            else
                labelY = componentReference.getHeight();

            if (justification == LabelJustification::CentredJustify)
                labelX = (getWidth() - label->getWidth()) / 2;

            else if (justification == LabelJustification::RightJustify)
                labelX = getWidth() - label->getWidth();
        }

        label->setTopLeftPosition(labelX, labelY);
        componentReference.setTopLeftPosition(compX, compY);
    }

    void paintOverChildren(Graphics& g) override
    {
        //g.setColour(Colours::red);
        //g.drawRect(getLocalBounds(), 1);
    }

    void setComponentSize(int widthIn, int heightIn)
    {
        componentReference.setSize(widthIn, heightIn);
        
        if (position % 2 == 0)
        {
            expectedWidth = labelWidth + componentReference.getWidth();
            expectedHeight = jmax(labelHeight, componentReference.getHeight());
        }
        else
        {
            expectedWidth = jmax(labelWidth, componentReference.getWidth());
            expectedHeight = labelHeight + componentReference.getHeight();
        }

        setSize(expectedWidth, expectedHeight);
    }

    void setLabelSize(int widthIn, int heightIn)
    {
        labelWidth = widthIn;
        labelHeight = heightIn;
        label->setSize(labelWidth, labelHeight);
        setComponentSize(componentReference.getWidth(), componentReference.getHeight());
    }

    void setLabelPosition(LabelPosition positionIn)
    {
        position = positionIn;
        setComponentSize(componentReference.getWidth(), componentReference.getHeight());
    }

    void setLabelJustification(LabelJustification justificationIn)
    {
        justification = justificationIn;
        resized();
    }

    Component* get()
    {
        return &componentReference;
    }

    Label* getLabel() const
    {
        return label.get();
    }

    String getLabelText() const
    {
        return label->getText();
    }

    bool hasOwnership() const
    {
        return deleteComponent;
    }

    LabelPosition getLabelPosition() const
    {
        return position;
    }

public:

    template <class ComponentType>
    static ComponentType* getComponentPointer(LabelledComponent* labelledComponentIn)
    {
        return dynamic_cast<ComponentType*>(labelledComponentIn->get());
    }

private:

    std::unique_ptr<Component> component;
    Component& componentReference;
    bool deleteComponent;
    LabelPosition position;
    LabelJustification justification;
    Font font;

    float expectedWidth, expectedHeight;

    int labelWidth, labelHeight;
    std::unique_ptr<Label> label;
};