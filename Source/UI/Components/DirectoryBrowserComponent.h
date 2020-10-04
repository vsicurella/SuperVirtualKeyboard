/*
  ==============================================================================

    DirectoryBrowserComponent.h
    Created: 5 Sep 2020 9:59:39pm
    Author:  Vincenzo

    Simple combination of a text editor and button to trigger browsing for a 
    directory and then displaying the full directory path.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DirectoryBrowserComponent : public Component, public Button::Listener
{
public:

    DirectoryBrowserComponent(const String dialogBoxTitle, const File& directoryIn = File())
        : chooser(dialogBoxTitle, directoryIn)
    {
        setName(dialogBoxTitle);

        editor.reset(new TextEditor(dialogBoxTitle + "Editor"));
        editor->setMultiLine(false, false);
        editor->setReadOnly(true);
        addAndMakeVisible(editor.get());

        openButton.reset(new TextButton(dialogBoxTitle + "Button", "Choose directory for " + dialogBoxTitle));
        openButton->setButtonText("...");
        openButton->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft);
        addAndMakeVisible(openButton.get());
        openButton->addListener(this);
    }

    ~DirectoryBrowserComponent()
    {
        editor = nullptr;
        openButton = nullptr;
    }


    void resized() override
    {
        openButton->setSize(getHeight(), getHeight());
        openButton->setTopRightPosition(getWidth() - 1, 0);
        editor->setBounds(0, 0, getWidth() - getHeight(), getHeight());
    }

    void buttonClicked(Button* buttonThatWasClicked) override
    {
        if (chooser.browseForDirectory())
        {
            editor->setText(chooser.getResult().getFullPathName(), false);
            listeners.call(&DirectoryBrowserComponent::Listener::directoryChanged, this, chooser.getResult());
        }
    }

    void setText(String textIn)
    {
        editor->setText(textIn);
    }

    class Listener
    {
    public:
        
        Listener() {};
        virtual ~Listener() {};

        virtual void directoryChanged(DirectoryBrowserComponent* source, File directorySelected) = 0;
    };

    void addListener(DirectoryBrowserComponent::Listener* listenerIn)
    {
        listeners.add(listenerIn);
    }

    void removeListener(DirectoryBrowserComponent::Listener* listenerIn)
    {
        listeners.remove(listenerIn);
    }

protected:

    ListenerList<DirectoryBrowserComponent::Listener> listeners;

private:

    FileChooser chooser;

    std::unique_ptr<TextEditor> editor;
    std::unique_ptr<TextButton> openButton;

    std::function<void(const File&)> directorySetCallback;
};
