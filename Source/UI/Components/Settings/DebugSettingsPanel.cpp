/*
  ==============================================================================

    DebugSettingsPanel.cpp
    Created: 14 Nov 2019 11:46:31pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "DebugSettingsPanel.h"

DebugSettingsPanel::DebugSettingsPanel(SvkPluginState* pluginStateIn)
    : pluginState(pluginStateIn)
{
    svkParameters = pluginState->getParameters();
    
    setSize(100, 100);
    
    Label* lbl;
    Component* c;
    Slider* s;
    Identifier paramID;
    RangedAudioParameter* param;
    
    SvkParameters::Iterator i = SvkParameters::Iterator(*svkParameters);
    
    while (i.next())
    {
        paramID = i.getKey();
        param = i.getValue();
        
        lbl = labels.add(new Label(paramID.toString(), paramID.toString()));
        lbl->setColour(Label::textColourId, Colours::black);
        addAndMakeVisible(lbl);
        
        const NormalisableRange<float>& range = param->getNormalisableRange();
        s = new Slider(paramID.toString());
        c = controls.add(s);
        
        s->setSliderStyle(Slider::LinearHorizontal);
        //s->setRange(range.convertFrom0to1(0), range.convertFrom0to1(1));
        s->setNormalisableRange(NormalisableRange<double>(range.start, range.end));
        s->setTextBoxStyle (Slider::TextBoxLeft, false, 60, 20);
        s->setColour(Slider::textBoxTextColourId, Colours::black);
        s->setValue(param->getValue());
        s->addListener (this);
        addAndMakeVisible(s);
    }
    
}

DebugSettingsPanel::~DebugSettingsPanel()
{
}

void DebugSettingsPanel::paint(Graphics& g)
{
    g.setColour(Colours::palegreen);
    g.fillAll();
    
    if (getNumChildComponents() == 0)
    {
        g.setColour(Colours::black);
        g.drawText("No parameters", getWidth()/2, getHeight()/2,
                   getWidth(), getHeight(), Justification::centred);
    }
}

void DebugSettingsPanel::resized()
{
    Array<Component*> children = getChildren();
    
    if (children.size() > 0)
    {
        int rowHeight = 50;
        int y;
        
        for (int i = 0; i < controls.size(); i++)
        {
            y = rowHeight * i;
            labels.getUnchecked(i)->setBounds(0, y-10, getWidth(), rowHeight);
            controls.getUnchecked(i)->setBounds(0, y+rowHeight/2-10, getWidth(), rowHeight);
        }
    }
}

void DebugSettingsPanel::sliderValueChanged(Slider *slider)
{
    RangedAudioParameter* param = svkParameters->grab(Identifier(slider->getName()));
    param->setValue(slider->getValue());
    
    if (slider->getName() == IDs::pianoWHRatio.toString())
    {
        pluginState->getKeyboard()->setKeySizeRatio(param->getValue());
    }
}

