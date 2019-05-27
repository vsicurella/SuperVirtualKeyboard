/*
  ==============================================================================

    MidiSettingsWindow.h
    Created: 24 May 2019 6:28:28pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"

class MidiSettingsWindow : public DialogWindow
{
public:
    
    MidiSettingsWindow()
    : DialogWindow("Midi Remapper Window", Colours::slategrey, true) {}
    
    void closeButtonPressed() override
    {
        setVisible(false);
    }
};
