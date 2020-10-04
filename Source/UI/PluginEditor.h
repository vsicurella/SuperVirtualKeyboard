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

class SvkPluginEditor : public AudioProcessorEditor
{
public:
    SvkPluginEditor (SvkAudioProcessor&);
    ~SvkPluginEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    SvkAudioProcessor& processor;
    SvkPluginState* pluginState;
    ValueTree pluginEditorNode;

    std::unique_ptr<PluginControlComponent> controlComponent;

    TooltipWindow tooltip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SvkPluginEditor)
};
