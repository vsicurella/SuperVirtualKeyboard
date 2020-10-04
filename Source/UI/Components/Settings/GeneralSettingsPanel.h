/*
  ==============================================================================

    GeneralSettingsPanel.h
    Created: 3 Sep 2020 9:11:33pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"

class GeneralSettingsPanel : public SvkSettingsPanel
{
    
public:
    
    GeneralSettingsPanel(SvkPluginState*);
    ~GeneralSettingsPanel() override;
    
    void directoryChanged(DirectoryBrowserComponent*, File) override;
    
private:

    LabelledComponent* presetLabel;
    LabelledComponent* modeLabel;
    LabelledComponent* settingsLabel;

    DirectoryBrowserComponent* presetDirectoryBrowser;
    DirectoryBrowserComponent* modeDirectoryBrowser;
    DirectoryBrowserComponent* settingsDirectoryBrowser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneralSettingsPanel)
};
