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

Keyboard::Keyboard()
{
    removeMouseListener(this);
       
    // Create children (virtualKeyboard keys)
    for (int i = 0; i < 128; i++)
    {
        String keyName = "Key " + String(i);
        Key* key = new Key(keyName, i);
        addChildComponent(keys.add(key));
    }
	
	keyColorsSingle.resize(keys.size());

    toBack();

    pianoNode = ValueTree(IDs::pianoNode);
    updatePianoNode();
    
    setSize(1000, 250);
    setOpaque(true);
}

//===============================================================================================

void Keyboard::restoreDataNode(ValueTree pianoNodeIn)
{
	if (pianoNodeIn.hasType(IDs::pianoNode))
	{
		pianoNode = pianoNodeIn;

		orientationSelected = pianoNode[IDs::pianoOrientation];
		keyPlacementSelected = pianoNode[IDs::pianoKeyPlacementType];
		lastKeyClicked = pianoNode[IDs::pianoLastKeyClicked];
		defaultKeyWHRatio = (float) pianoNode[IDs::pianoWHRatio];
        
        // safeguard
        if (defaultKeyWHRatio < 0.01)
            defaultKeyWHRatio = 0.25f;

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
    pianoNode.setProperty(IDs::pianoUIMode, UIMode::playMode, nullptr);
    pianoNode.setProperty(IDs::pianoMPEToggle, false, nullptr);
    pianoNode.setProperty(IDs::pianoWHRatio, 0.25f, nullptr);
    pianoNode.setProperty(IDs::pianoHasCustomColor, false, nullptr);
	
	pianoNode.setProperty(IDs::pianoOrientation, Orientation::horizontal, nullptr);
	pianoNode.setProperty(IDs::pianoKeyPlacementType, keyPlacementSelected, nullptr);
	pianoNode.setProperty(IDs::pianoLastKeyClicked, lastKeyClicked, nullptr);
    
	if (pianoNode[IDs::pianoHasCustomColor])
	{
		// Colors
		pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyColorsOrder), nullptr);
		add_array_to_node(pianoNode, keyColorsOrder, IDs::pianoKeyColorsOrder, "Color");

		pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyColorsDegree), nullptr);
		add_array_to_node(pianoNode, keyColorsDegree, IDs::pianoKeyColorsDegree, "Color");

		pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyColorSingle), nullptr);
		add_array_to_node(pianoNode, keyColorsSingle, IDs::pianoKeyColorSingle, "Color");
	}
    
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

void Keyboard::updateModeViewed(Mode* modeIn)
{
    modeViewed = modeIn;
    modeOffset = modeViewed->getOffset();
	numOrder0Keys = modeViewed->getKeyboardOrdersSize(0);
        
    keysOrder.clear();
    keysOrder.resize(modeViewed->getMaxStep());
    
    keyColorsDegree.ensureStorageAllocated(modeViewed->getScaleSize());
    
    Key* key;
    for (int i = 0; i < keys.size(); i++)
    {
        key = keys.getUnchecked(i);
        
        key->order = modeViewed->getOrders()[i];
        keysOrder.getReference(key->order).add(key);
        
        key->scaleDegree = totalModulus(key->keyNumber - modeViewed->getOffset(), modeViewed->getScaleSize());
        key->modeDegree = modeViewed->getModalDegrees()[i];
        
        key->step = modeViewed->getStepsOfOrders()[i];
    }

	// bring all keys to front with highest orders frontmost
	for (int o = 0; o < keysOrder.size(); o++)
		for (int k = 0; k < keysOrder.getUnchecked(o).size(); k++)
			keysOrder.getReference(o).getReference(k)->toFront(false);

	
	updateKeyPlacement();
}

void Keyboard::updateKeyPlacement()
{
	grid.reset(new KeyboardGrid(modeViewed));

	Key* key;
	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);

		setKeyProportions(key);
		key->setVisible(true);
	}

	// Calculate properties
	displayIsReady = true;
	resized();
}

void Keyboard::updateKeyColors()
{
    Key* key;
    for (int i = 0; i < keys.size(); i++)
    {
        key = keys.getUnchecked(i);
        
        key->setColour(0, getKeyColor(key));
        key->setColour(1, key->findColour(0).contrasting(0.25));
        key->setColour(2, key->findColour(0).contrasting(0.75));
    }

	repaint();
}

void Keyboard::updateKeyboard(Mode* modeIn)
{
    updateModeViewed(modeIn);
    updateKeyColors();
}

void Keyboard::updateKeyMapping(const MidiFilter* inputMap, const MidiFilter* outputMap)
{
	Key* key;
	for (int i = 0; i < keys.size(); i++)
	{
		key = keys.getUnchecked(i);

		key->mappedNoteIn = inputMap->getNoteRemapped(i);
		key->mappedNoteOut = outputMap->getNoteRemapped(i);
	}
}

//===============================================================================================

Key* Keyboard::getKey(int keyNumIn)
{
	return keys.getUnchecked(keyNumIn);
}

Array<Key*>* Keyboard::getKeysByOrder(int orderIn, Mode* modeToReference)
{
    // needs to reference mode
	return &keysOrder.getReference(orderIn & keysOrder.size());
}

