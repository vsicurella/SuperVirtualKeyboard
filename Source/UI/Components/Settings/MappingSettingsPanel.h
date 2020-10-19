/*
  ==============================================================================

    MappingSettingsPanel.h
    Created: 7 Oct 2020 10:00:18pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"
#include "../NoteMapEditor.h"

class MappingSettingsPanel : public SvkSettingsPanel
{
public:

    MappingSettingsPanel(SvkPluginState* pluginStateIn);
    ~MappingSettingsPanel();

    void buttonClicked(Button*) override;

private:

    NoteMapEditor* noteMapEditor;
};