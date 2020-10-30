/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "../PluginProcessor.h"

using namespace VirtualKeyboard;

//==============================================================================
SvkPluginEditor::SvkPluginEditor(SvkAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setName("SuperVirtualKeyboard");
    setResizable(true, true);
    setBroughtToFrontOnMouseClick(true);

    setMouseClickGrabsKeyboardFocus(true);
    addMouseListener(this, true);
    
    pluginState = processor.getPluginState();
    pluginEditorNode = pluginState->getPluginEditorNode();

    // Intialization
    if (!pluginEditorNode.isValid())
    {
        pluginEditorNode = ValueTree(IDs::pluginEditorNode);
        pluginEditorNode.setProperty(IDs::windowBoundsW, 1000, nullptr);
        pluginEditorNode.setProperty(IDs::windowBoundsH, 210, nullptr);
        pluginEditorNode.setProperty(IDs::viewportPosition, 60, nullptr);
        pluginEditorNode.setProperty(IDs::settingsOpen, false, nullptr);
        pluginState->pluginStateNode.appendChild(pluginEditorNode, nullptr);
    }

    controlComponent.reset(new PluginControlComponent(pluginState));
    addAndMakeVisible(controlComponent.get());

    setSize(controlComponent->getWidth(), controlComponent->getHeight());
    setResizeLimits(defaultMinWidth, defaultMinHeight, defaultMaxWidth, defaultMaxHeight);

    if (pluginEditorNode[IDs::settingsOpen])
        controlComponent->showSettingsDialog();
    
    pluginState->addListener(this);
    pluginEditorNode.addListener(this);
}

SvkPluginEditor::~SvkPluginEditor()
{
    pluginEditorNode.removeListener(this);
    pluginState->removeListener(this);
    controlComponent = nullptr;
}

//==============================================================================

void SvkPluginEditor::paint(Graphics& g)
{
    g.fillAll(Colour(0xff323e44));
}

void SvkPluginEditor::resized()
{    
    controlComponent->setBounds(0, 0, getWidth(), getHeight());
}

void SvkPluginEditor::presetLoaded(ValueTree presetNodeIn)
{
    if (pluginEditorNode.isValid())
        pluginEditorNode.removeListener(this);

    pluginEditorNode = pluginState->getPluginEditorNode();
 
    controlComponent->loadPresetNode(presetNodeIn);
    if (pluginEditorNode[IDs::settingsOpen])
        controlComponent->showSettingsDialog();

    pluginEditorNode.addListener(this);
}

//==============================================================================

void SvkPluginEditor::valueTreePropertyChanged(ValueTree& parent, const Identifier& property)
{
    if (parent == pluginEditorNode && property == IDs::windowBoundsH)
    {
        Viewport* viewport = controlComponent->getViewport();
        VirtualKeyboard::Keyboard* keyboard = controlComponent->getKeyboard();

        int viewportHeight = viewport->getHeight() - viewport->getScrollBarThickness();
        int keyboardWidth = keyboard->getPianoWidth(viewportHeight);

        // Not sure if this is a great solution, but should keep mins < maxes.
        if (viewportHeight > 1 && keyboardWidth > 1)
        {
            setResizeLimits(
                defaultMinWidth,
                jmax(defaultMinHeight, getHeight() - viewportHeight + 100 + 1),
                jmax(defaultMaxWidth, controlComponent->getViewport()->getX() * 2 + keyboardWidth),
                defaultMaxHeight
            );
        }
    }
}


// Would like to turn these command descriptions into tooltips...or something

