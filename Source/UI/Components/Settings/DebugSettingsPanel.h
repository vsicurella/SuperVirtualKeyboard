/*
  ==============================================================================

    DebugSettingsPanel.h
    Created: 14 Nov 2019 11:46:31pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once
#include "SvkSettingsPanel.h"

class DebugSettingsPanel : public SvkSettingsPanel, public Timer
{

public:

    DebugSettingsPanel(SvkPluginState*);

    ~DebugSettingsPanel();


    //void paint(Graphics& g) override;
    //void resized() override;

    void timerCallback() override;

private:

    SvkPluginState* pluginState;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DebugSettingsPanel)
};
