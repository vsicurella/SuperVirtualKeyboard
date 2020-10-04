/*
  ==============================================================================

    KeyboardViewPort.h
    Created: 31 Oct 2019 2:41:37pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"

class KeyboardViewport : public Viewport,
                         private Button::Listener
{
    int stepSmall = 0;
    int stepLarge = 0;

    int buttonWidth = 16;
    
    int scrollingModeSelected = 1;
    int scrollingStyleSelected = 1;
    
    std::unique_ptr<ImageButton> stepLeftSmall;
    std::unique_ptr<ImageButton> stepLeftLarge;
    std::unique_ptr<ImageButton> stepRightSmall;
    std::unique_ptr<ImageButton> stepRightLarge;

    // returns altitude
    int drawAngleBracket(Graphics& g, bool isRightPointing, int sideLength, int xCenter, int yCenter, float thickness=1.0f);

    void redrawButtons(int heightIn);
    
public:
    
    KeyboardViewport(const String &nameIn=String(), int scrollingModeIn=1, int scrollingStyleIn=1);
    
    int getStepSmall();
    int getStepLarge();
    
    int getButtonWidth();
    bool isShowingButtons();
    
    void setStepSmall(int smallSizeIn);
    void setStepLarge(int largeSizeIn);

    void setButtonWidth(int widthIn);
    
    void setScrollingMode(int modeIdIn);
    void setScrollingStyle(int styleIdIn);
    
    void stepSmallForward();
    void stepSmallBackward();
    void stepLargeForward();
    void stepLargeBackward();
    
    void resized() override;

    void buttonClicked(Button* button) override;
    
    enum ScrollingMode
    {
        NoScrolling = 0,
        Smooth,
        Stepped,
        SmoothWithButtons
    };
    
    enum ScrollingStyle
    {
        Hidden = 0,
        Bar,
        KeyboardView,
        Dots
    };
};
