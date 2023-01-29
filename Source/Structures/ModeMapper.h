/*
  ==============================================================================

    ModeMapper.h
    Created: 30 May 2019 8:02:42pm
    Author:  Vincenzo

    TODO: Lots of cleaning up, simplifying, and generalizing

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../PluginModes.h"
#include "Mode.h"
#include "NoteMap.h"
#include "Preset.h"

class ModeMapper
{
    ValueTree mappingNode;

    MappingStyle mappingStyle = MappingStyle::ModeToMode;
    
    int mapByOrderNum1 = 0;
    int mapByOrderNum2 = 0;
    int mapByOrderOffset1 = 0;
    int mapByOrderOffset2 = 0; 

    NoteMap previousOrderMap;

    struct MappingArguments
    {
        const Mode& mode1;
        const Mode& mode2;
        MappingStyle mapStyle = MappingStyle::ModeToMode;
        
        // Specify a non-primary order of a mode
        int order1 = 0;
        int order2 = 0;
        
        // Specify a note offset/transposition
        int offset1 = 0;
        int offset2 = 0;

        MappingArguments(const Mode& mode1, const Mode& mode2)
            : mode1(mode1), mode2(mode2) {}
    };
    
public:
    
    ModeMapper();
    
    ModeMapper(const SvkPreset& preset);

    ValueTree getMappingNode();

    int getMode1OrderNum() const;

    int getMode2OrderNum() const;

    int getMode1OrderOffset() const;

    int getMode2OrderOffset() const;

    void setMappingStyle(MappingStyle mapTypeIn);
    
    void setMapOrdersParameters(int order1, int order2, int offset1, int offset2);

    void setMode1OrderNum(int orderNumber);

    void setMode2OrderNum(int orderNumber);

    void setMode1OrderOffset(int orderOffset);

    void setMode2OrderOffset(int orderOffset);

    void setPreviousOrderNoteMap(NoteMap prevNoteMapIn);

    NoteMap map(const Mode& mode1, const Mode& mode2, NoteMap prevMap = NoteMap());
    
    // Returns certain type of mapping based off of passed in parameters
    NoteMap map(MappingArguments args, NoteMap prevMap = NoteMap());

    Array<int> getSelectedPeriodMap(const Mode& mode1, const Mode& mode2) const;

    // 
    static NoteMap mapFull(const Mode& mode1, const Mode& mode2, Array<int> degreeMapIn = Array<int>());

    // Creates a mapping of mode1 onto mode2 via aligning key orders (white vs black vs colored)
    static NoteMap mapByOrder(MappingArguments args, NoteMap prevMap=NoteMap());

    // 
    static NoteMap mapToMode1Period(const Mode& mapFrom, const Mode& mapTo, Array<int> degreeMapIn=Array<int>());
    
    // 
    static NoteMap mapToMode1Scale(const Mode& mode1, const Mode& mode2, int mode2Order=0);
    
    // Maps standard keyboard layout (white keys) to new mode (white keys)
    static NoteMap stdMidiToMode(const Mode& modeMapped, int rootNoteStd = 60);
    
    // A basic one-period mode-based mapping 
    static Array<int> getModeToModePeriodMap(const Mode& mode1, const Mode& mode2);

    // A basic one-period scale-based mapping, dependent on output mode's subscale being the same size as input mode's full scale
    static Array<int> getScaleToModePeriodMap(const Mode& mode1, const Mode& mode2);

    // 
    static Array<int> degreeMapFullMode(const Mode& mode1, const Mode& mode2);
};
