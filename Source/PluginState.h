/*
  ==============================================================================

    PluginState.h
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"



class SvkPluginState : public ChangeListener, public ValueTree::Listener
                        //private AudioProcessorValueTreeState::Listener
{
public:
    // AudioProcessorValueTreeState& svkTree;
    ValueTree pluginStateNode;
    //ValueTree pluginSettingsNode;
    //ValueTree midiSettingsNode;
    //ValueTree modeLibraryNode;
    //ValueTree pianoNode;
    
    SvkPluginState();
    ~SvkPluginState() {}


    


    //==============================================================================
    // Parameter Getters

    //float getParameterValue(Identifier paramId);
    //float getParameterMin(Identifier paramId);
    //float getParameterMax(Identifier paramId);
    //float getParameterDefaultValue(Identifier paramId);

 

    // ValueTree::Listener Implementation
    void valueTreePropertyChanged(ValueTree&, const Identifier&) override;

    void valueTreeChildAdded(ValueTree& parentTree, ValueTree& child) override;

    void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& child, int index) override;

    //==============================================================================

    // std::unique_ptr<TextFilterIntOrSpace> textFilterIntOrSpace;
    // std::unique_ptr<TextFilterInt> textFilterInt;

    //==============================================================================

    


    void syncNodes(bool sendChange=true);

private:

};
