/*
  ==============================================================================

    AbletonMidiSequence.h
    Created: 14 Sep 2020 1:05:22am
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../Structures/Mode.h"

static File abletonUserLibrary =
#if JUCE_WINDOWS
    File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Ableton").getChildFile("User Library")
#else
    File::getSpecialLocation(File::userMusicDirectory).getChildFile("Ableton").getChildFile("User Library")
#endif
;

class AbletonMidiWriter
{

public:

    AbletonMidiWriter(const Mode& modeIn)
        : mode(modeIn)
    {
        refreshSequence();
    };

    bool writeTo(File directory = abletonUserLibrary, String filename = "")
    {
        if (!directory.exists())
            directory = File::getSpecialLocation(File::userDocumentsDirectory);

        if (filename.length() == 0)
            filename = mode.getName();

        if (!filename.endsWith(".mid"))
            filename += ".mid";

        File fileOut = File::createTempFile(".mid");

        midiFile.clear();
        midiFile.addTrack(sequence);
        midiFile.setTicksPerQuarterNote(96);
        
        File intendedFile = directory.getChildFile(filename);
        FileOutputStream stream(intendedFile);
        if (midiFile.writeTo(stream))
        {
            stream.flush();
            DBG("File written");
            return true;
        }
        
        return false;
    }

    bool write(String filename = "")
    {
        if (filename.length() == 0)
            filename = mode.getName();

        if (!filename.endsWith(".mid"))
            filename += ".mid";

        File intendedFile = abletonUserLibrary.exists()
            ? abletonUserLibrary.getChildFile(filename)
            : File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(filename);

        FileChooser chooser("Save Ableton Midi Map", intendedFile, "*.mid");

        if (chooser.browseForFileToSave(true))
            return writeTo(chooser.getResult().getParentDirectory(), chooser.getResult().getFileName());
        
        return false;
    }

    void refreshSequence()
    {
        Array<int> noteNums = mode.getNotesOfOrder();
        sequence.clear();

        for (auto noteNum : noteNums)
        {
            MidiMessage msg = MidiMessage::noteOn(1, noteNum, (uint8)1);
            sequence.addEvent(msg);
        }

        for (auto noteNum : noteNums)
        {
            MidiMessage msg = MidiMessage::noteOff(1, noteNum);
            sequence.addEvent(msg, 96);
        }

        sequence.updateMatchedPairs();
    }


private:

    const Mode& mode;

    MidiFile midiFile;
    MidiMessageSequence sequence;
};