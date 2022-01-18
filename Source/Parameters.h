/*
  ==============================================================================

    Parameters.h
    Created: 15 Jan 2022 11:17:26pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "./PluginModes.h"
#include "./PluginIDs.h"
#include "./NoteMap.h"
#include "./Mode.h"

struct KeyboardParameters
{
    VirtualKeyboard::KeyPlacementType keyPlacement = VirtualKeyboard::KeyPlacementType::nestedRight;
    VirtualKeyboard::HighlightStyle highlightStyle = VirtualKeyboard::HighlightStyle::full;

    static String keyPlacementToString(VirtualKeyboard::KeyPlacementType placement)
    {
        switch (placement)
        {
        case VirtualKeyboard::KeyPlacementType::nestedRight:
            return String("NestedRight");
        case VirtualKeyboard::KeyPlacementType::nestedCenter:
            return String("NestedCenter");
        case VirtualKeyboard::KeyPlacementType::adjacent:
            return String("Adjacent");
        case VirtualKeyboard::KeyPlacementType::flat:
            return String("Flat");
        default:
            return String();
        }
    }

    VirtualKeyboard::KeyPlacementType stringToKeyPlacement(String placement)
    {
        if (placement == "NestedRight")
            return VirtualKeyboard::KeyPlacementType::nestedRight;

        if (placement == "NestedCenter")
            return VirtualKeyboard::KeyPlacementType::nestedCenter;

        if (placement == "Adjacent")
            return VirtualKeyboard::KeyPlacementType::adjacent;

        if (placement == "Flat")
            return VirtualKeyboard::KeyPlacementType::flat;

        return VirtualKeyboard::KeyPlacementType::nestedRight;
    }

    static String highlightStyleToString(VirtualKeyboard::HighlightStyle style)
    {
        switch (style)
        {
        case VirtualKeyboard::HighlightStyle::full:
            return String("Full");
        case VirtualKeyboard::HighlightStyle::outline:
            return String("Outline");
        case VirtualKeyboard::HighlightStyle::inside:
            return String("Inside");
        case VirtualKeyboard::HighlightStyle::circles:
            return String("Circles");
        case VirtualKeyboard::HighlightStyle::squares:
            return String("Squares");
        default:
            return String();
        }
    }

    static VirtualKeyboard::HighlightStyle stringToHighlightStyle(String style)
    {
        if (style == "Full")
            return VirtualKeyboard::HighlightStyle::full;
        if (style == "Outline")
            return VirtualKeyboard::HighlightStyle::outline;
        if (style == "Inside")
            return VirtualKeyboard::HighlightStyle::inside;
        if (style == "Circles")
            return VirtualKeyboard::HighlightStyle::circles;
        if (style == "Squares")
            return VirtualKeyboard::HighlightStyle::squares;

        return VirtualKeyboard::HighlightStyle::full;
    }

    KeyboardParameters() {}
    KeyboardParameters(VirtualKeyboard::KeyPlacementType placement, VirtualKeyboard::HighlightStyle highlight)
        : keyPlacement(placement), highlightStyle(highlight) {}

    KeyboardParameters(ValueTree tree)
    {
        if (tree.hasType(IDs::pianoNode))
        {
            keyPlacement = stringToKeyPlacement(tree[IDs::keyboardKeysStyle].toString());
            highlightStyle = stringToHighlightStyle(tree[IDs::keyboardHighlightStyle].toString());
        }
    }

    bool operator==(const KeyboardParameters& params) const
    {
        return highlightStyle == params.highlightStyle
            && keyPlacement == keyPlacement;
    }

    ValueTree toValueTree() const
    {
        auto tree = ValueTree(IDs::pianoNode);

        tree.setProperty(IDs::keyboardKeysStyle, keyPlacementToString(keyPlacement), nullptr);
        tree.setProperty(IDs::keyboardHighlightStyle, highlightStyleToString(highlightStyle), nullptr);
    }
};

struct MappingParameters
{
    MappingMode mode = MappingMode::None;
    MappingStyle style = MappingStyle::ModeToMode;

    NoteMap inputMap;
    NoteMap outputMap;


    static String mappingModeToString(MappingMode mode)
    {
        switch (mode)
        {
        case MappingMode::None:
            return String("None");
        case MappingMode::Auto:
            return String("Auto");
        case MappingMode::Manual:
            return String("Manual");
        default:
            return String();
        }
    }

    static MappingMode stringToMappingMode(String mode)
    {
        if (mode == "None")
            return MappingMode::None;
        if (mode == "Auto")
            return MappingMode::Auto;
        if (mode == "Manual")
            return MappingMode::Manual;
    }

    static String mappingStyleToString(MappingStyle style)
    {
        switch (style)
        {
        case MappingStyle::ModeToMode:
            return String("ModeToMode");
        case MappingStyle::ModeToScale:
            return String("ModeToScale");
        case MappingStyle::ModeByOrder:
            return String("ModeByOrder");
        default:
            return String();
        }
    }

    static MappingStyle stringToMappingStyle(String style)
    {
        if (style == "ModeToMode")
            return MappingStyle::ModeToMode;
        if (style == "ModeToScale")
            return MappingStyle::ModeToScale;
        if (style == "ModeByOrder")
            return MappingStyle::ModeByOrder;

        return MappingStyle::ModeToMode;
    }

    MappingParameters() {}
    MappingParameters(MappingMode modeIn, MappingStyle styleIn, NoteMap inputMapIn = NoteMap(), NoteMap outputMapIn = NoteMap())
        : mode(modeIn), style(styleIn), inputMap(inputMapIn), outputMap(outputMapIn) {}

    MappingParameters(ValueTree tree)
    {
        if (tree.hasType(IDs::midiMapNode))
        {
            mode = stringToMappingMode(tree[IDs::mappingMode].toString());
            style = stringToMappingStyle(tree[IDs::autoMappingStyle].toString());
            
            auto inputMapNode = tree.getChildWithName(IDs::midiInputRemap);
            if (inputMapNode.isValid())
                inputMap = NoteMap(inputMapNode);

            auto outputMapNode = tree.getChildWithName(IDs::midiOutputFilter);
            if (outputMapNode.isValid())
                inputMap = NoteMap(outputMapNode);
        }
    }

    bool operator==(const MappingParameters& params) const
    {
        return mode == params.mode
            && style == params.style
            && inputMap == params.inputMap
            && outputMap == params.outputMap;
    }

    ValueTree toValueTree() const
    {
        auto tree = ValueTree(IDs::midiMapNode);
        tree.setProperty(IDs::mappingMode, mappingModeToString(mode), nullptr);
        tree.setProperty(IDs::autoMappingStyle, mappingStyleToString(style), nullptr);
        tree.addChild(inputMap.getAsValueTree(IDs::midiInputRemap), 0, nullptr);
        tree.addChild(outputMap.getAsValueTree(IDs::midiOutputFilter), 0, nullptr);
    }
};

struct ScaleParameters
{
    int sourceModeRootNote = 60;
    int targetModeRootNote = 60;

    ValueTree sourceMode;
    ValueTree targetMode;

    bool targetModeViewed = true;

    ScaleParameters()
    {
        sourceMode = Mode::createNode("2 2 1 2 2 2 1", "Meantone", "Diatonic Scale");
        targetMode = sourceMode.createCopy();
    }

    ScaleParameters(ValueTree sourceIn, ValueTree targetIn, bool targetViewed = true, int sourceRoot = -1, int targetRoot = -1)
    {
        auto defaultMode = Mode::createNode("2 2 1 2 2 2 1", "Meantone", "Diatonic Scale");

        if (!Mode::isValidMode(sourceMode))
            sourceIn = defaultMode.createCopy();

        if (!Mode::isValidMode(targetMode))
            targetIn = defaultMode.createCopy();

        sourceMode = sourceIn;
        targetMode = targetIn;

        if (sourceRoot < 0)
            sourceRoot = (int)sourceMode[IDs::modeRootNote];

        if (sourceRoot >= 0)
            sourceModeRootNote = sourceRoot;
        
        if (targetRoot < 0)
            targetRoot = (int)targetMode[IDs::modeRootNote];
        
        if (targetRoot >= 0)
            targetModeRootNote = targetRoot;

        targetModeViewed = targetViewed;
    }

    ScaleParameters(ValueTree tree)
    {
        // TODO
    }

    ValueTree toValueTree() const
    {
        // TODO
    }
};

struct MidiParameters
{
    int periodShiftAmount = 0;
    int transposeAmount = 0;

    String midiInputDeviceId;
    String midiOutputDeviceId;

    MidiParameters() {}

    MidiParameters(int periodShift, int transpose, String inputDeviceId = String(), String outputDeviceId = String())
        : periodShiftAmount(periodShift), transposeAmount(transposeAmount), midiInputDeviceId(inputDeviceId), midiOutputDeviceId(outputDeviceId) {}

    MidiParameters(ValueTree tree)
    {
        auto settings = tree.getChildWithName(IDs::midiSettingsNode);
        if (settings.isValid())
        {
            periodShiftAmount = (int)settings[IDs::periodShift];
            transposeAmount = (int)settings[IDs::transposeAmt];
        }

        auto device = tree.getChildWithName(IDs::midiDeviceSettingsNode);
        if (device.isValid())
        {
            midiInputDeviceId = device[IDs::midiInputName].toString();
            midiOutputDeviceId = device[IDs::midiOutputName].toString();
        }
    }

    ValueTree toValueTree() const
    {
        ValueTree parent("MidiNode");

        auto midiSettings = ValueTree(IDs::midiSettingsNode);

        midiSettings.setProperty(IDs::periodShift, periodShiftAmount, nullptr);
        midiSettings.setProperty(IDs::transposeAmt, transposeAmount, nullptr);
        parent.addChild(midiSettings, 0, nullptr);

        if (JUCEApplication::isStandaloneApp())
        {
            auto midiDeviceSettings = ValueTree(IDs::midiDeviceSettingsNode);
            midiDeviceSettings.setProperty(IDs::midiInputName, midiInputDeviceId, nullptr);
            midiDeviceSettings.setProperty(IDs::midiOutputName, midiOutputDeviceId, nullptr);
            parent.addChild(midiDeviceSettings, 1, nullptr);
        }
    }

};

struct SvkParameters
{
    KeyboardParameters keyboard;
    ScaleParameters scale;
    MappingParameters mapping;
    MidiParameters midi;

    SvkParameters() {}
    SvkParameters(KeyboardParameters keyParams, ScaleParameters scaleParams, MappingParameters mappingParams, MidiParameters midiParams)
        : keyboard(keyParams), scale(scaleParams), mapping(mappingParams), midi(midiParams) {}

    SvkParameters(ValueTree presetNode)
        : keyboard(presetNode.getChildWithName(IDs::pianoNode)),
          scale(presetNode.getChildWithName(IDs::modeSlotsNode)), // TODO make a correct parent node
          mapping(presetNode.getChildWithName(IDs::midiMapNode)),
          midi(presetNode.getChildWithName(IDs::midiSettingsNode))
    {

    }
};