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
            "Current Mapping"
            //"Advanced Mapping"
        },
        {
            IDs::noteMapNode
        },
        {
            SvkControlProperties(ControlTypeNames::GenericControl, "Current Mapping", false, 0)
            //SvkControlProperties(ControlTypeNames::GenericControl, "Advanced Mapping Note", false, 1)
        }
    )
{
    noteMapEditor = new NoteMapEditor(pluginState->getMidiInputFilterMap());
    controls.set(0, noteMapEditor, true);
    addAndMakeVisible(noteMapEditor);

    getSectionFlexBox(0)->items.getReference(1).associatedComponent = noteMapEditor;

    flexParent.items.getReference(0).maxWidth = 400;
}

MappingSettingsPanel::~MappingSettingsPanel()
{

}

void MappingSettingsPanel::buttonClicked(Button* button)
{

}