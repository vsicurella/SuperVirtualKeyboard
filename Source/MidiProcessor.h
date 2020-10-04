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
    
    float* midiChannelOut;
    float* periodShift;
    float* transposeAmt;
    bool* useModePeriod;

    std::unique_ptr<MidiFilter> midiInputFilter;
    std::unique_ptr<MidiFilter> midiOutputFilter;
    
    std::unique_ptr<MPEInstrument> mpeInst;
    
    bool mpeOn = false;
    bool mpeThru = false;
    bool mpeLegacyOn = false;
    MPEZoneLayout mpeZone;
    
    int pitchBendGlobalMax = 2;
    int pitchBendNoteMax = 48;
    int mpePitchTrackingMode = 0;
    int mpePressureTrackingMode = 0;
    int mpeTimbreTrackingMode = 0;
    
    bool midiInputPaused = false;
    bool isInputRemapped = true;
    bool isOutputRemapped = true;
    
    bool inputMapIsCustom = false;
    bool outputMapIsCustom = false;
    
    std::unique_ptr<MidiKeyboardState> originalKeyboardState; // used for sending unchanged MIDI messages
    std::unique_ptr<MidiKeyboardState> remappedKeyboardState; // used for displaying on VirtualKeyboard
    
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
    
    int getRootNote() const;
    int getPeriodShift() const;
    int getTransposeAmt() const;
    int getMidiChannelOut() const;

    NoteMap* getInputNoteMap();
    NoteMap* getOutputNoteMap();

    MidiFilter* getMidiInputFilter();
    MidiFilter* getMidiOutputFilter();

    int getInputNote(int midiNoteIn);
    int getOutputNote(int midiNoteIn);
    
    bool isMPEOn() const;
    bool isMPELegacyMode() const;
    bool isMPEThru() const;
    MPEZoneLayout getMPEZone() const;

    int getPitchBendNoteMax() const;
    int getPitchBendGlobalMax() const;
    int getPitchTrackingMode() const;
    int getPressureTrackingMode() const;
    int getTimbreTrackingMode() const;
    
    bool isAutoRemapping();
    
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
    
    void setMPEOn(bool turnOnMPE);
    void setMPEThru(bool mpeOnThru);
    void setMPELegacy(bool turnOnLegacy);
    void setMPEZone(MPEZoneLayout zoneIn);
    
    void setPitchBendNoteMax(int bendAmtIn);
    void setPitchBendGlobalMax(int bendAmtIn);
    void setPitchTrackingMode(int modeIn);
    void setPressureTrackingMode(int modeIn);
    void setTimbreTrackingMode(int modeIn);
    
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
    
    void processMidi(MidiBuffer& midiMessages);
    void sendMsgToOutputs(const MidiMessage& bufferToSend);
    
    void pauseMidiInput(bool setPaused=true);
    bool isMidiPaused();
    
    // Listen to UI input from VirtualKeyboard
    void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg) override;
};
