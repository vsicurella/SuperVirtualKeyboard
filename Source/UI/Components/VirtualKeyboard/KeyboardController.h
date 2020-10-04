/*
  ==============================================================================

    KeyboardController.h
    Created: 1 Sep 2019 8:22:03pm
    Author:  Vincenzo

    Will be used for higher level keyboard controlling.

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "KeyboardComponent.h"
#include "../../../PluginIDs.h"

namespace VirtualKeyboard
{

    class SvkController : public MidiKeyboardStateListener
    {
    public:

        SvkController(Keyboard* keyboardIn);
        ~SvkController();

        ValueTree getNode();
        void restoreNode(ValueTree nodeIn);

        Keyboard* getKeyboard();
         
        void setInputSource(MidiKeyboardState* sourceIn);
        void setInputFilteredSource(MidiKeyboardState* sourceIn);


        // Keyboard Actions
        void triggerNoteOn(int midiChannel, int midiNote, float velocity);
        void triggerNoteOff(int midiChannel, int midiNote);
        void allNotesOff();

        // Interfacing
        void setMidiChannelOut(int midiChannelOutIn);
        void setNoteColor(int midiNoteIn, Colour colorIn);
        void setDegreeColor(int degreeIn, Colour colorIn);
        void setChannelTriggeredColor(int midiChannelIn, Colour colorIn);



        void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;
        void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;


    private:

        Keyboard* keyboard;

        MidiKeyboardState* input;
        MidiKeyboardState* inputFiltered;

        Array<Key> keyProperties;
        ValueTree keyNodes;

        bool showNoteNumbers = false;
        bool showNoteNames = false;
        bool showInput = true;
        bool showInputFiltered = false;

        Array<Colour> colorsDegrees;

        Array<Point<int>> offsetsByOrder;
        Array<Point<int>> offsetsByDegree;

        Array<Point<float>> ratiosByOrder;
        Array<Point<float>> ratiosByDegree;
    };
};