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
        
    Label* lbl;
    Component* c;
    Slider* s;
    String paramName;
    RangedAudioParameter* param;
    AudioParameterInt* api;
    
    for (int i = 0; i < svkParameters->getSize(); i++)
    {
        param = svkParameters->getUnchecked(i);
        paramName = param->paramID;
        
        lbl = labels.add(new Label("Lbl" + paramName, param->name));
        lbl->setColour(Label::textColourId, Colours::black);
        addAndMakeVisible(lbl);
        
        const NormalisableRange<float>& range = param->getNormalisableRange();
        s = new Slider("Sld" + paramName);
        c = controls.add(s);
        
        s->setSliderStyle(Slider::LinearHorizontal);
        
        api = dynamic_cast<AudioParameterInt*>(param);
        if (api)
        {
            s->setNormalisableRange(NormalisableRange<double>(range.start, range.end, 1));
        }
        else
        {
            s->setNormalisableRange(NormalisableRange<double>(range.start, range.end, 0.01f));
        }
        
        s->setScrollWheelEnabled(false);
        s->setTextBoxStyle (Slider::TextBoxLeft, false, 60, 20);
        s->setColour(Slider::textBoxTextColourId, Colours::black);
        s->setValue(param->getValue(), dontSendNotification);
        s->addListener (this);
        addAndMakeVisible(s);
    }
    
    setSize(100, 100);
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
    int rowHeight = 50;
    int y;
    
    for (int i = 0; i < controls.size(); i++)
    {
        y = rowHeight * i;
        labels.getUnchecked(i)->setBounds(0, y-10, getWidth(), rowHeight);
        controls.getUnchecked(i)->setBounds(0, y+rowHeight/2-10, getWidth(), rowHeight);
    }
}

void DebugSettingsPanel::sliderValueChanged(Slider *slider)
{
    String paramName = slider->getName().substring(3);
    RangedAudioParameter* param = svkParameters->grab(Identifier(paramName));
    param->setValue(slider->getValue());
    
    if (paramName == IDs::pianoWHRatio.toString())
    {
        pluginState->getKeyboard()->setKeySizeRatio(param->getValue());
    }
}

