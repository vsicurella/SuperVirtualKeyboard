/*
  ==============================================================================

    SvkMPEChannelAssigner.cpp
    Created: 30 Nov 2019 8:45:21pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "ChannelAssigner.h"

SvkMpeChannelAssigner::SvkMpeChannelAssigner(MPEInstrument* mpeInstIn)
{
    mpeInst = mpeInstIn;
    channelStates.resize(17);
    channelStates.fill(MidiPitch(-1, 0));
}

SvkMpeChannelAssigner::~SvkMpeChannelAssigner()
{
    
}

int SvkMpeChannelAssigner::findMidiChannelForNote(const MidiPitch& newNote)
{
    int channel = getFreeOrExistingChannel(newNote);
    if (mpeInst->isMemberChannel(channel))
    {
        channelStates.set(channel, newNote);
        return channel;
    }

    return 0;
}

int SvkMpeChannelAssigner::noteOff(MidiPitch pitchOff)
{
    int channel = getChannelOfPitch(pitchOff);
    
    channelStates.set(channel, MidiPitch(-1, 0));
    return channel;
}

void SvkMpeChannelAssigner::allNotesOff()
{
    channelStates.fill(MidiPitch(-1, 0));
}

int SvkMpeChannelAssigner::getFreeOrExistingChannel(const MidiPitch& pitchIn) const
{
    // TODO: implement voice limit
    int pitchChannel = -1;
    int freeChannel = -1;
    int channelOut = -1;
    
    bool isSameNote = false;
    
    int channelMax = mpeInst->getZoneLayout().getLowerZone().getLastMemberChannel();
    
    for (int i = 1; i <= channelMax; i++)
    {
        const MidiPitch& pitch = channelStates.getReference(i);
        
        if (!mpeInst->isMemberChannel(i))
        {
            continue;
        }
        
        if (pitch.key < 0 && freeChannel < 0)
        {
            freeChannel = i;
        }
        
        isSameNote = pitchIn.key == pitch.key;
        
        //if (!ignorePitchbend)
        //{
            isSameNote *= pitchIn.value == pitch.value;
//        }
        
        if (isSameNote)
        {
            pitchChannel = i;
            break;
        }
    }
    
    if (pitchChannel > -1)
    {
        channelOut = pitchChannel;
    }
    else if (freeChannel > -1)
    {
        channelOut = freeChannel;
    }
    
    return channelOut;
}

int SvkMpeChannelAssigner::getChannelOfPitch(const MidiPitch& pitchIn) const
{
    // TODO: implement voice limit

    int pitchChannel = 0;
    bool isSameNote = false;

    int channelMax = mpeInst->getZoneLayout().getLowerZone().getLastMemberChannel();
    
    for (int i = 1; i <= channelMax; i++)
    {
        if (!mpeInst->isMemberChannel(i))
        {
            continue;
        }
        
        const MidiPitch& pitch = channelStates.getReference(i);
        
        isSameNote = pitchIn.key == pitch.key;
        
        if (!ignorePitchbend)
        {
            isSameNote *= pitchIn.value == pitch.value;
        }
        
        if (isSameNote)
        {
            pitchChannel = i;
            break;
        }
    }
    
    return pitchChannel;
}

Array<MidiChannelPitch> SvkMpeChannelAssigner::getAllNotesOn() const
{
    Array<MidiChannelPitch> pitchesOut;
    
    for (int i = 0; i < channelStates.size(); i++)
    {
        const MidiPitch& pitch = channelStates.getReference(i);
        if (pitch.key > -1)
        {
            pitchesOut.add(MidiChannelPitch(i, pitch));
        }
    }
    
    return pitchesOut;
}

int SvkMpeChannelAssigner::getVoiceLimit() const
{
    return voiceLimit;
}

int SvkMpeChannelAssigner::getNumNotesOn() const
{
    return getAllNotesOn().size();
}

bool SvkMpeChannelAssigner::isIgnoringPitchbend() const
{
    return ignorePitchbend;
}

void SvkMpeChannelAssigner::setVoiceLimit(int voiceLimitIn)
{
    voiceLimit = voiceLimitIn;
}

void SvkMpeChannelAssigner::setIgnorePitchbend(bool toIgnore)
{
    ignorePitchbend = toIgnore;
}
