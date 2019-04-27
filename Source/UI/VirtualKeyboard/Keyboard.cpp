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
    removeMouseListener(this);
    
    pluginState = pluginStateIn;
    undo = pluginState->get_undo_mgr();
    appCmdMgr = cmdMgrIn;
    
    // Create children (piano keys)
    for (int i = 0; i < 128; i++)
    {
        String keyName = "Key " + String(i);
        Key* key = new Key(keyName, i);
        addChildComponent(keys.add(key));
    }
    
    addMouseListener(this, true);
    toBack();
    
    setSize(1000, 250);
    setOpaque(true);
}

//===============================================================================================

void Keyboard::init_data_node()
{
    pianoNode = ValueTree(IDs::pianoNode);
    
    pianoNode.setProperty(IDs::pianoUIMode, uiModeSelected, undo);
    pianoNode.setProperty(IDs::pianoOrientation, orientationSelected, undo);
    pianoNode.setProperty(IDs::pianoKeyPlacement, keyPlacementSelected, undo);
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
        /*
        key->pianoKeyNode.setProperty(IDs::pianoKeyColorDefault, key->findColour(0).toString(), nullptr);
        key->pianoKeyNode.setProperty(IDs::pianoKeyColorHighlighted, key->findColour(1).toString(), nullptr);
        key->pianoKeyNode.setProperty(IDs::pianoKeyColorPressed, key->findColour(2).toString(), nullptr);
         */

        pianoNode.addChild(key->pianoKeyNode, i, nullptr);
    }
    
    pluginState->pianoNode = pianoNode;
}

void Keyboard::restore_data_node(ValueTree pianoNodeIn)
{
    pianoNode = pianoNodeIn;
    
    uiModeSelected = pianoNode[IDs::pianoUIMode];
    orientationSelected = pianoNode[IDs::pianoOrientation];
    keyPlacementSelected = pianoNode[IDs::pianoKeyPlacement];
    midiChannelSelected = pianoNode[IDs::pianoMidiChannel];
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

int Keyboard::getWidthFromHeight(int heightIn)
{
    int wOut = 100;
    
    if (displayIsReady)
        wOut = mode->getKeyboardOrdersSize(0) * heightIn * defaultKeyWHRatio;
    
    return wOut;
}

//===============================================================================================

void Keyboard::setUIMode(UIMode uiModeIn)
{
    uiModeSelected = uiModeIn;
}

void Keyboard::setKeyPlacement(KeyPlacementType placementIn)
{
	keyPlacementSelected = placementIn;
	apply_mode_layout(mode);
}

void Keyboard::setKeyProportions(Key* keyIn)
{
	float spread = 4;
	float stepHeight;
	float keyHeight;
	float width;

	switch (keyPlacementSelected)
	{
	case 1:

		stepHeight = 0.55 + (keyIn->step - 2) / 100.0f * spread;
		keyHeight = stepHeight - stepHeight * (keyIn->order - 1) / keyIn->step;
		width = 0.8;

		break;

	case 2:
		break;

	default:

		stepHeight = 0.55 + (keyIn->step - 2) / 100.0f * spread;
		keyHeight = stepHeight - stepHeight * (keyIn->order - 1) / keyIn->step;
		width = 0.8 - (keyIn->order - 1) * 0.1;

		break;
	}
    
    if (keyIn->order == 0)
    {
        keyHeight = 1;
        width = 1;
    }
    
    keyIn->degreeWidthRatio = width;
    keyIn->degreeHeightRatio = keyHeight;
}

//===============================================================================================

void Keyboard::apply_mode_layout(Mode* modeIn)
{
    mode = modeIn;
    grid.reset(new KeyboardGrid(mode));
    
    keysOrder.clear();
    keysOrder.resize(mode->getMaxStep());
        
    Key* key;
    for (int i = 0; i < keys.size(); i++)
    {
        key = keys.getUnchecked(i);
        
        key->order = mode->getOrders()[i];
        keysOrder.getReference(key->order).add(key);

        key->setColour(0, get_key_color(key));
        key->setColour(1, get_key_color(key).contrasting(0.25));
        key->setColour(2, get_key_color(key).contrasting(0.75));
        
        key->degree = mode->getDegrees()[i];
        key->step = mode->getStepsOfOrders()[i];
        
        setKeyProportions(key);

        key->setVisible(true);
    }
    
    // bring all keys to front with highest orders frontmost
    for (int o = 0; o < keysOrder.size(); o++)
        for (int k = 0; k < keysOrder.getUnchecked(o).size(); k++)
            keysOrder.getReference(o).getReference(k)->toFront(false);
    
    // Calculate properties
    displayIsReady = true;
    resized();
}

//===============================================================================================

Colour Keyboard::get_key_color(Key* keyIn)
{
    return keyOrderColors.getUnchecked(keyIn->order % keyOrderColors.size());
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
    orderDetected = keysOn.getUnchecked(0)->order;
    
    for (int i = 1; i < keysOn.size(); i++)
    {
        if (keysOn.getUnchecked(i)->order != orderDetected)
            return false;
    }
    
    return true;
}

Key* Keyboard::transpose_key_modal(Key* key, int stepsIn)
{
    Key* keyOut = nullptr;
    int newKey = -1;
    int theOrder = key->order;
	Array<Key*>* orderArray = &keysOrder.getReference(theOrder);
    
    for (int i = 0; i < orderArray->size(); i++)
    {
		if (key->degree == orderArray->getUnchecked(i)->degree)
        {
            newKey = i;
            break;
        }
    }
    
    newKey += stepsIn;
    
    if (newKey >= 0 && newKey < orderArray->size())
        keyOut = orderArray->getUnchecked(newKey);
    
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
        Array<Key*> oldKeys = Array<Key*>(keysOn);
        Array<Key*> newKeys;
		Array<Key*>* orderSelected = &keysOrder.getReference(theOrder);

        Key* key;
        Key* newKey;
        int newDeg = -1;
        float velocity;
        
        for (int i = 0; i < oldKeys.size(); i++)
        {
            key = oldKeys.getUnchecked(i);
            velocity = key->velocity;

			newDeg = orderSelected->indexOf(key) + modalStepsIn;
            
            if (newDeg < 0 || newDeg >= orderSelected->size())
                continue;
            
            newKey = orderSelected->getUnchecked(newDeg);
            newKey->velocity = velocity;
            
            if (lastKeyClicked == key->keyNumber)
                lastKeyClicked = newKey->keyNumber;
            
            newKeys.add(newKey);
        }
        
		for (int i = 0; i < oldKeys.size(); i++)
			triggerKeyNoteOff(oldKeys.getReference(i));
        
        for (int i = 0; i < newKeys.size(); i++)
            triggerKeyNoteOn(newKeys.getReference(i), newKeys.getReference(i)->velocity);
        
        return true;
    }
    return false;
}

