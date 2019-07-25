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
#include "Structures/Mode.h"

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

	Mode* modeViewed;
	Mode* mode1;
	Mode* mode2;
    
    int rootMidiNote = 60;

    int periodShift = 0;
	bool useModeSize = false;
    
	int midiChannelOut = 1;

    std::unique_ptr<MidiFilter> midiInputFilter;
    std::unique_ptr<MidiFilter> midiOutputFilter;
    
    bool midiInputPaused = false;
    bool isInputRemapped = true;
    bool isOutputRemapped = true;
    bool setAutoMap = true;
    
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
    int getPeriodShift();
    int getMidiChannelOut();

	NoteMap* getInputNoteMap();
	NoteMap* getOutputNoteMap();

	MidiFilter* getMidiInputFilter();
	MidiFilter* getMidiOutputFilter();

    int getInputNote(int midiNoteIn);
    int getOutputNote(int midiNoteIn);
    
    bool isAutoRemapping();
    
    String setMidiInput(int deviceIndex);
    String setMidiOutput(int deviceIndex);

	void setModeViewed(Mode* modeViewedIn);
	void setMode1(Mode* mode1In);
	void setMode2(Mode* mode2In);
    
    void setRootNote(int rootNoteIn);
    
	void setPeriodShift(int shiftIn);
	void periodUsesModeSize(bool useMode);

    void setMidiChannelOut(int channelOut);
    
    void setInputToRemap(bool doRemap=true);
    void setOutputToRemap(bool doRemap=true);
    
    void setMidiMaps(ValueTree midiMapIn);
    
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
