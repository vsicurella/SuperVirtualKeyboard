/*
  ==============================================================================

    Tuning.cpp
    Created: 26 Nov 2019 9:16:30pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "Tuning.h"

Tuning::Tuning()
{
    semitonesFromUnison.resize(128);
    semitonesFromUnison.fill(0);
    
    pitchBendTable.resize(128);
    pitchBendTable.fill(0);
}

Tuning::Tuning(double period, double divisions, int rootNote, int noteMin, int noteMax)
{
    int note = rootNote - noteMin;
    int noteIndex = noteMin;
    double interval = ratioToCents(period) / divisions;
    double cents;
    
    while(noteIndex <= noteMax)
    {
        cents = interval * note++;
        semitonesFromUnison.add(cents);
        pitchBendTable.add(semitonesToPitchBend(cents));
        noteIndex++;
    }
    
    intervalsFromRoot.add(interval);
}

Tuning::Tuning(double period, double generator, int scaleSize, int stepsDown, int rootNote, int noteMin, int noteMax)
{
    
    double cents = generator * -(stepsDown + 1);
    
    // make sure this is still within range
    if (abs(cents) >= period)
    {
        double factor = abs(cents) / period;
        cents += floor(factor) * period;
    }
    
    if (cents + generator < 0)
    {
        cents += period;
    }
    
    for (int i = stepsDown; i < scaleSize - stepsDown; i++)
    {
        cents += generator;
        
        if (cents == 0)
            continue;
        
        else if (cents == period)
            break;
        
        else if (cents > period)
            cents -= period;
        
        intervalsFromRoot.add(cents);
    }
    
    intervalsFromRoot.sort();
    intervalsFromRoot.add(period);
    
    // TODO: Finish rank-2 algorithm
    int note = rootNote - noteMin;
    int noteIndex = 0;
    
    int intervalTableOffset = rootNote % scaleSize;
    
    while (noteIndex - noteMin <= noteMax)
    {
        cents = 0;
        semitonesFromUnison.add(cents);
    }
}

Array<double> Tuning::getSemitoneTable() const
{
    return semitonesFromUnison;
}

Array<int> Tuning::getPitchBendTable() const
{
    return pitchBendTable;
}

int Tuning::getNoteMin() const
{
    return noteMin;
}

int Tuning::getPitchBendMax() const
{
    return pitchBendMax;
}

int Tuning::getNoteInSemitones(int midiNoteIn) const
{
    int noteIndex = midiNoteIn + noteMin;
    
    if (noteIndex >= noteMin && noteIndex < semitonesFromUnison.size())
        return semitonesFromUnison[noteIndex] * 100;
    
    return 0;
}

int Tuning::getNoteInCents(int midiNoteIn) const
{
    return getNoteInSemitones(midiNoteIn) * 100;
}

int Tuning::getNoteInPitchBend(int midiNoteIn) const
{
    int noteIndex = midiNoteIn + noteMin;

    if (noteIndex >= noteMin && noteIndex < semitonesFromUnison.size())
        return pitchBendTable[midiNoteIn];
    
    return 0;
}

int Tuning::getClosestNoteForSemitone(double semitoneIn, int& pitchBendReturn)
{
    int midiNoteOut = (int) (semitoneIn + rootMidiNote);
    double microtone = semitoneIn - floor(semitoneIn);
    
    if (microtone < 0.5)
    {
        pitchBendReturn = semitonesToPitchBend(microtone);
    }
    else
    {
        midiNoteOut++;
        pitchBendReturn = semitonesToPitchBend(1 - microtone);
    }
    
    return midiNoteOut;
}

int Tuning::getPitchBendAtMidiNote(int midiNoteIn)
{
    // TODO: implement root / destination frequency
    int destinationSemitone = midiNoteIn - destinationSemitone;
    int semitoneDifference = destinationSemitone - getNoteInSemitones(midiNoteIn);
    int pitchBend = semitonesToPitchBend(semitoneDifference);
    
    pitchBend = jlimit(0, 16834, pitchBend);
    
    return pitchBend;
}

MPEValue Tuning::getMPEValueOfNote(int midiNoteIn) const
{
    return MPEValue::from14BitInt(getNoteInPitchBend(midiNoteIn));
}

void Tuning::setPitchBendMax(int pitchBendMaxIn)
{
    pitchBendMax = pitchBendMaxIn;
    semitoneMax = pitchBendMax / 2;
}

int Tuning::semitonesToPitchBend(double semitonesIn)
{
    semitonesIn = jlimit((double) -semitoneMax, (double) semitoneMax, semitonesIn);
    int pitchBend = 8192 + (16834.0 / 48.0) * semitonesIn; // accuracy handling?
    
    return pitchBend;
}

double Tuning::pitchBendToSemitones(int pitchBendIn)
{
    pitchBendIn = jlimit(0, 16384, pitchBendIn);
    double semitones = (pitchBendIn - 8192) * 48 / 16834.0;
    return semitones;
}

int Tuning::ratioToPitchBend(double ratioIn)
{
    return semitonesToPitchBend(ratioToSemitones(ratioIn));
}

double Tuning::ratioToSemitones(double ratioIn)
{
    return log2(ratioIn) * 12.0;
}

double Tuning::ratioToCents(double ratioIn)
{
    return log2(ratioIn) * 1200.0;
}

double Tuning::centsToRatio(double centsIn)
{
    return pow(2, centsIn / 1200.0);
}
