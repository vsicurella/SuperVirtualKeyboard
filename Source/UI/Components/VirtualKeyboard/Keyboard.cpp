/*
 ==============================================================================
 
 VirtualKeyboard.h
 Created: 14 Mar 2019 4:50:31pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "Keyboard.h"

using namespace VirtualKeyboard;

Keyboard::Keyboard(SvkMidiProcessor* midiProcessorIn)
{
    removeMouseListener(this);
    
	midiProcessor = midiProcessorIn;
       
    // Create children (virtualKeyboard keys)
    for (int i = 0; i < 128; i++)
    {
        String keyName = "Key " + String(i);
        Key* key = new Key(keyName, i);
        addChildComponent(keys.add(key));
    }
	
	keyColorsSingle.resize(keys.size());

    toBack();

	initiateDataNode();
    
    setSize(1000, 250);
    setOpaque(true);
}

//===============================================================================================

void Keyboard::initiateDataNode()
{
	if (!pianoNode.isValid())
	{
		pianoNode = ValueTree(IDs::pianoNode);

		// Initialize default values
		for (int i = 0; i < 128; i++)
			keyMidiNoteMappings.add(i);

		pianoNode.setProperty(IDs::pianoUIMode, UIMode::playMode, nullptr);
		pianoNode.setProperty(IDs::pianoOrientation, Orientation::horizontal, nullptr);
		pianoNode.setProperty(IDs::pianoMPEToggle, false, nullptr);
		pianoNode.setProperty(IDs::pianoWHRatio, 0.25f, nullptr);
		pianoNode.setProperty(IDs::pianoHasCustomColor, false, nullptr);
        
        updatePianoNode();
	}
}

void Keyboard::restoreDataNode(ValueTree pianoNodeIn)
{
	if (pianoNodeIn.hasType(IDs::pianoNode))
	{
		pianoNode = pianoNodeIn;

		orientationSelected = pianoNode[IDs::pianoOrientation];
		keyPlacementSelected = pianoNode[IDs::pianoKeyPlacementType];
		lastKeyClicked = pianoNode[IDs::pianoLastKeyClicked];
		mpeOn = pianoNode[IDs::pianoMPEToggle];
		defaultKeyWHRatio = pianoNode[IDs::pianoWHRatio];

		if (pianoNode.getChildWithName(IDs::pianoKeyColorsOrder).isValid())
		{
			keyColorsOrder.clear();
			get_array_from_node(pianoNode, keyColorsOrder, IDs::pianoKeyColorsOrder, 9);
		}

		if (pianoNode.getChildWithName(IDs::pianoKeyColorsDegree).isValid())
		{
			keyColorsDegree.clear();
			get_array_from_node(pianoNode, keyColorsDegree, IDs::pianoKeyColorsDegree, keys.size());
		}

		if (pianoNode.getChildWithName(IDs::pianoKeyColorSingle).isValid())
		{
			keyColorsSingle.clear();
			get_array_from_node(pianoNode, keyColorsSingle, IDs::pianoKeyColorSingle, keys.size());
		}
	}
}

void Keyboard::updatePianoNode()
{
    // Colors
    pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyColorsOrder), nullptr);
    add_array_to_node(pianoNode, keyColorsOrder, IDs::pianoKeyColorsOrder, "Color");
    
    pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyColorsDegree), nullptr);
    add_array_to_node(pianoNode, keyColorsDegree, IDs::pianoKeyColorsDegree, "Color");
    
    pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyColorSingle), nullptr);
    add_array_to_node(pianoNode, keyColorsSingle, IDs::pianoKeyColorSingle, "Color");
    
    
    // Placements
    pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyPlaceOrder), nullptr);
    add_array_to_node(pianoNode, keyPlacesOrder, IDs::pianoKeyPlaceOrder, "Place");
    pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyPlaceDegree), nullptr);
    add_array_to_node(pianoNode, keyPlacesDegree, IDs::pianoKeyPlaceDegree, "Place");
    pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyPlaceSingle), nullptr);
    add_array_to_node(pianoNode, keyPlacesSingle, IDs::pianoKeyPlaceSingle, "Place");
    
    // Ratios
    pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyRatioOrder), nullptr);
    add_array_to_node(pianoNode, keyRatioOrder, IDs::pianoKeyRatioOrder, "Ratio");
    pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyRatioDegree), nullptr);
    add_array_to_node(pianoNode, keyRatioDegree, IDs::pianoKeyRatioDegree, "Ratio");
    pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyRatioSingle), nullptr);
    add_array_to_node(pianoNode, keyRatioSingle, IDs::pianoKeyRatioSingle, "Ratio");
    
}

ValueTree Keyboard::getNode()
{
    return pianoNode;
}

//===============================================================================================

void Keyboard::setMode(Mode* modeIn)
{
    mode = modeIn;
    modeOffset = mode->getOffset();
}

// NEED TO MAKE THIS MORE EFFICIENT
void Keyboard::updateKeys()
{
    grid.reset(new KeyboardGrid(mode));
    
    keysOrder.clear();
    keysOrder.resize(mode->getMaxStep());
    
    keyColorsDegree.resize(mode->getScaleSize());
    
    Key* key;
    for (int i = 0; i < keys.size(); i++)
    {
        key = keys.getUnchecked(i);
        
        key->order = mode->getOrders()[i];
        keysOrder.getReference(key->order).add(key);
        
        key->scaleDegree = totalModulus(key->keyNumber - mode->getOffset(), mode->getScaleSize());
        key->modeDegree = mode->getModalDegrees()[i];
        
        key->step = mode->getStepsOfOrders()[i];
        setKeyProportions(key);
        
        key->mappedNoteIn = midiProcessor->getInputNote(i);
        key->mappedNoteOut = midiProcessor->getOutputNote(i);
        
        key->setColour(0, getKeyColor(key));
        key->setColour(1, key->findColour(0).contrasting(0.25));
        key->setColour(2, key->findColour(0).contrasting(0.75));
        
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

Mode* Keyboard::getModeInSlot()
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

void Keyboard::updateKeyMidiNotes()
{
    Key* key;
    for (int i = 0; i < keys.size(); i++)
    {
        key = keys.getUnchecked(i);
        
        key->mappedNoteIn = midiProcessor->getInputNote(i);
        key->mappedNoteIn = midiProcessor->getOutputNote(i);
    }
}

void Keyboard::setUIMode(UIMode uiModeIn)
{
    uiModeSelected = uiModeIn;
    
	Key* key;
	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
        
        if (uiModeIn == UIMode::colorMode)
        {
            key->activeState = 3;
            key->setColour(3, key->findColour(0));
            key->showNoteNumber = showNoteNumbers;
        }
        
        else if (uiModeIn == UIMode::mapMode)
        {
            key->showNoteNumber = true;
            key->activeState = 0;
        }
        
        else
        {
            key->showNoteNumber = showNoteNumbers;
            key->activeState = 0;
        }
	}
    
	pianoNode.setProperty(IDs::pianoUIMode, uiModeIn, nullptr);
    repaint();
}

void Keyboard::setKeyPlacement(KeyPlacementType placementIn)
{
	keyPlacementSelected = placementIn;
	pianoNode.setProperty(IDs::pianoKeyPlacementType, placementIn, nullptr);
	updateKeys();
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

void Keyboard::selectKeyToMap(Key* keyIn)
{
    keys.getUnchecked(lastKeyClicked)->activeState = 0;
    
    lastKeyClicked = keyIn->keyNumber;
    keyIn->setColour(4, getKeyColor(keyIn).interpolatedWith(Colours::yellow, 0.618f));
    keyIn->activeState = 4;
}

//===============================================================================================

Colour Keyboard::getKeyColor(Key* keyIn)
{
	Colour c;
	
	int offsetKeyNum = totalModulus(keyIn->keyNumber - mode->getOffset(), 128);

	// If has its own color, or else if it has a degree color, or else the default order color
	if (keyColorsSingle[offsetKeyNum].isOpaque())
		c = keyColorsSingle[offsetKeyNum];
	else if (keyColorsDegree[keyIn->scaleDegree].isOpaque())
		c = keyColorsDegree[keyIn->scaleDegree];
	else
		c = keyColorsOrder[keyIn->order % keyColorsOrder.size()];

	return c;
}

Colour Keyboard::getKeyOrderColor(int orderIn)
{
	return keyColorsOrder[orderIn % keyColorsOrder.size()];
}

Colour Keyboard::getKeyDegreeColor(int degIn)
{
	int deg = totalModulus(degIn, mode->getScaleSize());
	return keyColorsDegree[deg];
}

Colour Keyboard::getKeySingleColor(int keyIn)
{
	int keyNum = totalModulus(keyIn, keys.size());
	return keyColorsSingle[keyNum];
}

void Keyboard::beginColorEditing(Key* keyIn, int colorIndex, Colour colorIn, bool useColor)
{
	keyIn->setColour(colorIndex, colorIn);
	keyIn->customColor = useColor;
	keyIn->repaint();

	int keyNumOffset = totalModulus(keyIn->keyNumber - mode->getOffset(), 128);
	keyColorsSingle.set(keyNumOffset, colorIn);
	pianoNode.setProperty(IDs::pianoHasCustomColor, true, undo);
}

void Keyboard::beginColorEditing(int keyNumIn, int colorIndex, Colour colorIn, bool useColor)
{
	beginColorEditing(keys.getUnchecked(keyNumIn), colorIndex, colorIn, useColor);
}

void Keyboard::setKeyColorOrder(int orderIn, int colorIndex, Colour colorIn)
{
	keyColorsOrder.set(orderIn % keyColorsOrder.size(), colorIn);
	Array<Key*>* orderArray = &keysOrder.getReference(orderIn % keysOrder.size());

	Key* key;
	for (int i = 0; i < orderArray->size(); i++)
	{
		key = orderArray->getUnchecked(i);
		key->customColor = false; // reset key so that it changes no matter what
		key->setColour(colorIndex, getKeyColor(key));
		key->repaint();
	}

	pianoNode.setProperty(IDs::pianoHasCustomColor, true, undo);
}

void Keyboard::setKeyColorDegree(int tuningDegreeIn, int colorIndex, Colour colorIn)
{
	Key* key = keys.getUnchecked(tuningDegreeIn);

	int deg = key->scaleDegree;
	keyColorsDegree.set(deg, colorIn);

	// needed to display changes
	Array<Key*>* orderArray = &keysOrder.getReference(key->order);

	for (int i = 0; i < orderArray->size(); i++)
	{
		key = orderArray->getUnchecked(i);

		if (key->scaleDegree == deg && !key->customColor)
		{
			key->setColour(colorIndex, colorIn);
			key->repaint();
		}
	}

	pianoNode.setProperty(IDs::pianoHasCustomColor, true, nullptr);
}

void Keyboard::resetKeyOrderColors(int orderIn, bool resetDegrees)
{
	Key* key;
	Array<Key*>* orderArray = &keysOrder.getReference(orderIn % keysOrder.size());
	int degOffset;

	for (int i = 0; i < orderArray->size(); i++)
	{
		key = orderArray->getUnchecked(i);
		key->customColor = false;

		degOffset = (key->keyNumber + mode->getOffset()) % mode->getScaleSize();
		if (keyColorsDegree[degOffset % mode->getScaleSize()].isOpaque())
		{
			keyColorsDegree.set(degOffset % mode->getScaleSize(), Colours::transparentBlack);
		}

		key->setColour(3, getKeyOrderColor(key->order));
	}
}

void Keyboard::resetKeyDegreeColors(int tuningDegreeIn)
{
	keyColorsDegree.set(tuningDegreeIn, Colours::transparentBlack);

	// needed to display changes
	Key* key = keys.getUnchecked(tuningDegreeIn);
	Array<Key*>* orderArray = &keysOrder.getReference(key->order);

	for (int i = 0; i < orderArray->size(); i++)
	{
		key = orderArray->getUnchecked(i);

		if (key->scaleDegree == tuningDegreeIn)
		{
			key->setColour(3, getKeyOrderColor(key->order));
		}
	}

}

void Keyboard::resetKeySingleColor(int keyNumberIn)
{
	Key* key = keys.getUnchecked(totalModulus(keyNumberIn, keys.size()));
	key->customColor = false;

	int keyNumOffset = totalModulus(key->keyNumber - mode->getOffset(), 128);

	keyColorsSingle.set(keyNumOffset, Colours::transparentBlack);
	key->setColour(3, getKeyOrderColor(key->order));
}


void Keyboard::resetKeyColors(bool resetDegrees)
{
	Key* key;

	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);
		key->customColor = false;
		keyColorsSingle.set(totalModulus(key->keyNumber - modeOffset, keys.size()), Colours::transparentBlack);
	}

	if (resetDegrees)
	{
		keyColorsDegree.clear();
		pianoNode.setProperty(IDs::pianoHasCustomColor, false, nullptr);
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
		if (key->modeDegree == orderArray->getUnchecked(i)->modeDegree)
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
        noteOn(1, key->keyNumber, velocityIn);
		key->activeState = 2;
		key->velocity = velocityIn;
        keysOn.add(key);
    }
}

void Keyboard::triggerKeyNoteOff(Key* key)
{
    noteOff(1, key->keyNumber, 0);

	if (key->isMouseOver())
		key->activeState = 1;
	else
		key->activeState = 0;

	keysOn.removeAllInstancesOf(key);
}

void Keyboard::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
    Key* key;
    int keyTriggered = midiNote;

    if (uiModeSelected == playMode)
    {
        key = keys.getUnchecked(keyTriggered);
        key->externalMidiState = midiChannel;
    }
    else if (uiModeSelected == mapMode)
    {
        key = keys.getUnchecked(lastKeyClicked);
        key->mappedNoteIn = midiNote;
    }
}

void Keyboard::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
    Key* key;
    int keyTriggered = midiNote;

    if (uiModeSelected == playMode)
    {
        key = keys.getUnchecked(keyTriggered);
        key->externalMidiState = 0;
    }
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
		IDs::CommandIDs::setKeysFlat,
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
	case IDs::CommandIDs::setKeysFlat:
		result.setInfo("Keys Aligned Center", "The keys outside of the mode will be centered.", "Piano", 0);
		result.setTicked(keyPlacementSelected == KeyPlacementType::nestedCenter);
		break;
	case IDs::CommandIDs::setKeysAdjacent:
		result.setInfo("Keys Adjacent", "The keys outside of the mode will placed adjacent to each other.", "Piano", 0);
		result.setTicked(keyPlacementSelected == KeyPlacementType::adjacent);
        result.setActive(false);
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
	case IDs::CommandIDs::setKeysFlat:
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
