/*
  ==============================================================================

    SettingsContainer.h
    Created: 13 Nov 2019 7:13:39pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"

#include "../../../PluginState.h"

#include "GeneralSettingsPanel.h"
#include "ViewSettingsPanel.h"
#include "ControlSettingsPanel.h"
#include "DeviceSettingsPanel.h"

//==============================================================================
/*
*/
class SettingsContainer : public TabbedComponent
{
public:
    SettingsContainer(SvkPluginState* pluginStateIn)
    : TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop), pluginState(pluginStateIn)
    {
        panels.add(new GeneralSettingsPanel(pluginState));
        panels.add(new ViewSettingsPanel(pluginState));
        panels.add(new ControlSettingsPanel(pluginState));
        panels.add(new DeviceSettingsPanel(pluginState));
        
        addTab("General", Colours::lightgrey, panels.getUnchecked(0), true);
        addTab("View", Colours::lightgrey, panels.getUnchecked(0), true);
        addTab("Device", Colours::lightgrey, panels.getUnchecked(0), true);
        addTab("Control", Colours::lightgrey, panels.getUnchecked(0), true);

        setCurrentTabIndex(0);
        
        componentViewed = panels.getUnchecked(0);
    }

    ~SettingsContainer()
    {
    }
    
    Component* getComponentViewed()
    {
        return componentViewed;
    }
    
    void currentTabChanged (int newCurrentTabIndex, const String &newCurrentTabName) override
    {
        componentViewed = panels.getUnchecked(newCurrentTabIndex);
    }

    void popupMenuClickOnTab (int tabIndex, const String &tabName) override
    {
        
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsContainer)
    
    SvkPluginState* pluginState;
    
    OwnedArray<Component> panels;
    Component* componentViewed = nullptr;
};
