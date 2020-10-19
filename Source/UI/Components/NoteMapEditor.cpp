/*
  ==============================================================================

    NoteMapEditor.cpp
    Created: 18 Oct 2020 5:13:12pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "NoteMapEditor.h"


NoteMapEditor::NoteMapEditor(NoteMap* noteMapIn)
{
    header = &table.getHeader();
    header->addColumn("Note In", ColumnType::MidiNoteIn,font.getStringWidth("128"), font.getStringWidth("Note In"));
    header->addColumn("Key", ColumnType::KeyNumber, font.getStringWidth("128"), font.getStringWidth("Key"));
    header->addColumn("Note Out", ColumnType::MidiNoteOut, font.getStringWidth("128"), font.getStringWidth("Note Out"));
    header->addColumn("+/-", ColumnType::ToggleMapping, font.getStringWidth("+/-"), font.getStringWidth("+/-"));
    header->setStretchToFitActive(true);

    table.setColour(ListBox::outlineColourId, Colours::darkgrey);
    table.setOutlineThickness(1);

    addAndMakeVisible(table);

    setCurrentNoteMap(noteMapIn);
}

NoteMapEditor::~NoteMapEditor()
{

}

void NoteMapEditor::resized()
{
    table.setBoundsInset(BorderSize<int>(1));
    header->resizeAllColumnsToFit(table.getWidth() - table.getVerticalScrollBar().getWidth());
}

void NoteMapEditor::setCurrentNoteMap(NoteMap* noteMapIn)
{
    currentNoteMap = noteMapIn;

    if (currentNoteMap)
        currentNoteMapNode = currentNoteMap->getAsValueTree(IDs::midiInputRemap);
    else
        currentNoteMapNode = ValueTree(IDs::midiInputRemap);

    DBG("MAP EDITOR LOADED:" + currentNoteMapNode.toXmlString());
}

//=============================================================================

int NoteMapEditor::getNumRows()
{
    return currentNoteMapNode.getNumChildren() + 1;
}

void NoteMapEditor::paintRowBackground(Graphics& g, int rowNumber, int w, int h, bool rowIsSelected)
{
    g.setColour(rowNumber % 2 == 0 ? Colours::grey : Colours::lightgrey);
    g.fillAll();

    if (rowNumber == currentNoteMapNode.getNumChildren())
    {
        g.setColour(Colours::black);
        g.drawText(newMappingTrans, 0, 0, w, h, Justification::left);
    }
}

void NoteMapEditor::paintCell(Graphics& g, int rowNumber, int columnId, int w, int h, bool isSelected)
{

}

Component* NoteMapEditor::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    if (columnId < ColumnType::ToggleMapping)
    {
        if (rowNumber < currentNoteMapNode.getNumChildren())
        {
            ValueTree node = currentNoteMapNode.getChild(rowNumber);
            var value;

            switch (columnId)
            {

            case ColumnType::MidiNoteIn:
                value = node[IDs::mappingMidiNoteIn];
                break;

            case ColumnType::KeyNumber:
                value = node[IDs::pianoKeyNumber];
                
                if ((int)value > 127)
                    value = -1;

                break;

            case ColumnType::MidiNoteOut:
                // TODO: note out mapping implementation
                value = node[IDs::pianoKeyNumber];

                if ((int)value > 127)
                    value = -1;

                break;
            }

            if (value.isVoid())
                value = 0;

            auto* valueLabel = static_cast<TableLabel*> (existingComponentToUpdate);
            if (valueLabel == nullptr)
            {
                valueLabel = new TableLabel();
                valueLabel->setColour(Label::ColourIds::textColourId, Colours::black);
                valueLabel->setColour(Label::ColourIds::outlineColourId, Colours::black);
                valueLabel->setJustificationType(Justification::centred);

                // TODO: note out mapping implementation
                if (columnId < ColumnType::MidiNoteOut)
                {
                    valueLabel->setEditable(true);
                    valueLabel->addListener(this);
                }
            }

            valueLabel->setRowNumber(rowNumber);
            valueLabel->setColumnId(columnId);
            valueLabel->setText(value.toString(), dontSendNotification);

            return valueLabel;
        }
        else
        {
            // don't understand why I have to do this
            delete existingComponentToUpdate;
        }
    }

    // Add / Delete Column
    else
    {
        auto* button = static_cast<TableButton*>(existingComponentToUpdate);

        if (existingComponentToUpdate == nullptr)
        {
            button = new TableButton();
            button->addListener(this);
        }

        button->setRowNumber(rowNumber);
        button->setColumnId(columnId);

        if (rowNumber < currentNoteMapNode.getNumChildren())
            button->setButtonText("-");
        else
            button->setButtonText("+");

        return button;
    }

    return nullptr;
}

int NoteMapEditor::getColumnAutoSizeWidth(int columnId)
{
    switch (ColumnType(columnId))
    {
    case ColumnType::ToggleMapping:
        return font.getStringWidth("+");

    default:
        return font.getStringWidth("128");
    }
}

String NoteMapEditor::getCellTooltip(int rowNumber, int columnId)
{
    return String();
}

void NoteMapEditor::deleteKeyPressed(int lastRowSelected)
{

}

void NoteMapEditor::returnKeyPressed(int lastRowSelected)
{

}

//=============================================================================

void NoteMapEditor::editorShown(Label* label, TextEditor& editor)
{

}

void NoteMapEditor::labelTextChanged(Label* label)
{

}

void NoteMapEditor::buttonClicked(Button* button)
{

}