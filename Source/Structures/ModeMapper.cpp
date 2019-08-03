/*
  ==============================================================================

    ModeMapper.cpp
    Created: 30 May 2019 8:02:42pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ModeMapper.h"

void ModeMapper::setMapType(int mapTypeIn)
{
    mapType = mapTypeIn;
}

void ModeMapper::setMapOrdersParameters(int order1, int order2, int offset1, int offset2)
{
    mapByOrderNum1 = order1;
    mapByOrderNum2 = order2;

    mapByOrderOffset1 = offset1;
    mapByOrderOffset2 = offset2;
}

NoteMap ModeMapper::map(const Mode& mode1, const Mode& mode2, int mapTypeIn, int order1, int order2, int offset1, int offset2,
                        NoteMap prevMap)
{
    NoteMap mapOut;
    
    int mappingTypeUsed = mapType;
    
    if (mapTypeIn >= 0)
        mappingTypeUsed = mapTypeIn;
    
    switch (mappingTypeUsed)
    {
        case ModeToScale:
            mapOut = mapToMode1Scale(mode1, mode2);
            break;
            
        case ModeByOrder:
            
            mapOut = mapByOrder(mode1, mode2, order1, order2, offset1, offset2, prevMap);
            break;
            
        default:
            mapOut = mapFull(mode1, mode2);
            break;
    }
    
    return mapOut;
}

NoteMap ModeMapper::mapFull(const Mode& mode1, const Mode& mode2, Array<int> degreeMapIn)
{
	if (degreeMapIn.size() != mode1.getOrders().size())
		degreeMapIn = degreeMapFullMode(mode1, mode2);
    else
        degreeMapIn = degreeMapPeriod(mode1, mode2);

	NoteMap mappingOut;
	
	int midiNote;

	for (int m = 0; m < mappingOut.getSize(); m++)
	{
		midiNote = degreeMapIn[m];

		if (midiNote >= 0)
		{
			mappingOut.setValue(m, midiNote);
		}
		else
		{
			mappingOut.setValue(m, -1);
		}
	}

	//DBG("Root note from mode 1 is " + String(mode1.getRootNote()) + 
	//	" and it produces the note " + String(mappingOut.getValue(mode1.getRootNote())) + 
	//	". Root note from mode 2 is " + String(mode2.getRootNote()));

	return mappingOut;
}


NoteMap ModeMapper::mapByOrder(const Mode& mode1, const Mode& mode2, int mode1Order, int mode2Order, int mode1Offset, int mode2Offset, NoteMap prevMap)
{
    mode1Order = jlimit(0, mode1.getMaxStep() - 1, mode1Order);
    mode2Order = jlimit(0, mode2.getMaxStep() - 1, mode2Order);

    Array<int> midiNotesFrom = mode1.getNotesOfOrder(mode1Order);
    Array<int> midiNotesTo = mode2.getNotesOfOrder(mode2Order);
    
    mode1Offset = jlimit(0, midiNotesFrom.size() - 1, mode1Offset);
    mode2Offset = jlimit(0, midiNotesTo.size() - 1, mode2Offset);
    
    int rootNoteFrom = mode1.getRootNote();
    int rootNoteTo = mode2.getRootNote();

    int rootIndexFrom = mode1.getNotesOfOrder().indexOf(rootNoteFrom);
    int rootIndexTo = mode2.getNotesOfOrder().indexOf(rootNoteTo);
    int rootIndexOffset = rootIndexTo - rootIndexFrom + mode2Offset - mode1Offset;
    
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
			prevMap.setValue(noteFrom, noteTo);
    }
    
    return prevMap;
}

NoteMap ModeMapper::mapToMode1Period(const Mode& mode1, const Mode& mode2, Array<int> degreeMapIn)
{
    // ensure degree map is the same size as Mode1 Period
    if (degreeMapIn.size() != mode1.getScaleSize())
        degreeMapIn = degreeMapPeriod(mode1, mode2);
    
	int rootNoteFrom = mode1.getRootNote();
	int rootNoteTo = mode2.getRootNote();
	int rootNoteOffset = rootNoteTo - rootNoteFrom;

    int degreeOffset = rootNoteTo - (ceil(rootNoteTo / (float) mode2.getScaleSize())) * degreeMapIn.size();
	int midiOffset = (int)(ceil(rootNoteTo / (float) mode2.getScaleSize())) * mode2.getScaleSize() - rootNoteTo + rootNoteOffset;

    //DBG("Degree Offset is " + String(degreeOffset));
    //DBG("Midi Offset is " + String(midiOffset));

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
        
		//if (m == rootNoteFrom)
		//	DBG("Root Note From (" + String(rootNoteFrom) + ") produces note " + String(mappingOut.getValue(rootNoteFrom)) + " and comapre that to the Root note to (" + String(rootNoteTo) + ")");
	}
    
	return mappingOut;
}

NoteMap ModeMapper::mapToMode1Scale(const Mode& mode1, const Mode& mode2, int mode2Order)
{
    Array<int> mode2ModalNotes = mode2.getNotesOfOrder(mode2Order);
    
    int mode1RootIndex = mode1.getRootNote();
    int mode2RootIndex = mode2ModalNotes.indexOf(mode2.getRootNote());
    int rootIndexOffset = mode2RootIndex - mode1RootIndex;
    
    NoteMap mappingOut;
    
    int degToAdd;
    int mode2ModeIndex = 0;
    
    for (int m = 0; m < mode1.getOrders().size(); m++)
    {
        mode2ModeIndex = m + rootIndexOffset;
        
        if (mode2ModeIndex < 0)
        {
            degToAdd = mode2ModeIndex;
        }
        else if (mode2ModeIndex >= mode2ModalNotes.size())
        {
            degToAdd = 128;
        }
        else
        {
            degToAdd = mode2ModalNotes[mode2ModeIndex];
        }
        
        mappingOut.setValue(m, degToAdd);
        
        //if (m == mode1.getRootNote())
        //    DBG("Root Note From (" + String(mode1.getRootNote()) + ") produces note " + String(mappingOut.getValue(mode1.getRootNote())) + " and comapre that to the Root note to (" + String(mode2.getRootNote()) + ")");
    }
    
    return mappingOut;
}


NoteMap ModeMapper::stdMidiToMode(const Mode& modeMapped, int rootNoteStd)
{
    Mode meantone7_12 = Mode("2 2 1 2 2 2 1", "Meantone", rootNoteStd);
	
	if (modeMapped.getModeSize() == meantone7_12.getModeSize())
		return mapToMode1Period(meantone7_12, modeMapped);
	else
		return mapFull(meantone7_12, modeMapped);
}

Array<int> ModeMapper::degreeMapPeriod(const Mode& mode1, const Mode& mode2)
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

	//DBGArray(degreeMapOut, "Mode1 -> Mode2 Scale Degrees");
	return degreeMapOut;
}

Array<int> ModeMapper::degreeMapFullMode(const Mode& mode1, const Mode& mode2)
{
    Array<int> degreeMapOut;
    
    //DBG("Mode1 Root: " + String(mode1.getRootNote()) + "\tMode2Root: " + String(mode2.getRootNote()));
    
	Array<int> mode1Steps = Mode::orders_to_steps(mode1.getOrders());
    Array<int> mode2Steps = Mode::orders_to_steps(mode2.getOrders());

	Array<int> mode1MidiNotes = Mode::sum_of_steps(mode1Steps, 0, true);
	Array<int> mode2MidiNotes = Mode::sum_of_steps(mode2Steps, 0, true);

	int mode1RootIndex = mode1MidiNotes.indexOf(mode1.getRootNote());
	int mode2RootIndex = mode2MidiNotes.indexOf(mode2.getRootNote());
	int rootIndexOffset = mode2RootIndex - mode1RootIndex;
    
    //DBG("Mode1 Root Index: " + String(mode1RootIndex) + "\tMode2Root: " + String(mode2RootIndex));
    
    //DBGArray(mode2MidiNotes, "Mode2 Midi Notes");

    
    int mode2ScaleIndex = 0;
    
    if (rootIndexOffset > 0)
    {
        mode2ScaleIndex = sumUpToIndex(mode2Steps, rootIndexOffset);
    }
    
    int degToAdd, degSub;
    float stepFraction;
    
    float mode1Step, mode2Step;
    
    int mode2ModeIndex = 0;
    
    for (int m = 0; m < mode1Steps.size(); m++)
	{
        mode2ModeIndex = m + rootIndexOffset;
        
		// this is assuming that the order will always start at 0
		mode1Step = mode1Steps[m];
        
        if (mode2ModeIndex < 0)
        {
            mode2Step = 0;
            degToAdd = mode2ModeIndex;
        }
        else if (mode2ModeIndex >= mode2Steps.size())
        {
            mode2Step = 0;
            degToAdd = 128;
        }
        else
        {
            mode2Step = mode2Steps[mode2ModeIndex];
            degToAdd = mode2ScaleIndex;
        }
        
        degreeMapOut.add(degToAdd);

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
            
            if (mode2ModeIndex < 0)
                degreeMapOut.add(mode2ModeIndex);
            else
                degreeMapOut.add(degToAdd);
		}
        
        mode2ScaleIndex += mode2Step;
	}
    
    return degreeMapOut;
}
