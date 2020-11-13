/*
 ==============================================================================
 
 VirtualKeyboard.h
 Created: 14 Mar 2019 4:50:31pm
 Author:  Vincenzo
 
 ==============================================================================
 */

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "KeyboardComponent.h"

using namespace VirtualKeyboard;

Keyboard::Keyboard()
    : keyPositioner(this)
{
    setName("VirtualKeyboard");
    addMouseListener(this, true);
    setWantsKeyboardFocus(true);

    initializeKeys();
    reset();

    setSize(1000, 250);
    setOpaque(true);
}

Keyboard::Keyboard(ValueTree keyboardNodeIn, Mode* modeIn, NoteMap* inputFilterMapIn)
    : keyPositioner(this)
{
    setName("VirtualKeyboard");
    addMouseListener(this, true);
    setWantsKeyboardFocus(true);

    initializeKeys(); // todo: load keyboard size
    restoreNode(keyboardNodeIn, true, true);
    
    if (modeIn)
        mode = modeIn;

    applyMode(mode);

    setSize(1000, 250);
    setOpaque(true);
}

Keyboard::~Keyboard()
{
    if (externalKeyboardToDisplay)
        externalKeyboardToDisplay->removeListener(this);
}

//===============================================================================================

void Keyboard::restoreNode(ValueTree pianoNodeIn, bool reinitializeKeys, bool resetIfInvalid)
{
    if (pianoNodeIn.hasType(IDs::pianoNode))
    {        
        pianoNode = pianoNodeIn;
        DBG("RESTORING KEYBOARD NODE: " + pianoNode.toXmlString());
        
        showNoteNumbers = pianoNode[IDs::pianoKeysShowNoteNumbers];
        orientationSelected = pianoNode[IDs::keyboardOrientation];
        keyPlacementSelected = pianoNode[IDs::keyboardKeysStyle];
        highlightSelected = pianoNode[IDs::keyboardHighlightStyle];
        lastKeyClicked = pianoNode[IDs::pianoLastKeyClicked];
        keySizeRatio = (float) pianoNode[IDs::pianoWHRatio];

        keyPositioner.setKeyPlacement(keyPlacementSelected);
        
        // safeguard
        if (keySizeRatio < 0.01)
            keySizeRatio = 0.25f;
        
        if (reinitializeKeys)
        {
            // TODO: keyboard key editing
            pianoNode.removeChild(pianoNode.getChildWithName(IDs::pianoKeyTreeNode), nullptr);
            // unpack key data
            //keyTreeNode = pianoNode.getOrCreateChildWithName(IDs::pianoKeyTreeNode, nullptr);

            //if (keyTreeNode.getNumChildren() == keys.size())
            //{
            //    for (auto key : keys)
            //        key->applyParameters(keyTreeNode.getChild(key->keyNumber));
            //}
            //else
            //{
            //    keyTreeNode.removeAllChildren(nullptr);

            initializeKeys();

            //for (auto key : keys)
            //{
            //    keyTreeNode.addChild(key->node, key->keyNumber, nullptr);
            //}
            //}        
        }
    }
    else
    {
        DBG("Invalid Piano Node.");
        if (resetIfInvalid)
        {
            reset();
            DBG("Keyboard is reset.");
        }
    }
}

void Keyboard::reset()
{
    pianoNode = ValueTree(IDs::pianoNode);

    setUIMode(UIMode::playMode);
    setOrientation(Orientation::horizontal);
    setKeyStyle(KeyPlacementType::nestedRight);
    setHighlightStyle(HighlightStyle::full);
    setVelocityBehavior(VelocityStyle::linear);
    setScrollingStyle(ScrollingStyle::smooth);

    setShowNoteLabels(false);
    setShowNoteNumbers(false);
    setShowFilteredNumbers(false);
    
    setMidiChannelOut(1);
    setVelocityFixed(1);
    setInputVelocityScaled(false);

    setKeySizeRatio(0.25f);
    setKeyOrderSizeScalar(1);

    keyOnColorsByChannel.resize(16);
    keyOnColorsByChannel.fill(Colour());

    mode = &modeDefault;
    applyMode(mode);
}

void Keyboard::initializeKeys(int size)
{
    keys.clear();

    for (int i = 0; i < size; i++)
    {
        Key* key = keys.add(new Key(i));
        addAndMakeVisible(key); 
        key->showNoteNumber = showNoteNumbers; // hacky ?
        key->highlightStyleId = highlightSelected;
    }

    keyColorsIndividual.resize(keys.size());

    keysOn = Array<int>();
    keysByMouseTouch = Array<int>();
}

//===============================================================================================

void Keyboard::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void Keyboard::paintOverChildren(juce::Graphics& g)
{
    g.setColour(Colours::black);
    g.drawRect(getLocalBounds(), 1);
}

