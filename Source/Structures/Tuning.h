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
    Array<int> pitchBendTable;
    
    Array<double> intervalsFromRoot;
    
    int noteMin = 0;
    int pitchBendMax = 48;
    int semitoneMax = 24;
    
    int midiNoteOffset = 0;
    
    int rootMidiNote = 60;     // note tuning is centered on
    double rootFrequency = 261.625565301; // frequency of root note

    int destinationRootMidi = 69; // note synth is centered on
    double destinationRootFrequency = 440; // frequency of root synth note
    
public:
    
    // unison tuning
    Tuning();
    
    // equal temperaments
    Tuning(double period, double divisions, int rootNote=60, int noteMin=0, int noteMax=127);
    
    // rank-2 regular temperaments
    Tuning(double period, double generator, int scaleSize, int stepsDown=0, int rootNote=60, int noteMin=0, int noteMax=127);
    
    Tuning(Array<double>& tuningInCents);
    Tuning(Array<int>& tuningInPitchBend);
    ~Tuning();
    
    int getNoteMin() const;
    int getPitchBendMax() const;
    
    // maybe add return type for noteMin?
    Array<double> getSemitoneTable() const;
    Array<int> getPitchBendTable() const;
    
    int getNoteInSemitones(int midiNoteIn) const;
    int getNoteInCents(int midiNoteIn) const;
    int getNoteInPitchBend(int midiNoteIn) const;
    MPEValue getMPEValueOfNote(int midiNoteIn) const;
    
    /*
        Given a rooted semitone interval, pass in an int to receive a pitch bend value
        and returns a midi note corresponding to the closest representation relative to
        destination synth tuning
     */
    int getClosestNoteForSemitone(double semitoneIn, int& pitchBendReturn);
    
    /*
        Given a midi note this returns a pitch bend value needed to tune this note
        from the same midi note in the destination synth tuning
     */
    int getPitchBendAtMidiNote(int midiNoteIn);
    
    int getRootNote() const;
    double getRootFrequency() const;
    
    int getDestinationRootNote() const;
    int getDestinationRootFrequency() const;
    
    void setPitchBendMax(int pitchBendMaxIn);
    
    void changeRootNote(int rootNoteIn);
    void setRootFrequency(double freqIn);
    
    void setDestinationRootNote(int rootNoteIn);
    void getDestinationRootFrequency(double freqIn);
    
    int semitonesToPitchBend(double semitonesIn);
    double pitchBendToSemitones(int pitchBendIn);
    int ratioToPitchBend(double ratioIn);
    
    static double ratioToSemitones(double ratioIn);
    static double ratioToCents(double ratioIn);
    static double centsToRatio(double centsIn);
    
    static int getSemitoneOfStd(int midiNoteIn);
    
};
