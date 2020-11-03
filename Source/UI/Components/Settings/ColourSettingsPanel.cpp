/*
  ==============================================================================

    ColourSettingsPanel.cpp
    Created: 13 Sep 2020 2:41:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ColourSettingsPanel.h"

ColourSettingsPanel::ColourSettingsPanel(SvkPluginState* stateIn)
    : SvkSettingsPanel("ColourSettingsPanel", stateIn,
        {
            "",
            "Choose settings then click keys to paint:",
            "Select a box to adjust the color:",
            ""
        },
        {
            IDs::pianoKeyColor,
            IDs::pianoKeyColorsDegree,
            IDs::pianoKeyColorsLayer,
            IDs::pianoKeyColorsIndividual,
            IDs::pianoKeyColorReset,
            Identifier("KeyColourLibrary"),
            Identifier("NoteOnColourLibrary")
        },
        {
            SvkControlProperties(ControlTypeNames::GenericControl, "Color Selector"),
            SvkControlProperties(ControlTypeNames::ToggleControl, "Paint by Scale Degree", false, 1),
            SvkControlProperties(ControlTypeNames::ToggleControl, "Paint by Layer (hold Shift)", false, 1),
            SvkControlProperties(ControlTypeNames::ToggleControl, "Paint by Individual Key (hold Ctrl)", false, 1),
            SvkControlProperties(ControlTypeNames::ToggleControl, "Reset Color to Default (Right-click)", false, 1),
            SvkControlProperties(ControlTypeNames::GenericControl, "Key Colors", false, 2),
            SvkControlProperties(ControlTypeNames::GenericControl, "Note On Colors", false, 3)
        },
        FlexBox(), {}, { FlexItem() }
    )
{
    // Set up Color Selector and replace generic control
    colourSelector = new FocussedColourSelector();
    addAndMakeVisible(colourSelector);
    controls.set(0, colourSelector, true);
    getSectionFlexBox(0)->items.getReference(0).associatedComponent = colourSelector;

    degreePaintButton = static_cast<TextButton*>(idToControl[IDs::pianoKeyColorsDegree]);
    degreePaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
    degreePaintButton->setToggleState(true, dontSendNotification);
    degreePaintButton->addListener(this);

    layerPaintButton = static_cast<TextButton*>(idToControl[IDs::pianoKeyColorsLayer]);
    layerPaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
    layerPaintButton->addListener(this);

    keyPaintButton = static_cast<TextButton*>(idToControl[IDs::pianoKeyColorsIndividual]);
    keyPaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
    keyPaintButton->addListener(this);

    resetColourToggle = static_cast<TextButton*>(idToControl[IDs::pianoKeyColorReset]);
    resetColourToggle->addListener(this);

    flexParent.items.getReference(numSections - 1).margin = FlexItem::Margin(0, 5, 0, 0);

    addMouseListener(this, true);

    setSize(100, 100);
}

ColourSettingsPanel::~ColourSettingsPanel()
{
    colourSelector = nullptr;

    if (virtualKeyboard)
        virtualKeyboard->removeMouseListener(this);
}

void ColourSettingsPanel::resized()
{
    getSectionFlexBox(0)->items.getReference(0).height = getHeight();
    SvkSettingsPanel::resized();
}

void ColourSettingsPanel::buttonClicked(Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == degreePaintButton)
    {
        virtualKeyboard->getProperties().set(IDs::pianoKeyPaintType, 0);
    }
    else if (buttonThatWasClicked == layerPaintButton)
    {
        virtualKeyboard->getProperties().set(IDs::pianoKeyPaintType, 1);
    }
    else if (buttonThatWasClicked == keyPaintButton)
    {
        virtualKeyboard->getProperties().set(IDs::pianoKeyPaintType, 2);
    }

    if (buttonThatWasClicked == resetColourToggle)
    {
        virtualKeyboard->getProperties().set(IDs::pianoKeyColorReset, buttonThatWasClicked->getToggleState());
        userClickedReset = buttonThatWasClicked->getToggleState();
    }
}

void ColourSettingsPanel::modifierKeysChanged(const ModifierKeys& modifiers)
{
    checkModifiers(modifiers);
}

void ColourSettingsPanel::mouseMove(const MouseEvent& event)
{
    if (!virtualKeyboard->isMouseOver())
        checkModifiers(event.mods);
}

void ColourSettingsPanel::mouseDown(const MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        resetColourToggle->setToggleState(true, dontSendNotification);
        userClickedReset = false;
    }

    // TODO: allow one click on virtual keyboard to set focus before painting?
    if (event.eventComponent->getParentComponent() == virtualKeyboard)
    {
        colourSelector->setComponentToFocusOn(virtualKeyboard);

        // TODO: better messaging for VirtualKeyboard changes
        virtualKeyboard->updateKeyColors(false);
        keyColourLibrary->refreshSwatches();
    }

    else if (colourSelector->isMouseOver() || event.eventComponent->getParentComponent() == colourSelector)
        return;

    else if (dynamic_cast<FocusableComponent*>(event.eventComponent))
    {
        colourSelector->setComponentToFocusOn(event.eventComponent);

        // TODO: find out why this isn't being done in ColourSelector focusser callback
        virtualKeyboard->getProperties().set(IDs::colorSelected, colourSelector->getCurrentColour().toString());
    }
    
    else
        colourSelector->setComponentToFocusOn(nullptr);
}

void ColourSettingsPanel::mouseUp(const MouseEvent& event)
{
    if (!userClickedReset && resetColourToggle->getToggleState())
        resetColourToggle->setToggleState(false, dontSendNotification);
}

void ColourSettingsPanel::setKeyboardPointer(VirtualKeyboard::Keyboard* keyboardPointer)
{
    virtualKeyboard = keyboardPointer;

    Colour init = Colour::fromString(virtualKeyboard->getProperties()[IDs::colorSelected].toString());
    if (init.isTransparent())
        init = virtualKeyboard->getKeyLayerColor(0);
    colourSelector->setCurrentColour(init, dontSendNotification);

    // fill key color swatches
    keyColourLibrary = new ColourLibraryComponent(
        { "Key Layer Colors:",
            "Scale Degree Colors:",
            "Individual Key Colors:" },
        { virtualKeyboard->getKeyLayerColours(),
          virtualKeyboard->getKeyDegreeColours(),
          virtualKeyboard->getKeyIndividualColours() }
        , true, true
    );
    keyColourLibrary->setName("KeyColourLibrary");
    keyColourLibrary->setColour(ColourLibraryComponent::ColourIds::backgroundColourId, Colour(Colour(0xff323e44).brighter(0.125f)));
    controls.set(5, keyColourLibrary, true);
    addAndMakeVisible(keyColourLibrary);
    getSectionFlexBox(2)->items.getReference(1).associatedComponent = keyColourLibrary;

    // fill note on color swatches
    noteOnColourLibrary = new ColourLibraryComponent({ "Midi Channel Note On Colors:" }, { virtualKeyboard->getKeyNoteOnColours() }, true);
    noteOnColourLibrary->setName("NoteOnColourLibrary");
    noteOnColourLibrary->setColour(ColourLibraryComponent::ColourIds::backgroundColourId, Colour(Colour(0xff323e44).brighter(0.125f)));
    noteOnColourLibrary->forceSwatchColumns(8);
    controls.set(6, noteOnColourLibrary, true);
    addAndMakeVisible(noteOnColourLibrary);
    getSectionFlexBox(3)->items.getReference(0).associatedComponent = noteOnColourLibrary;

    // callback for setting color in keyboard
    colourSelector->setFocusserCallback([&](Component* c, var dataIn) 
    { 
        if (dynamic_cast<PaintSwatch*>(c))
            virtualKeyboard->updateKeyColors();
        else
            virtualKeyboard->getProperties().set(IDs::colorSelected, dataIn); 
    });
    
    virtualKeyboard->addMouseListener(this, true);
}

void ColourSettingsPanel::refreshPanel()
{
    if (keyColourLibrary)
        keyColourLibrary->refreshSwatches();
}

ColourSelector* ColourSettingsPanel::getColourSelector()
{
    return colourSelector;
}

void ColourSettingsPanel::checkModifiers(const ModifierKeys& modifiers)
{
    bool valueChanged = true;

    if (modifiers.isCtrlDown() && !ctrlHeld)
        ctrlHeld = true;

    else if (ctrlHeld && !modifiers.isCtrlDown())
        ctrlHeld = false;

    else if (modifiers.isShiftDown() && !shiftHeld)
        shiftHeld = true;

    else if (shiftHeld && !modifiers.isShiftDown())
        shiftHeld = false;

    else
        valueChanged = false;

    if (valueChanged)
    {
        if (ctrlHeld)
            keyPaintButton->setToggleState(true, sendNotification);

        else if (shiftHeld)
            layerPaintButton->setToggleState(true, sendNotification);

        else
            degreePaintButton->setToggleState(true, sendNotification);
    }
}