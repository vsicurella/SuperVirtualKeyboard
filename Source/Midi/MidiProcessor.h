/*
  ==============================================================================

    MidiProcessor.h
    Created: 21 May 2019 6:37:57pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../CommonFunctions.h"

#include "ChannelAssigner.h"
#include "TunedNoteInterpreter.h"

#include "../Structures/MidiFilter.h"
#include "../Structures/Mode.h"
#include "../Structures/Tuning.h"

class SvkMidiProcessor : public MidiMessageCollector
{
    std::unique_ptr<MidiInput> midiInput;
    std::unique_ptr<MidiOutput> midiOutput;
    
    String midiInputName = "";
    String midiOutputName = "";
    
    String inputSelected = "";
    String outputSelected = "";
    
    MidiBuffer midiBuffer;
    int msgCount = 0;

	Mode* modeViewed;
	Mode* mode1;
	Mode* mode2;
    
    int rootMidiNote = 60;
    int midiChannelOut = 1;
    int periodShift = 0;
    int transposeAmt = 0;
    bool useModePeriod = false;
    
    int maxNumVoices = 15;
    int pitchBendNoteMax = 48;
    int pitchBendGlobalMax = 2;
    
    bool mpeTuningPreserveMidiNote = true;

    std::unique_ptr<MidiFilter> midiInputFilter;
    std::unique_ptr<MidiFilter> midiInputRemap;
    std::unique_ptr<MidiFilter> midiOutputFilter;
	Tuning* tuning = nullptr;

    std::unique_ptr<MPEInstrument> mpeInst;
    std::unique_ptr<TunedNoteInterpreter> retuner;
    std::unique_ptr<SvkMpeChannelAssigner> channelAssigner;
    MPEZoneLayout mpeZone;
    
    int mpePitchbendTrackingMode = 0;
    int mpePressureTrackingMode = 0;
    int mpeTimbreTrackingMode = 0;
    
    bool mpeOn = false; 	
    bool doRetuning = false;
    
    bool midiInputPaused = false;
    bool isInputFiltered = false;
    bool isInputRemapped = true;
    bool isOutputFiltered = false;
    
    bool noteMapIsCustom = false;
    
    std::unique_ptr<MidiKeyboardState> originalKeyboardState; // post-filtered input
    std::unique_ptr<MidiKeyboardState> remappedKeyboardState; // remapped input
    
public:
    
    SvkMidiProcessor();
    ~SvkMidiProcessor();
    
    ValueTree midiSettingsNode;
    ValueTree midiMapNode;
    
    void updateNode();
    bool restoreFromNode(ValueTree midiSettingsNodeIn);
    
    StringArray getAvailableInputs() const;
    Array<MidiDeviceInfo> getAvailableOutputs() const;
    
    MidiInput* getInputDevice();
    MidiOutput* getOutputDevice();
    
    String getInputName() const;
    String getOutputName() const;
    
    MidiKeyboardState* getOriginalKeyboardState();
    MidiKeyboardState* getRemappedKeyboardState();
    
    bool isAutoRemapping();

    int getRootNote() const;
    int getPeriodShift() const;
    int getTransposeAmt() const;
    int getMidiChannelOut() const;

    NoteMap* getInputNoteFilter();
	NoteMap* getInputNoteRemap();
	NoteMap* getOutputNoteFilter();

    MidiFilter* getInputMidiFilter();
	MidiFilter* getInputRemapMidiFilter();
	MidiFilter* getOutputMidiFilter();
    
    MPEInstrument* getMPEInstrument();

    int getInputNote(int midiNoteIn);
    int getOutputNote(int midiNoteIn);
    
    bool isMPEOn() const;
    bool isMPELegacyMode() const;
    int isTuningPreservesMidiNote() const;
    int getPitchbendNoteMax() const;
    
    int getPitchTrackingMode() const;
    int getPressureTrackingMode() const;
    int getTimbreTrackingMode() const;
    
    int getVoiceLimit() const;
    
    bool isRetuning() const;
    
    String setMidiInput(String deviceID);
    String setMidiOutput(String deviceID);

	void setModeViewed(Mode* modeViewedIn);
	void setMode1(Mode* mode1In);
	void setMode2(Mode* mode2In);
    
    void setRootNote(int rootNoteIn);
    void setMidiChannelOut(int channelOut);
	void setPeriodShift(int shiftIn);
    void setTransposeAmt(int notesToTranspose);
	void periodUsesModeSize(bool useMode);

    void setInputToFilter(bool doRemap=true);
    void setInputToRemap(bool doRemap=true);
    void setOutputToFilter(bool doFilter=true);
    
    void setMidiMaps(ValueTree midiMapIn);
    
    void setInputFilter(Array<int> mapIn, bool updateNode=true);
    void setInputFilter(NoteMap mapIn, bool updateNode=true);
    void setInputRemap(Array<int> mapIn, bool updateNode=true);
	void setInputRemap(NoteMap mapIn, bool updateNode = true);
    void setOutputFilter(Array<int> mapIn, bool updateNode = true);
	void setOutputFilter(NoteMap mapIn, bool updateNode = true);

    
    void setMPEOn(bool turnOnMPE);
    void setPitchBendNoteMax(int bendAmtIn);
    void setPitchBendGlobalMax(int bendAmtIn);
    void setTuningPreservesMidiNote(bool preserveMidiNote);
    
    void setPitchbendTrackingMode(int modeIn);
    void setPressureTrackingMode(int modeIn);
    void setTimbreTrackingMode(int modeIn);
    
    void setVoiceLimit(int maxVoicesIn);
    
    void setTuning(Tuning* tuningIn);
    void setRetuneOn(bool retuneOn);

    void mapNoteForInputFilter(int noteIn, int noteOut, bool updateNode = true);
    void mapNoteForInputRemap(int noteIn, int noteOut, bool updateNode = true);
    void mapNoteForOutputFilter(int noteIn, int noteOut, bool updateNode = true);
    
    void resetInputFilter(bool updateNode=true);
    void resetInputMap(bool updateNode=true);
    void resetOutputFilter(bool updateNode=true);
    
    void processMidi(MidiBuffer& midiMessages);
    MidiBuffer convertToMPE(const MidiBuffer& bufferToConvert);
    
    void sendMsgToOutputs(const MidiMessage& msgToSend);
    void sendBufferToOutputs(const MidiBuffer& bufferToSend);
    
    void pauseMidiInput(bool setPaused=true);
    bool isMidiPaused();
    
	// Listen to UI input from VirtualKeyboard
    void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg) override;
    
    //void noteOn(int midiChannel, int midiNoteNumber, MPEValue noteOnVelocity) override;
    //void noteOff(int midiChannel, int midiNoteNumber, MPEValue noteOffVelocity) override;
    
//    void noteAdded(MPENote newNote) override;
//    void notePressureChanged (MPENote changedNote) override;
//    void notePitchbendChanged (MPENote changedNote) override;
//    void noteTimbreChanged (MPENote changedNote) override;
//    void noteKeyStateChanged (MPENote changedNote) override;
//    void noteReleased (MPENote finishedNote) override;
};
