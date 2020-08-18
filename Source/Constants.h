/*
  ==============================================================================

    Constants.h
    Created: 16 Aug 2020 12:41:04pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Structures/Mode.h"

#define STD_TUNING_MODE_NODE Mode::createNode("2 2 1 2 2 2 1", "Meantone")

const int MAX_MODE_SLOTS_INDEX = 127;
const float SVK_PRESET_VERSION = 1.0;