/*
 ==============================================================================
 
 ViewPianoComponent.h
 Created: 14 Mar 2019 4:50:31pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../../../CommonFunctions.h"
#include "../../../PluginIDs.h"
#include "../../../MidiProcessor.h"
#include "../../../Structures/Mode.h"
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
        colorMode,
        mapMode
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
		public MidiKeyboardState,
        public MidiKeyboardStateListener // displaying external MIDI input
    {
        
    public:
        //===============================================================================================
        
        Keyboard(SvkMidiProcessor* midiProcessorIn);
		~Keyboard() {};
        
        //===============================================================================================
        
        void initiateDataNode();
        
        void restoreDataNode(ValueTree pianoNodeIn);

		void updatePianoNode();

		void updateKeyProperties();
        
        ValueTree getNode();
        
        //===============================================================================================
        
		Key* getKey(int keyNumIn);

		Array<Key*>* getKeysByOrder(int orderIn);

		Array<Key*> getKeysByScaleDegree(int scaleDegreeIn);

		int getLastKeyClicked();
        
        Point<int> getPositionOfKey(int midiNoteIn);
        
        Key* getKeyFromPosition(Point<int> posIn);
        
        Key* getKeyFromPosition(const MouseEvent& e);

        float getKeyVelocity(Key* keyIn, const MouseEvent& e);

		Mode* getMode();
        
		int getWidthFromHeight(int heightIn);

		//===============================================================================================

		int getUIMode();
        
        void updateKeyMidiNotes();

        void setUIMode(UIMode uiModeIn);
        
        void setKeyPlacement(KeyPlacementType placementIn);
        
        void setKeyProportions(Key* keyIn);

		// might want to restructure so this is not necessary
		void setLastKeyClicked(int keyNumIn);
        
        //===============================================================================================

        void selectKeyToMap(Key* keyIn);

        //===============================================================================================
        
        Colour getKeyColor(Key* keyIn);

		Colour getKeyOrderColor(int orderIn);

		Colour getKeyDegreeColor(int degIn);

		Colour getKeySingleColor(int keyIn);


		void setKeyColor(Key* keyIn, int colorIndex, Colour colorIn, bool useColor = true);
		
		void setKeyColor(int keyNumIn, int colorIndex, Colour colorIn, bool useColor = true);

		void setKeyColorOrder(int orderIn, int colorIndex, Colour colorIn);

		void setKeyColorDegree(int tuningDegreeIn, int colorIndex, Colour colorIn);

		void resetKeyOrderColors(int orderIn, bool resetDegrees = false);

		void resetKeyDegreeColors(int tuningDegreeIn);

		void resetKeySingleColor(int keyNumberIn);

		void resetKeyColors(bool resetDegrees=false);

		//===============================================================================================
        
        void applyMode(Mode* layoutIn);
                
        void allNoteOff();
        
        void isolateLastNote();
        
        void retriggerNotes();
        
        bool keysAreInSameOrder(int& orderDetected);
        
        Key* transposeKeyModally(Key* key, int stepsIn);
        
        Key* transposeKeyChromatically(Key* key, int stepsIn);
        
        bool transposeKeysOnModally(int modalStepsIn);
        
        void transposeKeysOnChromatically(int modalStepsIn);
        
        //===============================================================================================

		void paint(Graphics& g) override;

		void resized() override;

		//===============================================================================================

		void mouseMove(const MouseEvent& e) override;

		//===============================================================================================
        
        bool keyStateChanged(bool isKeyDown) override;
        
        bool keyPressed(const KeyPress& key) override;
        
        void modifierKeysChanged(const ModifierKeys& modifiers) override;
        
        //===============================================================================================
        
        void triggerKeyNoteOn(Key* key, float velocityIn);
        
        void triggerKeyNoteOff(Key* key);
        
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
		SvkMidiProcessor* midiProcessor;
		UndoManager* undo;
        
		// Functionality
        std::unique_ptr<KeyboardGrid> grid;
        MidiKeyboardState* keyboardState;
        MidiBuffer buffer;
		Array<Key*> keysPause;

		// Parameters
		int uiModeSelected = 0;
		int orientationSelected = 0;
		int keyPlacementSelected = 0;

		int midiChannelSelected = 1;
		int midiNoteOffset = 0;
		bool mpeOn = false;
        
        bool showNoteNumbers = false;
                
        // Data
        ValueTree pianoNode;
        OwnedArray<Key> keys;
        
        Array<Array<Key*>> keysOrder;
        Array<Key*> keysOn;
        Mode* mode;

		Array<int> keyMidiNoteMappings;
                
		Array<Colour> keyColorsOrder = { Colours::white, Colours::black, Colours::crimson.withSaturation(1.0f), Colours::cornflowerblue.withSaturation(0.8f), Colours::mediumseagreen.withSaturation(0.9f),
            Colours::gold.withBrightness(0.75f), Colours::mediumpurple, Colours::orangered, Colours::saddlebrown };
		
		Array<Colour> keyColorsDegree;
		Array<Colour> keyColorsSingle; // organized by (keyNumber - Mode offset)

		Array<Point<int>> keyPlacesOrder;
		Array<Point<int>> keyPlacesDegree;
		Array<Point<int>> keyPlacesSingle;

		Array<Point<int>> keyRatioOrder;
		Array<Point<int>> keyRatioDegree;
		Array<Point<int>> keyRatioSingle;
		        
        // Properties
        int keyWidth = 50;
        int keyHeight = 200;
        float defaultKeyWHRatio = 0.25f;

		int lastKeyOver = 0;
		int lastKeyClicked = 0;

		int currentOffset = 0;
        
        float pianoWidth;
        float minWindowHeight;
        
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
