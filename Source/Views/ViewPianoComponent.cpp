/*
  ==============================================================================

	ViewPianoComponent.h
	Created: 14 Mar 2019 4:50:31pm
	Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "ViewPianoComponent.h"


//==============================================================================

ViewPianoComponent::PianoKeyComponent::PianoKeyComponent(String nameIn, int keyNumIn)
	: Button(nameIn)
{
	keyNumber = keyNumIn;
	mappedMIDInote = keyNumber;

	setOpaque(true);
}

void ViewPianoComponent::PianoKeyComponent::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
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

String ViewPianoComponent::PianoKeyComponent::toString()
{
	String out = "";

	out << keyNumber << "|" << modeDegree << "|" << order << "|" << mappedMIDInote << "|"
		<< widthMod << "|" << heightMod << "|" << xOffset << "|" << yOffset << "|"
		<< findColour(0).toString() << findColour(1).toString() << findColour(2).toString();

	return out;
}

//===============================================================================================

ViewPianoComponent::PianoKeyGrid::PianoKeyGrid()
{
	order = 1;
	modeSize = 1;
	layout;
}

ViewPianoComponent::PianoKeyGrid::PianoKeyGrid(ModeLayout* layoutIn)
{
	layout = layoutIn;
	order = layout->get_highest_order();
	modeSize = layout->modeSize;

	set_grid(layout->get_num_modal_notes(), 1);
}

void ViewPianoComponent::PianoKeyGrid::set_ordered_key_view(ViewPianoComponent::PianoKeyOrderPlacement placementType)
{
	orderedKeyRatios.clear();

	switch (placementType)
	{
	case ViewPianoComponent::PianoKeyOrderPlacement::nestedCenter:
		break;
	case ViewPianoComponent::PianoKeyOrderPlacement::adjacent:
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

void ViewPianoComponent::PianoKeyGrid::resize_ordered_key(PianoKeyComponent* key)
{
	if (orderedKeyRatios.size() < 1)
		set_ordered_key_view(ViewPianoComponent::PianoKeyOrderPlacement::nestedRight);

	key->orderHeightRatio = orderedKeyRatios[key->keyNumber % layout->scaleSize];
	key->orderWidthRatio = 1.0f - (key->order > 0) * 1.25f * key->order / 8.0f;

}

void ViewPianoComponent::PianoKeyGrid::resize_ordered_keys(OwnedArray<PianoKeyComponent>* keys)
{
	PianoKeyComponent* key;

	for (int i = 0; i < keys->size(); i++)
	{
		key = keys->getUnchecked(i);
		resize_ordered_key(key);
	}
}


void ViewPianoComponent::PianoKeyGrid::place_key(PianoKeyComponent* key)
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

void ViewPianoComponent::PianoKeyGrid::place_key_layout(OwnedArray<PianoKeyComponent>* keys)
{
	if (needs_to_update())
		update_grid();

	for (int i = 0; i < keys->size(); i++)
	{
		place_key(keys->getUnchecked(i));
	}
}

//===============================================================================================

ViewPianoComponent::PianoMenuBar::PianoMenuBar(ApplicationCommandManager* cmdMgrIn)
{
	menu.reset(new MenuBarComponent(this));
	addAndMakeVisible(menu.get());
	setApplicationCommandManagerToWatch(cmdMgrIn);

	setSize(1000, 80);
}

ViewPianoComponent::PianoMenuBar::~PianoMenuBar()
{

}


StringArray ViewPianoComponent::PianoMenuBar::getMenuBarNames()
{
	return {"File", "Mode", "Orientation"};
}

void ViewPianoComponent::PianoMenuBar::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{

}

PopupMenu ViewPianoComponent::PianoMenuBar::getMenuForIndex(int topLevelMenuIndex, const String &menuName)
{
	return PopupMenu();
}

void ViewPianoComponent::PianoMenuBar::resized()
{
	setSize(getParentWidth(), 80);
}

//===============================================================================================

ViewPianoComponent::ViewPianoComponent(ModeLayout* layoutIn, ApplicationCommandManager& cmdMgrIn)
{
	// Default values
	tuningSize = 12;
	modeSize = 7;
	notesToShow = 128;
	rows = 1;

	removeMouseListener(this);

	appCmdMgr = &cmdMgrIn;
	menu.reset(new PianoMenuBar(appCmdMgr));
	menu.get()->setName("Piano Menu");
	//addAndMakeVisible(menu.get());
	

	// Create children (piano keys)        
	for (int i = 0; i < notesToShow; i++)
	{
		String keyName = "Key " + String(i);
		PianoKeyComponent* key = new PianoKeyComponent(keyName, i);
		addChildComponent(keys.add(key));
		keysPtr.push_back(key);
	}

	addMouseListener(this, true);
	//setRepaintsOnMouseActivity(true);
	toBack();
	
	setSize(1000, 250);
	setOpaque(true);
	apply_layout(layoutIn);
}

//===============================================================================================

MidiKeyboardState* ViewPianoComponent::get_keyboard_state()
{
	return &keyboardState;
}

Point<int> ViewPianoComponent::get_position_of_key(int midiNoteIn)
{
	PianoKeyComponent* key = keys.getUnchecked(midiNoteIn % keys.size());
	return key->getPosition();
}

ViewPianoComponent::PianoKeyComponent* ViewPianoComponent::get_key_from_position(Point<int> posIn)
{
	PianoKeyComponent* keyOut = nullptr;
	
	if (reallyContains(posIn, true))
	{
		Component* c = getComponentAt(posIn);
		
		if (c->getName().startsWith("Key"))
			keyOut = dynamic_cast<PianoKeyComponent*>(c);	
	}

	return keyOut;
}

ViewPianoComponent::PianoKeyComponent* ViewPianoComponent::get_key_from_position(const MouseEvent& e)
{
	PianoKeyComponent* keyOut = nullptr;
	Point<int> mousePosition = e.getScreenPosition() - getScreenBounds().getPosition();

	if (e.eventComponent->getName().startsWith("Key") &&
		reallyContains(mousePosition, true))
	{
		keyOut = dynamic_cast<PianoKeyComponent*>(getComponentAt(mousePosition));
	}

	return keyOut;
}

float ViewPianoComponent::get_velocity(PianoKeyComponent* keyIn, const MouseEvent& e)
{
	Point<int> mousePosition = e.getScreenPosition() - getScreenBounds().getPosition();
	Point<int> mouseInKey = mousePosition.withX(mousePosition.x - e.eventComponent->getX());

	float velocity = (float)mouseInKey.y / (float)keyIn->getHeight();

	return velocity;
}

int ViewPianoComponent::get_min_height()
{
	// not sure if this is the right approach to window sizing
	return 10;
}


//===============================================================================================

void ViewPianoComponent::apply_layout(ModeLayout* layoutIn)
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
	PianoKeyComponent* key;
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

Colour ViewPianoComponent::get_key_color(PianoKeyComponent* keyIn)
{
	// implement custom colors
	return keyOrderColors.at(keyIn->order % keyOrderColors.size());
}

void ViewPianoComponent::all_notes_off()
{
	PianoKeyComponent* key;

	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		triggerKeyNoteOff(key);
	}
	repaint();
}

void ViewPianoComponent::isolate_last_note()
{
	if (lastKeyClicked >= 0 && lastKeyClicked < 128)
	{
		PianoKeyComponent* last = keys.getUnchecked(lastKeyClicked);
		PianoKeyComponent* key;

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

bool ViewPianoComponent::check_keys_modal(int& orderDetected)
{
	orderDetected = keysOn[0]->order;

	for (int i = 1; i < keysOn.size(); i++)
	{
		if (keysOn[i]->order != orderDetected)
			return false;
	}

	return true;
}

ViewPianoComponent::PianoKeyComponent* ViewPianoComponent::transpose_key_modal(PianoKeyComponent* key, int stepsIn)
{
	PianoKeyComponent* keyOut = nullptr;
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

ViewPianoComponent::PianoKeyComponent* ViewPianoComponent::transpose_key(PianoKeyComponent* key, int stepsIn)
{
	PianoKeyComponent* keyOut = nullptr;
	int newKey;

	newKey = key->keyNumber + stepsIn;

	if (newKey >= 0 && newKey <= 127)
	{
		keyOut = keys.getUnchecked(newKey);
	}

	return keyOut;
}


bool ViewPianoComponent::transpose_keys_modal(int modalStepsIn)
{
	int theOrder;
	if (check_keys_modal(theOrder))
	{
		std::vector<PianoKeyComponent*> oldKeys = std::vector<PianoKeyComponent*>(keysOn);
		std::vector<PianoKeyComponent*> newKeys;
		PianoKeyComponent* key;
		PianoKeyComponent* newKey;
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

void ViewPianoComponent::transpose_keys(int stepsIn)
{
	std::vector<PianoKeyComponent*> oldKeys = std::vector<PianoKeyComponent*>(keysOn);
	std::vector<PianoKeyComponent*> newKeys;
	PianoKeyComponent* key;
	PianoKeyComponent* newKey;
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



//===============================================================================================

ApplicationCommandTarget* ViewPianoComponent::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void ViewPianoComponent::getAllCommands(Array< CommandID > &c)
{
	Array<CommandID> commands{
		CommandIDs::setPianoHorizontal,
		CommandIDs::setPianoVerticalL,
		CommandIDs::setPianoVerticalR,
		CommandIDs::sendScaleToPiano };

	c.addArray(commands);
}

void ViewPianoComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
{
	switch (commandID)
	{
	case CommandIDs::setPianoHorizontal:
		result.setInfo("Horizontal Layout", "Draws the piano as you'd sit down and play one", "Piano", 0);
		result.setTicked(pianoOrientationSelected == PianoOrientation::horizontal);
		result.addDefaultKeypress('w', ModifierKeys::shiftModifier);
		break;
	case CommandIDs::setPianoVerticalL:
		result.setInfo("Vertical Left Layout", "Draws the piano with the left facing orientation", "Piano", 0);
		result.setTicked(pianoOrientationSelected == PianoOrientation::verticalLeft);
		result.addDefaultKeypress('a', ModifierKeys::shiftModifier);
		break;
	case CommandIDs::setPianoVerticalR:
		result.setInfo("Vertical Right Layout", "Draws the piano with the right facing orientation", "Piano", 0);
		result.setTicked(pianoOrientationSelected == PianoOrientation::verticalRight);
		result.addDefaultKeypress('d', ModifierKeys::shiftModifier);
		break;
	default:
		break;
	}
}

bool ViewPianoComponent::perform(const InvocationInfo &info)
{
	switch (info.commandID)
	{
	case CommandIDs::setPianoHorizontal:
		// TBI
		break;
	case CommandIDs::setPianoVerticalL:
		// TBI
		break;
	case CommandIDs::setPianoVerticalR:
		// TBI
		break;
	default:
		return false;
	}
	return true;
}

//===============================================================================================


void ViewPianoComponent::mouseExit(const MouseEvent& e)
{
	PianoKeyComponent* key = keys.getUnchecked(lastKeyOver);

	if (!shiftHeld || key->activeColor < 2)
		key->activeColor = 0;
}

void ViewPianoComponent::mouseDown(const MouseEvent& e)
{
	PianoKeyComponent* key = get_key_from_position(e);
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
				PianoKeyComponent* oldKey = keys.getUnchecked(lastKeyClicked);
				triggerKeyNoteOff(oldKey);
			}

            triggerKeyNoteOn(key, get_velocity(key, e));
            lastKeyClicked = key->keyNumber;
        }
	}
}

void ViewPianoComponent::mouseDrag(const MouseEvent& e)
{
	PianoKeyComponent* key = get_key_from_position(e);

	if (key)
	{
		if (key->keyNumber != lastKeyClicked)
		{
			PianoKeyComponent* oldKey = keys.getUnchecked(lastKeyClicked);
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

void ViewPianoComponent::mouseUp(const MouseEvent& e)
{
	PianoKeyComponent* key = get_key_from_position(e);

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

void ViewPianoComponent::mouseMove(const MouseEvent& e)
{
	PianoKeyComponent* key = get_key_from_position(e);

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

bool ViewPianoComponent::keyStateChanged(bool isKeyDown)
{
	if (!KeyPress::isKeyCurrentlyDown(KeyPress::upKey) && upHeld)
	{
		upHeld = false;
	}

	if (!KeyPress::isKeyCurrentlyDown(KeyPress::downKey) && downHeld)
	{
		downHeld = false;
	}

	if (!KeyPress::isKeyCurrentlyDown(KeyPress::leftKey) && leftHeld)
	{
		leftHeld = false;
	}

	if (!KeyPress::isKeyCurrentlyDown(KeyPress::rightKey) && rightHeld)
	{
		rightHeld = false;
	}

	return isKeyDown;
}
bool ViewPianoComponent::keyPressed(const KeyPress& key)
{
	if (KeyPress::isKeyCurrentlyDown(KeyPress::upKey) && !upHeld)
	{
		upHeld = true;

		if (shiftHeld)
		{
			transpose_keys(1);
			repaint();
		}
	}

	if (KeyPress::isKeyCurrentlyDown(KeyPress::downKey) && !downHeld)
	{
		downHeld = true;

		if (shiftHeld)
		{
			transpose_keys(-1);
			repaint();
		}
	}

	if (KeyPress::isKeyCurrentlyDown(KeyPress::leftKey) && !leftHeld)
	{
		leftHeld = true;

		if (shiftHeld)
		{
			if (transpose_keys_modal(-1))
				repaint();
		}
	}

	if (KeyPress::isKeyCurrentlyDown(KeyPress::rightKey) && !rightHeld)
	{
		rightHeld = true;

		if (shiftHeld)
		{
			if (transpose_keys_modal(1))
				repaint();
		}
	}

	return false;
}

void ViewPianoComponent::modifierKeysChanged(const ModifierKeys& modifiers)
{
	if (!shiftHeld && modifiers.isShiftDown())
	{
		shiftHeld = true;
	}
	else if (shiftHeld && !modifiers.isShiftDown())
	{
		shiftHeld = false;

		isolate_last_note();

		PianoKeyComponent* key = keys.getUnchecked(lastKeyClicked);

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


void ViewPianoComponent::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background
	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	if (!displayIsReady)
	{
		g.setColour(Colours::white);
		g.setFont(14.0f);
		g.drawText("ViewPianoComponent", getLocalBounds(), Justification::centred, true);   // draw some placeholder text
	}
}

void ViewPianoComponent::resized()
{
	// Calculate key sizes
	keyHeight = getHeight() - 1;
	keyWidth = keyHeight * defaultKeyWHRatio;

	// Adjust Parent bounds and grid
    pianoWidth = modalKeysSize * keyWidth;
	setSize(pianoWidth, getHeight());
	grid.setBounds(Rectangle<int>(0, 0, pianoWidth, getHeight()));

	// Resize keys
	PianoKeyComponent* key;
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

void ViewPianoComponent::visibilityChanged()
{
	if (isShowing())
		setWantsKeyboardFocus(true);
	else
		setWantsKeyboardFocus(false);
}

//===============================================================================================

void ViewPianoComponent::triggerKeyNoteOn(PianoKeyComponent* key, float velocityIn)
{
	if (velocityIn > 0)
	{
		keyboardState.noteOn(midiChannelSelected, key->mappedMIDInote, velocityIn);
		key->activeColor = 2;
		key->velocity = velocityIn;
		keysOn.push_back(key);
	}
}

void ViewPianoComponent::triggerKeyNoteOff(PianoKeyComponent* key)
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
		PianoKeyComponent* temp = keysOn[keysOn.size() - 1];
		keysOn[keysOn.size() - 1] = keysOn[ind];
		keysOn[ind] = temp;
		keysOn.pop_back();
	}
}

void ViewPianoComponent::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
	PianoKeyComponent* key = keys.getUnchecked(midiNote);
	key->externalMidiState = 1;
}

void ViewPianoComponent::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
	PianoKeyComponent* key = keys.getUnchecked(midiNote);
	key->externalMidiState = 0;
}
