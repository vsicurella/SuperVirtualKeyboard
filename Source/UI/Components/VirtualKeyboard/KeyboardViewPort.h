/*
  ==============================================================================

    KeyboardViewPort.h
    Created: 31 Oct 2019 2:41:37pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"

class KeyboardViewport : public Viewport
{
    int stepSmall = 0;
    int stepLarge = 0;
    
    
public:
    
    KeyboardViewport(const String &nameIn=String());
    KeyboardViewport(Component* contentComponentIn, bool deleteWhenNotNeeded=true);
    
    int getStepSmall();
    int getStepLarge();
    
    void setStepSmall(int smallSizeIn);
    void setStepLarge(int largeSizeIn);
    
    void stepSmallForward();
    void stepSmallBackward();
    void stepLargeForward();
    void stepLargeBackward();
    
    void paint(Graphics& g) override;
    void resized() override;
};
