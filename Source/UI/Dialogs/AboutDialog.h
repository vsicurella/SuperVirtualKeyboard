/*
  ==============================================================================

    AboutDialog.h
    Created: 15 Jul 2026
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Version.h"

// A simple pop-up shown from the main menu's "About" item. Displays the
// application blurb and is dismissed by clicking outside its CallOutBox.
class AboutDialog : public Component
{
public:

    AboutDialog()
    {
        aboutLabel.reset(new Label("AboutLabel", getBlurb()));
        aboutLabel->setJustificationType(Justification::topLeft);
        aboutLabel->setMinimumHorizontalScale(1.0f);
        aboutLabel->setColour(Label::textColourId, backgroundColour.contrasting());
        addAndMakeVisible(aboutLabel.get());

        setSize(320, 160);
    }

    ~AboutDialog() override {}

    void paint(Graphics& g) override
    {
        g.fillAll(backgroundColour);
    }

    void resized() override
    {
        aboutLabel->setBounds(getLocalBounds().reduced(16));
    }

private:

    static String getBlurb()
    {
        return String("SuperVirtualKeyboard") + newLine
             + "Version " + SVK_VERSION_STRING + newLine + newLine
             + "A microtonal virtual MIDI keyboard by Everytone." + newLine
             + "Licensed under Unlicense." + newLine + newLine
            //  + "https://www.everytone.zone"
            ;
    }

    const Colour backgroundColour = Colour(0xff323e44);
    std::unique_ptr<Label> aboutLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AboutDialog)
};