Array<Key*> Keyboard::getKeysByScaleDegree(int scaleDegreeIn, Mode* modeToReference)
{
    if (!modeToReference)
        modeToReference = modeViewed;
    
	int deg = scaleDegreeIn % modeToReference->getScaleSize();
    
	Key* key = keys.getUnchecked(deg);
	Array<Key*>* orderArray = &keysOrder.getReference(key->order);
	Array<Key*> degreeArray;

	for (int i = 0; i < orderArray->size(); i++)
	{
		key = orderArray->getUnchecked(i);

		if (key->keyNumber % modeToReference->getScaleSize() == deg)
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

int Keyboard::getWidthFromHeight(int heightIn)
{
    int wOut = 100;
    
    if (displayIsReady)
        wOut = modeViewed->getKeyboardOrdersSize(0) * heightIn * defaultKeyWHRatio;
    
    return wOut;
}

//===============================================================================================

int Keyboard::getUIMode()
{
	return uiModeSelected;
}

int Keyboard::getKeyPlacementStyle()
{
	return keyPlacementSelected;
}

int Keyboard::isShowingNoteNumbers()
{
	return showNoteNumbers;
}

int Keyboard::getHighlightStyle()
{
	return highlightSelected;
}

//===============================================================================================

void Keyboard::setModes(Mode* mode1In, Mode* mode2In)
{
    mode1 = mode1In;
    mode2 = mode2In;
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
    
    waitingForKeyMapInput = false;
    
	pianoNode.setProperty(IDs::pianoUIMode, uiModeIn, nullptr);
    repaint();
}

void Keyboard::setKeyPlacementStyle(int placementIn)
{
	keyPlacementSelected = placementIn;
	pianoNode.setProperty(IDs::pianoKeyPlacementType, placementIn, nullptr);
	updateKeyPlacement();
}

void Keyboard::setKeyProportions(Key* keyIn)
{
	float spread = 4;
	float stepHeight;
	float keyHeight;
	float width;

	switch (keyPlacementSelected)
	{

	case KeyPlacementType::nestedCenter:
		break;



	case KeyPlacementType::flat:

		stepHeight = 0.55 + (keyIn->step - 2) / 100.0f * spread;
		keyHeight = stepHeight - stepHeight * (keyIn->order - 1) / keyIn->step;
		width = 0.8;

		break;

	default: // Nested Right

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

void Keyboard::setNoteNumbersVisible(bool showNoteNumsIn)
{
	showNoteNumbers = showNoteNumsIn;
	pianoNode.setProperty(IDs::pianoKeysShowNoteNumbers, showNoteNumbers, nullptr);

	for (int i = 0; i < keys.size(); i++)
	{
		keys.getUnchecked(i)->showNoteNumber = showNoteNumbers;
		//keys.getUnchecked(i)->isKeyboardOut = true;
	}

	repaint();
}

void Keyboard::setHighlightStyle(int styleIn)
{
	highlightSelected = styleIn;
	pianoNode.setProperty(IDs::pianoKeysHighlightStyle, highlightSelected, nullptr);
}

void Keyboard::setMidiChannelOut(int midiChannelOutIn)
{
	midiChannelOut = jlimit(1, 16, midiChannelOutIn);
}

//===============================================================================================

int Keyboard::getMidiChannelOut()
{
	return midiChannelOut;
}

//===============================================================================================

void Keyboard::selectKeyToMap(Key* keyIn, bool mapAllPeriods)
{
    waitingForKeyMapInput = true;
    
    if (mapAllPeriods)
    {
        keysToMap = getKeysByScaleDegree(keyIn->scaleDegree);
    }
    else
    {
        keysToMap = Array<Key*>(keyIn);
    }
    
    keys.getUnchecked(lastKeyClicked)->activeState = 0;
    lastKeyClicked = keyIn->keyNumber;
    
    highlightKeysForMapping(keysToMap);
}

void Keyboard::highlightKeysForMapping(Array<Key*> keysToHighlight, bool highlightOn)
{
    Key* key;
    for (int i = 0; keysToHighlight.size(); i++)
    {
        key = keysToHighlight[i];
        
        if (highlightOn)
        {
            key->setColour(4, getKeyColor(key).interpolatedWith(Colours::yellow, 0.618f));
            key->activeState = 4;
        }
        else
        {
            key->activeState = 0;
        }
    }
}

//===============================================================================================

Colour Keyboard::getKeyColor(Key* keyIn)
{
	Colour c;
	
	int offsetKeyNum = totalModulus(keyIn->keyNumber - modeViewed->getOffset(), 128);

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
	int deg = totalModulus(degIn, modeViewed->getScaleSize());
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

	int keyNumOffset = totalModulus(keyIn->keyNumber - modeViewed->getOffset(), 128);
	keyColorsSingle.set(keyNumOffset, colorIn);
	pianoNode.setProperty(IDs::pianoHasCustomColor, true, nullptr);
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

	pianoNode.setProperty(IDs::pianoHasCustomColor, true, nullptr);
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

		degOffset = (key->keyNumber + modeViewed->getOffset()) % modeViewed->getScaleSize();
		if (keyColorsDegree[degOffset % modeViewed->getScaleSize()].isOpaque())
		{
			keyColorsDegree.set(degOffset % modeViewed->getScaleSize(), Colours::transparentBlack);
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

	int keyNumOffset = totalModulus(key->keyNumber - modeViewed->getOffset(), 128);

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
		pianoWidth = numOrder0Keys * keyWidth;
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
        noteOn(midiChannelOut, key->keyNumber, velocityIn);
		key->activeState = 2;
		key->velocity = velocityIn;
        keysOn.add(key);
    }
}

void Keyboard::triggerKeyNoteOff(Key* key)
{
    noteOff(midiChannelOut, key->keyNumber, 0);

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
    else if (uiModeSelected == mapMode && waitingForKeyMapInput)
    {
        int notePeriod = midiNote / mode1->getScaleSize();
        
        for (int i = 0; i < keysToMap.size(); i++)
        {
            key = keysToMap[i];
            key->mappedNoteIn = midiNote + (i - notePeriod) * mode1->getScaleSize();
        }
        
        highlightKeysForMapping(keysToMap, false);
        waitingForKeyMapInput = false;
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
