/*
  ==============================================================================

    MidiInputFilter.h
    Created: 14 May 2019 10:25:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MidiInputFilter
{
	MidiKeyboardState* externalKeyboardState;
	Array<int>* midiNoteMapping;


public:

	MidiInputFilter(MidiKeyboardState* keyboardStateIn);
};