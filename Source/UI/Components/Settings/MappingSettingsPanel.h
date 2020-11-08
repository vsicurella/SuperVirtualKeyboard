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

class MappingSettingsPanel : public SvkSettingsPanel, 
                             public SvkPluginState::Listener, 
                             public MappingEditor::Listener
{
public:

    MappingSettingsPanel(SvkPluginState* pluginStateIn);
    ~MappingSettingsPanel();

    void visibilityChanged() override;

    void mappingModeChanged(int mappingModeId) override;

    void inputMappingChanged(NoteMap&) override;

    void mappingEditorChanged(NoteMap&) override;

    void registerEditorListener(MappingEditor::Listener* listenerIn);

private:

    Label* currentMappingLabel;
    NoteMapEditor* noteMapEditor;
};