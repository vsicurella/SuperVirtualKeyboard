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
#include "../../../Constants.h"
#include "../../../PluginIDs.h"
#include "../../../Midi/MidiProcessor.h"
#include "../../../Structures/Mode.h"
#include "../../../Structures/MappingHelper.h"
#include "VirtualKeyboardKey.h"
#include "VirtualKeyboardGrid.h"
#include "VirtualKeyboardGraphics.h"

#include <iostream>     


//==============================================================================
/*
 */

namespace VirtualKeyboard
{
    class Keyboard :
        public Component,
        public MidiKeyboardState,
        public MidiKeyboardStateListener
        // private Timer
    {
        
    public:
        //===============================================================================================
        
        Keyboard();
        Keyboard(ValueTree keyboardNodeIn, Mode* modeIn=nullptr, NoteMap* inputFilterMapIn = nullptr);
        ~Keyboard();
        
        //===============================================================================================
                
        void restoreNode(ValueTree keyboardNodeIn, bool reinitializeKeys = true, bool resetIfInvalid=false);
        
        void reset();

        void initializeKeys(int size=128);

        //===============================================================================================

        void paint(juce::Graphics& g) override;

        void paintOverChildren(juce::Graphics& g) override;

        void resized() override;

        //===============================================================================================

        void mouseMove(const MouseEvent& e) override;

        void mouseExit(const MouseEvent& e) override;

        void mouseDown(const MouseEvent& e) override;

        void mouseDrag(const MouseEvent& e) override;

        void mouseUp(const MouseEvent& e) override;

        //===============================================================================================

        bool keyStateChanged(bool isKeyDown) override;

        bool keyPressed(const KeyPress& key) override;

        void modifierKeysChanged(const ModifierKeys& modifiers) override;

        //===============================================================================================

        // void timerCallback() override;
        
        //===============================================================================================

        /*
            Returns Keyboard settings node
        */
        ValueTree getNode();

        /*
            Returns a struct with the given key's data.
        */
        Key* getKey(int keyNumIn);

        /*
            Returns a value tree with the given key's data.
        */
        ValueTree getKeyNode(int keyNumIn);

        /*
            Returns an array of pointers to the keys corresponding to the given order.
        */
        Array<Key*> getKeysByOrder(int orderIn);

        /*
        Returns an array of pointers to the keys corresponding to the given order.
        */
        Array<Key*> getKeysByScaleDegree(int degreeIn);

        /*
        Returns an array of pointers to the keys corresponding to the given order.
        */
        Array<Key*> getKeysByModalDegree(int degreeIn);

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
            Returns the key width in pixels.
        */
        int getKeyWidth();

        /* 
            Returns the key height in pixels.
        */
        int getKeyHeight();

        /*
            Returns the piano width from given height
        */
        int getPianoWidth(int heightIn);

        /*
            Returns the current proportion of key width to height.
        */
        float getKeySizeRatio();

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
            Returns the key which holds the given MouseEvent point relative to the top left corner of the component.
        */
        Key* getKeyFromPositionMouseEvent(const MouseEvent& e);

        /*
            Returns the velocity for a given key and point relative to the key's top left corner
        */
        float getKeyVelocity(Key* keyIn, Point<int> posIn);

        /*
            Returns the Midi Channel the keyboard is sending output on.
        */
        int getMidiChannelOut() const;

        /*
            Returns the current mode size
        */
        int getModeSize() const;
        
        //===============================================================================================


        /*
            Returns the UI mode of which the keyboard is in.
        */
        int getUIMode();
        
        /*
            Returns the orientation of which the keyboard is in.
        */
        int getOrientation();

        /*
            Returns the style of which the keys are nested.
        */
        int getKeyPlacementStyle();

        /*
            Returns whether or not note numbers are showing
        */
        bool isShowingNoteNumbers();
        
        /*
            Returns whether or not filtered note numbers are showing
        */
        bool isShowingFilteredNumbers();
        
        /*
            Returns whether or not note names are showing
        */
        
        bool isShowingNoteNames();

        /*
            Returns the style of which the keys get highlighted.
        */
        int getHighlightStyle();

        /*
            Returns the style of note velocity.
        */
        int getVelocityStyle();
        
        /*
            Returns the value of velocity if in fixed mode.
        */
        float getVelocityFixed();

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
            Rearranges the keys to fit the current mode, without changing other key data.
        */
        void applyMode(Mode* modeIn, bool resize = false);

        /*
            Applies the key data to the keyboard so that it matches the data passed in.
        */
        void applyKeyData(ValueTree keyDataTreeIn);

        /*
        Allows the keyboard to listen the filtered midi input
        */
        void displayKeyboardState(MidiKeyboardState* keyboardStateIn);

        //===============================================================================================

        /*
            Sets the keyboards UI mode.
        */
        void setUIMode(int uiModeIn);

        /*
            Sets the orientation of the keyboard.
        */
        void setOrientation(int orientationIn);
        
        /*
            Sets the amount of rows to split the keyboard into
         */
        void setNumRows(int numRowsIn);
        
        /*
            Sets the style of which the keys are nested in.
        */
        void setKeyStyle(int placementIn);
        
        /*
            Set the way the keys are highlighted
        */
        void setHighlightStyle(int styleIn);

        /*
        Sets the velocity behavior of the keyboard
        */
        void setVelocityBehavior(int behaviorNumIn, bool scaleInputVelocity = false);

        /*
        Sets the scrolling style of the keyboard.
        */
        void setScrollingStyle(int scrollingStyleIn);

        /*
            Set the Midi Channel that the keyboard outputs on clicks.
        */
        void setMidiChannelOut(int midiChannelOutIn);

        /*
            Set the fixed output velocity
        */
        void setVelocityFixed(float velocityIn);

        /*
            Sets whether or not midi input velocity should be scaled
        */
        void setInputVelocityScaled(bool shouldBeScaled);

        /*
            Set whether note numbers are showing
        */
        void setShowNoteNumbers(bool shouldShowNumbers);

        /*
            Set whether filtered note numbers are showing
        */
        void setShowFilteredNumbers(bool shouldShowNumbers);

        /*
            Set whether pitch names are shown
        */
        void setShowNoteLabels(bool shouldShowPitchNames);

        // might want to restructure these so this is not necessary
        void setLastKeyClicked(int keyNumIn);
  
        /*
            Set the Input NoteMap being used.
        */
        void setInputNoteMap(NoteMap& noteMapIn);
        
        //===============================================================================================

        /*
            Adds a reference to a mapping helper for manual mapping
        */
        void setMappingHelper(MappingHelper*);

        /*
            Hightlights the given key, or removes highlight if false is passed in.
        */
        void highlightKey(int keyNumberIn, bool highlightOn = true);

        //===============================================================================================
        
        /*
            Returns a pointer to the individual key colors
        */
        Array<Colour>* getKeyIndividualColours();

        /*
            Returns the color of the given key.
        */
        Colour getKeyColor(int keyNumIn);

        /*
            Returns a pointer to the key layer colors
        */
        Array<Colour>* getKeyLayerColours();

        /*
            Returns the current key layer of the given order.
        */
        Colour getKeyLayerColor(int orderIn);

        /*
            Returns a pointer to the key scale degree colors
        */
        Array<Colour>* getKeyDegreeColours();

        /*
            Returns the current key color of the given scale degree.
        */
        Colour getKeyDegreeColor(int degIn);

        /*
            Returns a pointer to the note on colors by midi channel
        */
        Array<Colour>* getKeyNoteOnColours();

        //===============================================================================================

        /*
            Sets the display color of all the keys
        */
        void updateKeyColors(bool writeToNode = true);

        /*
            Resets the color of all the keys in the given order to the current order color.
        */
        void resetLayerColors();

        /*
            Resets the color of all the keys of the scale degree to the current order color.
        */
        void resetDegreeColors();

        /*
            Reset all keys to the current colors of their orders.
        */
        void resetKeyColors();

        //===============================================================================================

        /*
            Sets the color of all keys of the given scale degree
        */
        void setKeyDegreeColor(int scaleDegreeIn, Colour colourIn);

        /*
            Sets the color of all keys of the given layer
        */
        void setKeyLayerColor(int layerNumIn, Colour colourIn);

        /*
            Sets the color of the given key
        */
        void setKeyColor(int keyNumIn, Colour colourIn);

        //===============================================================================================

        /*
            Sets the proportion of the keys' width to height. Default is 0.25.
        */
        void setKeySizeRatio(float keySizeRatioIn, bool resizeSelf = true);

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
            Turns all notes off with the selected midi channel
        */
        void allNotesOff();
        
        /*
            Turns all notes off except for the last one clicked.
        */
        void isolateLastNote();
        
        /*
            Will resend a Note On message for all notes currently on.
        */
        void retriggerNotes();
        
        /*
            Will trigger a midi message for the key number given
        */
        void triggerKey(int keyNumberIn, bool doNoteOn = true, float velocity = 1);

        /*
            Will trigger a midi messages for the key numbers given with the given velocity.
        */
        void triggerKeys(Array<int> keyNumbers, bool doNoteOn = true, float velocity = 1);
        
        /*
            Returns the order of which all held notes are a part of.
            Values less than 0 mean the notes are a part of different orders
        */
        int getOrderOfNotesOn();
        
        /*
            Transposes the given key (if on) to a certain amount of modal steps.
        */
        int transposeKeyModally(int keyNumIn, int stepsIn);
        
        /*
            Transposes the given key (if on) to a certain amount of scale degrees.
        */
        int transposeKeyChromatically(int keyNumIn, int degreesIn);
        
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

        void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;

        void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;

        //===============================================================================================

        
    private:

        // Functionality
        UndoManager* undo;
        KeyboardPositioner keyPositioner;
        MidiKeyboardState* externalKeyboardToDisplay = nullptr;
        NoteMap noteMapOnDisplay;
        bool hasDirtyKeys = true;
        
        MidiBuffer buffer;

        MappingHelper* manualMappingHelper;
        Array<Key*> highlightedKeys;
        
        //Array<Path> keyboardKeysPath;

        // Parameters
        int uiModeSelected = 0;
        int orientationSelected = 0;
        int keyPlacementSelected = 1;
        int highlightSelected = 1;
        int velocitySelected = 1;
        int scrollingSelected = 1;
        
        int numRows = 1;

        bool showNoteLabels = false;
        bool showNoteNumbers = false;
        bool showFilteredNoteNums = false;

        int midiChannelOut = 1;
        float velocityFixed = 1;
        bool scaleMidiInputVelocity = false;

        float keySizeRatio = 0.25f;
                
        // Data
        ValueTree pianoNode;
        ValueTree keyTreeNode;
        OwnedArray<Key> keys;
        Array<Colour> keyColorsOrders;
        Array<Colour> keyColorsDegrees;
        Array<Colour> keyColorsIndividual;
        Array<Colour> keyOnColorsByChannel;
        
        Mode* mode;
        Mode modeDefault = Mode(STD_TUNING_MODE_NODE, true);
        
        Array<Colour> colorsDefaultOrders = {
            Colours::white,
            Colours::black,
            Colours::crimson.withSaturation(1.0f),
            Colours::cornflowerblue.withSaturation(0.8f),
            Colours::mediumseagreen.withSaturation(0.9f),
            Colours::gold.withBrightness(0.75f),
            Colours::mediumpurple,
            Colours::orangered,
            Colours::saddlebrown
        };
                                
        // Properties
        int keyWidth = 50;
        int keyHeight = 200;

        float layerKeysWidthRatio = 0.8f;
        float layerKeysHeightRatio = 0.55f;

        int lastKeyOver = 0;
        int lastKeyClicked = 0;

        int numOrder0Keys = 128;
        int pianoWidth;
        int minWindowHeight;

        int modeSize;

        Array<Array<int>> keysOrder;
        Array<Array<int>> keysScaleDegree;
        Array<Array<int>> keysModalDegree;
        Array<int> keysOn;
        Array<int> keysByMouseTouch;
                
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
