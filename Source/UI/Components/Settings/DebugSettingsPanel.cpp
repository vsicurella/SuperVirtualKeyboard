/*
  ==============================================================================

    DebugSettingsPanel.cpp
    Created: 14 Nov 2019 11:46:31pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "DebugSettingsPanel.h"

DebugSettingsPanel::DebugSettingsPanel(AudioProcessorValueTreeState& processorTreeIn, Array<String>* paramIDsIn)
    : SvkUiPanel(processorTreeIn), paramIDs(paramIDsIn)
{
    setSize(100, 100);
}

DebugSettingsPanel::~DebugSettingsPanel()
{
}

void DebugSettingsPanel::connectToProcessor()
{
    Label* lbl;
    Component* c;
    Slider* s;
    String paramName;
    RangedAudioParameter* param;
    AudioParameterInt* api;
        
    for (int i = 0; i < paramIDs->size(); i++)
    {
        paramName = paramIDs->getUnchecked(i);
        param = processorTree.getParameter(paramName);
        
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
            s->setSliderStyle(Slider::IncDecButtons);
            s->setValue(api->get(), dontSendNotification);
        }
        else
        {
            s->setNormalisableRange(NormalisableRange<double>(range.start, range.end, 0.01f));
            s->setValue(param->getValue(), dontSendNotification);
        }
        
        s->setScrollWheelEnabled(false);
        s->setTextBoxStyle (Slider::TextBoxLeft, false, 60, 20);
        s->setColour(Slider::textBoxTextColourId, Colours::black);
        s->addListener (this);
        
        sliderAttachments.add(new SliderAttachment(processorTree, paramName, *s));
        addAndMakeVisible(s);
    }
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
    Slider* s;
    
    for (int i = 0; i < controls.size(); i++)
    {
        y = rowHeight * i;
        labels.getUnchecked(i)->setBounds(0, y-10, getWidth(), rowHeight);
        
        s = dynamic_cast<Slider*>(controls.getUnchecked(i));
        
        if (s && s->getSliderStyle() == Slider::IncDecButtons)
            controls.getUnchecked(i)->setBounds(0, y+rowHeight/2-10, getWidth() * 0.2, rowHeight);
        else
            controls.getUnchecked(i)->setBounds(0, y+rowHeight/2-10, getWidth(), rowHeight);
    }
}

void DebugSettingsPanel::sliderValueChanged(Slider *slider)
{
    String paramName = slider->getName().substring(3);
    RangedAudioParameter* param = processorTree.getParameter(paramName);
    AudioParameterInt* api;
    
    if (paramName == IDs::pianoWHRatio.toString())
    {
        //param->setValue(slider->getValue());
        //pluginState->getKeyboard()->setKeySizeRatio(param->getValue());
    }
    
    else if (paramName == IDs::keyboardNumRows.toString())
    {
        //api = dynamic_cast<AudioParameterInt*>(param);
        //*api = slider->getValue();
        //pluginState->getKeyboard()->setNumRows(api->get());
    }
    
}

