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
    
    toBack();
    
    setSize(1000, 250);
    setOpaque(true);
}

//===============================================================================================

void Keyboard::initiateDataNode()
{
    pianoNode = ValueTree(IDs::pianoNode);
    
	updatePianoNode();
    
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
		key->pianoKeyNode.setProperty(IDs::pianoKeyColorIsCustom, key->customColor, nullptr);
		key->pianoKeyNode.setProperty(IDs::pianoKeyColorDefault, key->findColour(0).toString(), nullptr);

        pianoNode.addChild(key->pianoKeyNode, i, nullptr);
    }
    
	SuperVirtualKeyboardPluginState::add_array_to_node(pianoNode, keyOrderColors, IDs::pianoKeyOrderColors, "Color");
	SuperVirtualKeyboardPluginState::add_array_to_node(pianoNode, keyDegreeColors, IDs::pianoKeyDegreeColors, "Color");
    pluginState->pianoNode = pianoNode;
}

void Keyboard::restoreDataNode(ValueTree pianoNodeIn)
{
    pianoNode = pianoNodeIn;
    
    uiModeSelected = pianoNode[IDs::pianoUIMode];
    orientationSelected = pianoNode[IDs::pianoOrientation];
    keyPlacementSelected = pianoNode[IDs::pianoKeyPlacement];
	lastKeyClicked = pianoNode[IDs::pianoLastKeyClicked];
    midiChannelSelected = pianoNode[IDs::pianoMidiChannel];
    midiNoteOffset = pianoNode[IDs::pianoMidiNoteOffset];
    mpeOn = pianoNode[IDs::pianoMPEToggle];
    defaultKeyWHRatio = pianoNode[IDs::pianoWHRatio];

	keyOrderColors.clear();
	SuperVirtualKeyboardPluginState::get_array_from_node(pianoNode, keyOrderColors, IDs::pianoKeyOrderColors);
	keyDegreeColors.clear();
	SuperVirtualKeyboardPluginState::get_array_from_node(pianoNode, keyDegreeColors, IDs::pianoKeyDegreeColors);
    
    Key* key;
    for (int i = 0; i < keys.size(); i++)
    {
        key = keys.getUnchecked(i);
        key->restore_from_node(pianoNode.getChildWithProperty(IDs::pianoKeyNumber, i));
    }
}

void Keyboard::updatePianoNode()
{
	pianoNode.setProperty(IDs::pianoUIMode, uiModeSelected, undo);
	pianoNode.setProperty(IDs::pianoOrientation, orientationSelected, undo);
	pianoNode.setProperty(IDs::pianoKeyPlacement, keyPlacementSelected, undo);
	pianoNode.setProperty(IDs::pianoMidiChannel, midiChannelSelected, undo);
	pianoNode.setProperty(IDs::pianoLastKeyClicked, lastKeyClicked, nullptr);
	pianoNode.setProperty(IDs::pianoMidiNoteOffset, midiNoteOffset, undo);
	pianoNode.setProperty(IDs::pianoMPEToggle, mpeOn, undo);
	pianoNode.setProperty(IDs::pianoWHRatio, defaultKeyWHRatio, undo);

	pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyOrderColors), nullptr);
	SuperVirtualKeyboardPluginState::add_array_to_node(pianoNode, keyOrderColors, IDs::pianoKeyOrderColors, "Color");
	pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyDegreeColors), nullptr);
	SuperVirtualKeyboardPluginState::add_array_to_node(pianoNode, keyDegreeColors, IDs::pianoKeyDegreeColors, "Color");

}

void Keyboard::updateKeyNode(int keyNumIn)
{
	Key* key = keys.getUnchecked(keyNumIn);

	key->pianoKeyNode.setProperty(IDs::pianoKeyNumber, key->keyNumber, nullptr);
	key->pianoKeyNode.setProperty(IDs::pianoKeyMidiNote, key->mappedMIDInote, nullptr);
	key->pianoKeyNode.setProperty(IDs::pianoKeyWidthMod, key->widthMod, nullptr);
	key->pianoKeyNode.setProperty(IDs::pianoKeyHeightMod, key->heightMod, nullptr);
	key->pianoKeyNode.setProperty(IDs::pianoKeyXOffset, key->xOffset, nullptr);
	key->pianoKeyNode.setProperty(IDs::pianoKeyYOffset, key->yOffset, nullptr);
	key->pianoKeyNode.setProperty(IDs::pianoKeyColorIsCustom, key->customColor, nullptr);
	key->pianoKeyNode.setProperty(IDs::pianoKeyColorDefault, key->findColour(0).toString(), nullptr);
}

void Keyboard::updateKeyNodes()
{
	for (int i = 0; i < keys.size(); i++)
	{
		updateKeyNode(i);
	}
}

ValueTree Keyboard::getNode()
{
    return pianoNode;
}

//===============================================================================================

