/*
 ==============================================================================
 
 ViewPianoComponent.h
 Created: 14 Mar 2019 4:50:31pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../PluginState.h"
#include "../../PluginIDs.h"
#include "../../Structures/ModeLayout.h"
#include "VirtualKeyboardKey.h"
#include "VirtualKeyboardGrid.h"

#include <iostream>     


//==============================================================================
/*
 */

namespace VirtualKeyboard
{
    enum PianoMode
    {
        playMode = 0,
        editMode
    };
    
    enum PianoOrientation
    {
        horizontal = 0,
        verticalLeft,
        verticalRight
    };
    
    enum KeyPlacement
    {
        nestedRight = 0,
        nestedCenter,
        adjacent
    };
    
    class Keyboard :
        public Component,
        public ApplicationCommandTarget,
        public MidiKeyboardStateListener // Only for displaying external MIDI input
    {
    public:
        //===============================================================================================
        
        Keyboard(SuperVirtualKeyboardPluginState* pluginStateIn, ApplicationCommandManager* cmdMgrIn);
        ~Keyboard() {};
        
        //===============================================================================================
        
        void init_data_node();
        
        void restore_data_node(ValueTree pianoNodeIn);
        
        ValueTree get_node();
        
        //===============================================================================================
        
        MidiKeyboardState* get_keyboard_state();
        
        Point<int> get_position_of_key(int midiNoteIn);
        
        VirtualKeyboard::Key* get_key_from_position(Point<int> posIn);
        
        VirtualKeyboard::Key* get_key_from_position(const MouseEvent& e);
        
        float get_velocity(Key* keyIn, const MouseEvent& e);
        
        int get_min_height();
        
        //===============================================================================================
        
        Colour get_key_color(Key* keyIn);
        
        void apply_mode_layout(ModeLayout* layoutIn);
        
        void all_notes_off();
        
        void isolate_last_note();
        
        void retrigger_notes();
        
        bool check_keys_modal(int& orderDetected);
        
        Key* transpose_key_modal(Key* key, int stepsIn);
        
        Key* transpose_key(Key* key, int stepsIn);
        
        bool transpose_keys_modal(int modalStepsIn);
        
        void transpose_keys(int modalStepsIn);
        
        //===============================================================================================
        
        ApplicationCommandTarget* getNextCommandTarget() override;
        
        void getAllCommands(Array< CommandID > &commands) override;
        
        void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) override;
        
        bool perform(const InvocationInfo &info) override;
        
        //===============================================================================================
        
        
        void mouseExit(const MouseEvent& e) override;
        
        void mouseDown(const MouseEvent& e) override;
        
        void mouseDrag(const MouseEvent& e) override;
        
        void mouseUp(const MouseEvent& e) override;
        
        void mouseMove(const MouseEvent& e) override;
        
        //===============================================================================================
        
        bool keyStateChanged(bool isKeyDown) override;
        
        bool keyPressed(const KeyPress& key) override;
        
        void modifierKeysChanged(const ModifierKeys& modifiers) override;
        
        //===============================================================================================
        
        
        void paint(Graphics& g) override;
        
        void resized() override;
        
        void visibilityChanged() override;
        
        //===============================================================================================
        
        void triggerKeyNoteOn(Key* key, float velocityIn);
        
        void triggerKeyNoteOff(VirtualKeyboard::Key* key);
        
        void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;
        
        void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;
        
        //===============================================================================================
        
        
    private:
        
        SuperVirtualKeyboardPluginState* pluginState;
        ApplicationCommandManager* appCmdMgr;
        UndoManager* undo;
        
        KeyboardGrid grid;
        MidiKeyboardState keyboardState;
        MidiBuffer buffer;
        
        int lastKeyOver = 0;
        int lastKeyClicked = 0;
        
        // Data
        ValueTree pianoNode;
        OwnedArray<Key> keys;
        std::vector<Key*> keysPtr;// debug
        
        std::vector<std::vector<Key*>> keysOrder;
        std::vector<Key*> keysOn;
        ModeLayout* modeLayout;
        
        // Parameters
        Array<int> scaleLayout;
        Array<int> scaleOrder;
        String defaultMOS = "2 2 1 2 2 2 1";
        
        int tuningSize;
        int notesToShow;
        int rows;
        
        int pianoModeSelected = 1;
        int pianoOrientationSelected = 1;
        
        int midiChannelSelected = 1;
        int midiNoteOffset = 0;
        bool mpeOn = false;
        
        std::unique_ptr<std::vector<std::vector<int>>> keyOrdersRatios;
        std::vector<Colour> keyOrderColors = { Colours::white, Colours::black, Colours::maroon, Colours::darkslateblue, Colours::forestgreen,
            Colours::darkgoldenrod, Colours::mediumpurple, Colours::orangered, Colours::saddlebrown };
        
        // Properties
        
        int modeSize;
        int modeOrder;
        int modalKeysSize;
        
        int keyWidth = 50;
        int keyHeight = 200;
        float defaultKeyWHRatio = 0.25;
        
        float pianoWidth;
        float minWindowHeight;
        
        // Key locks
        bool rightMouseHeld = false;
        bool shiftHeld = false;
        bool altHeld = false;
        bool ctrlHeld = false;
        bool upHeld = false;
        bool downHeld = false;
        bool leftHeld = false;
        bool rightHeld = false;
        bool spaceHeld = false;
        
        bool displayIsReady = false;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Keyboard)
    };
}
