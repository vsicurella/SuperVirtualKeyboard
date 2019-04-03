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

ViewPianoComponent::PianoKeyComponent::PianoKeyComponent(int* sizeIn, int keyNumIn)
	: Button("Key " + String(keyNumIn))
{
	keyboardSize = sizeIn;
	keyNumber = keyNumIn;
	mappedMIDInote = keyNumber;

	setOpaque(true);
}

void ViewPianoComponent::PianoKeyComponent::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    // idk why getBounds() doesn't work properly. maybe it's set in the parent bounds
    Rectangle<int> fillBounds = Rectangle<int>(0, 0, getWidth(), getHeight());
	color = findColour(activeColor);
    
    if (activeColor == 2)
    {
        color = findColour(0).interpolatedWith(findColour(2), 0.5);
        fillBounds.reduce(1, 1);
    }

	if (externalMidiState > 0)
		color = color.interpolatedWith(Colours::lightblue, 0.5);

    
    g.setColour(color);
    g.fillRect(fillBounds);
}

//===============================================================================================

ViewPianoComponent::PianoKeyGrid::PianoKeyGrid()
{
	order = 1;
	modeSize = 7;
}

int ViewPianoComponent::PianoKeyGrid::set_order(int orderIn, float* orderWidthRatio, float* orderHeightRatio)
{
	order = orderIn;
	return order;
}

int ViewPianoComponent::PianoKeyGrid::set_mode_keys(int modeSizeIn)
{
	modeSize = modeSizeIn;
	set_columns(modeSizeIn);
	return modeSize;
}

void ViewPianoComponent::PianoKeyGrid::set_mode_keys(int modeSizeIn, int highestOrder)
{
	order = highestOrder;
	set_mode_keys(modeSizeIn);
}

void ViewPianoComponent::PianoKeyGrid::set_order_layout(std::vector<int> layoutIn)
{
	orderLayout = layoutIn;
}

