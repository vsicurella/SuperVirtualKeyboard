/*
  ==============================================================================

    FractionalGrid.h
    Created: 24 Mar 2019 1:58:51pm
    Author:  Vincenzo

    Wrapper class for setting preset data in a ValueTree.

    The general idea is you can load up to 128 modes into a preset, and this will allow you to
    maniuplate the mode slots and keeps track of which mode slots are loaded,
    with mode selectors 0 and 1 being the main ones (0 = input, 1 = output).

    A custom mode has it's own slot and there can only be one at a time. To take advantage of 
    mode slot features, the custom mode must be saved to file and loaded into a slot.

    Slot numbers and slot indicies do not need to match, and the slot indicies are only
    used internally

  ==============================================================================
*/

#pragma once

#include "../PluginIDs.h"
#include "../CommonFunctions.h"
#include "../Constants.h"
#include "../PluginModes.h"
#include "./NoteMap.h"
#include "./Mode.h"

class SvkPreset
{
    ValueTree parentNode;
    ValueTree thePropertiesNode;
    ValueTree theModeSlots;
    ValueTree theModeSelectors;
    ValueTree theCustomMode;
    ValueTree theKeyboardNode;
    ValueTree theMidiSettingsNode;
    ValueTree theMappingsNode;

public:

    SvkPreset();
    SvkPreset(const ValueTree presetNodeIn);
    SvkPreset(const SvkPreset& presetToCopy);
    ~SvkPreset();

    bool operator==(const SvkPreset& preset) const;
    bool operator!=(const SvkPreset& preset) const { return !operator==(preset); }

    /*
        Returns the parent node of the preset
        Will sort mode slots by slot number if called with true.
    */
    ValueTree getPresetNode(bool sortModeSlots = false);

    /*
        Returns the keyboard node of the preset
    */
    ValueTree getKeyboardNode();

    /*
        Returns the midi settings node of the preset
    */
    ValueTree getMidiSettingsNode();

    /*
        Returns the mappings node of the preset
    */
    ValueTree getMappingsNode();

    /*
        Recreate a preset from a ValueTree
    */
    bool restoreFromNode(ValueTree presetNodeIn, bool createCopy = false); // TODO: Revise default parameter

    /*
        Returns array of slot numbers in use
    */
    Array<int> getSlotNumbersInUse() const;

    /*
        Returns the mode selector that's currently in view
    */
    int getModeSelectorViewed() const;

    /*
        Returns true if given slot number is being used
    */
    bool isSlotNumberInUse(int modeSlotNumberIn) const;

    /*
        Returns the amount of modes loaded into slots
    */
    int getModeSlotsSize() const;

    /*
        Returns the slot number of the mode loaded by the given selector
    */
    int getSlotNumberBySelector(int selectorNumIn) const;

    /*
        Returns the slot number of the mode loaded by selector 0 (input keyboard)
    */
    int getMode1SlotNumber() const;

    /*
        Returns the slot number of the mode loaded by selector 1 (output keyboard)
    */
    int getMode2SlotNumber() const;

    /*
        Returns the mode slot number of the mode selector currently viewed
    */
    int getModeViewedSlotNumber() const;

    /*
        Return the root note of the given mode selector
    */
    int getModeSelectorRootNote(int modeSelectorNum) const;

    /*
        Returns the mode slots node
    */
    ValueTree getModeSlots();

    /*
        Returns the mode with the given slot number (which may be different from the index it's in)
    */
    ValueTree getModeInSlot(int slotNum);

    /*
        Returns the mode loaded in the given mode selector
    */
    ValueTree getModeBySelector(int selectorNumIn);

    /*
        Returns the mode loaded in selector 0 (input keyboard)
    */
    ValueTree getMode1();

    /*
        Returns the mode loaded in selector 1 (output keyboard)
    */
    ValueTree getMode2();

    /*
        Returns the mode currently being viewed
    */
    ValueTree getModeViewed();

    /*
        Returns the current custom mode
    */
    ValueTree getCustomMode();

    /*
        Finds the slot index of a given slot number, or returns -1 if not found
    */
    int getSlotNumberIndex(int slotNumIn);

    /*
        Sets the slot number used by the given selector, and returns the slot index
        If slot number doesn't exist, nothing will happen and this will return -1
    */
    int setModeSelectorSlotNum(int selectorNumIn, int slotNumIn);
    void setMode1SlotNumber(int slotNumIn);
    void setMode2SlotNumber(int slotNumIn);

    /*
        Sets the midi root note used by the mode selector
    */
    void setModeSelectorRootNote(int modeSelectorNumIn, int rootNoteIn);
    
    /*
        Loads the given mode into the given slot number, and returns the slot index
    */
    int setModeSlot(ValueTree modeNodeIn, int slotNumber);

    /*
        Puts the given mode in the smallest available free slot and returns the slot number
    */
    int addModeSlot(ValueTree modeNodeIn);

    /*
        Sets the custom mode and returns a reference to the mode node
    */
    ValueTree setCustomMode(ValueTree customModeNodeIn, bool createCopy = false);

