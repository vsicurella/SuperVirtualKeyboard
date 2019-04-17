/*
  ==============================================================================

	VirtualKeyboard.h
	Created: 14 Mar 2019 4:50:31pm
	Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "Keyboard.h"

using namespace VirtualKeyboard;

Keyboard::Keyboard(SuperVirtualKeyboardPluginState* pluginStateIn, ApplicationCommandManager* cmdMgrIn)
{
	// Default values
	tuningSize = 12;
	modeSize = 7;
	notesToShow = 128;	
	rows = 1;

	removeMouseListener(this);

    pluginState = pluginStateIn;
	undo = pluginState->get_undo_mgr();
    appCmdMgr = cmdMgrIn;

	// Create children (piano keys)        
	for (int i = 0; i < notesToShow; i++)
	{
		String keyName = "Key " + String(i);
		Key* key = new Key(keyName, i);
		addChildComponent(keys.add(key));
		keysPtr.push_back(key);
	}

	addMouseListener(this, true);
	//setRepaintsOnMouseActivity(true);
	toBack();
	
	setSize(1000, 250);
	setOpaque(true);	
}

//===============================================================================================

void Keyboard::init_data_node()
{
    pianoNode = ValueTree(IDs::pianoNode);

	pianoNode.setProperty(IDs::pianoUIMode, pianoModeSelected, undo);
	pianoNode.setProperty(IDs::pianoOrientation, pianoOrientationSelected, undo);
	pianoNode.setProperty(IDs::pianoMidiChannel, midiChannelSelected, undo);
	pianoNode.setProperty(IDs::pianoMidiNoteOffset, midiNoteOffset, undo);
	pianoNode.setProperty(IDs::pianoMPEToggle, mpeOn, undo);
	pianoNode.setProperty(IDs::pianoWHRatio, defaultKeyWHRatio, undo);

	Key* key;
	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		key->pianoKeyNode = ValueTree(IDs::pianoKeyNode);

		key->pianoKeyNode.setProperty(IDs::pianoKeyNumber, i, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianoKeyMidiNote, key->mappedMIDInote, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianoKeyWidthMod, key->widthMod, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianoKeyHeightMod, key->heightMod, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianoKeyXOffset, key->xOffset, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianoKeyYOffset, key->yOffset, nullptr);

		pianoNode.addChild(key->pianoKeyNode, i, nullptr);
	}
}

void Keyboard::restore_data_node(ValueTree pianoNodeIn)
{
	pianoNode = pianoNodeIn;

	pianoModeSelected = pianoNode[IDs::pianoUIMode];
	// update UI things
	pianoOrientationSelected = pianoNode[IDs::pianoOrientation];

	midiChannelSelected = pianoNode[IDs::pianoMidiChannel];
    jassert(midiChannelSelected);

	midiNoteOffset = pianoNode[IDs::pianoMidiNoteOffset];

	mpeOn = pianoNode[IDs::pianoMPEToggle];

	defaultKeyWHRatio = pianoNode[IDs::pianoWHRatio];

	Key* key;
	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		key->restore_from_node(pianoNode);
	}
}

ValueTree Keyboard::get_node()
{
    return pianoNode;
}

//===============================================================================================

MidiKeyboardState* Keyboard::get_keyboard_state()
{
	return &keyboardState;
}

Point<int> Keyboard::get_position_of_key(int midiNoteIn)
{
	Key* key = keys.getUnchecked(midiNoteIn % keys.size());
	return key->getPosition();
}

Key* Keyboard::get_key_from_position(Point<int> posIn)
{
	Key* keyOut = nullptr;
	
	if (reallyContains(posIn, true))
	{
		Component* c = getComponentAt(posIn);
		
		if (c->getName().startsWith("Key"))
			keyOut = dynamic_cast<Key*>(c);
	}

	return keyOut;
}

Key* Keyboard::get_key_from_position(const MouseEvent& e)
{
	Key* keyOut = nullptr;
	Point<int> mousePosition = e.getScreenPosition() - getScreenBounds().getPosition();

	if (e.eventComponent->getName().startsWith("Key") &&
		reallyContains(mousePosition, true))
	{
		keyOut = dynamic_cast<Key*>(getComponentAt(mousePosition));
	}

	return keyOut;
}

float Keyboard::get_velocity(Key* keyIn, const MouseEvent& e)
{
	Point<int> mousePosition = e.getScreenPosition() - getScreenBounds().getPosition();
	Point<int> mouseInKey = mousePosition.withX(mousePosition.x - e.eventComponent->getX());

	float velocity = (float)mouseInKey.y / (float)keyIn->getHeight();

	return velocity;
}

int Keyboard::get_min_height()
{
	// not sure if this is the right approach to window sizing
	return 10;
}


//===============================================================================================

void Keyboard::apply_mode_layout(ModeLayout* layoutIn)
{
	modeLayout = layoutIn;
 	grid = KeyboardGrid(modeLayout);

	// for convenience
	scaleLayout = modeLayout->get_order();
	tuningSize = scaleLayout.size();
	modeSize = modeLayout->modeSize;
	modeOrder = modeLayout->get_highest_order();
	modalKeysSize = modeLayout->get_num_modal_notes();


	// Setup keys for layout
	keysOrder.resize(modeOrder + 1);
	Key* key;
	for (int i = 0; i < notesToShow; i++)
	{
		key = keys.getUnchecked(i);

		key->order = scaleLayout[i % tuningSize];
		keysOrder[key->order].push_back(key);

		grid.resize_ordered_key(key);

		key->setColour(0, get_key_color(key));
		key->setColour(1, get_key_color(key).contrasting(0.25));
		key->setColour(2, Colours::yellow.darker());

		key->modeDegree = modeLayout->modeDegrees[(i % tuningSize)] + modeSize * (i / tuningSize);

		if (key->order == 0)
		{
			key->toBack();
		}
		else
		{
			key->toFront(false);
		}

		key->setVisible(true);
	}

	// Calculate properties
	displayIsReady = true;
	resized();
}

//===============================================================================================

Colour Keyboard::get_key_color(Key* keyIn)
{
	// implement custom colors
	return keyOrderColors.at(keyIn->order % keyOrderColors.size());
}

void Keyboard::all_notes_off()
{
	Key* key;

	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		triggerKeyNoteOff(key);
	}
	repaint();
}

void Keyboard::isolate_last_note()
{
	if (lastKeyClicked >= 0 && lastKeyClicked < 128)
	{
		Key* last = keys.getUnchecked(lastKeyClicked);
		Key* key;

		for (int i = 0; i < keys.size(); i++)
		{
			key = keys.getUnchecked(i);

			if (key == last)
				continue;

			triggerKeyNoteOff(key);
		}
		repaint();
	}
}


bool Keyboard::check_keys_modal(int& orderDetected)
{
	orderDetected = keysOn[0]->order;

	for (int i = 1; i < keysOn.size(); i++)
	{
		if (keysOn[i]->order != orderDetected)
			return false;
	}

	return true;
}

Key* Keyboard::transpose_key_modal(Key* key, int stepsIn)
{
    Key* keyOut = nullptr;
	int newKey = -1;
	int theOrder = key->order;

	for (int i = 0; i < keysOrder[theOrder].size(); i++)
	{
		if (key->modeDegree == keysOrder[theOrder][i]->modeDegree)
		{
			newKey = i;
			break;
		}
	}

	newKey += stepsIn;

	if (newKey >= 0 && newKey < keysOrder[theOrder].size())
		keyOut = keysOrder[theOrder][newKey];

	return keyOut;
}

Key* Keyboard::transpose_key(Key* key, int stepsIn)
{
    Key* keyOut = nullptr;
	int newKey;

	newKey = key->keyNumber + stepsIn;

	if (newKey >= 0 && newKey <= 127)
	{
		keyOut = keys.getUnchecked(newKey);
	}

	return keyOut;
}


bool Keyboard::transpose_keys_modal(int modalStepsIn)
{
	int theOrder;
	if (check_keys_modal(theOrder))
	{
		std::vector<Key*> oldKeys = std::vector<Key*>(keysOn);
		std::vector<Key*> newKeys;
	    Key* key;
	    Key* newKey;
		int newDeg = -1;
		float velocity;

		for (int i = 0; i < oldKeys.size(); i++)
		{
			key = oldKeys[i];
			velocity = key->velocity;

			// Find index of key in the order array, and get new index
			for (int j = 0; j < keysOrder[theOrder].size(); j++)
			{
				if (key->modeDegree == keysOrder[theOrder].at(j)->modeDegree)
				{
					newDeg = j + modalStepsIn;
				}
			}

			if (newDeg < 0 || newDeg >= keysOrder[theOrder].size())
				continue;

			newKey = keysOrder[theOrder][newDeg];
			newKey->velocity = velocity;

			if (lastKeyClicked == key->keyNumber)
				lastKeyClicked = newKey->keyNumber;

			newKeys.push_back(newKey);
		}

		for (int i = 0; i < oldKeys.size(); i++)
			triggerKeyNoteOff(oldKeys[i]);

		for (int i = 0; i < newKeys.size(); i++)
			triggerKeyNoteOn(newKeys[i], newKeys[i]->velocity);
		
		return true;
	}
	return false;
}

void Keyboard::transpose_keys(int stepsIn)
{
	std::vector<Key*> oldKeys = std::vector<Key*>(keysOn);
	std::vector<Key*> newKeys;
    Key* key;
    Key* newKey;
	int newKeyInd;
	float velocity;


	for (int i = 0; i < oldKeys.size(); i++)
	{
		key = oldKeys[i];
		velocity = key->velocity;
		newKeyInd = key->keyNumber + stepsIn;

		if (newKeyInd < 0 || newKeyInd > 127)
			continue;

		if (lastKeyClicked == key->keyNumber)
			lastKeyClicked = newKeyInd;

		newKey = keys.getUnchecked(newKeyInd);
		newKey->velocity = velocity;
		newKeys.push_back(newKey);
	}

	for (int i = 0; i < oldKeys.size(); i++)
		triggerKeyNoteOff(oldKeys[i]);

	for (int i = 0; i < newKeys.size(); i++)
		triggerKeyNoteOn(newKeys[i], newKeys[i]->velocity);
}

void Keyboard::retrigger_notes()
{
	std::vector<VirtualKeyboard::Key*> retrigger = std::vector<VirtualKeyboard::Key*>(keysOn);
	
	all_notes_off();

	for (int i = 0; i < retrigger.size(); i++)
		triggerKeyNoteOn(retrigger.at(i), retrigger.at(i)->velocity);
}

//===============================================================================================

ApplicationCommandTarget* Keyboard::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void Keyboard::getAllCommands(Array< CommandID > &c)
{
	Array<CommandID> commands{
		IDs::CommandIDs::setKeyColor,
		IDs::CommandIDs::setMidiNoteOffset,
		IDs::CommandIDs::pianoPlayMode,
		IDs::CommandIDs::pianoEditMode,
		IDs::CommandIDs::setPianoHorizontal,
		IDs::CommandIDs::setPianoVerticalL,
		IDs::CommandIDs::setPianoVerticalR };

	c.addArray(commands);
}

void Keyboard::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
{
	switch (commandID)
	{
	case IDs::CommandIDs::setKeyColor:
		result.setInfo("Change Keyboard Colors", "Allows you to change the default colors for the rows of keys.", "Piano", 0);
		result.setTicked(pianoOrientationSelected == PianoOrientation::horizontal);
		result.addDefaultKeypress('c', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::setMidiNoteOffset:
		result.setInfo("Midi Note Offset", "Shift the layout so that your center is on a different MIDI note.", "Piano", 0);
		//result.setTicked(pianoOrientationSelected == PianoOrientation::verticalLeft);
		//result.addDefaultKeypress('a', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::pianoPlayMode:
		result.setInfo("Piano Play Mode", "Click the keyboard keys to play the mode and send MIDI data.", "Piano", 0);
		//result.setTicked(pianoOrientationSelected == PianoOrientation::verticalRight);
		//result.addDefaultKeypress('d', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::pianoEditMode:
		result.setInfo("Piano Edit Mode", "Adjust the size and proportions of the keyboard to your preferences.", "Piano", 0);
		//result.setTicked(pianoOrientationSelected == PianoOrientation::verticalRight);
		//result.addDefaultKeypress('d', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::setPianoHorizontal:
		result.setInfo("Horizontal Layout", "Draws the piano as you'd sit down and play one", "Piano", 0);
		result.setTicked(pianoOrientationSelected == PianoOrientation::horizontal);
		result.addDefaultKeypress('w', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::setPianoVerticalL:
		result.setInfo("Vertical Left Layout", "Draws the piano with the left facing orientation", "Piano", 0);
		result.setTicked(pianoOrientationSelected == PianoOrientation::verticalLeft);
		result.addDefaultKeypress('a', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::setPianoVerticalR:
		result.setInfo("Vertical Right Layout", "Draws the piano with the right facing orientation", "Piano", 0);
		result.setTicked(pianoOrientationSelected == PianoOrientation::verticalRight);
		result.addDefaultKeypress('d', ModifierKeys::shiftModifier);
		break;
	default:
		break;
	}
}

bool Keyboard::perform(const InvocationInfo &info)
{
	switch (info.commandID)
	{
	case IDs::CommandIDs::setKeyColor:
		break;
	case IDs::CommandIDs::setMidiNoteOffset:
		break;
	case IDs::CommandIDs::pianoPlayMode:
		break;
	case IDs::CommandIDs::pianoEditMode:
		break;
	case IDs::CommandIDs::setPianoHorizontal:
		// TBI
		break;
	case IDs::CommandIDs::setPianoVerticalL:
		// TBI
		break;
	case IDs::CommandIDs::setPianoVerticalR:
		// TBI
		break;
	default:
		return false;
	}
	return true;
}

//===============================================================================================


void Keyboard::mouseExit(const MouseEvent& e)
{
    Key* key = keys.getUnchecked(lastKeyOver);

	if (!shiftHeld || key->activeColor < 2)
		key->activeColor = 0;
}

void Keyboard::mouseDown(const MouseEvent& e)
{
    Key* key = get_key_from_position(e);
	if (key)
	{		
        if (shiftHeld && !altHeld && key->activeColor == 2)
        {
            // note off
            triggerKeyNoteOff(key);
            lastKeyClicked = 0;
        }
        else
        {
			if (altHeld)
			{
			    Key* oldKey = keys.getUnchecked(lastKeyClicked);
				triggerKeyNoteOff(oldKey);
			}

            triggerKeyNoteOn(key, get_velocity(key, e));
            lastKeyClicked = key->keyNumber;
        }
	}
}

void Keyboard::mouseDrag(const MouseEvent& e)
{
    Key* key = get_key_from_position(e);

	if (key)
	{
		if (key->keyNumber != lastKeyClicked)
		{
		    Key* oldKey = keys.getUnchecked(lastKeyClicked);
			if (!shiftHeld)
			{
				triggerKeyNoteOff(oldKey);
			}

			triggerKeyNoteOn(key, get_velocity(key, e));
			lastKeyClicked = key->keyNumber;
			repaint();
		}
	}
}

void Keyboard::mouseUp(const MouseEvent& e)
{
    Key* key = get_key_from_position(e);

	if (key)
	{
		if (!shiftHeld)
		{
			triggerKeyNoteOff(key);
			key->activeColor = 1;
			repaint();
		}
	}
}

void Keyboard::mouseMove(const MouseEvent& e)
{
    Key* key = get_key_from_position(e);

	if (key)
	{
		if (key->activeColor < 2)
		{
			key->activeColor = 1;
			repaint();
		}
		lastKeyOver = key->keyNumber;
	}
}

//===============================================================================================

bool Keyboard::keyStateChanged(bool isKeyDown)
{
	if (!KeyPress::isKeyCurrentlyDown(KeyPress::upKey) && upHeld)
	{
		upHeld = false;
		return true;
	}

	if (!KeyPress::isKeyCurrentlyDown(KeyPress::downKey) && downHeld)
	{
		downHeld = false;
		return true;
	}

	if (!KeyPress::isKeyCurrentlyDown(KeyPress::leftKey) && leftHeld)
	{
		leftHeld = false;
		return true;
	}

	if (!KeyPress::isKeyCurrentlyDown(KeyPress::rightKey) && rightHeld)
	{
		rightHeld = false;
		return true;
	}

	if (!KeyPress::isKeyCurrentlyDown(KeyPress::spaceKey) && spaceHeld)
	{
		spaceHeld = false;
		return true;
	}

	return false;
}

bool Keyboard::keyPressed(const KeyPress& key)
{
	if (KeyPress::isKeyCurrentlyDown(KeyPress::upKey) && !upHeld)
	{
		upHeld = true;

		if (shiftHeld)
		{
			transpose_keys(1);
			repaint();
		}
		return true;
	}

	if (KeyPress::isKeyCurrentlyDown(KeyPress::downKey) && !downHeld)
	{
		downHeld = true;

		if (shiftHeld)
		{
			transpose_keys(-1);
			repaint();
		}
		return true;
	}

	if (KeyPress::isKeyCurrentlyDown(KeyPress::leftKey) && !leftHeld)
	{
		leftHeld = true;

		if (shiftHeld)
		{
			if (transpose_keys_modal(-1))
				repaint();
		}
		return true;
	}

	if (KeyPress::isKeyCurrentlyDown(KeyPress::rightKey) && !rightHeld)
	{
		rightHeld = true;

		if (shiftHeld)
		{
			if (transpose_keys_modal(1))
				repaint();
		}
		return true;
	}

	if (KeyPress::isKeyCurrentlyDown(KeyPress::spaceKey) && !spaceHeld)
	{
		spaceHeld = true;

		if (shiftHeld)
		{
			retrigger_notes();
		}
		return true;
	}

	return false;
}

void Keyboard::modifierKeysChanged(const ModifierKeys& modifiers)
{
    
    if (!rightMouseHeld && modifiers.isRightButtonDown())
    {
        rightMouseHeld = true;
    }
    if (rightMouseHeld && !modifiers.isRightButtonDown())
    {
        rightMouseHeld = false;
    }
    
	if (!shiftHeld && modifiers.isShiftDown())
	{
		shiftHeld = true;
	}
    
	if (shiftHeld && !modifiers.isShiftDown())
	{
		shiftHeld = false;

		isolate_last_note();

	    Key* key = keys.getUnchecked(lastKeyClicked);

		if (!(key->isMouseOver() && key->isMouseButtonDownAnywhere()))
			triggerKeyNoteOff(keys.getUnchecked(lastKeyClicked));
	
		repaint();
	}
    
    if (!altHeld && modifiers.isAltDown())
    {
        altHeld = true;
		isolate_last_note();
		repaint();
    }
    
    else if (altHeld && !modifiers.isAltDown())
    {
        altHeld = false;
    }
    
    if (!ctrlHeld && modifiers.isCtrlDown())
    {
        ctrlHeld = true;
    }
    
    else if (ctrlHeld && !modifiers.isCtrlDown())
    {
        ctrlHeld = false;
    }
}

//===============================================================================================


void Keyboard::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background
	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	if (!displayIsReady)
	{
		g.setColour(Colours::white);
		g.setFont(14.0f);
		//g.drawText("ViewPianoComponent", getLocalBounds(), Justification::centred, true);   // draw some placeholder text
	}
}

void Keyboard::resized()
{
	if (displayIsReady)
	{
		// Calculate key sizes
		keyHeight = getHeight() - 1;
		keyWidth = keyHeight * defaultKeyWHRatio;

		// Adjust Parent bounds and grid
		pianoWidth = modalKeysSize * keyWidth;
		setSize(pianoWidth, getHeight());
		grid.setBounds(Rectangle<int>(0, 0, pianoWidth, getHeight()));

		// Resize keys
	    Key* key;
		int w, h;
		for (int i = 0; i < keys.size(); i++)
		{
			key = keys.getUnchecked(i);
			w = keyWidth * key->orderWidthRatio;
			h = keyHeight * key->orderHeightRatio;
			key->setSize(w, h);
			grid.place_key(key);
		}
	}
}

void Keyboard::visibilityChanged()
{
	if (isShowing())
		setWantsKeyboardFocus(true);
	else
		setWantsKeyboardFocus(false);
}

//===============================================================================================

void Keyboard::triggerKeyNoteOn(Key* key, float velocityIn)
{
	if (velocityIn > 0)
	{
		keyboardState.noteOn(midiChannelSelected, key->mappedMIDInote, velocityIn);
		key->activeColor = 2;
		key->velocity = velocityIn;
		keysOn.push_back(key);
	}
}

void Keyboard::triggerKeyNoteOff(Key* key)
{
    keyboardState.noteOff(midiChannelSelected, key->mappedMIDInote, 0);
	
	key->activeColor = 0;

	if (key->isMouseOver())
		key->activeColor = 1;
	
	int ind = -1;
	// find key in vector, swap with end, and pop off
	for (int i = 0; i < keysOn.size(); i++)
	{
		if (key->keyNumber == keysOn[i]->keyNumber)
		{
			ind = i;
			break;
		}
	}
	if (ind > -1)
	{
	    Key* temp = keysOn[keysOn.size() - 1];
		keysOn[keysOn.size() - 1] = keysOn[ind];
		keysOn[ind] = temp;
		keysOn.pop_back();
	}
}

void Keyboard::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
    Key* key = keys.getUnchecked(midiNote);
	key->externalMidiState = 1;
}

void Keyboard::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
    Key* key = keys.getUnchecked(midiNote);
	key->externalMidiState = 0;
}
