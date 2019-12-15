/*
  ==============================================================================

    TunedNoteInterpreter.h
    Created: 30 Nov 2019 5:11:32pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../CommonFunctions.h"
#include "../Structures/Tuning.h"

class TunedNoteInterpreter
{
    Array<int> pitchBendTable;
    Tuning* tuning;
    
    int destinationRootMidi = 69; // note synth is centered on
    double destinationRootFrequency = 440; // frequency of root synth note
    
    int pitchBendMax = 48;
    int semitoneMax = 24;
    
    bool mpeOn = false;
    MPEZoneLayout mpeZone;

    int mpePitchbendTrackingMode = 0;
    int mpePressureTrackingMode = 0;
    int mpeTimbreTrackingMode = 0;
    
    int pitchBendNoteMax = 48;
    int pitchBendGlobalMax = 2;

public:
    
    TunedNoteInterpreter(Tuning* tuningToUse=nullptr);
    ~TunedNoteInterpreter();
    
    void resetTuning(Tuning* newTuning);
    
    Array<int> getPitchBendTable() const;

    int getPitchBendMax() const;
    
    int getTuningRootMidiNote() const;
    double getTuningRootFrequency() const;
    
    int getDestinationRootMidiNote() const;
    double getDestinationRootFrequency() const;

    int getPitchbendFromStdMidiNote(int midiNoteIn) const;
    
    void setPitchBendMax(int pitchBendMaxIn);
    
    void setDestinationRootNote(int rootNoteIn);
    void setDestinationRootFrequency(double freqIn);

    /*
        Given a rooted semitone interval, pass in an int to receive a pitch bend value
        and returns a midi note corresponding to the closest representation relative to
        destination synth tuning
     */
    MidiPitch calculateClosestMidiPitchForSemitone(double semitoneIn);
    
    /*
        Given a midi note this returns a pitch bend value needed to tune this note
        from the same midi note in the destination synth tuning
     */
    int calculatePitchbendFromStdMidiNote(int midiNoteIn);

    double calculateDifferenceInSemitonesFromStd(int midiNoteIn);
    
    int semitonesToPitchBend(double semitonesIn);
    double pitchBendToSemitones(int pitchBendIn);
    int ratioToPitchBend(double ratioIn);
    
    static int getSemitoneOfStd(int midiNoteIn);
};
