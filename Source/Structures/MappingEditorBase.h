/*
  ==============================================================================

    MappingEditorBase.h
    Created: 18 Oct 2020 8:32:13pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "NoteMap.h"

class MappingEditor
{
public:

    MappingEditor() {};

    virtual NoteMap getCurrentNoteMapping() const = 0;

    virtual void mapMidiNoteToKey(int midiNoteIn, int keyNumberOut) = 0;

    virtual void resetMapping(NoteMap mappingIn, bool sendMessage) = 0;

public:

    class Listener
    {
    public:

        virtual void keyMappingStatusChanged(int keyNumber, bool preparedToMap) {};

        virtual void keyMapConfirmed(int keyNumber, int midiNote) {};

        virtual void mappingEditorChanged(NoteMap& newMapping) {};
    };

    void addListener(Listener* listenerIn) { listeners.add(listenerIn); }

    void removeListener(Listener* listenerIn) { listeners.remove(listenerIn); }

    void removeAllListeners() { listeners.clear(); }

protected:

    ListenerList<Listener> listeners;
};