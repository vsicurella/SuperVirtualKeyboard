/*
  ==============================================================================

    ModeMapper.cpp
    Created: 30 May 2019 8:02:42pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ModeMapper.h"


NoteMap ModeMapper::map(const Mode& mapFrom, const Mode& mapTo)
{
    
    Array<int> midiNotesFrom = mapFrom.getNotesOfOrder();
    Array<int> midiNotesTo = mapTo.getNotesOfOrder();
    
    int rootNoteFrom = mapFrom.getRootNote();
    int rootNoteTo = mapTo.getRootNote();

    int rootIndexFrom = midiNotesFrom.indexOf(rootNoteFrom);
    int rootIndexTo = midiNotesTo.indexOf(rootNoteTo);
    int rootIndexOffset = rootIndexTo - rootIndexFrom;

    
    DBG("ModeFrom has " + String(midiNotesFrom.size()) + " modal notes and ModeTo has " + String(midiNotesTo.size()) + " modal notes.");
    String txt = "Root note from is " + String(rootNoteFrom) + " and index is " + String(rootIndexFrom);
    DBG(txt);
    txt = "Root note to is " + String(rootNoteTo) + " and index is " + String(rootIndexTo);
    DBG(txt);
    DBG("Offset is " + String(rootIndexOffset));
    

    
    NoteMap mappingOut;
    int degCountFrom = 0;
    int degCountTo = 0;
    
    for (int m = 0; m < mappingOut.getSize(); m++)
    {
        if (m == midiNotesFrom[degCountFrom])
        {
            int indexTo = degCountTo + rootIndexOffset;
            
            if (indexTo > -1 && indexTo < midiNotesTo.size())
            {
                mappingOut.setValue(m, midiNotesTo.getUnchecked(indexTo));
            }
            else
                mappingOut.setValue(m, -1);
            
            degCountFrom++;
            degCountTo++;
        }
        else
            mappingOut.setValue(m, -1);
        
        if (m == rootNoteFrom)
        {
            DBG("Root Note From (" + String(rootNoteFrom) + ") produces note " + String(mappingOut.getValue(rootNoteFrom)) + " and comapre that to the Root note to (" + String(rootNoteTo) +")");
            jassert(mappingOut.getValue(rootNoteFrom) == rootNoteTo);
        }
    }
    
    return mappingOut;
}

NoteMap ModeMapper::map(const Mode& mapFrom, const Mode& mapTo, Array<int> degreeMapIn)
{

	Array<int> midiNotesFrom = mapFrom.getNotesOfOrder();
	Array<int> midiNotesTo = mapTo.getNotesOfOrder();

	int rootNoteFrom = mapFrom.getRootNote();
	int rootNoteTo = mapTo.getRootNote();
	int rootNoteOffset = rootNoteTo - rootNoteFrom;

	int rootIndexFrom = midiNotesFrom.indexOf(rootNoteFrom);
	int rootIndexTo = midiNotesTo.indexOf(rootNoteTo);
	int rootIndexOffset = rootIndexTo - rootIndexFrom;


	DBG("ModeFrom has " + String(midiNotesFrom.size()) + " modal notes and ModeTo has " + String(midiNotesTo.size()) + " modal notes.");
	String txt = "Root note from is " + String(rootNoteFrom) + " and index is " + String(rootIndexFrom);
	DBG(txt);
	txt = "Root note to is " + String(rootNoteTo) + " and index is " + String(rootIndexTo);
	DBG(txt);
	DBG("Offset is " + String(rootIndexOffset));

	NoteMap mappingOut;

	int degTo;
	int index;
	int periods = 0;

	for (int m = 0; m < mappingOut.getSize(); m++)
	{
		index = totalModulus(m + rootNoteOffset, degreeMapIn.size());
		periods = m / degreeMapIn.size();
		degTo = degreeMapIn[index] + periods*mapTo.getScaleSize();
		mappingOut.setValue(m, mapTo.getMidiNote(degTo));

		if (m == rootNoteFrom)
			DBG("Root Note From (" + String(rootNoteFrom) + ") produces note " + String(mappingOut.getValue(rootNoteFrom)) + " and comapre that to the Root note to (" + String(rootNoteTo) + ")");
	}

	return mappingOut;
}


NoteMap ModeMapper::stdMidiToMode(const Mode& modeMapped, int rootNoteStd, Array<int> degreeMapIn)
{
    Mode meantone7_12 = Mode("2 2 1 2 2 2 1", "Meantone", rootNoteStd);
    return map(meantone7_12, modeMapped);
}

Array<int> ModeMapper::autoDegreeMap(const Mode& mode1, const Mode& mode2)
{
    Array<int> degreeMapOut;
    
    Array<int> mode1MidiNotes = mode1.getNotesOfOrder();
    Array<int> mode2MidiNotes = mode2.getNotesOfOrder();
	Array<float> mode2ModalDegrees = mode2.getModalDegrees();
    
    int mode1ScaleSize = mode1.getScaleSize();
    int mode2ScaleSize = mode2.getScaleSize();
    int mode1ModeSize = mode1.getModeSize();
    int mode2ModeSize = mode2.getModeSize();
    
    float modeMultiplier = (float) mode2ModeSize / mode1ModeSize;
    
    int mode1RootNote = mode1.getRootNote();
    int mode2RootNote = mode2.getRootNote();
    
    int mode1RootIndex = mode1MidiNotes.indexOf(mode1RootNote);
    int mode2RootIndex = mode2MidiNotes.indexOf(mode2RootNote);
    int rootOffset = mode2RootIndex - mode1RootIndex;
    
	int mapSize;

	if (mode1ModeSize == mode2ModeSize)
		mapSize = mode1ScaleSize;
	else
		mapSize = sumUpToRingIndex(mode1.getSteps(), mode2ModeSize - 1);
	
	int mode1Index = 0;
    int mode2Index = 0;
    
    int midiNote;
	int degToAdd;
	float modalDegree;

    for (int m = 0; m < mapSize; m++)
    {
        midiNote = m + mode1RootNote;
        
		if (mode1MidiNotes.contains(midiNote))
		{
			degToAdd = mode2.getScaleDegree(mode2MidiNotes[mode2RootIndex + mode2Index]);
			degreeMapOut.add(degToAdd);
			mode1Index++;
			mode2Index++;
		}
		else
		{
			DefaultElementComparator<float> sorter;
			float modalDegree = mode1.getModeDegree(m) * modeMultiplier;
			
			// find closest degree
			int degFloor = mode2ModalDegrees.indexOfSorted(sorter, floor(modalDegree));
			int degCeil = mode2ModalDegrees.indexOfSorted(sorter, ceil(modalDegree));

			int closestDeg = degCeil;
			int d;

			for (int i = 0; i < (degCeil - degFloor); i++)
			{
				d = degFloor + i;
				if (abs(mode2ModalDegrees[d] - modalDegree) < abs(mode2ModalDegrees[closestDeg] - modalDegree))
					closestDeg = d;
			}

			degreeMapOut.add(closestDeg);
		}
    }

	DBGArray(degreeMapOut, "Mode1 -> Mode2 Scale Degrees");
	return degreeMapOut;
}
