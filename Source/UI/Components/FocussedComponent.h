/*
  ==============================================================================

    FocussedComponent.h
    Created: 17 Sep 2020 6:31:58pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FocussedComponent;

class FocusableComponent
{
public:

    FocusableComponent(FocussedComponent* focusserIn = nullptr, std::function<void(var)> callbackIn = {})
        : currentFocusser(focusserIn)
    {

    }

    virtual void setNewFocusser(FocussedComponent* newFocusserIn)
    {
        currentFocusser = newFocusserIn;
    }

    bool isFocussedOn() const
    {
        return currentFocusser != nullptr;
    }

    virtual void performFocusFunction(var dataIn)
    {
        focusFunctionCallback(dataIn);
    }
    
protected:

    FocussedComponent* currentFocusser;

    std::function<void(var)> focusFunctionCallback;

};

class FocussedComponent
{

public:

    FocussedComponent(Component* initialComponentToFocus = nullptr, std::function<void(Component*, var)> focusserCallbackIn = {})
        : currentFocus(initialComponentToFocus), focusserCallback(focusserCallbackIn)
    {

    }

    virtual void setComponentToFocusOn(Component* componentPtr)
    {
        FocusableComponent* focusable = dynamic_cast<FocusableComponent*>(currentFocus);
        if (focusable)
            focusable->setNewFocusser(nullptr);

        currentFocus = componentPtr;

        focusable = dynamic_cast<FocusableComponent*>(currentFocus);
        if (focusable)
            focusable->setNewFocusser(this);
    }

    void setFocusserCallback(std::function<void(Component*, var)> focusserCallbackIn)
    {
        focusserCallback = focusserCallbackIn;
    }

protected:

    Component* currentFocus;
    std::function<void(Component*, var)> focusserCallback;
};
