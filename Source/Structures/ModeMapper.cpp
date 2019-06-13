/*
  ==============================================================================

    ModeMapper.cpp
    Created: 30 May 2019 8:02:42pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ModeMapper.h"


NoteMap ModeMapper::mapByOrder(const Mode& mode1, const Mode& mode2, int mode1Order, int mode2Order, int mode1Offset, int mode2Offset)
{
    Array<int> midiNotesFrom = mode1.getNotesOfOrder(mode1Order);
    Array<int> midiNotesTo = mode2.getNotesOfOrder(mode2Order);
    
    int rootNoteFrom = mode1.getRootNote();
    int rootNoteTo = mode2.getRootNote();

    int rootIndexFrom = mode1.getNotesOfOrder().indexOf(rootNoteFrom);
    int rootIndexTo = mode2.getNotesOfOrder().indexOf(rootNoteTo);
    int rootIndexOffset = rootIndexTo - rootIndexFrom;

    NoteMap mappingOut;
    
    int mode1Index, mode2Index;
    int noteFrom, noteTo;
    
    for (int m = 0; m < midiNotesFrom.size(); m++)
    {
        // might make more sense to not have a mode1Offset
        mode1Index = totalModulus(m + mode1Offset, midiNotesFrom.size());
        mode2Index = totalModulus(m + rootIndexOffset + mode2Offset, midiNotesTo.size());
        
        noteFrom = midiNotesFrom[mode1Index];
        noteTo = midiNotesTo[mode2Index];
        
        if (noteFrom > 0)
            mappingOut.setValue(noteFrom, noteTo);
        
        if (m == rootNoteFrom)
        {
            DBG("Root Note From (" + String(rootNoteFrom) + ") produces note " + String(mappingOut.getValue(rootNoteFrom)) + " and comapre that to the Root note to (" + String(rootNoteTo) +")");
        }
    }
    
    return mappingOut;
}

NoteMap ModeMapper::mapToMode1Period(const Mode& mode1, const Mode& mode2, Array<int> degreeMapIn)
{
    // ensure degree map is the same size as Mode1 Period
    if (degreeMapIn.size() != mode1.getScaleSize())
        degreeMapIn = autoDegreeMapPeriod(mode1, mode2);
    
	int rootNoteFrom = mode1.getRootNote();
	int rootNoteTo = mode2.getRootNote();
	int rootNoteOffset = rootNoteTo - rootNoteFrom;

    int degreeOffset = rootNoteTo - (ceil(rootNoteTo / mode2.getScaleSize()) + 1) * degreeMapIn.size();
	int midiOffset = (int)(ceil(rootNoteTo / mode2.getScaleSize()) + 1) * mode2.getScaleSize() - rootNoteTo + rootNoteOffset;

    DBG("Degree Offset is " + String(degreeOffset));
    DBG("Midi Offset is " + String(midiOffset));

	NoteMap mappingOut;

	int mapIndex;
	int periods;
    int midiNote;

	for (int m = 0; m < mappingOut.getSize(); m++)
	{
		mapIndex = m - degreeOffset;
        
        if (mapIndex >= 0)
        {
			periods = mapIndex / degreeMapIn.size();
			midiNote = degreeMapIn[mapIndex % degreeMapIn.size()] + periods * mode2.getScaleSize() - midiOffset;
        }
        else
            midiNote = -1;
        
        mappingOut.setValue(m, midiNote);
        
		if (m == rootNoteFrom)
			DBG("Root Note From (" + String(rootNoteFrom) + ") produces note " + String(mappingOut.getValue(rootNoteFrom)) + " and comapre that to the Root note to (" + String(rootNoteTo) + ")");
	}
    
	return mappingOut;
}


NoteMap ModeMapper::stdMidiToMode(const Mode& modeMapped, int rootNoteStd, Array<int> degreeMapIn)
{
    Mode meantone7_12 = Mode("2 2 1 2 2 2 1", "Meantone", rootNoteStd);
    return mapByOrder(meantone7_12, modeMapped);
}

Array<int> ModeMapper::autoDegreeMapPeriod(const Mode& mode1, const Mode& mode2)
{
    Array<int> degreeMapOut;
    
    Array<int> mode1Steps = mode1.getSteps();
    Array<int> mode2Steps = mode2.getSteps();
    
    int mode1ModeSize = mode1.getModeSize();
    int mode2ModeSize = mode2.getModeSize();
    
    int mode2ScaleIndex = 0;
    
	int degToAdd;
    int degSub;
	float stepFraction;
    
    float mode1Step;
    float mode2Step;
    
    for (int m1 = 0; m1 < mode2ModeSize; m1++)
    {
        mode1Step = mode1Steps[m1 % mode1ModeSize];
        mode2Step = mode2Steps[m1];
        
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
            degToAdd = mode2ScaleIndex + degSub;
            degreeMapOut.add(degToAdd);
        }
        
        mode2ScaleIndex += mode2Step;
    }

	DBGArray(degreeMapOut, "Mode1 -> Mode2 Scale Degrees");
	return degreeMapOut;
}

Array<int> ModeMapper::autoDegreeMapFull(const Mode& mode1, const Mode& mode2)
{
    Array<int> degreeMapOut;
    
    Array<int> mode1Steps = mode1.getSteps();
    Array<int> mode2Steps = mode2.getSteps();
    
    int mode1ModeSize = mode1.getModeSize();
    int mode2ModeSize = mode2.getModeSize();
    
    int mode2ScaleIndex = 0;
    
    int degToAdd;
    int degSub;
    float stepFraction;
    
    float mode1Step;
    float mode2Step;
    
    for (int m1 = 0; m1 < mode2ModeSize; m1++)
    {
        mode1Step = mode1Steps[m1 % mode1ModeSize];
        mode2Step = mode2Steps[m1];
        
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
            degToAdd = mode2ScaleIndex + degSub;
            degreeMapOut.add(degToAdd);
        }
        
        mode2ScaleIndex += mode2Step;
    }
    
    DBGArray(degreeMapOut, "Mode1 -> Mode2 Scale Degrees");
    return degreeMapOut;
}
