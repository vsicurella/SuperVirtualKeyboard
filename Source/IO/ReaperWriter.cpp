/*
  ==============================================================================

    ReaperWriter.cpp
    Created: 18 Apr 2019 12:22:34pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ReaperWriter.h"

ReaperWriter::ReaperWriter(const Mode* modeIn, Options optionsIn)
    : mode(modeIn), options(optionsIn)
{
    setup_default_symbols();
}

ReaperWriter::~ReaperWriter()
{
}

void ReaperWriter::set_mode(const Mode* modeIn)
{
    mode = modeIn;
    setup_default_symbols();
}

void ReaperWriter::set_options(Options optionsIn)
{
    options = optionsIn;
}

void ReaperWriter::set_symbol(int orderIndex, String symbolIn)
{
    orderSymbols.set(orderIndex % orderSymbols.size(), symbolIn);
}

Array<String> ReaperWriter::get_symbols()
{
    return orderSymbols;
}

String ReaperWriter::get_symbol(int orderIndexIn)
{
    return orderSymbols[orderIndexIn];
}

void ReaperWriter::setup_default_symbols()
{
    orderSymbols.clear();

    CharPointer_UTF8 block = CharPointer_UTF8("\xe2\x96\x88");
    String symbol;
    int repeats = 11;

    for (int i = 0; i < repeats; i++)
    {
        symbol = "";

        for (int o = 0; o < (repeats - i); o++)
        {
            symbol += *block;
        }

        orderSymbols.add(symbol);
    }
}

String ReaperWriter::build_label(int midiNote) const
{
    int scaleSize = mode->getScaleSize();
    int modeSize = mode->getModeSize();

    String degreeText;
    int octave = 0;

    if (options.useScaleDegrees)
    {
        // Number every scale step 0..(scaleSize-1) from the key center.
        int rel = midiNote - options.keyCenterNote;
        int degree = totalModulus(rel, scaleSize);
        degreeText = String(degree);

        // Floored division so notes below the key center land in the previous octave.
        octave = (rel - degree) / scaleSize;
    }
    else
    {
        // Modal degrees are monotonic across the keyboard and advance by exactly
        // modeSize per period, so subtracting the key center's value and wrapping
        // to modeSize yields a key-center-relative degree with matching octaves.
        Array<float> modeDegrees = mode->getModalDegrees();
        float relDegree = modeDegrees[midiNote] - modeDegrees[options.keyCenterNote];

        int period = (int) std::floor(relDegree / (float) modeSize);
        float degree = relDegree - period * modeSize;

        degreeText = String(degree);
        octave = period;
    }

    if (options.includeOctaves)
        degreeText += options.octaveDelimiter + String(octave + options.keyCenterOctave);

    return degreeText;
}

bool ReaperWriter::write(File fileOut)
{
    if (!fileOut.getParentDirectory().exists())
        return false;

    std::unique_ptr<FileOutputStream> outStream(fileOut.createOutputStream());

    if (!outStream->openedOk())
        return false;

    outStream->setPosition(0);
    outStream->truncate();

    // Header
    outStream->writeText("# MIDI note / CC name map\n", false, false, nullptr);

    Array<int> modeOrders = mode->getOrders();

    for (int i = 0; i < 128; i++)
    {
        int index = 127 - i;
        int order = modeOrders[index];

        outStream->writeText(String(index) + " ", false, false, nullptr);

        // Block characters designate the note's accidental level within the mode.
        if (order != 0)
            outStream->writeText(orderSymbols[order - 1], false, false, nullptr);

        outStream->writeText(" " + build_label(index), false, false, nullptr);
        outStream->write("\n", 1);
    }

    outStream->flush();
    return true;
}
