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
    int rootNoteOffset = rootNoteTo % degreeMapIn.size();

	int rootIndexFrom = midiNotesFrom.indexOf(rootNoteFrom);
	int rootIndexTo = midiNotesTo.indexOf(rootNoteTo);
	int rootIndexOffset = rootIndexTo - rootIndexFrom;

    int degreeOffset = rootNoteTo - (ceil(rootNoteTo / mapTo.getScaleSize()) + 1) * degreeMapIn.size();
    int midiOffset = (int) (ceil(rootNoteTo / mapTo.getScaleSize()) + 1) * mapTo.getScaleSize() - (2 * rootNoteTo - rootNoteFrom);

/*
	DBG("ModeFrom has " + String(midiNotesFrom.size()) + " modal notes and ModeTo has " + String(midiNotesTo.size()) + " modal notes.");
	String txt = "Root note from is " + String(rootNoteFrom) + " and index is " + String(rootIndexFrom);
	DBG(txt);
	txt = "Root note to is " + String(rootNoteTo) + " and index is " + String(rootIndexTo);
	DBG(txt);
	DBG("Offset is " + String(rootNoteOffset));
 */
    DBG("Degree Map Size is " + String(degreeMapIn.size()));
    DBG("Degree Offset is " + String(degreeOffset));
    DBG("Midi Offset is " + String(midiOffset));

	NoteMap mappingOut;

	int degTo;
	int mapIndex;
	int periods;
    int midiNote;

	for (int m = 0; m < mappingOut.getSize(); m++)
	{
        mapIndex = m - degreeOffset;
        
        if (mapIndex >= 0)
        {
			periods = mapIndex / degreeMapIn.size();
			midiNote = degreeMapIn[mapIndex % degreeMapIn.size()] + periods * mapTo.getScaleSize() - midiOffset;
        }
        else
            midiNote = -1;
        
        mappingOut.setValue(m, midiNote);
        
		if (m == rootNoteFrom)
			DBG("Root Note From (" + String(rootNoteFrom) + ") produces note " + String(mappingOut.getValue(rootNoteFrom)) + " and comapre that to the Root note to (" + String(rootNoteTo) + ")");
	}
    
    //DBG("Note Mapping:\n" + mappingOut.toString());

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
    Array<int> mode1Steps = mode1.getSteps();
    Array<float> mode1ModalDegrees = mode1.getModalDegrees();
    
    Array<int> mode2MidiNotes = mode2.getNotesOfOrder();
    Array<int> mode2Steps = mode2.getSteps();
    Array<float> mode2ModalDegrees = mode2.getModalDegrees();
    
    int mode2ModeSize = mode2.getModeSize();
    
    int mode1ModeIndex = 0;
    int mode2ScaleIndex = 0;
    int mode2ModeIndex = 0;
    
	int degToAdd;
    int degSub;
	float stepFraction;
    
    float mode1Step;
    float mode2Step;
    
    
    while (mode2ModeIndex < mode2ModeSize)
    {
        mode1Step = mode1Steps[mode1ModeIndex];
        mode2Step = mode2Steps[mode2ModeIndex];
        
        degreeMapOut.add(mode2ScaleIndex);
        
        for (int d1 = 1; d1 < mode1Step; d1++)
        {
            stepFraction = d1 / mode1Step;
            
            // round up to the next mode degree
            degSub = mode2Step;
            
            // find closest degree
            for (int d2 = 1; d2 < mode2Step; d2++)
            {
                if (abs(d2 / mode2Step - stepFraction) < abs(degSub / mode2Step - stepFraction))
                    degSub = d2;
            }
            
            // resulting degree is the sum of previous steps, plus the next closest sub degree within the modal step
            int degSum = mode2ScaleIndex;
            degToAdd = degSum + degSub;
            degreeMapOut.add(degToAdd);
        }
        
        mode2ScaleIndex += mode2Step;
        mode1ModeIndex++;
        mode2ModeIndex++;
    }
    
	DBGArray(degreeMapOut, "Mode1 -> Mode2 Scale Degrees");
	return degreeMapOut;
}
