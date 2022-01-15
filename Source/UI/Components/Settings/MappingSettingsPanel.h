/*
  ==============================================================================

    MappingSettingsPanel.h
    Created: 7 Oct 2020 10:00:18pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../../../PresetManager.h"
#include "SvkSettingsPanel.h"
#include "../NoteMapEditor.h"

class MappingSettingsPanel : public SvkSettingsPanel, 
                             public SvkPresetManager::Listener, 
                             public MappingEditor::Listener
{
public:

    MappingSettingsPanel(SvkPreset& presetIn);
    ~MappingSettingsPanel();

    void visibilityChanged() override;

    //void mappingModeChanged(int mappingModeId) override;

    //void inputMappingChanged(NoteMap&) override;

    void mappingEditorChanged(NoteMap&) override;

    void registerEditorListener(MappingEditor::Listener* listenerIn);

private:

    Label* currentMappingLabel;
    NoteMapEditor* noteMapEditor;
};