/*
  ==============================================================================

    PluginModes.h
    Created: 25 Jul 2020 5:39:15pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace VirtualKeyboard
{
    enum class UIMode
    {
        playMode = 0,
        editMode,
        mapMode
    };

    enum class Orientation
    {
        horizontal = 0,
        verticalLeft,
        verticalRight
    };

    enum class HighlightStyle
    {
        full = 1,
        inside,
        outline,
        circles,
        squares
    };

    enum class VelocityStyle
    {
        linear = 1,
        curved,
        fixed
    };

    enum ScrollingStyle
    {
        smooth = 1,
        stepped
    };

    enum class KeyPlacementType
    {
        nestedRight = 1,
        nestedCenter,
        flat,
        adjacent
    };

    enum KeyShapeFlags
    {
        Basic = 0x0001,
        NotchPrevious = 0x0010,
        NotchNext = 0x0100,
        NotchCenter = 0x1000
    };
}

enum class MappingMode
{
    None = 0,
    Auto,
    Manual
};

enum class MappingStyle
{
    ModeToMode = 1,
    ModeToScale,
    ModeByOrder
};

enum PluginStateNodeStatus
{
    InvalidNode = 0,
    CurrentVersion,
    AlphaVersion,
    IsPresetNode = 0x0100,
    IsAPVTSNode = 0x1000
};