/*
  ==============================================================================

    ViewSettingsPanel.h
    Created: 13 Nov 2019 7:09:31pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"

class ViewSettingsPanel : public SvkSettingsPanel
{
    
public:
    
    ViewSettingsPanel(SvkPluginState*);
    ~ViewSettingsPanel();
      
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViewSettingsPanel)
};
