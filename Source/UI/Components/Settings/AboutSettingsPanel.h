/*
  ==============================================================================

    AboutSettingsPanel.h
    Created: 28 Jun 2026
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"
#include "Version.h"

class AboutSettingsPanel : public SvkSettingsPanel
{
public:

    AboutSettingsPanel(SvkState& presetIn)
        : SvkSettingsPanel("AboutSettingsPanel", presetIn,
            StringArray(), Array<Identifier>(), Array<SvkControlProperties>())
    {
        aboutLabel.reset(new Label("AboutLabel", getBlurb()));
        aboutLabel->setJustificationType(Justification::topLeft);
        aboutLabel->setMinimumHorizontalScale(1.0f);
        aboutLabel->setColour(Label::textColourId, Colour(0xff323e44).contrasting());
        addAndMakeVisible(aboutLabel.get());

        setSize(100, 100);
    }

    ~AboutSettingsPanel() override {}

    // No sections/controls to lay out; the base implementation only draws
    // section-header backgrounds, which we don't have here.
    void paint(Graphics&) override {}

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
             + "https://www.everytone.zone";
    }

    std::unique_ptr<Label> aboutLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AboutSettingsPanel)
};
