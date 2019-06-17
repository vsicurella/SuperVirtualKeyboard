/*
  ==============================================================================

    MidiProcessor.h
    Created: 21 May 2019 6:37:57pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Structures/MidiFilter.h"
#include "CommonFunctions.h"

class SvkMidiProcessor : public MidiMessageCollector
{
    Array<String> inputList;
    Array<String> outputList;
    
    int inputSelected = 0;
    int outputSelected = 0;
    
    MidiInput* midiInput;
    MidiOutput* midiOutput;
    
    MidiBuffer midiBuffer;
    int msgCount = 0;
    
    int rootMidiNote = 60;

    std::unique_ptr<MidiFilter> midiInputFilter;
    std::unique_ptr<MidiFilter> midiOutputFilter;
    
    bool midiInputPaused = false;
    bool isInputRemapped = false;
    bool isOutputRemapped = false;
    bool autoRemap = true;
    
    std::unique_ptr<MidiKeyboardState> keyboardState; // used for displaying on VirtualKeyboard
    
public:
    
    SvkMidiProcessor();
    ~SvkMidiProcessor();
    
    ValueTree midiSettingsNode;
    ValueTree midiMapNode;
    
    void updateNode();
    bool restoreFromNode(ValueTree midiSettingsNodeIn);

	void resetWithRate(double sampleRateIn);
    
    StringArray getAvailableInputs();
    StringArray getAvailableOutputs();
    
    MidiInput* getInputDevice();
    MidiOutput* getOutputDevice();
    MidiKeyboardState* getKeyboardState();
    
    int getRootNote();

	NoteMap* getInputNoteMap();
	NoteMap* getOutputNoteMap();

	MidiFilter* getMidiInputFilter();
	MidiFilter* getMidiOutputFilter();

    int getInputNote(int midiNoteIn);
    int getOutputNote(int midiNoteIn);
    
    bool isAutoRemapping();
    
    String setMidiInput(int deviceIndex);
    String setMidiOutput(int deviceIndex);

    void setRootNote(int rootNoteIn);
    
    void setInputToRemap(bool doRemap=true);
    void setOutputToRemap(bool doRemap=true);
    
    void setMidiInputMap(Array<int> mapIn, bool updateNode=true);
	void setMidiInputMap(NoteMap mapIn, bool updateNode = true);
    void setMidiOutputMap(Array<int> mapIn, bool updateNode = true);
	void setMidiOutputMap(NoteMap mapIn, bool updateNode = true);
    
    void mapInputNote(int noteIn, int noteOut, bool updateNode = true);
    void mapOutputNode(int noteIn, int noteOut, bool updateNode = true);
    
    void resetInputMap(bool updateNode=true);
    void resetOutputMap(bool updateNode=true);

	void setAutoRemapOn(bool remapIn = true);
    
    void processMidi(MidiBuffer& midiMessages);
    
    void pauseMidiInput(bool setPaused=true);
    bool isMidiPaused();
    
    void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg) override;
};
