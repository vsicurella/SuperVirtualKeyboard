/*
  ==============================================================================

	VirtualKeyboard.h
	Created: 14 Mar 2019 4:50:31pm
	Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "VirtualKeyboard.h"


//==============================================================================

VirtualKeyboard::PianoKey::PianoKey(String nameIn, int keyNumIn)
	: Button(nameIn)
{
	keyNumber = keyNumIn;
	mappedMIDInote = keyNumber;

	setOpaque(true);
}

void VirtualKeyboard::PianoKey::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    // idk why getBounds() doesn't work properly. maybe it's set in the parent bounds
    Rectangle<int> fillBounds = Rectangle<int>(0, 0, getWidth() - 1, getHeight());
	color = findColour(activeColor);
    
    if (activeColor == 2)
    {
        color = findColour(0).interpolatedWith(findColour(2), 0.618);
        fillBounds.reduce(1, 0);
    }

	if (externalMidiState > 0)
		color = color.interpolatedWith(Colours::lightblue, 0.75);

    
    g.setColour(color);
    g.fillRect(fillBounds);
}

void VirtualKeyboard::PianoKey::restore_from_node(ValueTree parentNodeIn)
{
    pianoKeyNode = parentNodeIn.getChild(keyNumber);
	mappedMIDInote = pianoKeyNode[IDs::pianokeyMidiNote];
	widthMod = pianoKeyNode[IDs::pianoKeyWidthMod];
	heightMod = pianoKeyNode[IDs::pianoKeyHeightMod];
	xOffset = pianoKeyNode[IDs::pianoKeyXOffset];
	yOffset = pianoKeyNode[IDs::pianoKeyYOffset];
}

//===============================================================================================

VirtualKeyboard::PianoKeyGrid::PianoKeyGrid()
{
	order = 1;
	modeSize = 1;
	layout;
}

VirtualKeyboard::PianoKeyGrid::PianoKeyGrid(ModeLayout* layoutIn)
{
	layout = layoutIn;
	order = layout->get_highest_order();
	modeSize = layout->modeSize;

	set_grid(layout->get_num_modal_notes(), 1);
}

void VirtualKeyboard::PianoKeyGrid::set_ordered_key_view(VirtualKeyboard::PianoKeyOrderPlacement placementType)
{
	orderedKeyRatios.clear();

	switch (placementType)
	{
	case VirtualKeyboard::PianoKeyOrderPlacement::nestedCenter:
		break;
	case VirtualKeyboard::PianoKeyOrderPlacement::adjacent:
		break;
	default: // aka nestedRight

		float localOrder;
		float height;
		float heightMod;
		float xp = 0.618;


		for (int i = 0; i < modeSize; i++)
		{
			localOrder = layout->get_steps()[i];

			if (localOrder < 2)
				orderedKeyRatios.push_back(1);
			else
			{
				for (int j = 0; j < localOrder; j++)
				{
					if (j == 0)
						height = 1;
					else
						height = (0.5 + 0.2 * localOrder / 6.0) * (pow(localOrder - j, xp) / pow(localOrder-1, xp));
					
					orderedKeyRatios.push_back(height);
				}
			}
		}
	}
}

void VirtualKeyboard::PianoKeyGrid::resize_ordered_key(PianoKey* key)
{
	if (orderedKeyRatios.size() < 1)
		set_ordered_key_view(VirtualKeyboard::PianoKeyOrderPlacement::nestedRight);

	key->orderHeightRatio = orderedKeyRatios[key->keyNumber % layout->scaleSize];
	key->orderWidthRatio = 1.0f - (key->order > 0) * 1.25f * key->order / 8.0f;

}

void VirtualKeyboard::PianoKeyGrid::resize_ordered_keys(OwnedArray<PianoKey>* keys)
{
	PianoKey* key;

	for (int i = 0; i < keys->size(); i++)
	{
		key = keys->getUnchecked(i);
		resize_ordered_key(key);
	}
}


void VirtualKeyboard::PianoKeyGrid::place_key(PianoKey* key)
{
	if (needs_to_update())
		update_grid();

	Point<int> pt;

	float colToPlace = ceil(key->modeDegree);
	int offset = (key->order > 0) * (int)(column_size() / 2.0);
	offset = (int)(offset * 1.2); // not sure why i have to do this to center the ordered keys
	
	if (key->order > 0)
	{
		colToPlace = ceil(colToPlace);
	}

	pt = Point<int>((int)((colToPlace + 1) * (column_size() + columnGap) - offset), 0 + rowGap);

	key->setTopRightPosition(pt.x, pt.y);
}

void VirtualKeyboard::PianoKeyGrid::place_key_layout(OwnedArray<PianoKey>* keys)
{
	if (needs_to_update())
		update_grid();

	for (int i = 0; i < keys->size(); i++)
	{
		place_key(keys->getUnchecked(i));
	}
}

//===============================================================================================

//===============================================================================================

VirtualKeyboard::VirtualKeyboard(ApplicationCommandManager* cmdMgrIn, UndoManager* undoIn)
{
	// Default values
	tuningSize = 12;
	modeSize = 7;
	notesToShow = 128;
	rows = 1;

	removeMouseListener(this);

	undo = undoIn;
	appCmdMgr = cmdMgrIn;

	// Create children (piano keys)        
	for (int i = 0; i < notesToShow; i++)
	{
		String keyName = "Key " + String(i);
		PianoKey* key = new PianoKey(keyName, i);
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

void VirtualKeyboard::init_data_node()
{
    pianoNode = ValueTree(IDs::pianoNode);

	pianoNode.setProperty(IDs::pianoUIMode, pianoModeSelected, undo);
	pianoNode.setProperty(IDs::pianoOrientation, pianoOrientationSelected, undo);
	pianoNode.setProperty(IDs::pianoMidiChannel, midiChannelSelected, undo);
	pianoNode.setProperty(IDs::pianoMidiNoteOffset, midiNoteOffset, undo);
	pianoNode.setProperty(IDs::pianoMPEToggle, mpeOn, undo);
	pianoNode.setProperty(IDs::pianoWHRatio, defaultKeyWHRatio, undo);

	PianoKey* key;
	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		key->pianoKeyNode = ValueTree(IDs::pianoKeyNode);

		key->pianoKeyNode.setProperty(IDs::pianokeyNumber, i, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianokeyMidiNote, key->mappedMIDInote, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianoKeyWidthMod, key->widthMod, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianoKeyHeightMod, key->heightMod, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianoKeyXOffset, key->xOffset, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianoKeyYOffset, key->yOffset, nullptr);

		pianoNode.addChild(key->pianoKeyNode, i, nullptr);
	}
}

void VirtualKeyboard::restore_data_node(ValueTree pianoNodeIn)
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

	PianoKey* key;
	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		key->restore_from_node(pianoNode);
	}
}

ValueTree VirtualKeyboard::get_node()
{
    return pianoNode;
}

//===============================================================================================

MidiKeyboardState* VirtualKeyboard::get_keyboard_state()
{
	return &keyboardState;
}

Point<int> VirtualKeyboard::get_position_of_key(int midiNoteIn)
{
	PianoKey* key = keys.getUnchecked(midiNoteIn % keys.size());
	return key->getPosition();
}

VirtualKeyboard::PianoKey* VirtualKeyboard::get_key_from_position(Point<int> posIn)
{
	PianoKey* keyOut = nullptr;
	
	if (reallyContains(posIn, true))
	{
		Component* c = getComponentAt(posIn);
		
		if (c->getName().startsWith("Key"))
			keyOut = dynamic_cast<PianoKey*>(c);	
	}

	return keyOut;
}

VirtualKeyboard::PianoKey* VirtualKeyboard::get_key_from_position(const MouseEvent& e)
{
	PianoKey* keyOut = nullptr;
	Point<int> mousePosition = e.getScreenPosition() - getScreenBounds().getPosition();

	if (e.eventComponent->getName().startsWith("Key") &&
		reallyContains(mousePosition, true))
	{
		keyOut = dynamic_cast<PianoKey*>(getComponentAt(mousePosition));
	}

	return keyOut;
}

float VirtualKeyboard::get_velocity(PianoKey* keyIn, const MouseEvent& e)
{
	Point<int> mousePosition = e.getScreenPosition() - getScreenBounds().getPosition();
	Point<int> mouseInKey = mousePosition.withX(mousePosition.x - e.eventComponent->getX());

	float velocity = (float)mouseInKey.y / (float)keyIn->getHeight();

	return velocity;
}

int VirtualKeyboard::get_min_height()
{
	// not sure if this is the right approach to window sizing
	return 10;
}


//===============================================================================================

void VirtualKeyboard::apply_layout(ModeLayout* layoutIn)
{
	modeLayout = layoutIn;
 	grid = PianoKeyGrid(modeLayout);

	// for convenience
	scaleLayout = modeLayout->get_order();
	tuningSize = scaleLayout.size();
	modeSize = modeLayout->modeSize;
	modeOrder = modeLayout->get_highest_order();
	modalKeysSize = modeLayout->get_num_modal_notes();


	// Setup keys for layout
	keysOrder.resize(modeOrder + 1);
	PianoKey* key;
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

Colour VirtualKeyboard::get_key_color(PianoKey* keyIn)
{
	// implement custom colors
	return keyOrderColors.at(keyIn->order % keyOrderColors.size());
}

void VirtualKeyboard::all_notes_off()
{
	PianoKey* key;

	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		triggerKeyNoteOff(key);
	}
	repaint();
}

void VirtualKeyboard::isolate_last_note()
{
	if (lastKeyClicked >= 0 && lastKeyClicked < 128)
	{
		PianoKey* last = keys.getUnchecked(lastKeyClicked);
		PianoKey* key;

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


bool VirtualKeyboard::check_keys_modal(int& orderDetected)
{
	orderDetected = keysOn[0]->order;

	for (int i = 1; i < keysOn.size(); i++)
	{
		if (keysOn[i]->order != orderDetected)
			return false;
	}

	return true;
}

VirtualKeyboard::PianoKey* VirtualKeyboard::transpose_key_modal(PianoKey* key, int stepsIn)
{
	PianoKey* keyOut = nullptr;
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

VirtualKeyboard::PianoKey* VirtualKeyboard::transpose_key(PianoKey* key, int stepsIn)
{
	PianoKey* keyOut = nullptr;
	int newKey;

	newKey = key->keyNumber + stepsIn;

	if (newKey >= 0 && newKey <= 127)
	{
		keyOut = keys.getUnchecked(newKey);
	}

	return keyOut;
}


bool VirtualKeyboard::transpose_keys_modal(int modalStepsIn)
{
	int theOrder;
	if (check_keys_modal(theOrder))
	{
		std::vector<PianoKey*> oldKeys = std::vector<PianoKey*>(keysOn);
		std::vector<PianoKey*> newKeys;
		PianoKey* key;
		PianoKey* newKey;
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

void VirtualKeyboard::transpose_keys(int stepsIn)
{
	std::vector<PianoKey*> oldKeys = std::vector<PianoKey*>(keysOn);
	std::vector<PianoKey*> newKeys;
	PianoKey* key;
	PianoKey* newKey;
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

void VirtualKeyboard::retrigger_notes()
{
	std::vector<PianoKey*> retrigger = std::vector<PianoKey*>(keysOn);
	
	all_notes_off();

	for (int i = 0; i < retrigger.size(); i++)
		triggerKeyNoteOn(retrigger.at(i), retrigger.at(i)->velocity);
}

//===============================================================================================

ApplicationCommandTarget* VirtualKeyboard::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void VirtualKeyboard::getAllCommands(Array< CommandID > &c)
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

void VirtualKeyboard::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
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

bool VirtualKeyboard::perform(const InvocationInfo &info)
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


void VirtualKeyboard::mouseExit(const MouseEvent& e)
{
	PianoKey* key = keys.getUnchecked(lastKeyOver);

	if (!shiftHeld || key->activeColor < 2)
		key->activeColor = 0;
}

void VirtualKeyboard::mouseDown(const MouseEvent& e)
{
	PianoKey* key = get_key_from_position(e);
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
				PianoKey* oldKey = keys.getUnchecked(lastKeyClicked);
				triggerKeyNoteOff(oldKey);
			}

            triggerKeyNoteOn(key, get_velocity(key, e));
            lastKeyClicked = key->keyNumber;
        }
	}
}

void VirtualKeyboard::mouseDrag(const MouseEvent& e)
{
	PianoKey* key = get_key_from_position(e);

	if (key)
	{
		if (key->keyNumber != lastKeyClicked)
		{
			PianoKey* oldKey = keys.getUnchecked(lastKeyClicked);
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

void VirtualKeyboard::mouseUp(const MouseEvent& e)
{
	PianoKey* key = get_key_from_position(e);

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

void VirtualKeyboard::mouseMove(const MouseEvent& e)
{
	PianoKey* key = get_key_from_position(e);

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

bool VirtualKeyboard::keyStateChanged(bool isKeyDown)
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

bool VirtualKeyboard::keyPressed(const KeyPress& key)
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

void VirtualKeyboard::modifierKeysChanged(const ModifierKeys& modifiers)
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

		PianoKey* key = keys.getUnchecked(lastKeyClicked);

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


void VirtualKeyboard::paint(Graphics& g)
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

void VirtualKeyboard::resized()
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
		PianoKey* key;
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

void VirtualKeyboard::visibilityChanged()
{
	if (isShowing())
		setWantsKeyboardFocus(true);
	else
		setWantsKeyboardFocus(false);
}

//===============================================================================================

void VirtualKeyboard::triggerKeyNoteOn(PianoKey* key, float velocityIn)
{
	if (velocityIn > 0)
	{
		keyboardState.noteOn(midiChannelSelected, key->mappedMIDInote, velocityIn);
		key->activeColor = 2;
		key->velocity = velocityIn;
		keysOn.push_back(key);
	}
}

void VirtualKeyboard::triggerKeyNoteOff(PianoKey* key)
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
		PianoKey* temp = keysOn[keysOn.size() - 1];
		keysOn[keysOn.size() - 1] = keysOn[ind];
		keysOn[ind] = temp;
		keysOn.pop_back();
	}
}

void VirtualKeyboard::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
	PianoKey* key = keys.getUnchecked(midiNote);
	key->externalMidiState = 1;
}

void VirtualKeyboard::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
	PianoKey* key = keys.getUnchecked(midiNote);
	key->externalMidiState = 0;
}
