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

}

Tuning::Tuning(double period, double divisions, int rootNote, int noteMin, int noteMax)
{
    int note = noteMin - rootNote;
    int noteIndex = noteMin;
    double interval = ratioToSemitones(period) / divisions;
    double semitone;
    
    while(noteIndex <= noteMax)
    {
        semitone = interval * note++;
        semitonesFromUnison.add(semitone);
        noteIndex++;
        //DBG(String(noteIndex-1) + ": " + String(semitone) + "\tPB: " + String(semitonesToPitchBend(semitone)));
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

Tuning::~Tuning()
{
    
}

Array<double> Tuning::getSemitoneTable() const
{
    return semitonesFromUnison;
}

int Tuning::getNoteMin() const
{
    return noteMin;
}

int Tuning::getTuningBankSize() const
{
    return semitonesFromUnison.size();
}

int Tuning::getRootNote() const
{
    return rootMidiNote;
}

double Tuning::getRootFrequency() const
{
    return rootFrequency;
}

double Tuning::getNoteInSemitones(int midiNoteIn) const
{
    int noteIndex = midiNoteIn + noteMin;
    
    if (noteIndex >= noteMin && noteIndex < semitonesFromUnison.size())
        return semitonesFromUnison[noteIndex];
    
    return 0;
}

double Tuning::getNoteInCents(int midiNoteIn) const
{
    return getNoteInSemitones(midiNoteIn) * 100;
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