/*
  ==============================================================================

    SettingsContainer.h
    Created: 13 Nov 2019 7:13:39pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once
#include "../../../PluginState.h"

#include "GeneralSettingsPanel.h"
#include "MidiSettingsPanel.h"
#include "KeyboardSettingsPanel.h"
#include "MappingSettingsPanel.h"
#include "ColourSettingsPanel.h"
//#include "ControlSettingsPanel.h"
#include "DebugSettingsPanel.h"

//==============================================================================
/*
*/
class SettingsContainer : public TabbedComponent, public SvkPluginState::Listener
{
public:
    
    SettingsContainer(SvkPluginState* pluginStateIn)
    : TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop),
        pluginState(pluginStateIn)
    {
        tabColour = Colour(0xff323e44);
        backgroundColour = tabColour.darker();
        textColour = tabColour.contrasting();

        setColour(backgroundColourId, backgroundColour);

        //view.reset(new Viewport("SettingsViewport"));
        //view->setScrollOnDragEnabled(true);
        //view->setScrollBarsShown(true, false);
        //addAndMakeVisible(view.get());

        for (auto panelName : panelNames)
        {
            if (panelName == "General")
                panels.add(new GeneralSettingsPanel(pluginState));

            else if (panelName == "Midi")
                panels.add(new MidiSettingsPanel(pluginState));

            else if (panelName == "Keyboard")
                panels.add(new KeyboardSettingsPanel(pluginState));

            else if (panelName == "Mapping")
                panels.add(new MappingSettingsPanel(pluginState));

            else if (panelName == "Colors")
                panels.add(new ColourSettingsPanel(pluginState));

            else if (panelName == "Debug")
                panels.add(new DebugSettingsPanel(pluginState));
        }

        for (int i = 0; i < panels.size(); i++)
        {
            addTab(panelNames[i], tabColour, panels[i], true);
        }

        setSize(100, 100);
    }

    ~SettingsContainer()
    {
        view = nullptr;
    }
    
    SvkSettingsPanel* getComponentViewed()
    {
        return componentViewed;
    }
    
    void currentTabChanged (int newCurrentTabIndex, const String &newCurrentTabName) override
    {
        componentViewed = panels.getUnchecked(newCurrentTabIndex);
        componentViewed->resized();

        listeners.call(&SettingsContainer::Listener::settingsTabChanged, 
            newCurrentTabIndex, 
            newCurrentTabName, 
            componentViewed
        );
    }

    void setKeyboardPointer(VirtualKeyboard::Keyboard* keyboardPtrIn)
    {
        for (int i = 0; i < panelNames.size(); i++)
        {
            if (keyboardPanels.contains(panelNames[i]))
                panels[i]->setKeyboardPointer(keyboardPtrIn);
        }
    }

    void modeViewedChanged(Mode* modeIn, int selectorNumber, int slotNumber) override
    {
        for (auto p : panels)
        {
            if (p->getName() == "ColourSettingsPanel")
                p->refreshPanel();
        }
    }

public:

    class Listener
    {
    public:
        virtual void settingsTabChanged(int newTabIndex, const String& tabName, SvkSettingsPanel* panelChangedTo) = 0;
    };

    void addListener(SettingsContainer::Listener* listenerIn) { listeners.add(listenerIn); }

    void removeListener(SettingsContainer::Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

    ListenerList<Listener> listeners;

private:

    SvkPluginState* pluginState;

    Array<SvkSettingsPanel*> panels;
    std::unique_ptr<Viewport> view;

    SvkSettingsPanel* componentViewed = nullptr;

    int defaultControlHeight = 50;

    Colour backgroundColour;
    Colour tabColour;
    Colour textColour;

    StringArray panelNames =
    {
        "General"
        , "Midi"
        , "Keyboard"
        , "Mapping"
        , "Colors"
//#if JUCE_DEBUG
//        , "Debug"
//#endif
    };

    StringArray keyboardPanels = { "Keyboard", "Colors", "Debug" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsContainer)
};
