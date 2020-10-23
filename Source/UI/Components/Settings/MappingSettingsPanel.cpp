/*
  ==============================================================================

    MappingSettingsPanel.cpp
    Created: 7 Oct 2020 10:00:18pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "MappingSettingsPanel.h"

MappingSettingsPanel::MappingSettingsPanel(SvkPluginState* pluginStateIn)
    : SvkSettingsPanel(
        "MappingSettings", pluginStateIn,
        {
            "Current Mapping",
            ""
            //"Advanced Mapping"
        },
        {
            IDs::noteMapNode,
            Identifier("Message")
        },
        {
            SvkControlProperties(ControlTypeNames::GenericControl, "Current Mapping", false, 0),
            SvkControlProperties(ControlTypeNames::GenericControl, "New features message", false, 1)
            //SvkControlProperties(ControlTypeNames::GenericControl, "Advanced Mapping Note", false, 1)
        }
    )
{
    currentMappingLabel = static_cast<Label*>(getSectionFlexBox(0)->items.getReference(0).associatedComponent);

    noteMapEditor = new NoteMapEditor(*pluginState->getMidiInputFilterMap());
    controls.set(0, noteMapEditor, true);
    addAndMakeVisible(noteMapEditor);
    getSectionFlexBox(0)->items.getReference(1).associatedComponent = noteMapEditor;

    if (pluginState->getMappingMode() != 3)
        noteMapEditor->setEnabled(false);

    pluginState->addListener(this);

    Label* msg = new Label("Message", "Currently, you can only edit the key number. More mapping features on the way! :)");
    controls.set(1, msg, true);
    addAndMakeVisible(msg);
    getSectionFlexBox(1)->items.getReference(0).associatedComponent = msg;

    flexParent.items.getReference(0).maxWidth = 400;
}

MappingSettingsPanel::~MappingSettingsPanel()
{
}

void MappingSettingsPanel::visibilityChanged()
{
    if (!isVisible())
    {
        noteMapEditor->removeAllListeners();
    }
}

void MappingSettingsPanel::mappingModeChanged(int mappingModeId)
{
    if (mappingModeId == 3)
    {
        noteMapEditor->setEnabled(true);
        currentMappingLabel->setText("Current Mapping", dontSendNotification);
    }
    else
    {
        noteMapEditor->setEnabled(false);
        currentMappingLabel->setText("Current Mapping (Read-only)", dontSendNotification);
    }
}

void MappingSettingsPanel::inputMappingChanged(NoteMap& newNoteMap)
{
    noteMapEditor->resetMapping(newNoteMap, false);
}

void MappingSettingsPanel::mappingEditorChanged(NoteMap& newNoteMap)
{
    pluginState->setMidiInputMap(newNoteMap, true, false);
}