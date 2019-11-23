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
#include "DebugSettingsPanel.h"

#include "../../Dialogs/PluginSettingsDialog.h"

//==============================================================================
/*
*/
class SettingsContainer : public TabbedComponent,
                            public ChangeBroadcaster
                            
{
public:
    SettingsContainer(SvkPluginState* pluginStateIn)
    : TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop), pluginState(pluginStateIn)
    {
        view.reset(new Viewport("SettingsViewport"));
        view->setScrollOnDragEnabled(true);
        view->setScrollBarsShown(true, false);
        addAndMakeVisible(view.get());

        addChildComponent(panels.add(new Component()));
        //addChildComponent(panels.add(new GeneralDialog(pluginState)));
        addChildComponent(panels.add(new PluginSettingsDialog(pluginState)));
        addChildComponent(panels.add(new ViewSettingsPanel(pluginState)));
        addChildComponent(panels.add(new ControlSettingsPanel(pluginState)));
        addChildComponent(panels.add(new DeviceSettingsPanel(pluginState)));
        addChildComponent(panels.add(new DebugSettingsPanel(pluginState)));
        
        addTab("X", Colours::red, panels.getUnchecked(0), true);
        addTab("General", Colours::lightgrey, panels.getUnchecked(1), true);
        addTab("View", Colours::lightgrey, panels.getUnchecked(2), true);
        addTab("Device", Colours::lightgrey, panels.getUnchecked(3), true);
        addTab("Control", Colours::lightgrey, panels.getUnchecked(4), true);
        addTab("Debug", Colours::palegreen, panels.getUnchecked(5), true);
        
        setCurrentTabIndex(1);
        
        setSize(100, 100);
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
        if (newCurrentTabIndex == 0)
        {
            sendChangeMessage();
            return;
        }
        
        componentViewed = panels.getUnchecked(newCurrentTabIndex);
        view->setViewedComponent(componentViewed, false);
    }

    void popupMenuClickOnTab (int tabIndex, const String &tabName) override
    {
        
    }
    
    void resized() override
    {
        int tabHeight = getTabBarDepth();
        
        getTabbedButtonBar().setBounds(0, 0, getWidth(), tabHeight);
        view->setBounds(0, tabHeight, getWidth(), getHeight() - tabHeight);
        
        if (componentViewed && componentViewed->getNumChildComponents() > 0)
            componentViewed->setBounds(0, tabHeight, view->getMaximumVisibleWidth(), componentViewed->getChildComponent(componentViewed->getNumChildComponents()-1)->getBottom());
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsContainer)
    
    SvkPluginState* pluginState;
    
    OwnedArray<Component> panels;
    std::unique_ptr<Viewport> view;
    
    Component* componentViewed = nullptr;
    
    int defaultControlHeight = 50;
};
