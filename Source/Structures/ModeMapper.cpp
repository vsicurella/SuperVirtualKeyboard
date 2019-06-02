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
    
    int rootIndexFrom = midiNotesFrom.indexOf(rootNoteFrom);
    int rootIndexTo = midiNotesTo.indexOf(rootNoteTo);
    
    String txt = "Root note from is " + String(rootNoteFrom) + " and index is " + String(rootIndexFrom);
    
    DBG(txt);
    
    txt = "Root note to is " + String(rootNoteTo) + " and index is " + String(rootIndexTo);
    
    DBG(txt);

    int rootIndexOffset = rootIndexTo - rootIndexFrom;
    
    DBG("Offset is " + String(rootIndexOffset));
    
    
    NoteMap mappingOut;
    
    int mapSize = jmin(midiNotesFrom.size(), midiNotesTo.size(), mappingOut.getSize());
/*
    for (int i = 0; i < mapSize; i++)
    {
        int indexTo = i - rootIndexOffset;
        
        if (indexTo >= 0 && indexTo < mapSize)
            mappingOut.setValue(midiNotesFrom.getUnchecked(i), midiNotesTo.getUnchecked(indexTo));
    }
    */
    int degCount = 0;
    for (int m = 0; m < mappingOut.getSize(); m++)
    {
        
        if (degCount < mapSize && m == midiNotesFrom.getUnchecked(degCount))
        {
            int indexTo = degCount - rootIndexOffset;
            
            if (indexTo >= 0 && indexTo < mapSize && indexTo < mappingOut.getSize())
                mappingOut.setValue(midiNotesFrom.getUnchecked(degCount), midiNotesTo.getUnchecked(indexTo));
            else
                mappingOut.setValue(m, -1);
            
            degCount++;
        }
        else
            mappingOut.setValue(m, -1);
    }
    
    return mappingOut;
}

// static
NoteMap ModeMapper::stdMidiToMode(Mode* modeMapped, int rootNoteStd, int rootNoteMap)
{
    Mode meantone7_12 = Mode("2 2 1 2 2 2 1", "Meantone", rootNoteStd);
    return map(&meantone7_12, modeMapped, rootNoteStd, rootNoteMap);
}
