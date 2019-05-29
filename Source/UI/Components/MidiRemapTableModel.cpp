/*
  ==============================================================================

    MapEditTable.cpp
    Created: 26 May 2019 3:19:25pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "MidiRemapTableModel.h"

//==============================================================================

MidiRemapTableModel::MidiRemapTableModel(TableListBox* parentIn, MidiRemapper* inputMapIn, MidiRemapper* outputMapIn, TextEditor::InputFilter* filterIn)
	: parent(parentIn), inputRemapper(inputMapIn), outputRemapper(outputMapIn), inputFilter(filterIn)
{
	numRows = jmin(inputRemapper->size(), outputRemapper->size());
}

MidiRemapTableModel::~MidiRemapTableModel()
{
	while (noteLabels.size() > 0)
	{
		noteLabels.getLast()->removeListener(this);
		noteLabels.removeLast();
	}
}

int MidiRemapTableModel::getNumRows()
{
	return numRows;
}

Component* MidiRemapTableModel::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
	if (columnId > 1 && rowNumber == jlimit(0, numRows - 1, rowNumber))
	{
		NoteLabelBox* editor = editor = dynamic_cast<NoteLabelBox*>(existingComponentToUpdate);

		if (!editor)
		{
			editor = noteLabels.add(new NoteLabelBox());
			editor->setEditable(false, true);
			editor->setColour(Label::textColourId, Colours::black);
			editor->addListener(this);
		}

		editor->setCell(rowNumber, columnId);

		int noteNum = -1;

		if (columnId == 2)
			noteNum = inputRemapper->getNoteRemapped(rowNumber);
		else if (columnId == 3)
			noteNum = outputRemapper->getNoteRemapped(rowNumber);

		if (noteNum >= 0)
			editor->setText(String(noteNum), dontSendNotification);
		else
			editor->setText("None", dontSendNotification);

		return editor;
	}

	jassert(existingComponentToUpdate == nullptr);
	return nullptr;
}

void MidiRemapTableModel::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	if (rowNumber % 2 == 0)
		g.setColour(Colours::dimgrey);
	else
		g.setColour(Colours::lightgrey);

	g.fillRect(0, 0, width, height);
}

void MidiRemapTableModel::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
	if (columnId == 1)
	{
		g.setColour(Colours::black);
		g.drawText(String(rowNumber), 0, 0, width, height, Justification::centred);
	}

	g.drawLine(width, 0, width, height, 2.0);
}

void MidiRemapTableModel::labelTextChanged(Label* labelThatHasChanged)
{
	parent->updateContent();
}

void MidiRemapTableModel::editorShown(Label* label, TextEditor& editor)
{
	if (inputFilter)
		editor.setInputFilter(inputFilter, false);

	valOld = label->getText().getIntValue();
}

void MidiRemapTableModel::editorHidden(Label* label, TextEditor& editor)
{
	NoteLabelBox* box = dynamic_cast<NoteLabelBox*>(label);
	Point<int> cell = Point<int>(box->getRow(), box->getColId());
	int valNew = editor.getText().getIntValue();

	if (valOld != valNew)
	{
		DBG("Cell " + cell.toString() + " changed");

		if (cell.y == 2)
		{
			DBG("Remapping Note " + String(cell.x) + " --> " + String(valNew));
			inputRemapper->setNote(cell.x, valNew);
			DBG("Input Remapped, Note " + String(cell.x) + " --> " + String(inputRemapper->getNoteRemapped(cell.x)));
			DBG("Old note " + String(valOld) + " is triggered by " + String(inputRemapper->getNoteMidi(valOld)));
		}
		else if (cell.y == 3)
		{
			DBG("Remapping Note " + String(cell.x) + " --> " + String(valNew));
			outputRemapper->setNote(cell.x, valNew);
			DBG("Output Remapped, Note " + String(cell.x) + " --> " + String(outputRemapper->getNoteRemapped(cell.x)));
		}
	}
}

//======================================================================================================================


NoteLabelBox::NoteLabelBox()
{
	setEditable(false, true, true);
}

int NoteLabelBox::getRow()
{
	return rowNumber;
}

int NoteLabelBox::getColId()
{
	return columnId;
}

void NoteLabelBox::setCell(int rowNumIn, int columnIdIn)
{
	rowNumber = rowNumIn;
	columnId = columnIdIn;
}