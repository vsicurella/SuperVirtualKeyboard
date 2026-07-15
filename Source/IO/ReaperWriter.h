/*
  ==============================================================================

    ReaperWriter.h
    Created: 18 Apr 2019 12:22:34pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../Structures/Mode.h"

class ReaperWriter
{
public:

    /*
        How each MIDI note's label is generated. All numbering is anchored so that
        the key-center note reads as degree 0 (see build_label()).
    */
    struct Options
    {
        // false = mode degrees (diatonic degree with a fractional accidental level,
        // the historical behavior); true = scale degrees numbered 0..(scaleSize-1).
        bool useScaleDegrees = false;

        // Append an octave (period) number after the degree.
        bool includeOctaves = false;

        // Text inserted between the degree and the octave number.
        String octaveDelimiter = " ~ ";

        // MIDI note treated as degree 0 / the octave origin.
        int keyCenterNote = 60;

        // Octave number assigned to the key center's own octave (only used when
        // includeOctaves is true).
        int keyCenterOctave = 0;
    };

    ReaperWriter(const Mode* modeIn, Options optionsIn);
    ~ReaperWriter();

    void set_mode(const Mode* modeIn);
    void set_options(Options optionsIn);
    void set_symbol(int orderIndex, String symbolIn);

    Array<String> get_symbols();
    String get_symbol(int orderIndexIn);

    bool write(File fileOut);

private:

    void setup_default_symbols();

    // Builds the degree (and optional octave) label for a single MIDI note.
    String build_label(int midiNote) const;

    const Mode* mode;
    Options options;

    Array<String> orderSymbols;
};
