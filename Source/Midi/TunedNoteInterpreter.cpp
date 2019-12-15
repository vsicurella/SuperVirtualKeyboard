/*
  ==============================================================================

    TunedNoteInterpreter.cpp
    Created: 30 Nov 2019 5:11:32pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "TunedNoteInterpreter.h"

TunedNoteInterpreter::TunedNoteInterpreter(Tuning* tuningToUse)
{
    tuning = tuningToUse;
}

TunedNoteInterpreter::~TunedNoteInterpreter()
{
    
}

void TunedNoteInterpreter::resetTuning(Tuning* newTuning)
{
    tuning = newTuning;
}

Array<int> TunedNoteInterpreter::getPitchBendTable() const
{
    return pitchBendTable;
}

int TunedNoteInterpreter::getPitchBendMax() const
{
    return pitchBendMax;
}

int TunedNoteInterpreter::getTuningRootMidiNote() const
{
    return tuning->getRootNote();
}

double TunedNoteInterpreter::getTuningRootFrequency() const
{
    return tuning->getRootFrequency();
}

int TunedNoteInterpreter::getDestinationRootMidiNote() const
{
    return destinationRootMidi;
}

double TunedNoteInterpreter::getDestinationRootFrequency() const
{
    return destinationRootFrequency;
}

int TunedNoteInterpreter::getPitchbendFromStdMidiNote(int midiNoteIn) const
{
    int noteIndex = midiNoteIn + tuning->getNoteMin();

    if (noteIndex >= tuning->getNoteMin() && noteIndex < tuning->getTuningBankSize())
        return pitchBendTable[midiNoteIn];
    
    return 0;
}

void TunedNoteInterpreter::setPitchBendMax(int pitchBendMaxIn)
{
    pitchBendMax = pitchBendMaxIn;
    semitoneMax = pitchBendMax / 2;
}

void TunedNoteInterpreter::setDestinationRootNote(int rootNoteIn)
{
    destinationRootMidi = rootNoteIn;
}

void TunedNoteInterpreter::setDestinationRootFrequency(double freqIn)
{
    destinationRootFrequency = freqIn;
}

MidiPitch TunedNoteInterpreter::calculateClosestMidiPitchForSemitone(double semitoneIn)
{
    int midiNoteOut = (int) (semitoneIn + tuning->getRootNote());
    double microtone = semitoneIn - floor(semitoneIn);
    int pitchBend = 0;
    
    if (microtone < 0.5)
    {
        pitchBend = semitonesToPitchBend(microtone);
    }
    else
    {
        midiNoteOut++;
        pitchBend = semitonesToPitchBend(1 - microtone);
    }
    
    return MidiPitch(midiNoteOut, pitchBend);
}

int TunedNoteInterpreter::calculatePitchbendFromStdMidiNote(int midiNoteIn)
{
    // TODO: implement root / destination frequency

    double semitoneDifference = calculateDifferenceInSemitonesFromStd(midiNoteIn);
    int pitchBend = semitonesToPitchBend(semitoneDifference);
    
    pitchBend = jlimit(0, 16383, pitchBend);
    
    return pitchBend;
} 

double TunedNoteInterpreter::calculateDifferenceInSemitonesFromStd(int midiNoteIn)
{
    int destinationSemitone = midiNoteIn - 60;
    double semitoneDifference =  tuning->getNoteInSemitones(midiNoteIn) - destinationSemitone;
    
    return semitoneDifference;
}
int TunedNoteInterpreter::semitonesToPitchBend(double semitonesIn)
{
    semitonesIn = jlimit((double) -semitoneMax, (double) semitoneMax, semitonesIn);
    int pitchBend = 8192 + ((16383.0 / pitchBendNoteMax) * semitonesIn); // accuracy handling?
    
    return pitchBend;
}

double TunedNoteInterpreter::pitchBendToSemitones(int pitchBendIn)
{
    pitchBendIn = jlimit(0, 16383, pitchBendIn);
    double semitones = (pitchBendIn - 8192) * pitchBendNoteMax / 16383.0;
    return semitones;
}

int TunedNoteInterpreter::ratioToPitchBend(double ratioIn)
{
    return semitonesToPitchBend(Tuning::ratioToSemitones(ratioIn));
}

int TunedNoteInterpreter::getSemitoneOfStd(int midiNoteIn)
{
    return midiNoteIn - 60;
}
