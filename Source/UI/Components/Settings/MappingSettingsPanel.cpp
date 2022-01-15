/*
  ==============================================================================

    MappingSettingsPanel.cpp
    Created: 7 Oct 2020 10:00:18pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "MappingSettingsPanel.h"

MappingSettingsPanel::MappingSettingsPanel(SvkPreset& presetIn)
    : SvkSettingsPanel(
        "MappingSettings", presetIn,
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
    //noteMapEditor = new NoteMapEditor(*pluginState->getMidiInputFilterMap());
    controls.set(0, noteMapEditor, true);
    addAndMakeVisible(noteMapEditor);
    getSectionFlexBox(0)->items.getReference(1).associatedComponent = noteMapEditor;

    //mappingModeChanged(pluginState->getMappingMode());

    //pluginState->addListener(this);

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

//void MappingSettingsPanel::mappingModeChanged(int mappingModeId)
//{
//    preset.setMidiInputMap(noteMapIn, true, false);
//}

//void MappingSettingsPanel::inputMappingChanged(NoteMap& newNoteMap)
//{
//    noteMapEditor->resetMapping(newNoteMap, false);
//}

void MappingSettingsPanel::mappingEditorChanged(NoteMap& newNoteMap)
{
    //pluginState->setMidiInputMap(newNoteMap, true, false);
}

void MappingSettingsPanel::registerEditorListener(MappingEditor::Listener* listenerIn)
{
    noteMapEditor->addListener(listenerIn);
}