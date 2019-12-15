/*
  ==============================================================================

    SvkMPEChannelAssigner.h
    Created: 30 Nov 2019 8:45:21pm
    Author:  Vincenzo Sicurella
 
    This class is used to separate MIDI Data so that there is only a single
    MIDI note per channel, within the provided MPEInstrument.
    The advantage of this over the JUCE MPEChannelAssigner is that this explicitly only
    allows one note per channel plus notes are differentiated not just by MIDI note number
    but also pitchbend data. This allows the triggering of the same MIDI note with different
    pitchbend data across different channels.

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../CommonFunctions.h"

// Midi Channel with corresponding MidiPitch
typedef Pair<int, MidiPitch> MidiChannelPitch;

class SvkMpeChannelAssigner
{
    MPEInstrument* mpeInst;
    Array<MidiPitch> channelStates;
    
    int voiceLimit = -1;
    int numNotesOn = 0;
    
    bool ignorePitchbend = false;
    
    int getFreeOrExistingChannel(const MidiPitch& pitchIn) const;
    
public:
    
    SvkMpeChannelAssigner(MPEInstrument* mpeInstIn);
    ~SvkMpeChannelAssigner();
    
    /*
        Returns an available channel number for a given MidiPitch.
        Will return a used channel if an identical MidiPitch exists.
     */
    int findMidiChannelForNote(const MidiPitch& newNote);
    
    /*
        Will return the channel number the MidiPitch exists on.
     */
    int noteOff(MidiPitch pitchOff);
    int noteOff(int midiNoteNumber);
    
    void allNotesOff();
    
    int getChannelOfPitch(const MidiPitch& pitchIn) const;
    
    Array<MidiChannelPitch> getAllNotesOn() const;
    
    int getVoiceLimit() const;
    
    int getNumNotesOn() const;
    
    bool isIgnoringPitchbend() const;
    
    void setVoiceLimit(int voiceLimitIn);

    void setIgnorePitchbend(bool toIgnore);
};
