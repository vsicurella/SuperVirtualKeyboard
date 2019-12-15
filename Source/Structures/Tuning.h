/*
  ==============================================================================

    Tuning.h
    Created: 26 Nov 2019 9:16:30pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../CommonFunctions.h"
#include "Mode.h"

class Tuning
{
    Array<double> semitonesFromUnison;
    Array<double> intervalsFromRoot;
    
    int noteMin = 0;
    int midiNoteOffset = 0;
    
    int rootMidiNote = 60;     // note tuning is centered on
    double rootFrequency = 261.625565301; // frequency of root note
    
public:
    
    // unison tuning
    Tuning();
    
    // equal temperaments
    Tuning(double period, double divisions, int rootNote=60, int noteMin=0, int noteMax=127);
    
    // rank-2 regular temperaments
    Tuning(double period, double generator, int scaleSize, int stepsDown=0, int rootNote=60, int noteMin=0, int noteMax=127);
    
    Tuning(Array<double>& tuningInCents);
    ~Tuning();
    
    int getNoteMin() const;
    int getTuningBankSize() const;
    
    // maybe add return type for noteMin?
    Array<double> getSemitoneTable() const;
    
    double getNoteInSemitones(int midiNoteIn) const;
    double getNoteInCents(int midiNoteIn) const;
    
    int getRootNote() const;
    double getRootFrequency() const;
    
    void changeRootNote(int rootNoteIn);
    void setRootFrequency(double freqIn);
    
    static double ratioToSemitones(double ratioIn);
    static double ratioToCents(double ratioIn);
    static double centsToRatio(double centsIn);
};
