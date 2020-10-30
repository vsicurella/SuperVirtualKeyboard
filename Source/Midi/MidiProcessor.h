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

#include "../Structures/MidiFilter.h"
#include "../Structures/Mode.h"
#include "../Structures/MappingHelper.h"

class SvkMidiProcessor : public MidiInputCallback,
                         public MidiKeyboardStateListener
{
public:
    
    SvkMidiProcessor();
    ~SvkMidiProcessor();
    
    ValueTree midiSettingsNode;
    ValueTree midiMapNode;
    ValueTree midiDeviceNode;
    
    void updateNodes();
    bool restoreSettingsNode(ValueTree midiSettingsNodeIn);
    bool restoreMappingNode(ValueTree midiMapIn);
    bool restoreDevicesNode(ValueTree midiDevicesNodeIn);
    
    MidiInput* getInputDevice();
    MidiOutput* getOutputDevice();
    
    String getInputName() const;
    String getOutputName() const;
    
    MidiKeyboardState* getOriginalKeyboardState();
    MidiKeyboardState* getRemappedKeyboardState();
    
    int getPeriodShift() const;
    int getTransposeAmt() const;
    int getMidiChannelOut() const;

    NoteMap* getInputNoteFilter();
    NoteMap* getInputNoteRemap();
    NoteMap* getOutputNoteFilter();

    NoteMap getManualNoteMap() const;

    MidiFilter* getInputMidiFilter();
    MidiFilter* getInputRemapMidiFilter();
    MidiFilter* getOutputMidiFilter();
    
    int getInputNote(int midiNoteIn);
    int getOutputNote(int midiNoteIn);

    int getVoiceLimit() const;
    
    bool isRetuning() const;
    
    String setMidiInput(String deviceID);
    String setMidiOutput(String deviceID);

    void setModeViewed(Mode* modeViewedIn);
    void setMode1(Mode* mode1In);
    void setMode2(Mode* mode2In);

    void setInputToFilter(bool doRemap=true);
    void setInputToRemap(bool doRemap=true);
    void setOutputToFilter(bool doFilter=true);
    
    void setInputFilter(Array<int> mapIn, bool updateNode=true);
    void setInputFilter(NoteMap mapIn, bool updateNode=true);
    void setInputRemap(Array<int> mapIn, bool updateNode=true);
    void setInputRemap(NoteMap mapIn, bool updateNode = true);
    void setOutputFilter(Array<int> mapIn, bool updateNode = true);
    void setOutputFilter(NoteMap mapIn, bool updateNode = true);

    /*
        How many periods to shift notes.
        Notes held will be transposed for now.
    */
    void setPeriodShift(int periodsToShift);

    /*
        Sets the value to transpose all notes by
    */
    void setTransposeAmt(int transposeAmtIn);

    /*
        The MIDI channel for mouse-based virtual keyboard interaction
    */
    void setMidiChannelOut(int virtualKeyboardMidiChannelOut);

    /*
        Updates the note number transposition cache
    */
    void updateNoteTransposition();

    //void setVoiceLimit(int maxVoicesIn);
    //void setRetuneOn(bool retuneOn);

    void setMappingHelper(MappingHelper* helperIn);

    void setInManualMappingMode(bool manualModeOn);
    
    void resetInputFilter(bool updateNode=true);
    void resetInputMap(bool updateNode=true);
    void resetOutputFilter(bool updateNode=true);
    
    void processMidi(MidiBuffer& midiMessages);
    //MidiBuffer convertToMPE(const MidiBuffer& bufferToConvert);
    
    void sendMsgToOutputs(const MidiMessage& msgToSend);
    void sendBufferToOutputs(const MidiBuffer& bufferToSend);
    void allNotesOff();
    
    void pauseMidiInput(bool setPaused=true);
    bool isMidiPaused();
    
    //void parameterChanged(const String& paramID, float newValue) override;
    
    // Listen to UI input from VirtualKeyboard
    void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg) override;

private:

    std::unique_ptr<MidiInput> midiInput;
    std::unique_ptr<MidiOutput> midiOutput;

    String midiInputName = "";
    String midiOutputName = "";

    String inputSelected = "";
    String outputSelected = "";

    double startTime;

    MidiBuffer externalInputBuffer;
    int numInputMsgs = 0;

    MidiBuffer svkBuffer;
    int numSvkMsgs = 0;

    MidiBuffer directOutBuffer;
    int directMsgs = 0;

    MidiBuffer allNotesOffBuffer;

    Array<Array<int>> notesOnPerChannel;

    Mode* modeViewed;
    Mode* mode1;
    Mode* mode2;

    int periodShift = 0;
    bool periodShiftModeSize = false;
    int transposeAmt = 0;
    int currentNoteShift = 0;

    int midiChannelOut = 1;

    int maxNumVoices = 15;
    int pitchBendNoteMax = 48;
    int pitchBendGlobalMax = 2;

    bool mpeTuningPreserveMidiNote = true;

    std::unique_ptr<MidiFilter> midiInputFilter;
    std::unique_ptr<MidiFilter> midiInputRemap;
    std::unique_ptr<MidiFilter> midiOutputFilter;

    MappingHelper* manualMappingHelper;
    NoteMap currentRemap;
    NoteMap manualRemap;

    bool inManualMappingMode = false;

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
};
