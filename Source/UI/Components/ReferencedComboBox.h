/*
  ==============================================================================

    PresetLibraryBox.h
    Created: 27 May 2019 12:02:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"


class ReferencedComboBox : public ComboBox
{
    PopupMenu rootMenu;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferencedComboBox)

public:

    ReferencedComboBox(const String& nameIn={}, const PopupMenu* menuIn=nullptr);
    ~ReferencedComboBox();
    
    void setMenu(const PopupMenu& menuIn);
    
};
