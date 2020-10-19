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
    noteMapEditor = new NoteMapEditor(*pluginState->getMidiInputFilterMap());
    controls.set(0, noteMapEditor, true);
    addAndMakeVisible(noteMapEditor);
    noteMapEditor->addListener(this);
    getSectionFlexBox(0)->items.getReference(1).associatedComponent = noteMapEditor;

    Label* msg = new Label("Message", "More mapping features on the way :)");
    controls.set(1, msg, true);
    addAndMakeVisible(msg);
    getSectionFlexBox(1)->items.getReference(0).associatedComponent = msg;

    flexParent.items.getReference(0).maxWidth = 400;
}

MappingSettingsPanel::~MappingSettingsPanel()
{
}

void MappingSettingsPanel::setEditorToListenTo(MappingEditor* mappingEditor)
{
    if (externalEditor)
        externalEditor->removeListener(noteMapEditor);

    externalEditor = mappingEditor;

    externalEditor->addListener(noteMapEditor);
}

void MappingSettingsPanel::visibilityChanged()
{
    if (externalEditor)
        externalEditor->removeListener(noteMapEditor);
}

void MappingSettingsPanel::mappingChanged(NoteMap& noteMapIn)
{
    pluginState->setMidiInputMap(noteMapIn, true, false);
}

void MappingSettingsPanel::buttonClicked(Button* button)
{

}