void Keyboard::resized()
{
    keyPositioner.parentResized();

    keyWidth = keyPositioner.getKeyWidth();
    keyHeight = getHeight();

    keyPositioner.setLayerKeysWidthRatio(layerKeysWidthRatio);
    keyPositioner.setLayerKeysHeightRatio(layerKeysHeightRatio);

    for (int i = 0; i < keys.size(); i++)
    {
        Key* key = keys[i];
        keyPositioner.resizeKey(*key);
    }

    float keyOrder1Width = keyWidth * layerKeysWidthRatio;
    float adjacentStyleStepWidth;

    // Place Keys
    Array<int>& modeKeys = keysOrder.getReference(0);
    for (int i = 0; i < numOrder0Keys; i++)
    {
        Key& modeKey = *keys[modeKeys[i]];

        int keyX = (keyWidth + keyPositioner.getKeyGap() - 1) * i;
        modeKey.setTopLeftPosition(keyX, 0);

        if (modeKey.step > 1)
        {
            adjacentStyleStepWidth = keyOrder1Width / (modeKey.step - 1);

            for (int s = 1; s < modeKey.step; s++)
            {
                Key* stepKey = keys[modeKey.keyNumber + s];

                if (s > 1 && keyPlacementSelected == KeyPlacementType::nestedRight)
                {
                    keyX = modeKey.getX() + keyWidth + (keys[modeKey.keyNumber + 1]->getWidth() / 2) - stepKey->getWidth();
                }
                else if (stepKey->step > 2 && keyPlacementSelected == KeyPlacementType::adjacent)
                {
                    keyX = modeKey.getX() + keyWidth - (keyOrder1Width / 2) + (stepKey->order - 1) * adjacentStyleStepWidth;
                }
                else
                {
                    keyX = modeKey.getX() + keyWidth - stepKey->getWidth() / 2;
                }

                stepKey->setTopLeftPosition(keyX, 0);
            }
        }
    }
}

//===============================================================================================

void Keyboard::mouseMove(const MouseEvent& e)
{
    Key* key = getKeyFromPositionMouseEvent(e);

    if (uiModeSelected != UIMode::editMode)
    {
        if (key)
        {
            lastKeyOver = key->keyNumber;
        }
    }

    else
    {
        if (key)
        {
            lastKeyOver = key->keyNumber;
            setMouseCursor(MouseCursor::CrosshairCursor);
        }
        else
        {
            setMouseCursor(MouseCursor::NormalCursor);
        }
    }
}

void Keyboard::mouseExit(const MouseEvent& e)
{
    if (uiModeSelected != UIMode::editMode)
    {
        int touchIndex = e.source.getIndex();
        if (keysByMouseTouch[touchIndex] > 0)
        {
            Key* key = keys[keysByMouseTouch[touchIndex]];

            if (!e.source.isDragging())
            {
                triggerKey(key->keyNumber, false);
                keysByMouseTouch.set(touchIndex, -1);
            }
        }

        Key* key = keys[lastKeyOver];
        key->repaint();
    }
}

void Keyboard::mouseDown(const MouseEvent& e)
{
    Key* key = getKeyFromPositionMouseEvent(e);
    bool triggerNote = false;

    if (uiModeSelected == UIMode::editMode)
    {
        if (key)
        {
            if (e.mods.isRightButtonDown() || getProperties()[IDs::pianoKeyColorReset])
            {
                if (e.mods.isShiftDown())
                    keyColorsOrders.set(key->order, Colour());

                else if (e.mods.isCtrlDown())
                    keyColorsIndividual.set(key->keyNumber, Colour());

                else
                {
                    if (keyColorsIndividual[key->keyNumber].isOpaque())
                        keyColorsIndividual.set(key->keyNumber, Colour());

                    keyColorsDegrees.set(key->scaleDegree, Colour());
                }
            }

            else if (e.mods.isCtrlDown() || (int)getProperties()[IDs::pianoKeyPaintType] == 2)
            {
                keyColorsIndividual.set(key->keyNumber, Colour::fromString(getProperties()[IDs::colorSelected].toString()));
            }

            else if (e.mods.isShiftDown() || (int)getProperties()[IDs::pianoKeyPaintType] == 1)
            {
                if (getKeyDegreeColor(key->scaleDegree).isOpaque())
                    keyColorsDegrees.set(key->scaleDegree, Colour());

                else if (getKeyColor(key->keyNumber).isOpaque())
                    keyColorsIndividual.set(key->keyNumber, Colour());

                keyColorsOrders.set(key->order, Colour::fromString(getProperties()[IDs::colorSelected].toString()));
            }

            else
            {
                if (keyColorsIndividual[key->keyNumber].isOpaque())
                    keyColorsIndividual.set(key->keyNumber, Colour());

                keyColorsDegrees.set(key->scaleDegree, Colour::fromString(getProperties()[IDs::colorSelected].toString()));
            }

            updateKeyColors();
        }
    }
    else if (uiModeSelected == UIMode::mapMode)
    {
        if (key)
        {
            if (e.mods.isRightButtonDown())
            {
                bool allPeriods = true;

                if (e.mods.isCtrlDown())
                    allPeriods = false;

                manualMappingHelper->prepareKeyToMap(key->keyNumber, false);
                highlightKey(key->keyNumber);

                DBG("Preparing to map key: " + String(key->keyNumber));
            }
            else
            {
                triggerNote = true;
            }
        }
        
        if (!key || !isMouseOver(true))
        {
            highlightKey(manualMappingHelper->getVirtualKeyToMap(), false);
            manualMappingHelper->cancelKeyMap();
        }
    }

    if (uiModeSelected == UIMode::playMode || triggerNote)
    {
        if (key)
        {
            if (e.mods.isShiftDown() && !e.mods.isAltDown() && key->isClicked)
            {
                // note off
                lastKeyClicked = key->keyNumber;
                triggerKey(key->keyNumber, false);
            }
            else
            {
                // need to implement a velocity class
                triggerKey(key->keyNumber);
                lastKeyClicked = key->keyNumber;
                keysByMouseTouch.set(e.source.getIndex(), key->keyNumber);

                if (e.mods.isAltDown())
                {
                    isolateLastNote();
                }
            }
        }
    }
}