//void SvkPluginEditor::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
//{
//    switch (commandID)
//    {
//    case IDs::CommandIDs::savePresetToFile:
//        result.setInfo("Save Preset", "Save your custom layout to a file.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::saveMode:
//        result.setInfo("Save Mode", "Save the currently viewed mode.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::showSaveMenu:
//        result.setInfo("Show Saving Options", "Save current mode or whole preset.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::loadPreset:
//        result.setInfo("Load Preset", "Load a custom layout from a file.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::loadMode:
//        result.setInfo("Load Mode", "Load only the mode of a preset.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::showLoadMenu:
//        result.setInfo("Show Loading Options", "Load a mode or whole preset.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::exportReaperMap:
//        result.setInfo("Export for Reaper", "Exports the current preset to a MIDI Note Name text file for use in Reaper's piano roll.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::exportAbletonMap:
//        result.setInfo("Export for Ableton", "Exports the mode mapping to a MIDI file for to use in Ableton's piano roll for folding.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::showExportMenu:
//        result.setInfo("Show Export Options", "Shows different ways you can export a mode or preset.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::showSettingsDialog:
//        result.setInfo("Show Settings Dialog", "Change default directories", "Settings", 0);
//        break;
//    case IDs::CommandIDs::commitCustomScale:
//        result.setInfo("Commit custom scale", "Registers the entered scale steps as the current custom scale.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::setMode1:
//        result.setInfo("Set Mode 1", "Loads the mode into the Mode 1 slot.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::setMode2:
//        result.setInfo("Set Mode 2", "Loads the mode into the Mode 2 slot.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::setMode1RootNote:
//        result.setInfo("Set Mode 1 Root", "Applies the selected root note for Mode 1.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::setMode2RootNote:
//        result.setInfo("Set Mode 2 Root", "Applies the selected root note for Mode 2.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::setModeViewed:
//        result.setInfo("Set Mode Viewed", "Shows the mode slot on the keyboard.", "Keyboard", 0);
//        break;
//    case IDs::CommandIDs::showModeInfo:
//        result.setInfo("Show Mode Info", "Shows information regarding the selected Mode.", "Mode", 0);
//        break;
//    case IDs::CommandIDs::setMappingStyle:
//        result.setInfo("Mapping Style", "Choose a mapping style for remapping MIDI notes.", "Midi", 0);
//        break;
//    case IDs::CommandIDs::showMapOrderEdit:
//        result.setInfo("Edit Mappings by Order", "Choose how to map modes with the order mapping method.", "Preset", 0);
//        break;
//    case IDs::CommandIDs::applyMapping:
//        result.setInfo("Apply Mapping", "Map incoming MIDI notes to Mode 2 with the selected mapping style.", "Midi", 0);
//        break;
//    case IDs::CommandIDs::setMappingMode:
//        result.setInfo("Auto Map to Scale", "Remap Midi notes when scale changes", "Midi", 0);
//        break;
//    case IDs::CommandIDs::beginColorEditing:
//        result.setInfo("Change Keyboard Colors", "Allows you to change the default colors for the rows of keys.", "Keyboard", 0);
//        break;
    //case IDs::CommandIDs::setPeriodShift:
    //    result.setInfo("Shift by Mode 2 Period", "Shift the outgoing MIDI notes by the selected number of Mode 2 periods.", "Midi", 0);
    //    break;
    //case IDs::CommandIDs::setMidiChannelOut:
    //    result.setInfo("Set MIDI Channel Out", "Set the outgoing MIDI Notes to the selected MIDI Channel.", "Midi", 0);
    //    break;
    //case IDs::CommandIDs::showMidiNoteNumbers:
    //    result.setInfo("Show Midi Note Numbers", "Shows incoming MIDI notes on Mode 1 and outgoing MIDI Notes on Mode 2.", "Keyboard", 0);
    //    break;
    //case IDs::CommandIDs::setKeyStyle:
    //    result.setInfo("Set Key Style", "Sets the selected style for drawing overlapping degrees between mode degrees.", "Keyboard", 0);
    //    break;
    //case IDs::CommandIDs::setHighlightStyle:
    //    result.setInfo("Set Highlight Style", "Sets the selected style for drawing triggered notes.", "Keyboard", 0);
    //    break;
//    default:
//        break;
//    }
//}