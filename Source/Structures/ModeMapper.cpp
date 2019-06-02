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

NoteMap ModeMapper::mapTo(Mode* modeMapped)
{
    return mapTo(modeMapped, modeMapped->getRootNote());
}

NoteMap ModeMapper::mapTo(Mode* modeMapped, int rootNoteTo)
{
	return map(mode, modeMapped, mode->getRootNote(), rootNoteTo);
}

//static
NoteMap ModeMapper::map(Mode* mapFrom, Mode* mapTo)
{
    return map(mapFrom, mapTo, mapFrom->getRootNote(), mapTo->getRootNote());
}

// static
NoteMap ModeMapper::map(Mode* mapFrom, Mode* mapTo, int rootNoteFrom, int rootNoteTo)
{
    Array<int> midiNotesFrom = mapFrom->getModalMidiNotes(0, rootNoteFrom);
    Array<int> midiNotesTo = mapTo->getModalMidiNotes(0, rootNoteTo);
    /*
     DBGArray(midiNotesFrom, "midi notes from");
     DBGArray(midiNotesTo, "midi notes to");
     */
    
    DBG("ModeFrom has " + String(midiNotesFrom.size()) + " modal notes and ModeTo has " + String(midiNotesTo.size()) + " modal notes.");
    
    int rootIndexFrom = midiNotesFrom.indexOf(rootNoteFrom);
    int rootIndexTo = midiNotesTo.indexOf(rootNoteTo);
    
    String txt = "Root note from is " + String(rootNoteFrom) + " and index is " + String(rootIndexFrom);
    
    DBG(txt);
    
    txt = "Root note to is " + String(rootNoteTo) + " and index is " + String(rootIndexTo);
    
    DBG(txt);

    int rootIndexOffset = rootIndexTo - rootIndexFrom;
    //int sgn = rootIndexOffset / abs(rootIndexOffset);
    //rootIndexOffset = totalModulus(abs(rootIndexOffset), mapFrom->getScaleSize());
    //rootIndexOffset *= sgn;

    
    DBG("Offset is " + String(rootIndexOffset));
    
    
    NoteMap mappingOut;
    
    /*
    for (int i = 0; i < mapSize; i++)
    {
        int indexTo = i - rootIndexOffset;
        
        if (indexTo >= 0 && indexTo < mapSize)
            mappingOut.setValue(midiNotesFrom.getUnchecked(i), midiNotesTo.getUnchecked(indexTo));
    }
    */
    int degCountFrom = 0;
    int degCountTo = 0;
    for (int m = 0; m < mappingOut.getSize(); m++)
    {
        if (m == midiNotesFrom.getUnchecked(degCountFrom))
        {
            degCountFrom++;
            degCountTo++;
            
            int indexTo = degCountTo + rootIndexOffset - 1;
            
            if (indexTo > -1 && indexTo < midiNotesTo.size())
            {
                mappingOut.setValue(m, midiNotesTo.getUnchecked(indexTo));
            }
            else
                mappingOut.setValue(m, -1);
            
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
NoteMap ModeMapper::stdMidiToMode(Mode* modeMapped, int rootNoteStd, int rootNoteMap)
{
    Mode meantone7_12 = Mode("2 2 1 2 2 2 1", "Meantone", rootNoteStd);
    return map(&meantone7_12, modeMapped, rootNoteStd, rootNoteMap);
}
