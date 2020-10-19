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

class MappingSettingsPanel : public SvkSettingsPanel, public MappingEditor::Listener
{
public:

    MappingSettingsPanel(SvkPluginState* pluginStateIn);
    ~MappingSettingsPanel();

    void buttonClicked(Button*) override;

    void setEditorToListenTo(MappingEditor* mappingEditor);

    void visibilityChanged() override;

    //================================================================

    void mappingChanged(NoteMap&) override;

private:

    MappingEditor* externalEditor = nullptr;
    NoteMapEditor* noteMapEditor;
};