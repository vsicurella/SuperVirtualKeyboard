/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../PluginProcessor.h"
#include "Components/PluginControlComponent.h"

class SvkPluginEditor : public AudioProcessorEditor, public SvkPluginState::Listener, protected ValueTree::Listener
{
public:
    SvkPluginEditor (SvkAudioProcessor&);
    ~SvkPluginEditor();

    void paint (Graphics&) override;
    void resized() override;

    void presetLoaded(ValueTree presetNodeIn) override;

protected:

    void valueTreePropertyChanged(ValueTree&, const Identifier&) override;


private:
    SvkAudioProcessor& processor;
    SvkPluginState* pluginState;
    ValueTree pluginEditorNode;

    std::unique_ptr<PluginControlComponent> controlComponent;

    int defaultMinWidth = 750;
    int defaultMinHeight = 172;
    int defaultMaxWidth = 10e4;
    int defaultMaxHeight = 10e4;

    TooltipWindow tooltip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SvkPluginEditor)
};
