/*
  ==============================================================================
	
	by Vincenzo Sicurella, 04-26-2019

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Structures/Mode.h"



//==============================================================================
/**

*/
class ColorChooserWindow  : public DocumentWindow,
							public ChangeBroadcaster,
							public ChangeListener
{
public:
    //==============================================================================
    ColorChooserWindow (String windowName, Colour backgroundColor, int windowFlags);
    ~ColorChooserWindow();

    //==============================================================================
	Colour getColorSelected();

	void closeButtonPressed() override;

    void paint (Graphics& g) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

private:

	ColourSelector* colorChooser;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColorChooserWindow)
};