void Keyboard::mouseDrag(const MouseEvent& e)
{
    if (uiModeSelected != UIMode::editMode && !e.mods.isRightButtonDown())
    {
        Key* key = getKeyFromPositionMouseEvent(e);

        if (key)
        {
            //bool alreadyDown = false;
            int touchIndex = e.source.getIndex();
            int keyIndex = keysByMouseTouch[touchIndex];

            if (key->keyNumber != keyIndex)
            {
                if (!e.mods.isShiftDown())
                    triggerKey(keyIndex, false);
                
                triggerKey(key->keyNumber);
                lastKeyClicked = key->keyNumber;
                keysByMouseTouch.set(touchIndex, key->keyNumber);
            }
        }
    }

}

void Keyboard::mouseUp(const MouseEvent& e)
{
    if (uiModeSelected != UIMode::editMode)
    {
        int touchIndex = e.source.getIndex();
        int keyIndex = keysByMouseTouch[touchIndex];

        if (keyIndex > 0)
        {
            Key* key = keys[keyIndex];

            if (key)// && mappingHelper->getVirtualKeyToMap() != key->keyNumber)
            {
                keysByMouseTouch.set(touchIndex, -1);

                if (!shiftHeld)
                {
                    triggerKey(key->keyNumber, false);
                }
            }
        }
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
        if (uiModeSelected != UIMode::editMode)
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
        if (uiModeSelected != UIMode::editMode)
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
        if (uiModeSelected != UIMode::editMode)
        {
            if (shiftHeld && keysOn.size() > 0)
            {
                transposeKeysOnModally(-1);
            }
        }
        return true;
    }

    if (KeyPress::isKeyCurrentlyDown(KeyPress::rightKey) && !rightHeld)
    {
        rightHeld = true;
        if (uiModeSelected != UIMode::editMode)
        {
            if (shiftHeld && keysOn.size() > 0)
            {
                transposeKeysOnModally(1);
            }
        }
        return true;
    }

    if (KeyPress::isKeyCurrentlyDown(KeyPress::spaceKey) && !spaceHeld)
    {
        spaceHeld = true;
        if (uiModeSelected != UIMode::editMode)
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

        if (uiModeSelected != UIMode::editMode)
        {
            isolateLastNote();

            Key* key = keys[lastKeyClicked];

            if (!(key->isMouseOver() && key->isMouseButtonDownAnywhere()))
                triggerKey(lastKeyClicked, false);

        }
    }

    if (!altHeld && modifiers.isAltDown())
    {
        altHeld = true;
        if (uiModeSelected != UIMode::editMode)
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

//void Keyboard::timerCallback()
//{
    //if (hasDirtyKeys)
    //{
    //    for (auto key : *keys.get())
    //    {
    //        if (key->isDirty)
    //        {
    //            key->repaint();
    //        }
    //    }
    //}

    //hasDirtyKeys = false;
//}

//===============================================================================================

ValueTree Keyboard::getNode()
{
    return pianoNode;
}

Key* Keyboard::getKey(int keyNumIn)
{
    return keys[keyNumIn];
}

ValueTree Keyboard::getKeyNode(int keyNumIn)
{
    return keys[keyNumIn]->node;
}

Array<Key*> Keyboard::getKeysByOrder(int orderIn)
{
    Array<Key*> keysOut;
    Array<int> orderArray = keysOrder[orderIn];

    for (int i = 0; i < orderArray.size(); i++)
    {
        keysOut.add(keys[orderArray[i]]);
    }

    return keysOut;
}

Array<Key*> Keyboard::getKeysByScaleDegree(int degreeIn)
{
    Array<Key*> keysOut;
    Array<int> degreeArray = keysScaleDegree[degreeIn];

    for (int i = 0; i < degreeArray.size(); i++)
    {
        keysOut.add(keys[degreeArray[i]]);
    }

    return keysOut;
}

Array<Key*> Keyboard::getKeysByModalDegree(int degreeIn)
{
    Array<Key*> keysOut;
    Array<int> degreeArray = keysModalDegree[degreeIn];

    for (int i = 0; i < degreeArray.size(); i++)
    {
        keysOut.add(keys[degreeArray[i]]);
    }

    return keysOut;
}

int Keyboard::getLastKeyClicked()
{
    return lastKeyClicked;
}

Rectangle<int> Keyboard::getKeyArea(int midiNoteIn)
{
    // TODO
    return Rectangle<int>();
}

Rectangle<int> Keyboard::getKeyAreaRelative(int midiNoteIn)
{
    // TODO
    return Rectangle<int>();
}

int Keyboard::getKeyWidth()
{
    return keyWidth;
}

int Keyboard::getKeyHeight()
{
    return keyHeight;
}

int Keyboard::getPianoWidth(int heightIn)
{
    return numOrder0Keys  * (heightIn * keySizeRatio + keyPositioner.getKeyGap() - 1);
}

float Keyboard::getKeySizeRatio()
{
    return keySizeRatio;
}

Point<int> Keyboard::getKeyOrderSize(int orderIn)
{
    //TODO (use the scalar)
 return Point<int>();
}

Key* Keyboard::getKeyFromPosition(Point<int> posIn)
{
    Key* keyOut = nullptr;
    Component* c = getComponentAt(posIn);

    if (c != nullptr && c->getName().startsWith("Key"))
        keyOut = dynamic_cast<Key*>(c);

    return keyOut;
}

Key* Keyboard::getKeyFromPositionMouseEvent(const MouseEvent& e)
{    
    Point<int> ep = Point<int>(e.getScreenX() - getScreenX(), e.getPosition().getY());
    return getKeyFromPosition(ep);
}

float Keyboard::getKeyVelocity(Key* keyIn, Point<int> posIn)
{
    //TODO
    return velocityFixed;
}

int Keyboard::getMidiChannelOut() const
{
    return midiChannelOut;
}

int Keyboard::getModeSize() const
{
    return mode->getModeSize();
}

//===============================================================================================

int Keyboard::getUIMode()
{
    return uiModeSelected;
}

int Keyboard::getOrientation()
{
    return orientationSelected;
}

int Keyboard::getKeyPlacementStyle()
{
    return keyPlacementSelected;
}

bool Keyboard::isShowingNoteNumbers()
{
    return showNoteNumbers;
}

bool Keyboard::isShowingFilteredNumbers()
{
    return showFilteredNoteNums;
}

bool Keyboard::isShowingNoteNames()
{
    return showNoteLabels;
}

int Keyboard::getHighlightStyle()
{
    return highlightSelected;
}

int Keyboard::getVelocityStyle()
{
    return velocitySelected;
}

float Keyboard::getVelocityFixed()
{
    return velocityFixed;
}

bool Keyboard::isInputVelocityScaled()
{
    return scaleMidiInputVelocity;
}

int Keyboard::getScrollingStyle()
{
    return scrollingSelected;
}


//===============================================================================================


void Keyboard::applyMode(Mode* modeIn, bool resize)
{
    mode = modeIn;

    if (mode == nullptr)
        mode = &modeDefault;

    keysOrder.clear();
    keysOrder.resize(mode->getMaxStep());

    int period = 0;

    for (int i = 0; i < keys.size(); i++)
    {
        Key* key = keys[i];
        period = i / mode->getScaleSize();

        key->order = mode->getOrder(i);
        key->scaleDegree = mode->getScaleDegree(i);
        key->modeDegree = mode->getModeDegree(i);
        key->step = mode->getNoteStep(i);

        keysOrder.getReference(key->order).add(key->keyNumber);
    }

    numOrder0Keys = keysOrder.getReference(0).size();
    keyPositioner.setNumModeKeys(numOrder0Keys);

    keysScaleDegree = mode->getNotesInScaleDegrees();
    keysModalDegree = mode->getNotesInModalDegrees();

    // add from back to front
    for (int o = 0; o < keysOrder.size(); o++)
    {
        Array<int>& orderArray = keysOrder.getReference(o);

        for (int keyNum = 0; keyNum < orderArray.size(); keyNum++)
        {
            Key* key = keys[(orderArray.getReference(keyNum))];
            //addAndMakeVisible(key);
            key->toFront(false);
        }
    }

    keyColorsOrders.resize(mode->getMaxStep());
    keyColorsDegrees.resize(mode->getScaleSize());

    // unpack color data
    ValueTree keyColorsNode = mode->modeNode.getOrCreateChildWithName(IDs::pianoKeyColorsNode, nullptr);

    keyColorsOrders.fill(Colour());
    for (auto layerColor : keyColorsNode.getChildWithName(IDs::pianoKeyColorsLayer))
    {
        keyColorsOrders.set(
            layerColor[IDs::pianoKeyColorsLayer], 
            Colour::fromString(layerColor[IDs::pianoKeyColor].toString())
        );
    }

    keyColorsDegrees.fill(Colour());
    for (auto degreeColor : keyColorsNode.getChildWithName(IDs::pianoKeyColorsDegree))
    {
        keyColorsDegrees.set(
            degreeColor[IDs::pianoKeyColorsDegree],
            Colour::fromString(degreeColor[IDs::pianoKeyColor].toString())
        );
    }

    keyColorsIndividual.fill(Colour());
    for (auto keyColor : keyColorsNode.getChildWithName(IDs::pianoKeyColorsIndividual))
    {
        keyColorsIndividual.set(
            mode->fixedDegreeToNoteNumber(keyColor[IDs::pianoKeyColorsIndividual]),
            Colour::fromString(keyColor[IDs::pianoKeyColor].toString())
        );
    }

    keyOnColorsByChannel.fill(Colour());
    for (auto noteOnColor : keyColorsNode.getChildWithName(IDs::pianoKeyColorsNoteOn))
    {
        keyOnColorsByChannel.set(
            noteOnColor[IDs::pianoKeyColorsNoteOn],
            Colour::fromString(noteOnColor[IDs::pianoKeyColor].toString())
        );
    }

    updateKeyColors();

    if (resize)
        resized();
    else
        hasDirtyKeys = true;
}

void Keyboard::applyKeyData(ValueTree keyDataTreeIn)
{
    ValueTree keyNode;

    int numKeys = jmin(keys.size(), keyDataTreeIn.getNumChildren());

    for (int i = 0; i < numKeys; i++)
    {
        Key* key = keys[i];
        key->applyParameters(keyDataTreeIn.getChild(i));
    }
}

void Keyboard::displayKeyboardState(MidiKeyboardState* keyboardStateIn)
{
    if (externalKeyboardToDisplay)
        externalKeyboardToDisplay->removeListener(this);

    externalKeyboardToDisplay = keyboardStateIn;

    if (externalKeyboardToDisplay)
        externalKeyboardToDisplay->addListener(this);
}

//===============================================================================================

void Keyboard::setUIMode(int uiModeIn)
{
    if (uiModeSelected == UIMode::editMode && uiModeIn != uiModeSelected)
        updateKeyColors();

    uiModeSelected = uiModeIn;
    pianoNode.setProperty(IDs::pianoUIMode, uiModeSelected, nullptr);
 }

void Keyboard::setOrientation(int orientationIn)
{
    orientationSelected = orientationIn;
    pianoNode.setProperty(IDs::keyboardOrientation, orientationSelected, nullptr);

    // do stuff
}

void Keyboard::setNumRows(int numRowsIn)
{
    numRows = jlimit(1, 16, numRowsIn);
    pianoNode.setProperty(IDs::keyboardNumRows, numRows, nullptr);
    applyMode(mode);
}

void Keyboard::setKeyStyle(int placementIn)
{
    keyPlacementSelected = placementIn;
    keyPositioner.setKeyPlacement(keyPlacementSelected);
    pianoNode.setProperty(IDs::keyboardKeysStyle, keyPlacementSelected, nullptr);
}

void Keyboard::setHighlightStyle(int styleIn)
{
    highlightSelected = styleIn;
    pianoNode.setProperty(IDs::keyboardHighlightStyle, highlightSelected, nullptr);

    for (auto key : keys)
    {
        key->highlightStyleId = highlightSelected;
        key->repaint();
    }
}

void Keyboard::setVelocityBehavior(int behaviorNumIn, bool scaleInputVelocity)
{
    velocitySelected = behaviorNumIn;
    pianoNode.setProperty(IDs::pianoVelocityBehavior, velocitySelected, nullptr);

    scaleMidiInputVelocity = scaleInputVelocity;
    pianoNode.setProperty(IDs::pianoVelocityScaleInput, scaleMidiInputVelocity, nullptr);
}

void Keyboard::setScrollingStyle(int scrollingStyleIn)
{
    scrollingSelected = scrollingStyleIn;
    pianoNode.setProperty(IDs::keyboardScrollingStyle, scrollingSelected, nullptr);

    // do stuff?
}

void Keyboard::setMidiChannelOut(int midiChannelOutIn)
{
    midiChannelOut = jlimit(1, 16, midiChannelOutIn);
    pianoNode.setProperty(IDs::keyboardMidiChannel, midiChannelOut, nullptr);
}

void Keyboard::setVelocityFixed(float velocityIn)
{
    velocityFixed = velocityIn;
    pianoNode.setProperty(IDs::pianoVelocityValue, velocityFixed, nullptr);
}

void Keyboard::setInputVelocityScaled(bool shouldBeScaled)
{
    scaleMidiInputVelocity = shouldBeScaled;
    pianoNode.setProperty(IDs::pianoVelocityScaleInput, scaleMidiInputVelocity, nullptr);
}

void Keyboard::setShowNoteNumbers(bool shouldShowNumbers)
{
    showNoteNumbers = shouldShowNumbers;
    pianoNode.setProperty(IDs::pianoKeysShowNoteNumbers, showNoteNumbers, nullptr);

    for (int i = 0; i < keys.size(); i++)
    {
        Key* key = keys[i];
        key->showNoteNumber = shouldShowNumbers;
        key->repaint();
    }
}

void Keyboard::setShowFilteredNumbers(bool shouldShowNumbers)
{
    showFilteredNoteNums = shouldShowNumbers;
    pianoNode.setProperty(IDs::pianoKeysShowFilteredNotes, showFilteredNoteNums, nullptr);
}

void Keyboard::setShowNoteLabels(bool shouldShowPitchNames)
{
    showNoteLabels = shouldShowPitchNames;
    pianoNode.setProperty(IDs::keyboardShowsNoteLabels, showNoteLabels, nullptr);
}

void Keyboard::setLastKeyClicked(int keyNumIn)
{
    lastKeyClicked = keyNumIn;
    pianoNode.setProperty(IDs::pianoLastKeyClicked, lastKeyClicked, nullptr);
}

void Keyboard::setInputNoteMap(NoteMap& noteMapIn)
{
    noteMapOnDisplay = noteMapIn;
}

//===============================================================================================

void Keyboard::setMappingHelper(MappingHelper* mappingHelperIn)
{
    manualMappingHelper = mappingHelperIn;
}

void Keyboard::highlightKey(int keyNumberIn, bool highlightOn)
{
    if (keyNumberIn < 0 || keyNumberIn >= keys.size())
        return;

    if (highlightOn)
        highlightedKeys.add(keys[keyNumberIn]);
    else
        highlightedKeys.removeAllInstancesOf(keys[keyNumberIn]);
}

//===============================================================================================

Array<Colour>* Keyboard::getKeyIndividualColours()
{
    return &keyColorsIndividual;
}

Colour Keyboard::getKeyColor(int keyNumIn)
{
    Colour c;

    if (keyNumIn >= 0 && keyNumIn < keys.size())
    {
        Key* key = keys[keyNumIn];

        if (keyColorsIndividual[key->keyNumber].isOpaque()) // need to fix this for switching modes
            c = keyColorsIndividual[key->keyNumber];

        else if (keyColorsDegrees[key->scaleDegree].isOpaque())
            c = keyColorsDegrees[key->scaleDegree];

        else if (keyColorsOrders[key->order].isOpaque())
            c = keyColorsOrders[key->order];

        else
            c = colorsDefaultOrders[key->order];
    }

  return c;
}

Array<Colour>* Keyboard::getKeyLayerColours()
{
    return &keyColorsOrders;
}

Colour Keyboard::getKeyLayerColor(int orderIn)
{
    Colour c;

    if (keyColorsOrders[orderIn].isOpaque())
            c = keyColorsOrders[orderIn];

    else
        c = colorsDefaultOrders[orderIn];

    return c;
}

Array<Colour>* Keyboard::getKeyDegreeColours()
{
    return &keyColorsDegrees;
}

Colour Keyboard::getKeyDegreeColor(int degIn)
{
    Colour c;

    if (keyColorsDegrees[degIn].isOpaque())
            c = keyColorsDegrees[degIn];
    
    else // this deg->order conversion is probably wrong
        c = colorsDefaultOrders[mode->getOrder(degIn)];

    return c;
}

Array<Colour>* Keyboard::getKeyNoteOnColours()
{
    return &keyOnColorsByChannel;
}

void Keyboard::updateKeyColors(bool writeToNode)
{
    for (int i = 0; i < keys.size(); i++)
    {
        Key* key = keys[i];
         key->setDisplayColor(getKeyColor(i));
    }

    if (writeToNode)
    {
        ValueTree keyColorsNode = mode->modeNode.getOrCreateChildWithName(IDs::pianoKeyColorsNode, nullptr);

        ValueTree layersColorNode = ValueTree(IDs::pianoKeyColorsLayer);
        for (int i = 0; i < keyColorsOrders.size(); i++)
        {
            if (keyColorsOrders[i].isOpaque())
            {
                ValueTree node(IDs::pianoKeyColor);
                node.setProperty(IDs::pianoKeyColorsLayer, i, nullptr);
                node.setProperty(IDs::pianoKeyColor, keyColorsOrders[i].toString(), nullptr);
                layersColorNode.appendChild(node, nullptr);
            }
        }
        keyColorsNode.getOrCreateChildWithName(IDs::pianoKeyColorsLayer, nullptr).copyPropertiesAndChildrenFrom(layersColorNode, nullptr);


        ValueTree degreesColorNode = ValueTree(IDs::pianoKeyColorsDegree);
        for (int i = 0; i < keyColorsDegrees.size(); i++)
        {
            if (keyColorsDegrees[i].isOpaque())
            {
                ValueTree node(IDs::pianoKeyColor);
                node.setProperty(IDs::pianoKeyColorsDegree, i, nullptr);
                node.setProperty(IDs::pianoKeyColor, keyColorsDegrees[i].toString(), nullptr);
                degreesColorNode.appendChild(node, nullptr);
            }
        }
        keyColorsNode.getOrCreateChildWithName(IDs::pianoKeyColorsDegree, nullptr).copyPropertiesAndChildrenFrom(degreesColorNode, nullptr);

        ValueTree individualColorNode = ValueTree(IDs::pianoKeyColorsIndividual);
        for (int i = 0; i < keyColorsIndividual.size(); i++)
        {
            if (keyColorsIndividual[i].isOpaque())
            {
                ValueTree node(IDs::pianoKeyColor);
                node.setProperty(IDs::pianoKeyColorsIndividual, mode->getFixedDegree(i), nullptr);
                node.setProperty(IDs::pianoKeyColor, keyColorsIndividual[i].toString(), nullptr);
                individualColorNode.appendChild(node, nullptr);
            }
        }
        keyColorsNode.getOrCreateChildWithName(IDs::pianoKeyColorsIndividual, nullptr).copyPropertiesAndChildrenFrom(individualColorNode, nullptr);

        ValueTree keyOnColorNode = ValueTree(IDs::pianoKeyColorsNoteOn);
        for (int i = 0; i < 16; i++)
        {
            if (keyOnColorsByChannel[i].isOpaque())
            {
                ValueTree node(IDs::pianoKeyColor);
                node.setProperty(IDs::pianoKeyColorsNoteOn, i, nullptr);
                node.setProperty(IDs::pianoKeyColor, keyOnColorsByChannel[i].toString(), nullptr);
                keyOnColorNode.appendChild(node, nullptr);
            }
        }
        keyColorsNode.getOrCreateChildWithName(IDs::pianoKeyColorsNoteOn, nullptr).copyPropertiesAndChildrenFrom(keyOnColorNode, nullptr);
    }
}

void Keyboard::resetLayerColors()
{
    keyColorsOrders.fill(Colour());
}

void Keyboard::resetDegreeColors()
{
    keyColorsDegrees.fill(Colour());
}

void Keyboard::resetKeyColors()
{
    resetLayerColors();
    resetDegreeColors();
    keyColorsIndividual.fill(Colour());

    updateKeyColors();
}

//===============================================================================================

void Keyboard::setKeyDegreeColor(int scaleDegreeIn, Colour colourIn)
{
    int degree = totalModulus(scaleDegreeIn, mode->getScaleSize());
    keyColorsDegrees.set(degree, colourIn);
}

void Keyboard::setKeyLayerColor(int layerNumIn, Colour colourIn)
{
    int layer = totalModulus(layerNumIn, mode->getMaxStep());
    keyColorsOrders.set(layer, colourIn);
}

void Keyboard::setKeyColor(int keyNumIn, Colour colourIn)
{
    int keyNum = totalModulus(keyNumIn, keys.size());
    keyColorsIndividual.set(keyNum, colourIn);
}

//===============================================================================================

void Keyboard::setKeySizeRatio(float keySizeRatioIn, bool resizeSelf)
{
    keySizeRatio = keySizeRatioIn;
    pianoNode.setProperty(IDs::pianoWHRatio, keySizeRatio, nullptr);

    if (resizeSelf)
        setSize(getPianoWidth(getHeight()), getHeight());
}

void Keyboard::setKeyWidthSize(int widthSizeIn)
{
    // do stuff
}

void Keyboard::setKeyOrderSizeScalar(float scalarIn)
{
    // do stuff
}

//===============================================================================================

void Keyboard::allNotesOff()
{
    MidiKeyboardState::allNotesOff(midiChannelOut);

    for (int i = 0; i < keys.size(); i++)
    {
        triggerKey(i, false);
    }
}

void Keyboard::isolateLastNote()
{
    if (lastKeyClicked >= 0 && lastKeyClicked < 128)
    {
        for (int i = 0; i < keys.size(); i++)
        {
            if (i != lastKeyClicked)
                triggerKey(i, false);
        }
    }
}

void Keyboard::retriggerNotes()
{
    Array<int> retrigger = Array<int>(keysOn);
    triggerKeys(retrigger, false, 0);
    triggerKeys(retrigger, true, 0.7f);
}

void Keyboard::triggerKey(int keyNumberIn, bool doNoteOn, float velocity)
{
    if (keyNumberIn < 0 || keyNumberIn >= keys.size())
        return;

    Key* key = keys[keyNumberIn];

    if (doNoteOn)
    {
        noteOn(midiChannelOut, keyNumberIn, velocity);
        key->isClicked = true;
        keysOn.addIfNotAlreadyThere(keyNumberIn);
    }
    else
    {
        noteOff(midiChannelOut, keyNumberIn, 0);
        key->isClicked = false;
        keysOn.removeAllInstancesOf(keyNumberIn);
    }

    key->repaint();
}


void Keyboard::triggerKeys(Array<int> keyNumbers, bool doNoteOn, float velocity)
{
    for (auto keyNum : keyNumbers)
    {
        triggerKey(keyNum, doNoteOn, velocity);
    }
}

int Keyboard::getOrderOfNotesOn()
{
    float orderSum = 0;
    int orderDetected;
    
    for (int i = 0; i < keysOn.size(); i++)
    {
        Key* key = keys[keysOn[i]];
        orderSum += (key->order + 1);
    }

    orderDetected = orderSum / keysOn.size();

    if (orderDetected == (int)orderDetected)
        orderDetected -= 1;
    else
        orderDetected = -1;

    return orderDetected;
}

int Keyboard::transposeKeyModally(int keyNumIn, int stepsIn)
{
    if (keyNumIn < 0 || keyNumIn >= keys.size())
        return -1;

    Key* key = keys[keyNumIn];
    Array<int> orderArray = keysOrder[key->order];
    int newKey = -1;
    
    for (int i = 0; i < orderArray.size(); i++)
    {
        if (key->keyNumber == orderArray.getUnchecked(i))
        {
            newKey = i;
            break;
        }
    }
    
    newKey += stepsIn;
    
    if (newKey >= 0 && newKey < orderArray.size())
        return orderArray[newKey];
    else
        return -1;
}

int Keyboard::transposeKeyChromatically(int keyNumIn, int stepsIn)
{
    int newKey = keyNumIn + stepsIn;
    
    if (newKey >= 0 && newKey <= keys.size())
        return newKey;
    else
        return -1;
}

void Keyboard::transposeKeysOnModally(int modalStepsIn, bool needsSameOrder, bool useLastClickedRoot)
{
    int keyRoot;
    int rootOrder;

    int newKey;
    Array<int> oldKeys = Array<int>(keysOn);
    Array<int> newKeys;

    if (needsSameOrder)
    {
        rootOrder = keys[keysOn[0]]->order;

        if (rootOrder != getOrderOfNotesOn())
            return;

        for (int i = 0; i < oldKeys.size(); i++)
        {
            newKey = transposeKeyModally(oldKeys[i], modalStepsIn);

            if (newKey > -1)
                newKeys.add(newKey);
        }
    }

    else
    {
        int stepsToUse; // TODO: implement this so that the chord quality is retained

        if (useLastClickedRoot)
        {
            keyRoot = lastKeyClicked;
        }
        else // find lowest key
        {
            keyRoot = keys.size();

            for (int i = 0; i < keysOn.size(); i++)
            {
                if (keysOn[i] < keyRoot)
                    keyRoot = keysOn[i];
            }
        }

        rootOrder = keys[keyRoot]->order;

        for (int i = 0; i < oldKeys.size(); i++)
        {
            newKey = transposeKeyModally(oldKeys[i], modalStepsIn);

            if (newKey > -1)
                newKeys.add(newKey);
        }
    }

    // TODO: implement velocities further
    lastKeyClicked = transposeKeyModally(lastKeyClicked, modalStepsIn);
    triggerKeys(oldKeys, false);
    triggerKeys(newKeys);
}

void Keyboard::transposeKeysOnChromatically(int stepsIn)
{
    Array<int> oldKeys = Array<int>(keysOn);
    Array<int> newKeys;
    int keyNum;
    int newKey;

    float velocity;
    
    for (int i = 0; i < oldKeys.size(); i++)
    {
        keyNum = oldKeys.getUnchecked(i);

        Key* key = keys[keyNum];
        velocity = key->velocity;

        newKey = transposeKeyChromatically(keyNum, stepsIn);
        
        if (newKey > -1)
            newKeys.add(newKey);
    }
    
    // TODO: implement velocities further
    lastKeyClicked = transposeKeyChromatically(lastKeyClicked, stepsIn);
    triggerKeys(oldKeys, false);
    triggerKeys(newKeys);
}

//===============================================================================================

void Keyboard::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
    int keyTriggered = midiNote;

    Key* key = keys[keyTriggered];

    key->exPressed = true;

    Colour keyOnColour = keyOnColorsByChannel[midiChannel];
    if (!keyOnColour.isOpaque())
        keyOnColour = key->color.contrasting(0.75f);

    key->exInputColor = keyOnColour;
    key->isDirty = true;

    hasDirtyKeys = true;
}

void Keyboard::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNote, float velocity)
{
    int keyTriggered = midiNote;
    Key* key = keys[keyTriggered];
    key->exPressed = false;
    key->isDirty = true;
    hasDirtyKeys = true;
}
