/*
 ==============================================================================
 
 ViewPianoComponent.h
 Created: 14 Mar 2019 4:50:31pm
 Author:  Vincenzo

 A keyboard where keys can be rearranged and other stylization options.
 
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
        editMode,
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
        nestedRight = 1,
        nestedCenter,
		flat,
        adjacent
    };

	enum HighlightStyle
	{
		full = 1,
		inside,
		outline,
		circles,
		squares
	};

	enum VelocityStyle
	{
		linear = 1,
		curved,
		fixed
	};

	enum ScrollingStyle
	{
		smooth = 1,
		stepped
	};
    
    class Keyboard :
        public Component,
		public MidiKeyboardState,
        public MidiKeyboardStateListener,
		private Timer
    {
        
    public:
        //===============================================================================================
        
        Keyboard(MidiKeyboardState& keyboardStateIn);
		Keyboard(MidiKeyboardState& keyboardStateIn, ValueTree keyboardNodeIn);
		~Keyboard() {};
        
        //===============================================================================================
                
        void restoreNode(ValueTree keyboardNodeIn);
        
        ValueTree getNode();
        
        //===============================================================================================

		/*
			Rearranges the keys to fit the current mode, without changing other key data.
		*/
        void applyMode(Mode* modeIn, int newRootNote=-1);

		/*
			Applies the key data to the keyboard so that it matches the data passed in.
		*/
		void applyKeyData(ValueTree keyDataNodeIn);
        
        //===============================================================================================
        
		/*
			Returns a struct with the given key's data.
		*/
		Key* getKey(int keyNumIn);

		/*
			Returns a value tree with the given key's data.
		*/
		ValueTree getKeyNode(int keyNumIn);

		/*
			Returns an array of pointers to the keys corresponding to the given integer array.
		*/
		Array<Key*> getKeysByOrder(Array<int> keyNumsIn);

		/*
			Returns the number of the last key clicked by the user.
		*/
		int getLastKeyClicked();
        
		/*
			Returns the area of the given key relative to the top left corner of the full keyboard.
		*/
        Rectangle<int> getKeyArea(int midiNoteIn);

		/*
			Returns the area of the given key relative to the top left corner of the viewport.
		*/
		Rectangle<int> getKeyAreaRelative(int midiNoteIn);

		/*
		Returns the current proportion of key width to height.
		*/
		float getKeySizeRatio(int keyNumIn);

		/*
		Returns the current size of the keys within the given order.
		*/
		Point<int> getKeyOrderSize(int orderIn);

		/*
		Returns the current size of the keys within the given scale degree.
		*/
		Point<int> getKeyDegreeSize(int degreeIn);

        /*
			Returns the key which holds the given point relative to the top left corner of the component.
		*/
        Key* getKeyFromPosition(Point<int> posIn);
        
		/*
			Returns the key which holds the given point relative to the top left corner of the viewport.
		*/
        Key* getKeyFromPositionRelative(Point<int> posIn);

		/*
			Returns the velocity for a given key and point relative to the key's top left corner
		*/
        float getKeyVelocity(Key* keyIn, Point<int> posIn);

		/*
			Returns the Midi Channel the keyboard is sending output on.
		*/
		int getMidiChannelOut();
        
		//===============================================================================================

		/*
			Returns the UI mode of which the keyboard is in.
		*/
		int getUIMode();

		/*
			Returns the style of which the keys are nested.
		*/
		int getKeyPlacementStyle();

		/*
			Returns whether or not note numbers are showing
		*/
		int isShowingNoteNumbers();

		/*
			Returns the style of which the keys get highlighted.
		*/
		int getHighlightStyle();

		/*
			Returns the style of note velocity.
		*/
		int getVelocityStyle();

		/*
			Returns whether or not Midi input is being scaled to the selected velocity style
		*/
		bool isInputVelocityScaled();

		/*
			Returns the scrolling style selected
		*/
		int getScrollingStyle();

		//===============================================================================================
        
		/*
			Sets the keyboards UI mode.
		*/
        void setUIMode(UIMode uiModeIn);
        
		/*
			Sets the style of which the keys are nested in.
		*/
        void setKeyPlacementStyle(int placementIn);
        

		// might want to restructure these so this is not necessary
        void setKeyProportions(Key* keyIn);
		void setLastKeyClicked(int keyNumIn);

		/*
			Set whether note numbers are showing
		*/
		void setNoteNumbersVisible(bool showNoteNumsIn);

		/*
			Set the way the keys are highlighted
		*/
		void setHighlightStyle(int styleIn);

		/*
			Set the Midi Channel that the keyboard outputs on clicks.
		*/
		void setMidiChannelOut(int midiChannelOutIn);

		/*
			Sets the velocity behavior of the keyboard
		*/
		void setVelocityBehavior(int behaviorNumIn, bool scaleInputVelocity=false);

		/*
			Set the fixed output velocity
		*/
		void setVelocityFixed(float velocityIn);

		/*
			Sets the scrolling style of the keyboard.
		*/
		void setScrollingStyle(int scrollingStyleIn);

		/*
			Allows the keyboard to listen the filtered midi input
		*/
		void setAndListenToFilteredInput(const MidiKeyboardState& filteredInputStateIn);
        
        //===============================================================================================

		/*
			Flags a key to get mapped to the next Midi Note triggered
		*/
        void selectKeyToMap(Key* keyIn, bool mapAllPeriods=false);
        
		/*
			Hightlights the given key. If a transparent color is given, it uses the default hightlight color.
			If a blink rate above 0 is given, the key will alternate between default and highlight color every 
			given number of milliseconds.
		*/
		void highlightKey(int keyNumberIn, Colour colorIn=Colours::transparentBlack, int blinkRateMs = 0);

		/*
			Hightlights the given keys. If a transparent color is given, it uses the default hightlight colors.
			If a blink rate above 0 is given, the keys will alternate between default and highlight color every
			given number of milliseconds.
		*/
		void hightlightKeys(Array<int> keyNumsIn, Colour colorIn = Colours::transparentBlack, int blinkRateMs = 0);

        //===============================================================================================
        
		/*
			Returns the color of the given key.
		*/
        Colour getKeyColor(int keyNumIn);

		/*
			Returns the current key color of the given order.
		*/
		Colour getKeyOrderColor(int orderIn);

		/*
			Returns the current key color of the given scale degree.
		*/
		Colour getKeyDegreeColor(int degIn);

		/*
			Sets the color of the given order.
		*/
		void setKeyColorOrder(int orderIn, Colour colorIn);

		/*
			Sets the color of the given scale degree.
		*/
		void setKeyColorDegree(int tuningDegreeIn, Colour colorIn);

		/*
			Resets the color of all the keys in the given order to the current order color.
			If resetDegrees is true, it will set the keys their current degree color if non-default.
		*/
		void resetKeyColorsInOrder(int orderIn, bool resetDegrees = false);

		/*
			Resets the color of all the keys of the scale degree to the current order color.
		*/
		void resetDegreeColors(int tuningDegreeIn);

		/*
			Resets the given key's color to the current color of the key's order
		*/
		void resetKeyColor(int keyNumberIn);

		/*
			Reset all keys to the current colors of their orders.
		*/
		void resetKeyColors(bool resetDegrees=false);

		//===============================================================================================

		/*
			Sets the proportion of the keys' width to height. Default is 0.25.
		*/
		void setKeySizeRatio(float keySizeRatioIn);

		/*
			Sets the width of the keys to a given size and conforms the keyboard to the current proportions.
		*/
		void setKeyWidthSize(int widthSizeIn);

		/*
			Sets a scaling factor for different orders of keys.
		*/

		void setKeyOrderSizeScalar(float scalarIn);

		//===============================================================================================
        
		/*
			Turns all notes off except for the last one clicked.
		*/
        void isolateLastNote();
        
		/*
			Will resend a Note On message for all notes currently on.
		*/
        void retriggerNotes();
		
		/*
			Will trigger a midi messages for the key numbers given with the given velocity.
		*/
		void triggerNotes(Array<int> keyNumbers, bool noteOn = true, float velocity = 1);
        
		/*
			Returns the order of which all held notes are a part of.
			Values less than 0 mean the notes are a part of different orders
		*/
        int getOrderOfNotesOn();
        
		/*
			Transposes the given key (if on) to a certain amount of modal steps.
		*/
        Key* transposeKeyModally(int keyNumIn, int stepsIn);
        
		/*
			Transposes the given key (if on) to a certain amount of scale degrees.
		*/
        Key* transposeKeyChromatically(int keyNumIn, int degreesIn);
        
		/*
			Transposes the keys on to a certain amount of modal steps.
			If needsSameOrder is true, this will only work if all keys are in the same order.
			If it is false, then the keys in higher orders will be transposed the same amount of degrees
			of the modal steps of the lowest note if useLastClickedRoot is false, or the last clicked note 
			if that is true.
		*/
        void transposeKeysOnModally(int modalStepsIn, bool needsSameOrder=true, bool useLastClickedRoot=false);
        
		/*
			Transposes the keys on to a certain amount of scale degrees.
		*/
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
        
        void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;
        
        void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;
        
        //===============================================================================================

		void timerCallback() override;

		//===============================================================================================

        
    private:

		// Functionality
		UndoManager* undo;
        std::unique_ptr<KeyboardGrid> grid;

        MidiKeyboardState& keyboardInputState;
		MidiKeyboardState& keyboardInputFilteredState;
		NoteMap& inputFilterMap;
        
		MidiBuffer buffer;
		Array<Key*> keysPause;
        Array<Key*> keysToMap;

		// Parameters
		int uiModeSelected = 0;
		int orientationSelected = 0;
		int keyPlacementSelected = 1;
		int highlightSelected = 1;
		int velocitySelected = 1;
		int scrollingSelected = 1;

		bool showPitchNames = false;
		bool showNoteNumbers = false;
		bool showFilteredNoteNums = false;

		int midiChannelOut = 1;
		float velocityFixed = 1;
		bool scaleMidiInputVelocity = false;

		float keySizeRatio = 0.25f;
		float keyOrdersSizeScalar = 1;
                
        // Data
        ValueTree pianoNode;
        Array<Key>* keys;
        Array<Array<int>> keysOrder;
        Array<int> keysOn;
		Mode* mode;
                		        
        // Properties
        int keyWidth = 50;
        int keyHeight = 200;

		int lastKeyOver = 0;
		int lastKeyClicked = 0;

		int numOrder0Keys = 128;

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
