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
#include "../../Structures/Mode.h"
#include "VirtualKeyboardKey.h"
#include "VirtualKeyboardGrid.h"

#include <iostream>     


//==============================================================================
/*
 */

namespace VirtualKeyboard
{
    enum UIMode
    {
        playMode = 0,
        editMode
    };
    
    enum Orientation
    {
        horizontal = 0,
        verticalLeft,
        verticalRight
    };
    
    enum KeyPlacementType
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
        
		int getWidthFromHeight(int heightIn);

		//===============================================================================================

		void paint(Graphics& g) override;

		void resized() override;

		void visibilityChanged() override;
        
        //===============================================================================================

        void setUIMode(UIMode uiModeIn);
        
        void setKeyPlacement(KeyPlacementType placementIn);
        
        void setKeyProportions(Key* keyIn);

        //===============================================================================================
        
        Colour get_key_color(Key* keyIn);
        
        void apply_mode_layout(Mode* layoutIn);
                
        void all_notes_off();
        
        void isolate_last_note();
        
        void retrigger_notes();
        
        bool check_keys_modal(int& orderDetected);
        
        Key* transpose_key_modal(Key* key, int stepsIn);
        
        Key* transpose_key(Key* key, int stepsIn);
        
        bool transpose_keys_modal(int modalStepsIn);
        
        void transpose_keys(int modalStepsIn);
        
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
        
        void triggerKeyNoteOn(Key* key, float velocityIn);
        
        void triggerKeyNoteOff(VirtualKeyboard::Key* key);
        
        void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;
        
        void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;
        
        //===============================================================================================

		ApplicationCommandTarget* getNextCommandTarget() override;

		void getAllCommands(Array< CommandID > &commands) override;

		void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) override;

		bool perform(const InvocationInfo &info) override;

		//===============================================================================================       
        
    private:
        
		// Application pointers
        SuperVirtualKeyboardPluginState* pluginState;
        ApplicationCommandManager* appCmdMgr;
		UndoManager* undo;
        
		// Functionality
        std::unique_ptr<KeyboardGrid> grid;
        MidiKeyboardState keyboardState;
        MidiBuffer buffer;
        
        // Data
        ValueTree pianoNode;
        OwnedArray<Key> keys;
        
        std::vector<std::vector<Key*>> keysOrder;
        std::vector<Key*> keysOn;
        Mode* mode;
        
        // Parameters
        int uiModeSelected = 0;
        int orientationSelected = 0;
        int keyPlacementSelected = 0;
        
        int midiChannelSelected = 1;
        int midiNoteOffset = 0;
        bool mpeOn = false;
        
        Array<Point<float>> keyDegreeProportions;
        std::vector<Colour> keyOrderColors = { Colours::white, Colours::black, Colours::maroon, Colours::darkslateblue, Colours::forestgreen,
            Colours::darkgoldenrod, Colours::mediumpurple, Colours::orangered, Colours::saddlebrown };
        
        // Properties
        int keyWidth = 50;
        int keyHeight = 200;
        float defaultKeyWHRatio = 0.25;

		int lastKeyOver = 0;
		int lastKeyClicked = 0;
        
        float pianoWidth;
        float minWindowHeight;

		double resolution = 10e5;
        
        // Locks
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