void ViewPianoComponent::PianoKeyGrid::place_key(PianoKeyComponent* key)
{
	if (needs_to_update())
		update_grid();

	Point<int> pt;

	float colToPlace = ceil(key->modeDegree);
	int offset = (key->order > 0) * (int)(key->getWidth() / 2.0) * (*key->orderWidthRatio + 1);

	if (key->order > 0)
	{
		colToPlace = ceil(colToPlace);
	}

	pt = Point<int>((int)(colToPlace * (column_size() + columnGap) - offset), 0 + rowGap);

	key->setTopLeftPosition(pt);
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

ViewPianoComponent::PianoMenuBar::PianoMenuBar()
{
	menu = std::unique_ptr<MenuBarComponent>(new MenuBarComponent());

		
}

ViewPianoComponent::PianoMenuBar::~PianoMenuBar()
{

}

StringArray ViewPianoComponent::PianoMenuBar::getMenuBarNames()
{
	return { "Edit Mode",
		"Save as...",
		"Orientation" };
}

void ViewPianoComponent::PianoMenuBar::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{

}

PopupMenu ViewPianoComponent::PianoMenuBar::getMenuForIndex(int topLevelMenuIndex, const String &menuName)
{
	return PopupMenu();
}

//===============================================================================================

ViewPianoComponent::ViewPianoComponent()
{
	setName("Piano Viewer Component");

	// Default values
	tuningSize = 12;
	modeSize = 7;
	notesToShow = 128;
	rows = 1;

	modeKeysSize = 0;
	removeMouseListener(this);

	// Create children (piano keys)        
	for (int i = 0; i < notesToShow; i++)
	{
		PianoKeyComponent* key = new PianoKeyComponent(&notesToShow, i);
		addChildComponent(keys.add(key));
	}

	addMouseListener(this, true);

	setRepaintsOnMouseActivity(true);
	setOpaque(true);

	// Apply default layout
	apply_steps_layout(defaultMOS);
}

//===============================================================================================

float ViewPianoComponent::get_min_height()
{
	return minWindowHeight;
}

MidiKeyboardState* ViewPianoComponent::get_keyboard_state()
{
	return &keyboardState;
}

Point<int> ViewPianoComponent::get_position_of_key(int midiNoteIn)
{
	PianoKeyComponent* key = keys.getUnchecked(midiNoteIn % keys.size());
	return key->getPosition();
}

int ViewPianoComponent::get_key_from_position(Point<int> posIn)
{
	int keyNum = -1;

	if (reallyContains(posIn, true))
	{
		Component* c = getComponentAt(posIn);
		PianoKeyComponent* key = dynamic_cast<PianoKeyComponent*>(c);

		if (c != this)
			keyNum = key->keyNumber;
	}

	return keyNum;
}

float ViewPianoComponent::get_velocity(PianoKeyComponent* keyIn, const MouseEvent& e)
{
	Point<int> mousePosition = e.getScreenPosition() - getScreenBounds().getPosition();
	Point<int> mouseInKey = mousePosition.withX(mousePosition.x - e.eventComponent->getX());

	float velocity = (float)mouseInKey.y / (float)keyIn->getHeight();

	return velocity;
}

//===============================================================================================

void ViewPianoComponent::apply_layout(std::vector<int> layoutIn, bool samePeriod)
{
	/* default 12-tone : 0 1 0 1 0 0 1 0 1 0 1 0 */
	scaleLayout = layoutIn;
	tuningSize = scaleLayout.size();

	modeSize = 0;
	modeOrder = 0;

	// Figure out mode properties
	for (int i = 0; i < tuningSize; i++)
	{
		if (scaleLayout.at(i) == 0)
			modeSize++;

		if (scaleLayout.at(i) > modeOrder)
			modeOrder = scaleLayout.at(i);
	}

	// Setup keys for layout
	modeKeysSize = 0;
	PianoKeyComponent* key;
	for (int i = 0; i < notesToShow; i++)
	{
		key = keys.getUnchecked(i);

		key->order = scaleLayout.at(i % tuningSize);
		key->orderWidthRatio = &(keyOrderRatios.at(0)[key->order]);
		key->orderHeightRatio = &(keyOrderRatios.at(1)[key->order]);

		key->setColour(0, keyOrderColors[key->order]);
		key->setColour(1, keyOrderColorsH[key->order]);
		key->setColour(2, Colours::yellow);

		key->modeDegree = modeKeysSize + (key->order / (modeOrder + 1.0));

		if (key->order == 0)
		{
			modeKeysSize++;
			key->toBack();
		}
		else
		{
			key->modeDegree = key->modeDegree - 1;
			key->toFront(false);
		}

		key->setVisible(true);

		if (key->order >= keysOrder.size())
			keysOrder.resize(key->order + 1);

		keysOrder.at(key->order).push_back(key);
	}

	// Update grid properties
	grid.set_mode_keys(modeKeysSize, modeOrder);

	// Calculate properties
	toBack();
	displayIsReady = true;
	resized();
}

void ViewPianoComponent::apply_steps_layout(juce::String strIn)
{
	apply_layout(ModeLayout::steps_to_order(ModeLayout::parse_steps(strIn)));
}

void ViewPianoComponent::apply_steps_layout(std::vector<int> stepsIn)
{
	apply_layout(ModeLayout::steps_to_order(stepsIn));
}

//===============================================================================================

void ViewPianoComponent::mouseExit(const MouseEvent& e)
{
	if (e.eventComponent != this)
	{
		PianoKeyComponent* key = dynamic_cast<PianoKeyComponent*>(e.eventComponent);

		if (!shiftHeld || key->activeColor < 2)
			key->activeColor = 0;
	}
}

void ViewPianoComponent::mouseDown(const MouseEvent& e)
{
	if (e.eventComponent != this)
	{
		PianoKeyComponent* key = dynamic_cast<PianoKeyComponent*>(e.eventComponent);
		
        if (shiftHeld && !altHeld && key->activeColor == 2)
        {
            // note off
            triggerKeyNoteOff(key);
            key->activeColor = 0;
            lastNoteClicked = 0;
        }
        else
        {
			if (altHeld)
			{
				PianoKeyComponent* oldKey = keys.getUnchecked(lastNoteClicked);
				oldKey->activeColor = 0;
				triggerKeyNoteOff(oldKey);
			}

            key->activeColor = 2;
            triggerKeyNoteOn(key, get_velocity(key, e));
            lastNoteClicked = key->keyNumber;
        }
	}
}

void ViewPianoComponent::mouseDrag(const MouseEvent& e)
{
	Point<int> mousePosition = e.getScreenPosition() - getScreenBounds().getPosition();
	int currentKey = get_key_from_position(mousePosition);

	if (currentKey != -1)
	{
		PianoKeyComponent* key = keys.getUnchecked(currentKey);

		if (currentKey != lastNoteClicked)
		{
			PianoKeyComponent* oldKey = keys.getUnchecked(lastNoteClicked % keys.size());
			if (!shiftHeld)
			{
				oldKey->activeColor = 0;
                triggerKeyNoteOff(oldKey);
			}

			key->activeColor = 2;
            triggerKeyNoteOn(key, get_velocity(key, e));
			lastNoteClicked = currentKey;
			repaint();
		}
	}
}

void ViewPianoComponent::mouseUp(const MouseEvent& e)
{
	if (e.eventComponent != this)
	{
		if (!shiftHeld)
		{
			PianoKeyComponent* key;
			for (int i = 0; i < keys.size(); i++)
			{
				key = keys.getUnchecked(i);

				// MIDI
                triggerKeyNoteOff(key);

				// GUI
				key->activeColor = 0;
			}

			key = dynamic_cast<PianoKeyComponent*>(e.eventComponent);
			key->activeColor = 1;
			repaint();
		}
	}
}

void ViewPianoComponent::mouseMove(const MouseEvent& e)
{
	if (e.eventComponent != this)
	{
		PianoKeyComponent* key = dynamic_cast<PianoKeyComponent*>(e.eventComponent);
		if (key->activeColor < 2)
			key->activeColor = 1;
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

	return isKeyDown;
}
bool ViewPianoComponent::keyPressed(const KeyPress& key)
{
	if (KeyPress::isKeyCurrentlyDown(KeyPress::upKey) && !upHeld)
	{
		upHeld = true;

		PianoKeyComponent* pk;
		int inv;

		for (int i = 0; i < keys.size(); i++)
		{
			inv = keys.size() - i - 1;

			if (keyboardState.isNoteOn(midiChannelSelected, inv))
			{
				pk = keys.getUnchecked(inv);
				pk->activeColor = 0;
				triggerKeyNoteOff(pk);

				if ((inv + 1) >= keys.size())
					continue;

				pk = keys.getUnchecked(inv + 1);
				pk->activeColor = 2;
				triggerKeyNoteOn(pk, 0.75f);
			}
		}

		if (lastNoteClicked < keys.size() - 1)
			lastNoteClicked++;

		repaint();
	}

	if (KeyPress::isKeyCurrentlyDown(KeyPress::downKey) && !downHeld)
	{
		downHeld = true;

		PianoKeyComponent* pk;

		for (int i = 0; i < keys.size(); i++)
		{
			if (keyboardState.isNoteOn(midiChannelSelected, i))
			{
				pk = keys.getUnchecked(i);
				pk->activeColor = 0;
				triggerKeyNoteOff(pk);

				if (i == 0)
					continue;

				pk = keys.getUnchecked(i-1);
				pk->activeColor = 2;
				triggerKeyNoteOn(pk, 0.75f);
			}
		}

		if (lastNoteClicked > 0)
			lastNoteClicked--;

		repaint();
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
		PianoKeyComponent* key;
		for (int i = 0; i < keys.size(); i++)
		{
			key = keys.getUnchecked(i);

			// MIDI
			if (keyboardState.isNoteOn(midiChannelSelected, key->mappedMIDInote) &&
				!(key->isMouseOver() && isMouseButtonDownAnywhere()))
			{
				keyboardState.noteOff(midiChannelSelected, key->mappedMIDInote, 0);
			}
			// GUI
			if (!(key->isMouseOver() && isMouseButtonDownAnywhere()))
				key->activeColor = 0;
		}
		repaint();
	}
    
    if (!altHeld && modifiers.isAltDown())
    {
        altHeld = true;

		PianoKeyComponent* key;
		for (int i = 0; i < keys.size(); i++)
		{
			key = keys.getUnchecked(i);

			if (key->keyNumber != lastNoteClicked)
			{
				triggerKeyNoteOff(key);
				key->activeColor = 0;
			}

		}
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
    pianoWidth = modeKeysSize * keyWidth;
	setSize(pianoWidth, getHeight());
	grid.setBounds(Rectangle<int>(0, 0, pianoWidth - 1, getHeight()));
    
	// Min bound for window height
	minWindowHeight = pianoWidth / modeKeysSize * defaultKeyWHRatio;

	// Resize keys
	PianoKeyComponent* key;
	int w, h;
	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		w = keyWidth * *key->orderWidthRatio;
		h = keyHeight * *key->orderHeightRatio;
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
    keyboardState.noteOn(midiChannelSelected, key->mappedMIDInote, velocityIn);
}

void ViewPianoComponent::triggerKeyNoteOff(PianoKeyComponent* key)
{
    keyboardState.noteOff(midiChannelSelected, key->mappedMIDInote, 0);
}

void ViewPianoComponent::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
	PianoKeyComponent* key = keys.getUnchecked(midiNote);
	key->externalMidiState = 1;
	//buffer.addEvent(MidiMessage::noteOn(midiChannel, midiNote, velocity), 0);
}

void ViewPianoComponent::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
	PianoKeyComponent* key = keys.getUnchecked(midiNote);
	key->externalMidiState = 0;
	//buffer.addEvent(MidiMessage::noteOff(midiChannel, midiNote, velocity), 0);
}