    /*
        Empties the given mode slot and retunrs the slot index it was in
    */
    int removeModeSlot(int slotNumberIn);

    /*
        Replaces mode slots 1 & 2 with standard tuning, and removes other modes
        Also resets selectors 0 and 1 to respective mode slots
    */
    void resetModeSlots();

    /*
        Get the current midi input device name
    */
    juce::String getMidiInputName() const;

    /*
        Set the current midi input device
    */
    bool setMidiInputDevice(MidiDeviceInfo deviceInfo);

    /*
        Get the current midi output device name
    */
    juce::String getMidiOutputName() const;

    /*
        Set the current midi output device
    */
    bool setMidiOutputDevice(MidiDeviceInfo deviceInfo);

    /*
        Get the current period shift
    */
    int getPeriodShift() const;

    /*
        Set the current period shift
    */
    void setPeriodShift(int shiftAmt);

    /*
        Get period shift mode (scale or mode size)
    */
    bool periodShiftIsModeSize() const;

    /*
        Set period shift mode
    */
    void setPeriodShiftToModeSize(bool isModeSize);

    /*
        Get transpose amount
    */
    int getTransposeAmount() const;

    /*
        Set transpose amount
    */
    void setTransposeAmount(int steps);

    /*
        Get voice limit
    */
    int getVoiceLimit() const;

    /*
        Set voice limit
    */
    void setVoiceLimit(int maxVoices);

    /*
        Get MPE pitchbend range
    */
    int getMpePitchbendRange() const;
    
    /*
        Set MPE Pitchbend Range
    */
    void setMpePitchbendRange(int semitones);

    /*
        Get global pitchbend range
    */
    int getGlobalPitchbendRange() const;

    /*
        Set Global Pitchbend Range
    */
    void setGlobalPitchbendRange(int semitones);

    /*
        Get Midi Input mapping
    */
    NoteMap getMidiInputMap() const;

    /*
        Set MidiInput Mapping
    */
    void setMidiInputMap(const NoteMap& mapIn, bool updateNode, bool sendChangeMessage = true);

    /*
        Get Keyboard Highlight Style
    */
    VirtualKeyboard::HighlightStyle getKeyHighlightStyle() const;

    /*
        Set Keyboard Highlight Style
    */
    void setKeyHighlightStyle(VirtualKeyboard::HighlightStyle styleIn);

    /*
        Get Keyboard Placement Type
    */
    VirtualKeyboard::KeyPlacementType getKeyPlacementType() const;

    /*
        Set Keyboard Placement Type
    */
    void setKeyPlacementType(VirtualKeyboard::KeyPlacementType placementTypeIn);

    /*
        Get Key size ratio
    */
    float getKeySizeRatio() const;

    /*
        Set Key size ratio
    */
    void setKeySizeRatio(float keyRatio);

    /*
        Get whether or not note numbers are shown
    */
    bool areNoteNumbersShown() const;

    /*
        Set whether or not note numbres are shown
    */
    void showNoteNumbers(bool showNumbers);

    /*
        Returns a readable version of the parent node
    */
    String toString();

    static bool isValidPresetNode(ValueTree presetNodeIn);

    static SvkPreset getDefaultPreset();

public:

    class Listener
    {
    public:

        // When this preset's data gets replaced
        virtual void presetReloaded() = 0;

        virtual void modeSelectorSlotChanged(int selectorIndex, int slotIndex /*, ValueTree modeLoaded */) {}
        virtual void modeRootChanged(int selectorIndex, int newRootChannel, int newRootNote) {}
        virtual void customModeChanged() {}
        
        virtual void midiInputDeviceChanged(MidiDeviceInfo deviceInfo) {}
        virtual void midiOutputDeviceChanged(MidiDeviceInfo deviceInfo) {}
        virtual void periodShiftAmountChanged(int shiftAmount) {}
        virtual void periodShiftSizeChanged(bool isModeSize) {}
        virtual void transposeAmountChanged(int transposeAmount) {}
        virtual void voiceLimitChanged(int voiceLimit) {}
        virtual void mpePitchbendRangeChanged(int steps) {}
        virtual void globalPitchbendRangeChanged(int steps) {}
        virtual void midiInputMapChanged(const NoteMap* inputNoteMap) {}

        virtual void keyboardKeyPlacementTypeChanged(VirtualKeyboard::KeyPlacementType placementType) {}
        virtual void keyboardHighlightStyleChanged(VirtualKeyboard::HighlightStyle highlightStyle) {}
        virtual void keyboardKeyRatioChanged(float keyRatio) {}
        virtual void keyParametersShown(int keyParameters) {} // extension of note numbers shown

    };

    void addPresetListener(Listener* presetListenerIn) { listeners.add(presetListenerIn); }

    void removePresetListener(Listener* presetListenerIn) { listeners.remove(presetListenerIn); }

protected:

    ListenerList<Listener> listeners;

private:

    int getNextFreeSlotNumber();
    
private:

    Array<int> slotNumbersInUse;
};
