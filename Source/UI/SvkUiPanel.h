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

class SvkUiPanel : public Component
{

public:
        
    virtual void connectToProcessor(AudioProcessorValueTreeState& apvtsIn) = 0;
    
    virtual void paint(Graphics& g) override;
    
    virtual void resized() override;
    
};
