/*
  ==============================================================================

    ColourSettingsPanel.cpp
    Created: 13 Sep 2020 2:41:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ColourSettingsPanel.h"

ColourSettingsPanel::ColourSettingsPanel(SvkState& presetIn)
    : SvkSettingsPanel("ColourSettingsPanel", presetIn,
        {
            "",
            "Choose settings then click keys to paint:",
            "Select a box to adjust the color:",
            ""
        },
        {
            SvkProperty::pianoKeyColor,
            SvkProperty::pianoKeyColorsDegree,
            SvkProperty::pianoKeyColorsLayer,
            SvkProperty::pianoKeyColorsIndividual,
            SvkProperty::pianoKeyColorReset,
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

    degreePaintButton = static_cast<TextButton*>(idToControl[SvkProperty::pianoKeyColorsDegree]);
    degreePaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
    degreePaintButton->setToggleState(true, dontSendNotification);
    degreePaintButton->addListener(this);

    layerPaintButton = static_cast<TextButton*>(idToControl[SvkProperty::pianoKeyColorsLayer]);
    layerPaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
    layerPaintButton->addListener(this);

    keyPaintButton = static_cast<TextButton*>(idToControl[SvkProperty::pianoKeyColorsIndividual]);
    keyPaintButton->setRadioGroupId(paintTypeRadioGroup, dontSendNotification);
    keyPaintButton->addListener(this);

    resetColourToggle = static_cast<TextButton*>(idToControl[SvkProperty::pianoKeyColorReset]);
    resetColourToggle->addListener(this);

    flexParent.items.getReference(numSections - 1).margin = FlexItem::Margin(0, 5, 0, 0);

    addMouseListener(this, true);

    setSize(100, 100);
}

ColourSettingsPanel::~ColourSettingsPanel()
{
    if (virtualKeyboard)
        virtualKeyboard->removeMouseListener(this);

    colourSelector = nullptr;
}

void ColourSettingsPanel::resized()
{
    getSectionFlexBox(0)->items.set(0, FlexItem(100, 100, *colourSelector));
    getSectionFlexBox(0)->items.getReference(0).height = getHeight();
    SvkSettingsPanel::resized();
}

void ColourSettingsPanel::buttonClicked(Button* buttonThatWasClicked)
{
    if (!virtualKeyboard)
        return;

    if (buttonThatWasClicked == degreePaintButton)
        virtualKeyboard->getProperties().set(SvkProperty::pianoKeyPaintType, 0);
    else if (buttonThatWasClicked == layerPaintButton)
        virtualKeyboard->getProperties().set(SvkProperty::pianoKeyPaintType, 1);
    else if (buttonThatWasClicked == keyPaintButton)
        virtualKeyboard->getProperties().set(SvkProperty::pianoKeyPaintType, 2);

    if (buttonThatWasClicked == resetColourToggle)
    {
        userClickedReset = buttonThatWasClicked->getToggleState();
        virtualKeyboard->getProperties().set(SvkProperty::pianoKeyColorReset, userClickedReset);
    }
}

void ColourSettingsPanel::modifierKeysChanged(const ModifierKeys& modifiers)
{
    checkModifiers(modifiers);
}

void ColourSettingsPanel::mouseMove(const MouseEvent& event)
{
    //if (!virtualKeyboard->isMouseOver())
    //    checkModifiers(event.mods);
}

void ColourSettingsPanel::mouseDown(const MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        // Right-click on a swatch: reset it to transparent (unedited)
        if (auto* swatch = dynamic_cast<PaintSwatch*>(event.eventComponent))
        {
            swatch->resetToTransparent();
            colourSelector->setComponentToFocusOn(nullptr);
            if (virtualKeyboard)
                virtualKeyboard->updateKeyColors(true);
            return;
        }

        // Right-click elsewhere: activate reset mode for key painting
        resetColourToggle->setToggleState(true, dontSendNotification);
        userClickedReset = false;
        if (virtualKeyboard)
            virtualKeyboard->getProperties().set(SvkProperty::pianoKeyColorReset, true);
    }

    // Focus a PaintSwatch in the colour selector when clicked
    if (!colourSelector->isMouseOver(true))
    {
        if (dynamic_cast<FocusableComponent*>(event.eventComponent))
            colourSelector->setComponentToFocusOn(event.eventComponent);
        else
            colourSelector->setComponentToFocusOn(nullptr);
    }
}

void ColourSettingsPanel::mouseUp(const MouseEvent& event)
{
    if (!userClickedReset && resetColourToggle->getToggleState())
    {
        resetColourToggle->setToggleState(false, dontSendNotification);
        if (virtualKeyboard)
            virtualKeyboard->getProperties().set(SvkProperty::pianoKeyColorReset, false);
    }

    // Refresh swatch library after painting a key on the keyboard
    if (keyColourLibrary && virtualKeyboard && virtualKeyboard->isParentOf(event.eventComponent))
        keyColourLibrary->refreshSwatches();
}

void ColourSettingsPanel::setKeyboardPointer(VirtualKeyboard::Keyboard* keyboardPointer)
{
    if (virtualKeyboard)
        virtualKeyboard->removeMouseListener(this);

    virtualKeyboard = keyboardPointer;

    if (!virtualKeyboard)
    {
        colourSelector->setFocusserCallback({});
        return;
    }

    // Sync colourSelector to keyboard's current colorSelected (or layer 0 default)
    Colour init = Colour::fromString(virtualKeyboard->getProperties()[SvkProperty::colorSelected].toString());
    if (init.isTransparent())
        init = virtualKeyboard->getKeyLayerColor(0);
    colourSelector->setCurrentColour(init, dontSendNotification);
    virtualKeyboard->getProperties().set(SvkProperty::colorSelected, init.toString());

    // Create or refresh key colour library (layer + degree only; transparent swatches visible)
    if (keyColourLibrary == nullptr)
    {
        keyColourLibrary = new ColourLibraryComponent(
            { "Key Layer Colors:", "Scale Degree Colors:" },
            { virtualKeyboard->getKeyLayerColours(),
              virtualKeyboard->getKeyDegreeColours() },
            true, false
        );
        keyColourLibrary->setName("KeyColourLibrary");
        keyColourLibrary->setColour(ColourLibraryComponent::ColourIds::backgroundColourId,
            Colour(0xff323e44).brighter(0.125f));
        controls.set(5, keyColourLibrary, true);
        addAndMakeVisible(keyColourLibrary);
        getSectionFlexBox(2)->items.getReference(1).associatedComponent = keyColourLibrary;
    }
    else
    {
        keyColourLibrary->setNewRows({
            virtualKeyboard->getKeyLayerColours(),
            virtualKeyboard->getKeyDegreeColours()
        });
        keyColourLibrary->refreshSwatches();
    }

    // Create or refresh note-on colour library
    if (noteOnColourLibrary == nullptr)
    {
        noteOnColourLibrary = new ColourLibraryComponent(
            { "Midi Channel Note On Colors:" },
            { virtualKeyboard->getKeyNoteOnColours() },
            true
        );
        noteOnColourLibrary->setName("NoteOnColourLibrary");
        noteOnColourLibrary->setColour(ColourLibraryComponent::ColourIds::backgroundColourId,
            Colour(0xff323e44).brighter(0.125f));
        noteOnColourLibrary->forceSwatchColumns(8);
        controls.set(6, noteOnColourLibrary, true);
        addAndMakeVisible(noteOnColourLibrary);
        getSectionFlexBox(3)->items.getReference(0).associatedComponent = noteOnColourLibrary;
    }
    else
    {
        noteOnColourLibrary->setNewRows({ virtualKeyboard->getKeyNoteOnColours() });
        noteOnColourLibrary->refreshSwatches();
    }

    // Wire color selector: update colorSelected and repaint keyboard when a swatch changes
    colourSelector->setFocusserCallback([this](Component* c, var dataIn)
    {
        virtualKeyboard->getProperties().set(SvkProperty::colorSelected, dataIn);
        if (dynamic_cast<PaintSwatch*>(c))
            virtualKeyboard->updateKeyColors(true);
    });

    virtualKeyboard->addMouseListener(this, true);
    resized();
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
