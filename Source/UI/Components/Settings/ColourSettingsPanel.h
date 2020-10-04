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

        PaintSwatch* swatch = dynamic_cast<PaintSwatch*>(componentPtr);
        if (swatch)
            setCurrentColour(swatch->getCurrentColour());
    }


private:

    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        FocusableComponent* focussed = dynamic_cast<FocusableComponent*>(currentFocus);

        if (focussed)
            focussed->performFocusFunction(getCurrentColour().toString());
        
        focusserCallback(currentFocus, getCurrentColour().toString());
    }
};

class ColourSettingsPanel : public SvkSettingsPanel
{
public:

    ColourSettingsPanel(SvkPluginState*);

    ~ColourSettingsPanel() override;

    void resized() override;

    void buttonClicked(Button*) override;

    void modifierKeysChanged(const ModifierKeys&) override;

    void mouseMove(const MouseEvent& event) override;

    void mouseDown(const MouseEvent& event) override;

    void mouseUp(const MouseEvent& event) override;

    void setKeyboardPointer(VirtualKeyboard::Keyboard* keyboardPointer) override;

    void refreshPanel() override;

    ColourSelector* getColourSelector();

private:

    void checkModifiers(const ModifierKeys& keys);

private:

    FocussedColourSelector* colourSelector;
    
    ColourLibraryComponent* keyColourLibrary;
    ColourLibraryComponent* noteOnColourLibrary;

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

