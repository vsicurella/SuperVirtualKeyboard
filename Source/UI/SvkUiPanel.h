/*
  ==============================================================================

    SvkUiPanel.h
    Created: 26 Dec 2019 3:54:15pm
    Author:  Vincenzo Sicurella
    
    A base class for a UI Panel in SuperVirtualKeyboard, which currently just requires
    a helper function to be defined for connecting parameters to the AudioProcessor.

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

//typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
//typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;
//typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;


class SvkUiPanel : public Component
{
protected:
    
    //AudioProcessorValueTreeState& processorTree;

public:
    
    SvkUiPanel(/*AudioProcessorValueTreeState& processorTreeIn*/)
      //  : processorTree(processorTreeIn) 
        {};
    
    virtual ~SvkUiPanel() = 0;
        
    //virtual void connectToProcessor() = 0;
    
    virtual void paint(Graphics& g) override;
    
    virtual void resized() override;
    
};
