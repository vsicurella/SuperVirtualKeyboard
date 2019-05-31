/*
  ==============================================================================

    MapEditTable.h
    Created: 26 May 2019 3:19:25pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../CommonFunctions.h"
#include "../../Structures/MidiFilter.h"

//==============================================================================
/*
*/

class NoteLabelBox : public Label
{
	int rowNumber;
	int columnId;

public:

	NoteLabelBox();

	int getRow();
	int getColId();

	void setCell(int rowNumIn, int columnIdIn);
};

class MidiRemapTableModel : public TableListBoxModel,
	public Label::Listener
{
	int numRows;
	OwnedArray<NoteLabelBox> noteLabels;

	TableListBox* parent;

	MidiFilter* inputRemapper;
	MidiFilter* outputRemapper;

	int valOld = -1; // used to store old NoteMapBox value, might only be for debugging purposes

	TextEditor::InputFilter* inputFilter;

public:

	MidiRemapTableModel(TableListBox* parentIn, MidiFilter* inputRemapperIn, MidiFilter* outputRemapperIn, TextEditor::InputFilter* filterIn = nullptr);
	~MidiRemapTableModel();

	int getNumRows() override;

	Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

	void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

	void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

	void labelTextChanged(Label* labelThatHasChanged) override;

	void editorShown(Label* label, TextEditor& editor);

	void editorHidden(Label* label, TextEditor& editor);
};