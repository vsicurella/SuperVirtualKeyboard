/*
  ==============================================================================

    KbmWriter.h
    Created: 23 Oct 2020 11:37:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../Structures/ModeMapper.h"

class KbmWriter
{
public:

    KbmWriter(
        int sizeIn, int firstMidiNoteIn, int lastMidiNoteIn, int mappingRootNoteIn,
        int referenceMidiNoteIn, double referenceFreqIn, int periodDegreeIn, 
        Array<int> mappingIn
    ) :
        size(sizeIn), firstMidiNote(firstMidiNoteIn), lastMidiNote(lastMidiNoteIn), mappingRootMidiNote(mappingRootNoteIn),
        referenceMidiNote(referenceMidiNoteIn), referenceFrequency(referenceFreqIn), periodDegree(periodDegreeIn), 
        mapping(mappingIn) {}

    bool writeTo(const File& fileToWriteTo)
    {
        if (!fileToWriteTo.existsAsFile())
        {
            if (!fileToWriteTo.create())
                return false;
        }

        StringArray valuesOut =
        {
            String(size), String(firstMidiNote), String(lastMidiNote),
            String(mappingRootMidiNote), String(referenceMidiNote),
            String(referenceFrequency), String(periodDegree), ""
        };

        // Ensure frequency is floating point
        if (!valuesOut[5].containsChar('.'))
            valuesOut.getReference(5) += ".0";

        // Change OOB notes to x
        for (auto deg : mapping)
        {
            String degOut = (deg < 0 || deg > 127) ? "x" : String(deg);
            valuesOut.getReference(7) += degOut + '\n';
        }

        String fileTextOut;
        for (int i = 0; i < sectionLabels.size(); i++)
        {
            if (i > 0)
                fileTextOut += '\n';

            fileTextOut += sectionLabels[i];
            fileTextOut += valuesOut[i];
        }

        fileToWriteTo.replaceWithText(fileTextOut);

        return true;
    }

    static KbmWriter fromModes(
        const Mode* inputMode, const Mode* outputMode, const ModeMapper& mapperIn,
        int firstMidiNoteIn = 0, int lastMidiNoteIn = 127, int mappingRootNoteIn = -1,
        int referenceMidiNoteIn = 69, double referenceFreqIn = 440.0, int periodDegreeIn = -1)
    {
        Array<int> periodMap = mapperIn.getSelectedPeriodMap(*inputMode, *outputMode);

        int middleNote = mappingRootNoteIn;
        int period = periodDegreeIn;

        int size = periodMap.size();

        if (periodMap.size() >= 128)
        {
            middleNote = 0;
            period = 127;
            size = 0;
        }
        else
        {
            if (middleNote < 0 || middleNote > 127)
                middleNote = outputMode->getRootNote();

            if (period < 0)
                period = outputMode->getScaleSize();
        }
        
        return KbmWriter(
            size, jlimit(0, 127, firstMidiNoteIn), jlimit(0, 127, lastMidiNoteIn), middleNote,
            jlimit(0, 127, referenceMidiNoteIn), jlimit(10e-3, 20e3, referenceFreqIn), period, periodMap
        );
    }


    //static KbmWriter fromNoteMap(const NoteMap& noteMapIn)
    //{
    //    Array<int> mapping;
    //}


private:

    int size;
    int firstMidiNote;
    int lastMidiNote;
    int mappingRootMidiNote;
    int referenceMidiNote;
    double referenceFrequency;
    int periodDegree;
    Array<int> mapping;

    StringArray sectionLabels =
    {
        "! Template for a keyboard mapping\n!\n! Size of map. The pattern repeats every so many keys:\n",
        "! First MIDI note number to retune:\n",
        "! Last MIDI note number to retune:\n",
        "! Middle note where the first entry of the mapping is mapped to:\n",
        "! Reference note for which frequency is given:\n",
        "! Frequency to tune the above note to (floating point e.g. 440.0):\n",
        "! Scale degree to consider as formal octave (determines difference in pitch\n! between adjacent mapping patterns):\n",
        "! Mapping.\n! The numbers represent scale degrees mapped to keys. The first entry is for\n"
        "! the given middle note, the next for subsequent higher keys.\n"
        "! For an unmapped key, put in an \"x\". At the end, unmapped keys may be left out.\n"
    };
};