void Keyboard::transpose_keys(int stepsIn)
{
    Array<Key*> oldKeys = Array<Key*>(keysOn);
	Array<Key*> newKeys;
    Key* key;
    Key* newKey;
    int newKeyInd;
    float velocity;
    
    for (int i = 0; i < oldKeys.size(); i++)
    {
        key = oldKeys.getUnchecked(i);
        velocity = key->velocity;
        newKeyInd = key->keyNumber + stepsIn;
        
        if (newKeyInd < 0 || newKeyInd > 127)
            continue;
        
        if (lastKeyClicked == key->keyNumber)
            lastKeyClicked = newKeyInd;
        
        newKey = keys.getUnchecked(newKeyInd);
        newKey->velocity = velocity;
        newKeys.add(newKey);
    }
    
    for (int i = 0; i < oldKeys.size(); i++)
        triggerKeyNoteOff(oldKeys.getReference(i));
    
    for (int i = 0; i < newKeys.size(); i++)
        triggerKeyNoteOn(newKeys.getReference(i), newKeys.getReference(i)->velocity);
}

void Keyboard::retrigger_notes()
{
	Array<Key*> retrigger = Array <Key*>(keysOn);
    
    all_notes_off();
    
    for (int i = 0; i < retrigger.size(); i++)
        triggerKeyNoteOn(retrigger.getReference(i), retrigger.getUnchecked(i)->velocity);
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
		keyHeight = getHeight();
		keyWidth = keyHeight * defaultKeyWHRatio;

		// Adjust Parent bounds and grid
		pianoWidth = mode->getKeyboardOrdersSize(0) * keyWidth;
		grid->setBounds(Rectangle<int>(0, 0, pianoWidth, getHeight()));

		// Resize keys
		Key* key;
		int w, h;
		for (int i = 0; i < keys.size(); i++)
		{
			key = keys.getUnchecked(i);
			w = keyWidth * key->degreeWidthRatio;
			h = keyHeight * key->degreeHeightRatio;
			key->setSize(w, h);

			grid->placeKey(key);
		}

		setSize(keys.getLast()->getPosition().x + keyWidth, keyHeight);
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
            lastKeyClicked = 0;
            triggerKeyNoteOff(key);
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
    std::cout << e.eventComponent->getName() << std::endl;
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
        
        if (shiftHeld && keysOn.size() > 0)
        {
            transpose_keys(1);
            repaint();
        }
        return true;
    }
    
    if (KeyPress::isKeyCurrentlyDown(KeyPress::downKey) && !downHeld)
    {
        downHeld = true;
        
        if (shiftHeld && keysOn.size() > 0)
        {
            transpose_keys(-1);
            repaint();
        }
        return true;
    }
    
    if (KeyPress::isKeyCurrentlyDown(KeyPress::leftKey) && !leftHeld)
    {
        leftHeld = true;
        
        if (shiftHeld && keysOn.size() > 0)
        {
            if (transpose_keys_modal(-1))
                repaint();
        }
        return true;
    }
    
    if (KeyPress::isKeyCurrentlyDown(KeyPress::rightKey) && !rightHeld)
    {
        rightHeld = true;
        
        if (shiftHeld && keysOn.size() > 0)
        {
            if (transpose_keys_modal(1))
                repaint();
        }
        return true;
    }
    
    if (KeyPress::isKeyCurrentlyDown(KeyPress::spaceKey) && !spaceHeld)
    {
        spaceHeld = true;
        
        if (shiftHeld && keysOn.size() > 0)
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

void Keyboard::triggerKeyNoteOn(Key* key, float velocityIn)
{
    if (velocityIn > 0)
    {
        keyboardState.noteOn(midiChannelSelected, key->mappedMIDInote, velocityIn);
        key->activeColor = 2;
        key->velocity = velocityIn;
        keysOn.add(key);
    }
}

void Keyboard::triggerKeyNoteOff(Key* key)
{
    keyboardState.noteOff(midiChannelSelected, key->mappedMIDInote, 0);
    
    if (key->isMouseOver())
        key->activeColor = 1;
    else
        key->activeColor = 0;

	keysOn.swap(keysOn.indexOf(key), keysOn.size() - 1);
	keysOn.removeLast(1);
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

//===============================================================================================

ApplicationCommandTarget* Keyboard::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void Keyboard::getAllCommands(Array< CommandID > &c)
{
	Array<CommandID> commands{
		//IDs::CommandIDs::pianoPlayMode,
		//IDs::CommandIDs::pianoEditMode,
		//IDs::CommandIDs::setPianoHorizontal,
		//IDs::CommandIDs::setPianoVerticalL,
		//IDs::CommandIDs::setPianoVerticalR,
		IDs::CommandIDs::setKeysNestedRight,
		IDs::CommandIDs::setKeysNestedCenter,
		IDs::CommandIDs::setKeysAdjacent };

	c.addArray(commands);
}

void Keyboard::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
{
	switch (commandID)
	{
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
		result.setTicked(orientationSelected == Orientation::horizontal);
		result.addDefaultKeypress('w', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::setPianoVerticalL:
		result.setInfo("Vertical Left Layout", "Draws the piano with the left facing orientation", "Piano", 0);
		result.setTicked(orientationSelected == Orientation::verticalLeft);
		result.addDefaultKeypress('a', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::setPianoVerticalR:
		result.setInfo("Vertical Right Layout", "Draws the piano with the right facing orientation", "Piano", 0);
		result.setTicked(orientationSelected == Orientation::verticalRight);
		result.addDefaultKeypress('d', ModifierKeys::shiftModifier);
		break;
	case IDs::CommandIDs::setKeysNestedRight:
		result.setInfo("Keys Aligned Right", "The keys outside of the mode will be justified to the right.", "Piano", 0);
		result.setTicked(keyPlacementSelected == KeyPlacementType::nestedRight);
		break;
	case IDs::CommandIDs::setKeysNestedCenter:
		result.setInfo("Keys Aligned Center", "The keys outside of the mode will be centered.", "Piano", 0);
		result.setTicked(keyPlacementSelected == KeyPlacementType::nestedCenter);
		break;
	case IDs::CommandIDs::setKeysAdjacent:
		result.setInfo("Keys Adjacent", "The keys outside of the mode will placed adjacent to each other.", "Piano", 0);
		result.setTicked(keyPlacementSelected == KeyPlacementType::adjacent);
		break;
	default:
		break;
	}
}

bool Keyboard::perform(const InvocationInfo &info)
{
	switch (info.commandID)
	{
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
	case IDs::CommandIDs::setKeysNestedRight:
		setKeyPlacement(KeyPlacementType::nestedRight);
		break;
	case IDs::CommandIDs::setKeysNestedCenter:
		setKeyPlacement(KeyPlacementType::nestedCenter);
		break;
	case IDs::CommandIDs::setKeysAdjacent:
		setKeyPlacement(KeyPlacementType::adjacent);
		break;

	default:
		return false;
	}
	return true;
}
