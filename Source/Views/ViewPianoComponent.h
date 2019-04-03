/*
  ==============================================================================

    ViewPianoComponent.h
    Created: 14 Mar 2019 4:50:31pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Structures/FractionalGrid.h"
#include "../Structures/ModeLayout.h"

#include <iostream>


//==============================================================================
/*
*/

class ViewPianoComponent : public Component,
                           public MidiKeyboardStateListener // Only for displaying external MIDI input
{
public:
	// Might want to implement the Button class more in the future 
	// if it seems better for passing MIDI data around
	struct PianoKeyComponent : public Button
	{
		// Parameters
		int* keyboardSize;
		int keyNumber;
		float modeDegree;
        
        float widthMod = 1;
        float heightMod = 1;
        float xOffset = 0;
        float yOffset = 0;

//		Pitch* mappedPitch = nullptr;
		int mappedMIDInote;
        int externalMidiState = 0;

		// Properties
        float* orderWidthRatio;
        float* orderHeightRatio;
		int order; // front most keys are 0, "black" keys are 1, etc
        
        Colour color;
        int activeColor = 0;

		// Methods

		PianoKeyComponent(int* sizeIn, int keyNumIn);

		void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
	};

	struct PianoKeyGrid : public FractionalGrid
	{
		int order = 1;
		int modeSize = 7;

		std::vector<int> orderLayout = { 0, 1, 0, 1, 0,    0, 1, 0, 1, 0, 1, 0 };
		std::vector<int> MOSLayout = { 2,    2,    1,    2,    2,    2,    1 };
		std::vector<int> gridLayout = { 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0 };
		// grid col per key { 0, 1, 2, 3, 4,  6, 7, 8, 9, 10, 11, 12 };

		PianoKeyGrid();

		~PianoKeyGrid() {};

		int set_order(int orderIn, float* orderWidthRatio, float* orderHeightRatio);

		int set_mode_keys(int modeSizeIn);

		void set_mode_keys(int modeSizeIn, int highestOrder);

		void set_order_layout(std::vector<int> layoutIn);

		void place_key(PianoKeyComponent* key);

		void place_key_layout(OwnedArray<PianoKeyComponent>* keys);

	};

	struct PianoMenuBar : public Component,
							//public ApplicationCommandTarget,
							public MenuBarModel
	{
		std::unique_ptr<MenuBarComponent> menu;

		StringArray options;
		
		PianoMenuBar();

		~PianoMenuBar();

		StringArray getMenuBarNames() override;

		void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

		PopupMenu getMenuForIndex(int topLevelMenuIndex, const String &menuName) override;


	};

	ViewPianoComponent();

	~ViewPianoComponent() {};

	//===============================================================================================
    
	float get_min_height();

	MidiKeyboardState* get_keyboard_state();
    
	Point<int> get_position_of_key(int midiNoteIn);

	int get_key_from_position(Point<int> posIn);

	float get_velocity(PianoKeyComponent* keyIn, const MouseEvent& e);

	//===============================================================================================

	void apply_layout(std::vector<int> layoutIn, bool samePeriod = true);

	void apply_steps_layout(juce::String strIn);

	void apply_steps_layout(std::vector<int> stepsIn);
    
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
    
    void triggerKeyNoteOn(PianoKeyComponent* key, float velocityIn);
    
    void triggerKeyNoteOff(PianoKeyComponent* key);
    
	void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;
    
	void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity) override;
    
    //===============================================================================================

private:
    // Functionality
	PianoKeyGrid grid;
	MidiKeyboardState keyboardState;
	MidiBuffer buffer;
	int lastNoteClicked = 0;

	// Data
	OwnedArray<PianoKeyComponent> keys;
    std::vector<std::vector<PianoKeyComponent*>> keysOrder;

    // Parameters
	std::vector<int> scaleLayout;
	std::vector<int> scaleOrder;
	juce::String defaultMOS = "2,2,1,2,2,2,1,";

    int tuningSize;
    int notesToShow;
    int rows;

	int midiChannelSelected = 1;
	int midiNoteOffset = 0;
	bool mpeOn = false;
    
    // Properties

	int modeSize;
	int modeKeysSize;
	int modeOrder;
    
    int keyWidth = 50;
    int keyHeight = 200;
    float defaultKeyWHRatio = 0.25;

	float pianoWidth;
    float minWindowHeight;

	// Key locks

	bool shiftHeld = false;
    bool altHeld = false;
    bool ctrlHeld = false;
	bool upHeld = false;
	bool downHeld = false;
    
    std::vector<std::vector<float>> keyOrderRatios = { {1.0f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f},
													   {1.0f, 0.55f, 0.45f, 0.38f, 0.27f, 0.18f} };
    
    Colour keyOrderColors[6] = {Colours::white, Colours::black, Colours::red, Colours::blue, Colours::green, Colours::pink};
    Colour keyOrderColorsH[6] = {Colours::lightgrey, Colours::darkgrey, Colours::red.brighter(), Colours::blue.brighter(), Colours::green.brighter(), Colours::pink.brighter()};
    
    bool displayIsReady = false;
    
    Colour test = Colours::green;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ViewPianoComponent)
};
