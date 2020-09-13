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
#include "DeviceSettingsPanel.h"
#include "ViewSettingsPanel.h"
//#include "ControlSettingsPanel.h"
#include "DebugSettingsPanel.h"

//==============================================================================
/*
*/
class SettingsContainer : public TabbedComponent
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

		// Ability to change input and output device for Standalone version
		if (JUCEApplicationBase::isStandaloneApp())
			panelNames.insert(1, "Device");

		for (auto panelName : panelNames)
		{
			if (panelName == "General")
				panels.add(new GeneralSettingsPanel(pluginState));

			else if (panelName == "Device")
				panels.add(new DeviceSettingsPanel(pluginState));

			else if (panelName == "View")
				panels.add(new ViewSettingsPanel(pluginState));

			else if (panelName == "Debug")
				panels.add(new DebugSettingsPanel(pluginState));
		}

		for (int i = 0; i < panels.size(); i++)
		{
			addTab(panelNames[i], tabColour, panels[i], true);
		}
        
        setCurrentTabIndex(0);
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
		, "View"
#if JUCE_DEBUG
		, "Debug"
#endif
	};

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsContainer)
};
