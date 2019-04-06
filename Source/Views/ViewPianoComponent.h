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

	ViewPianoComponent(ApplicationCommandManager& cmdMgrIn);
	~ViewPianoComponent() {};

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

	//===============================================================================================

	MidiKeyboardState* get_keyboard_state();

	Point<int> get_position_of_key(int midiNoteIn);

	PianoKeyComponent* get_key_from_position(Point<int> posIn);
	PianoKeyComponent* get_key_from_position(const MouseEvent& e);

	float get_velocity(PianoKeyComponent* keyIn, const MouseEvent& e);

	//===============================================================================================

	void apply_layout(ModeLayout layoutIn);

	void apply_steps_layout(juce::String strIn);

	void apply_steps_layout(std::vector<int> stepsIn);

	//===============================================================================================

	Point<float> get_key_proportions(PianoKeyComponent* keyIn);

	Colour get_key_color(PianoKeyComponent* keyIn);

	void all_notes_off();

	void isolate_last_note();

	bool check_keys_modal();

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
	ApplicationCommandManager* appCmdMgr;
	PianoKeyGrid grid;
	MidiKeyboardState keyboardState;
	MidiBuffer buffer;

	std::unique_ptr<PianoMenuBar> menu;

	int lastKeyOver = 0;
	int lastKeyClicked = 0;

	// Data
	OwnedArray<PianoKeyComponent> keys;
	std::vector<std::vector<PianoKeyComponent*>> keysOrder;
	Array<PianoKeyComponent*> keysOn;
	std::unique_ptr<ModeLayout> modeDisplayed;

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
        
    bool displayIsReady = false;
    	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ViewPianoComponent)
};
