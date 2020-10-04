/*
  ==============================================================================

    VirtualKeyboardGraphics.h
    Created: 25 Jul 2020 1:49:41am
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../../../Structures/Mode.h"
#include "../../../PluginModes.h"

namespace VirtualKeyboard
{
    class KeyGraphics
    {
    public:

        KeyGraphics(const Mode& modeIn, int baseKeyHeightIn, int baseKeyWidthIn, float heightLayerMultIn, float widthLayerMultIn);

        void setBaseKeyWidth(int keyWidthIn);

        void setBaseKeyHeight(int keyHeightIn);

        void setWidthMultiplier(float widthMultIn);

        void setHeightMultiplier(float heightMultIn);

        void setKeyPlacementStyle(VirtualKeyboard::KeyPlacementType placementTypeIn);

        Array<Path> getKeyboardOctave(int scaleDegreeOffset = 0);

        Array<Path> getFullKeyboard();

        Path getKey(int keyNumber);

    private:
        const Mode& mode;

        int baseKeyHeight;
        int baseKeyWidth;
        float heightLayerMult;
        float widthLayerMult;

        VirtualKeyboard::KeyPlacementType keyPlacement = VirtualKeyboard::nestedRight;
    };
}