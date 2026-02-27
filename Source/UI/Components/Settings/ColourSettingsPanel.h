/*
  ==============================================================================

    ColourSettingsPanel.h
    Created: 13 Sep 2020 2:41:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "SvkSettingsPanel.h"
#include "../ColourLibraryComponent.h"

class FocussedColourSelector : public FocussedComponent, public ColourSelector, private ChangeListener
{
public:

    FocussedColourSelector(Component* componentToFocusOn = nullptr)
        : FocussedComponent(componentToFocusOn), ColourSelector(
            ColourSelector::ColourSelectorOptions::showColourAtTop
            + ColourSelector::ColourSelectorOptions::editableColour
            + ColourSelector::ColourSelectorOptions::showColourspace
            , 4, 0)
    {
        setName("FocussedColourSelector");

        // Called when colour changes
        addChangeListener(this);
    };

    ~FocussedColourSelector() {};

    void setComponentToFocusOn(Component* componentPtr) override
    {
        FocussedComponent::setComponentToFocusOn(componentPtr);

        // Only sync the selector to an already-painted (opaque) swatch.
        // For transparent/unedited swatches, keep the current selector color so
        // the swatch only gets colored when the user actually moves the picker.
        PaintSwatch* swatch = dynamic_cast<PaintSwatch*>(componentPtr);
        if (swatch && swatch->getCurrentColour().isOpaque())
            setCurrentColour(swatch->getCurrentColour(), dontSendNotification);
    }


private:

    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        FocusableComponent* focussed = dynamic_cast<FocusableComponent*>(currentFocus);

        if (focussed)
            focussed->performFocusFunction(getCurrentColour().toString());
        
        if (focusserCallback)
            focusserCallback(currentFocus, getCurrentColour().toString());
    }
};

class ColourSettingsPanel : public SvkSettingsPanel
{
public:

    ColourSettingsPanel(SvkState& presetIn);

    ~ColourSettingsPanel() override;

    void resized() override;

    void buttonClicked(Button*) override;

    void modifierKeysChanged(const ModifierKeys&) override;

    void mouseMove(const MouseEvent& event) override;

    void mouseDown(const MouseEvent& event) override;

    void mouseUp(const MouseEvent& event) override;

    void refreshPanel() override;

    void setKeyboardPointer(VirtualKeyboard::Keyboard* keyboardIn);

    ColourSelector* getColourSelector();

private:

    void checkModifiers(const ModifierKeys& keys);

private:

    VirtualKeyboard::Keyboard* virtualKeyboard = nullptr;

    FocussedColourSelector* colourSelector;

    ColourLibraryComponent* keyColourLibrary = nullptr;
    ColourLibraryComponent* noteOnColourLibrary = nullptr;

    const int paintTypeRadioGroup = 100;

    TextButton* degreePaintButton;
    TextButton* layerPaintButton;
    TextButton* keyPaintButton;

    TextButton* resetColourToggle;
    bool userClickedReset = false;

    bool ctrlHeld = false;
    bool shiftHeld = false;
    bool altOptionHeld = false;

};

