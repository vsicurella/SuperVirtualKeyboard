/*
  ==============================================================================

    NoteMapEditor.cpp
    Created: 18 Oct 2020 5:13:12pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "NoteMapEditor.h"


NoteMapEditor::NoteMapEditor(NoteMap noteMapIn)
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

    resetMapping(noteMapIn, false);
}

NoteMapEditor::~NoteMapEditor()
{

}

void NoteMapEditor::resized()
{
    table.setBoundsInset(BorderSize<int>(1));
    header->resizeAllColumnsToFit(table.getWidth() - table.getVerticalScrollBar().getWidth());
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

                if (node.hasProperty(IDs::mappingMidiNoteIn))
                    value = node[IDs::mappingMidiNoteIn];
                else
                    value = noneTrans;

                break;

            case ColumnType::KeyNumber:
                
                if (node.hasProperty(IDs::pianoKeyNumber))
                    value = node[IDs::pianoKeyNumber];
                else
                    value = noneTrans;

                break;

            case ColumnType::MidiNoteOut:
                
                // TODO: note out mapping implementation
                if (node.hasProperty(IDs::pianoKeyNumber))
                    value = node[IDs::pianoKeyNumber];
                else
                    value = noneTrans;

                break;
            }

            if ((int)value < 0)
                value = belowThreshTrans;

            else if ((int)value > 127)
                value = aboveThreshTrans;

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
                if (columnId == ColumnType::KeyNumber || (columnId != ColumnType::MidiNoteOut && value == noneTrans))
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
            button->setButtonText(deleteTrans);
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

NoteMap NoteMapEditor::getCurrentNoteMapping() const
{
    return currentNoteMap;
}

void NoteMapEditor::mapMidiNoteToKey(int midiNoteIn, int keyNumberOut)
{
    currentNoteMap.setValue(midiNoteIn, keyNumberOut);
    listeners.call(&MappingEditor::Listener::mappingEditorChanged, currentNoteMap);
    resetMapping(currentNoteMap, false);
}

void NoteMapEditor::resetMapping(NoteMap mappingIn, bool sendMessage)
{
    currentNoteMap = mappingIn;
    currentNoteMapNode = currentNoteMap.getAsValueTree(IDs::midiInputRemap);
    
    if (sendMessage)
        listeners.call(&MappingEditor::Listener::mappingEditorChanged, currentNoteMap);

    table.updateContent();
}

//=============================================================================

void NoteMapEditor::editorShown(Label* label, TextEditor& editor)
{
    TableLabel* tl = static_cast<TableLabel*>(label);
    
    ValueTree mapNode = currentNoteMapNode.getChild(tl->getRowNumber());

    editor.setInputRestrictions(4, "-0123456789");

    switch (tl->getColumnId())
    {
    case ColumnType::KeyNumber:
        editor.setText(mapNode[IDs::pianoKeyNumber]);
        break;

    // TODO: implement actual midi channel out
    case ColumnType::MidiChannelOut:
        editor.setText(mapNode[IDs::pianoKeyNumber]);
        break;
    }
}

void NoteMapEditor::labelTextChanged(Label* label)
{
    TableLabel* tl = static_cast<TableLabel*>(label);

    switch (tl->getColumnId())
    {
    case ColumnType::KeyNumber:

        if (label->getText().length() > 0)
        {
            int newKeyNum = label->getText().getIntValue();

            if (newKeyNum < 0)
                label->setText(belowThreshTrans, dontSendNotification);

            else if (newKeyNum > 127)
                label->setText(aboveThreshTrans, dontSendNotification);

            else
                editMapping(tl->getRowNumber(), newKeyNum, false);
        }
        else
        {
            label->setText(noneTrans, dontSendNotification);
            currentNoteMapNode.getChild(tl->getRowNumber()).removeProperty(IDs::pianoKeyNumber, nullptr);
        }

        listeners.call(&MappingEditor::Listener::mappingEditorChanged, currentNoteMap);
        break;

    case ColumnType::MidiNoteIn:

        if (tl->getText().length() > 0)
        {
            int newNoteIn = tl->getText().getIntValue();
            int keyMappedToNote = currentNoteMap.getValue(newNoteIn);

            // Midi Note In mapping already exists
            if (keyMappedToNote != newNoteIn)
            {
                AlertWindow::showMessageBox(
                    AlertWindow::AlertIconType::WarningIcon, 
                    mappingErrorTrans, 
                    getAlreadyMappedMessage(newNoteIn, keyMappedToNote), 
                    okTrans
                );
                
                label->setText(noneTrans, dontSendNotification);
            }
            else
            {
                currentNoteMapNode.getChild(tl->getRowNumber()).setProperty(IDs::mappingMidiNoteIn, newNoteIn, nullptr);
            }
        }
        else
        {
            label->setText(noneTrans, dontSendNotification);
            currentNoteMapNode.getChild(tl->getRowNumber()).removeProperty(IDs::mappingMidiNoteIn, nullptr);
        }

        break;
    }
}

void NoteMapEditor::buttonClicked(Button* button)
{
    TableButton* tb = static_cast<TableButton*>(button);
    int rowNum = tb->getRowNumber();

    // Delete mapping
    if (rowNum < currentNoteMapNode.getNumChildren())
    {
        removeMapping(rowNum);
    }

    // Add new mapping
    else
    {
        addNewMapping();
    }
}

//=============================================================================

void NoteMapEditor::addNewMapping()
{
    currentNoteMapNode.appendChild(ValueTree(IDs::noteMapNode), nullptr);
    table.updateContent();
}

void NoteMapEditor::editMapping(int rowNumber, int keyNumberOut, bool sendMappingChangedMessage)
{
    currentNoteMapNode.getChild(rowNumber).setProperty(IDs::pianoKeyNumber, keyNumberOut, nullptr);
    currentNoteMap = NoteMap(currentNoteMapNode);
    resetMapping(currentNoteMap, sendMappingChangedMessage);
}

void NoteMapEditor::removeMapping(int rowNumber)
{
    currentNoteMapNode.removeChild(rowNumber, nullptr);
    currentNoteMap = NoteMap(currentNoteMapNode);
    resetMapping(currentNoteMap);
}

String NoteMapEditor::getAlreadyMappedMessage(int noteIn, int keyNumOut)
{
    return "Note " + String(noteIn) + " is already mapped to key " + String(keyNumOut) + ". Edit key number instead.";
}