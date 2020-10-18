/*
  ==============================================================================

    KeyboardViewPort.h
    Created: 31 Oct 2019 2:41:37pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "KeyboardComponent.h"

using namespace VirtualKeyboard;

class KeyboardViewport : public Viewport,
    private Button::Listener
{
public:

    KeyboardViewport(VirtualKeyboard::Keyboard* keyboardIn, const String& nameIn = String(), int scrollingModeIn = 1, int scrollingStyleIn = 1);

    void viewedComponentChanged(Component*) override;

    void visibleAreaChanged(const Rectangle<int>&) override;

    int getStepSmall();
    int getStepLarge();

    int getButtonWidth();
    bool isShowingButtons();

    float getCenterKeyProportion() const;

    void setButtonWidth(int widthIn);

    void setScrollingMode(int modeIdIn);
    void setScrollingStyle(int styleIdIn);

    void stepSmallForward();
    void stepSmallBackward();
    void stepLargeForward();
    void stepLargeBackward();

    void centerOnKey(int keyNumberIn);
    void centerOnKey(float keyNumberProportionIn);

    void resizeKeyboard();

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

private:

    // returns altitude
    int drawAngleBracket(Graphics& g, bool isRightPointing, int sideLength, int xCenter, int yCenter, float thickness = 1.0f);

    void redrawButtons(int heightIn);

private:

    int stepSmall = 0;
    int stepLarge = 0;

    int buttonWidth = 16;

    int scrollingModeSelected = 1;
    int scrollingStyleSelected = 1;

    std::unique_ptr<ImageButton> stepLeftSmall;
    std::unique_ptr<ImageButton> stepLeftLarge;
    std::unique_ptr<ImageButton> stepRightSmall;
    std::unique_ptr<ImageButton> stepRightLarge;

    Keyboard* keyboard;
};
