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

	int buttonWidth = 75;
	
	bool showStepButtons = true;

	Image singleBracketImage;
	Image doubleBracketImage;
    
	std::unique_ptr<ImageButton> stepLeftSmall;
	std::unique_ptr<ImageButton> stepLeftLarge;
	std::unique_ptr<ImageButton> stepRightSmall;
	std::unique_ptr<ImageButton> stepRightLarge;

	// returns altitude
	int drawAngleBracket(Graphics& g, bool isLeft, int sideLength, int xCenter, int yCenter, float thickness=1.0f);

	void redrawButtons();
    
public:
    
    KeyboardViewport(const String &nameIn=String(), bool showStepButtonsIn=true);
    
    int getStepSmall();
    int getStepLarge();
	
	int getButtonWidth();
    
    void setStepSmall(int smallSizeIn);
    void setStepLarge(int largeSizeIn);

	void setButtonWidth(int widthIn);
	
	void setShowButtons(bool toShowButtons);
    
    void stepSmallForward();
    void stepSmallBackward();
    void stepLargeForward();
    void stepLargeBackward();
    
    void resized() override;

	void buttonClicked(Button* button) override;
};
