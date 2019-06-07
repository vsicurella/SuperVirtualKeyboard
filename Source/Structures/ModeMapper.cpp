/*
  ==============================================================================

    ModeMapper.cpp
    Created: 30 May 2019 8:02:42pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ModeMapper.h"

NoteMap ModeMapper::map(const Mode& mapFrom, const Mode& mapTo, Array<int> degreeMapIn)
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
    /*
    Array<float> modeFromDegrees = mapFrom.getModalDegrees();
    Array<float> modeToDegrees = mapTo.getModalDegrees();
    Array<int> modeFromOrders = mapFrom.getOrders();
    Array<int> modeToOrders = mapTo.getOrders();
    Array<int> modeToSteps = mapTo.getStepsOfOrders();
    Array<int> modeFromSteps = mapFrom.getStepsOfOrders();
    
    float degFrom, degTo;
    int orderFrom, orderTo;
    int stepFrom, stepTo;
    
    for (int m = 0; m < mappingOut.getSize(); m++)
    {
        degFrom = modeFromDegrees[m];
        degTo = modeToDegrees[m];
        orderFrom = modeFromOrders[m];
        orderTo = modeToOrders[m];
        stepFrom = modeToSteps[m];
        stepTo = modeToSteps[m];
        
        
    }
    
    */
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
    
    int mode1Deg = 0;
    int mode2Deg = 0;
    
    int midiNote;
    
    for (int m = 0; m < mode2ScaleSize; m++)
    {
        midiNote = m + mode1RootNote;
        
        if (midiNote)
    }
    
}
