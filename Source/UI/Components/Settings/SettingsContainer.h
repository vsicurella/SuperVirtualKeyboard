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
class SettingsContainer : public TabbedComponent,
                            public ChangeBroadcaster
                            
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

        
		addTab("Back", tabColour.brighter(), new Component(), true);

		for (int i = 0; i < panels.size(); i++)
		{
			addTab(panelNames[i], tabColour, panels[i], false);
		}
        
        setCurrentTabIndex(1);
        setSize(100, 100);
    }

    ~SettingsContainer()
    {
		view = nullptr;
		panels.clear();
    }
    
    Component* getComponentViewed()
    {
        return componentViewed;
    }
    
    void currentTabChanged (int newCurrentTabIndex, const String &newCurrentTabName) override
    {
        if (newCurrentTabIndex == 0)
        {
            sendChangeMessage();
            return;
        }
        
        componentViewed = panels.getUnchecked(newCurrentTabIndex - 1);
        
        if (getParentComponent())
            resized();
    }

    void popupMenuClickOnTab (int tabIndex, const String &tabName) override
    {
        
    }
    
    void resized() override
    {
		TabbedComponent::resized();
		int tabHeight = getTabBarDepth();

		if (componentViewed && componentViewed->getNumChildComponents() > 0)
			componentViewed->setBounds(getLocalBounds().withHeight(getHeight() - tabHeight).withY(tabHeight));
    }

private:

	SvkPluginState* pluginState;

	OwnedArray<SvkSettingsPanel> panels;
	std::unique_ptr<Viewport> view;

	Component* componentViewed = nullptr;

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
