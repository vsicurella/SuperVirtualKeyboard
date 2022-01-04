/*
  ==============================================================================

    SettingsContainer.h
    Created: 13 Nov 2019 7:13:39pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "GeneralSettingsPanel.h"
#include "MidiSettingsPanel.h"
#include "KeyboardSettingsPanel.h"
#include "MappingSettingsPanel.h"
#include "ColourSettingsPanel.h"
#include "DebugSettingsPanel.h"

//==============================================================================
/*
*/
class SettingsContainer : public TabbedComponent, public SvkPresetManager::Listener
{
public:
    
    SettingsContainer(SvkPluginSettings& globalSettings, SvkPreset& presetIn)
        : TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop)
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
                panels.add(new GeneralSettingsPanel(globalSettings, presetIn));

            else if (panelName == "Midi")
                panels.add(new MidiSettingsPanel(presetIn));

            else if (panelName == "Keyboard")
                panels.add(new KeyboardSettingsPanel(presetIn));

            else if (panelName == "Mapping")
                panels.add(new MappingSettingsPanel(presetIn));

            else if (panelName == "Colors")
                panels.add(new ColourSettingsPanel(presetIn));

            else if (panelName == "Debug")
                panels.add(new DebugSettingsPanel(presetIn));
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