MidiKeyboardState* Keyboard::getMidiKeyboardState()
{
    return &keyboardState;
}

Key* Keyboard::getKey(int keyNumIn)
{
	return keys.getUnchecked(keyNumIn);
}

Array<Key*>* Keyboard::getKeysByOrder(int orderIn)
{
	return &keysOrder.getReference(orderIn & keysOrder.size());
}

Array<Key*> Keyboard::getKeysByScaleDegree(int scaleDegreeIn)
{
	int deg = scaleDegreeIn % mode->getScaleSize();
	Key* key = keys.getUnchecked(deg);
	Array<Key*>* orderArray = &keysOrder.getReference(key->order);
	Array<Key*> degreeArray;

	for (int i = 0; i < orderArray->size(); i++)
	{
		key = orderArray->getUnchecked(i);

		if (key->keyNumber % mode->getScaleSize() == deg)
			degreeArray.add(key);
	}

	return degreeArray;
}

int Keyboard::getLastKeyClicked()
{
	return lastKeyClicked;
}

Point<int> Keyboard::getPositionOfKey(int midiNoteIn)
{
    Key* key = keys.getUnchecked(midiNoteIn % keys.size());
    return key->getPosition();
}

Key* Keyboard::getKeyFromPosition(Point<int> posIn)
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

Key* Keyboard::getKeyFromPosition(const MouseEvent& e)
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


float Keyboard::getKeyVelocity(Key* keyIn, const MouseEvent& e)
{
    Point<int> mousePosition = e.getScreenPosition() - getScreenBounds().getPosition();
    Point<int> mouseInKey = mousePosition.withX(mousePosition.x - e.eventComponent->getX());
    
    float velocity = (float)mouseInKey.y / (float)keyIn->getHeight();
    
    return velocity;
}

Mode* Keyboard::getMode()
{
	return mode;
}

int Keyboard::getWidthFromHeight(int heightIn)
{
    int wOut = 100;
    
    if (displayIsReady)
        wOut = mode->getKeyboardOrdersSize(0) * heightIn * defaultKeyWHRatio;
    
    return wOut;
}

//===============================================================================================

int Keyboard::getUIMode()
{
	return uiModeSelected;
}

void Keyboard::setUIMode(UIMode uiModeIn)
{
    uiModeSelected = uiModeIn;
	Key* key;
	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		key->setColour(3, key->findColour(0));
		key->activeState = uiModeSelected * 3;
	}
}

