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
	public ApplicationCommandTarget,
	public MidiKeyboardStateListener // Only for displaying external MIDI input
{
public:

	enum PianoKeyOrderPlacement
	{
		nestedRight = 1,
		nestedCenter,
		adjacent
	};

	// Might want to implement the Button class more in the future 
	// if it seems better for passing MIDI data around
	struct PianoKeyComponent : public Button
	{
		// Parameters
		int* keyboardSize;
		int keyNumber;
		float modeDegree;

		float widthMod = -1;
		float heightMod = -1;
		float xOffset = 0;
		float yOffset = 0;

		//Pitch* mappedPitch = nullptr;
		int mappedMIDInote;
		int externalMidiState = 0;

		// Properties
		int order; // front most keys are 0, "black" keys are 1, etc
		float orderWidthRatio;
		float orderHeightRatio;
		float velocity = 0;

		Colour color;
		int activeColor = 0;

		// Methods

		PianoKeyComponent(String nameIn, int keyNumIn);

		void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
	};

	struct PianoKeyGrid : public FractionalGrid
	{
		int order = 1;
		int modeSize = 7;

		ModeLayout* layout;

		std::vector<float> orderedKeyRatios;

		PianoKeyGrid();

		PianoKeyGrid(ModeLayout* layoutIn);

		~PianoKeyGrid() {};

		void set_ordered_key_view(PianoKeyOrderPlacement placementType);

		void resize_ordered_key(PianoKeyComponent* key);

		void resize_ordered_keys(OwnedArray<PianoKeyComponent>* keys);

		void place_key(PianoKeyComponent* key);

		void place_key_layout(OwnedArray<PianoKeyComponent>* keys);

	};

	struct PianoMenuBar : public Component,
		public MenuBarModel
	{
		std::unique_ptr<MenuBarComponent> menu;

		StringArray options;

		PianoMenuBar(ApplicationCommandManager* cmdMgrIn);
		~PianoMenuBar();

		StringArray getMenuBarNames() override;

		void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

		PopupMenu getMenuForIndex(int topLevelMenuIndex, const String &menuName) override;

		void PianoMenuBar::resized() override;

	};

	//===============================================================================================

	ViewPianoComponent(ModeLayout* layoutIn, ApplicationCommandManager& cmdMgrIn);
	~ViewPianoComponent() {};

	//===============================================================================================

	MidiKeyboardState* get_keyboard_state();

	Point<int> get_position_of_key(int midiNoteIn);

	PianoKeyComponent* get_key_from_position(Point<int> posIn);

	PianoKeyComponent* get_key_from_position(const MouseEvent& e);

	float get_velocity(PianoKeyComponent* keyIn, const MouseEvent& e);

	int get_min_height();

	//===============================================================================================

	Colour get_key_color(PianoKeyComponent* keyIn);

	void apply_layout(ModeLayout* layoutIn);

	void all_notes_off();

	void isolate_last_note();

	bool check_keys_modal();

	PianoKeyComponent* transpose_key_modal(PianoKeyComponent* key, int stepsIn);

	PianoKeyComponent* transpose_key(PianoKeyComponent* key, int stepsIn);

	void transpose_keys_modal(int modalStepsIn);

	void transpose_keys(int modalStepsIn);

	//===============================================================================================
	
	ApplicationCommandTarget* getNextCommandTarget();

	void getAllCommands(Array< CommandID > &commands);

	void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result);

	bool perform(const InvocationInfo &info);

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

	enum CommandIDs
	{
		setPianoHorizontal = 1,
		setPianoVerticalL,
		setPianoVerticalR,
		sendScaleToPiano,
		pianoPlayMode,
		pianoEditMode,
		setKeyMidiNote,
		setKeyColor,
		setMidiNoteOffset
	};

	enum PianoMode
	{
		playMode = 1,
		editMode
	};

	enum PianoOrientation
	{
		horizontal = 1,
		verticalLeft,
		verticalRight
	};

	ApplicationCommandManager* appCmdMgr;
	PianoKeyGrid grid;
	MidiKeyboardState keyboardState;
	MidiBuffer buffer;

	std::unique_ptr<PianoMenuBar> menu;

	int lastKeyOver = 0;
	int lastKeyClicked = 0;

	// Data
	OwnedArray<PianoKeyComponent> keys;
	std::vector<PianoKeyComponent*> keysPtr;// debug

	std::vector<std::vector<PianoKeyComponent*>> keysOrder;
	std::vector<PianoKeyComponent*> keysOn;
	ModeLayout* modeLayout;

	// Parameters
	std::vector<int> scaleLayout;
	std::vector<int> scaleOrder;
	juce::String defaultMOS = "2 2 1 2 2 2 1";

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

	bool shiftHeld = false;
    bool altHeld = false;
    bool ctrlHeld = false;
	bool upHeld = false;
	bool downHeld = false;
	bool leftHeld = false;
	bool rightHeld = false;
        
    bool displayIsReady = false;
    	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ViewPianoComponent)
};
