/*
  ==============================================================================

    ModeMapper.cpp
    Created: 30 May 2019 8:02:42pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ModeMapper.h"

ModeMapper::ModeMapper(Mode* modeIn)
{
	mode = modeIn;
}

ModeMapper::ModeMapper(Array<Mode>* modesIn)
{
	modeChain = modesIn;
}

NoteMap ModeMapper::mapTo(const Mode& modeMapped, Array<int> degreeMapIn)
{
    return map(*mode, modeMapped);
}

// static
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

// static
NoteMap ModeMapper::stdMidiToMode(const Mode& modeMapped, int rootNoteStd, Array<int> degreeMapIn)
{
    Mode meantone7_12 = Mode("2 2 1 2 2 2 1", "Meantone", rootNoteStd);
    return map(meantone7_12, modeMapped);
}