void Keyboard::setKeyPlacement(KeyPlacementType placementIn)
{
	keyPlacementSelected = placementIn;
	applyMode(mode);
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

void Keyboard::setLastKeyClicked(int keyNumIn)
{
	lastKeyClicked = keyNumIn;
	pianoNode.setProperty(IDs::pianoLastKeyClicked, lastKeyClicked, nullptr);
}


//===============================================================================================

void Keyboard::applyMode(Mode* modeIn)
{
    mode = modeIn;
    grid.reset(new KeyboardGrid(mode));
    
    keysOrder.clear();
    keysOrder.resize(mode->getMaxStep());

	keyDegreeColors.resize(mode->getScaleSize());
        
    Key* key;
    for (int i = 0; i < keys.size(); i++)
    {
        key = keys.getUnchecked(i);
        
        key->order = mode->getOrders()[i];
        keysOrder.getReference(key->order).add(key);

        key->setColour(0, getKeyColor(key));
        key->setColour(1, key->findColour(0).contrasting(0.25));
        key->setColour(2, key->findColour(0).contrasting(0.75));
        
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

Colour Keyboard::getKeyColor(Key* keyIn)
{
	Colour c;
	// If has its own color, or else if it has a degree color, or else the default order color
	if (keyIn->customColor)
		c = keyIn->findColour(0);
	else if (!keyDegreeColors[keyIn->keyNumber % mode->getScaleSize()].isTransparent())
		c = keyDegreeColors[keyIn->keyNumber % mode->getScaleSize()];
	else
		c = keyOrderColors[keyIn->order % keyOrderColors.size()];

	return c;
}

void Keyboard::setKeyColor(Key* keyIn, int colorIndex, Colour colorIn)
{
	keyIn->setColour(colorIndex, colorIn);
	keyIn->customColor = true;
}

void Keyboard::setKeyColor(int keyNumIn, int colorIndex, Colour colorIn)
{
	setKeyColor(keys.getUnchecked(keyNumIn), colorIndex, colorIn);
}

void Keyboard::setKeyColorOrder(int orderIn, int colorIndex, Colour colorIn)
{
	keyOrderColors.set(orderIn % keyOrderColors.size(), colorIn);
	Array<Key*>* orderArray = &keysOrder.getReference(orderIn % keysOrder.size());

	Key* key;
	for (int i = 0; i < orderArray->size(); i++)
	{
		key = orderArray->getUnchecked(i);
		key->customColor = false; // reset key so that it changes no matter what
		key->setColour(colorIndex, getKeyColor(key));
	}
}

void Keyboard::setKeyColorDegree(int tuningDegreeIn, int colorIndex, Colour colorIn)
{
	int deg = tuningDegreeIn % mode->getScaleSize();
	keyDegreeColors.set(deg, colorIn);
}

void Keyboard::resetKeyColors()
{
	Key* key;

	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		key->customColor = false;
	}
}

//===============================================================================================


void Keyboard::allNoteOff()
{
    Key* key;
    
    for (int i = 0; i < keys.size(); i++)
    {
        key = keys.getUnchecked(i);
        triggerKeyNoteOff(key);
    }
    repaint();
}

void Keyboard::isolateLastNote()
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


bool Keyboard::keysAreInSameOrder(int& orderDetected)
{
    orderDetected = keysOn.getUnchecked(0)->order;
    
    for (int i = 1; i < keysOn.size(); i++)
    {
        if (keysOn.getUnchecked(i)->order != orderDetected)
            return false;
    }
    
    return true;
}

Key* Keyboard::transposeKeyModally(Key* key, int stepsIn)
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

Key* Keyboard::transposeKeyChromatically(Key* key, int stepsIn)
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


bool Keyboard::transposeKeysOnModally(int modalStepsIn)
{
    int theOrder;
    if (keysAreInSameOrder(theOrder))
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

void Keyboard::transposeKeysOnChromatically(int stepsIn)
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

void Keyboard::retriggerNotes()
{
	Array<Key*> retrigger = Array <Key*>(keysOn);
    
    allNoteOff();
    
    for (int i = 0; i < retrigger.size(); i++)
        triggerKeyNoteOn(retrigger.getReference(i), retrigger.getUnchecked(i)->velocity);
}

//===============================================================================================

void Keyboard::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background
	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
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

//===============================================================================================


void Keyboard::mouseMove(const MouseEvent& e)
{
	if (uiModeSelected == UIMode::playMode)
	{
		Key* key = getKeyFromPosition(e);

		if (key)
		{
			lastKeyOver = key->keyNumber;
		}
	}

}


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
		if (uiModeSelected == UIMode::playMode)
		{
			if (shiftHeld && keysOn.size() > 0)
			{
				transposeKeysOnChromatically(1);
				repaint();
			}
		}
        return true;
    }
    
    if (KeyPress::isKeyCurrentlyDown(KeyPress::downKey) && !downHeld)
    {
        downHeld = true;
		if (uiModeSelected == UIMode::playMode)
		{
			if (shiftHeld && keysOn.size() > 0)
			{
				transposeKeysOnChromatically(-1);
				repaint();
			}
		}
        return true;
    }
    
    if (KeyPress::isKeyCurrentlyDown(KeyPress::leftKey) && !leftHeld)
    {
        leftHeld = true;
		if (uiModeSelected == UIMode::playMode)
		{
			if (shiftHeld && keysOn.size() > 0)
			{
				if (transposeKeysOnModally(-1))
					repaint();
			}
		}
        return true;
    }
    
    if (KeyPress::isKeyCurrentlyDown(KeyPress::rightKey) && !rightHeld)
    {
        rightHeld = true;
		if (uiModeSelected == UIMode::playMode)
		{
			if (shiftHeld && keysOn.size() > 0)
			{
				if (transposeKeysOnModally(1))
					repaint();
			}
		}
        return true;
    }
    
    if (KeyPress::isKeyCurrentlyDown(KeyPress::spaceKey) && !spaceHeld)
    {
        spaceHeld = true;
		if (uiModeSelected == UIMode::playMode)
		{
			if (shiftHeld && keysOn.size() > 0)
			{
				retriggerNotes();
			}
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
        
		if (uiModeSelected == UIMode::playMode)
		{
			isolateLastNote();

			Key* key = keys.getUnchecked(lastKeyClicked);

			if (!(key->isMouseOver() && key->isMouseButtonDownAnywhere()))
				triggerKeyNoteOff(keys.getUnchecked(lastKeyClicked));

			repaint();
		}
    }
    
    if (!altHeld && modifiers.isAltDown())
    {
        altHeld = true;
		if (uiModeSelected == UIMode::playMode)
		{
			isolateLastNote();
			repaint();
		}
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
		key->activeState = 2;
		key->velocity = velocityIn;
        keysOn.add(key);
    }
}

void Keyboard::triggerKeyNoteOff(Key* key)
{
    keyboardState.noteOff(midiChannelSelected, key->mappedMIDInote, 0);

	if (key->isMouseOver())
		key->activeState = 1;
	else
		key->activeState = 0;

	keysOn.removeAllInstancesOf(key);

	/*
	keysOn.swap(keysOn.indexOf(key), keysOn.size() - 1);
	keysOn.removeLast(1);
	*/
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
