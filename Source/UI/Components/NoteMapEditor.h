/*
  ==============================================================================

    NoteMapEditor.h
    Created: 18 Oct 2020 5:13:12pm
    Author:  Vincenzo

    A component for viewing and editing note mappings.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../../Structures/MappingEditorBase.h"
#include "../../CommonFunctions.h"

// Table Helpers

class TableComponent
{
    int rowNumber;
    int columnId;

public:

    void setRowNumber(int rowNumberIn)
    {
        rowNumber = rowNumberIn;
    }

    void setColumnId(int columnIdIn)
    {
        columnId = columnIdIn;
    }

    int getRowNumber() const
    {
        return rowNumber;
    }

    int getColumnId() const
    {
        return columnId;
    }
};

class TableLabel : public TableComponent, public Label {};
class TableButton : public TableComponent, public TextButton {};
class TableSlider : public TableComponent, public Slider {};

class NoteMapEditor : public Component, 
                      public TableListBoxModel, 
                      public MappingEditor,
                      private Button::Listener, 
                      private Label::Listener
{
public:

    enum ColumnType
    {
        MidiNoteIn = 1,
        MidiChannelIn,
        KeyNumber,
        MidiNoteOut,
        MidiChannelOut,
        ToggleMapping
    };

public:

    NoteMapEditor(NoteMap noteMapIn);
    ~NoteMapEditor();

    void resized() override;

    //=============================================================================

    // TableListBoxModel implementation

    int getNumRows() override;

    void paintRowBackground(Graphics&, int rowNumber, int w, int h, bool rowIsSelected) override;

    void paintCell(Graphics&, int rowNumber, int columnId, int w, int h, bool isSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component*) override;

    int getColumnAutoSizeWidth(int columnId) override;

    String getCellTooltip(int rowNumber, int columnId) override;

    void deleteKeyPressed(int lastRowSelected) override;

    void returnKeyPressed(int lastRowSelected) override;

    //=============================================================================

    // MappingEditor implementation

    NoteMap getCurrentNoteMapping() const override;

    void mapMidiNoteToKey(int midiNoteIn, int keyNumberOut) override;

    void resetMapping(NoteMap mappingIn = NoteMap(), bool sendMessage = true) override;

    //=============================================================================

    // Label::Listener implementation

    void editorShown(Label*, TextEditor&) override;

    void labelTextChanged(Label*) override;

    // Button::Listener implementation

    void buttonClicked(Button*) override;

    //=============================================================================

    void addNewMapping();

    // TODO: add midi note out & channel editing
    void editMapping(int rowNumber, int keyNumOut, bool sendMappingChangedMessage = true);

    void removeMapping(int rowNumber);

private:

    String getAlreadyMappedMessage(int noteIn, int keyNumOut);

private:

    NoteMap currentNoteMap;
    ValueTree currentNoteMapNode;

    Font font = Font(11, Font::plain);
    TableListBox table = { {}, this };
    TableHeaderComponent* header;

    String newMappingTrans = translate("  Add new mapping");
    String belowThreshTrans = translate("Too low");
    String aboveThreshTrans = translate("Too high");
    String mappingErrorTrans = translate("Mapping Error");
    String noneTrans = translate("None");
    String deleteTrans = translate("Delete");
    String okTrans = translate("Ok");
};
