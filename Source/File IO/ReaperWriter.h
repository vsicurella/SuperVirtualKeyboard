/*
  ==============================================================================

    ReaperWriter.h
    Created: 18 Apr 2019 12:22:34pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Structures/Mode.h"

class ReaperWriter
{
    Mode* mode;
    //Scale* scale;
    
    Array<String> orderSymbols;
 
    File filePath;
    File fileOut;

    void setup_default_symbols();
    
public:
    
    ReaperWriter(Mode* modeIn);
    ~ReaperWriter();
    
    void set_mode(Mode* modeIn);
    void set_symbol(int orderIndex, String symbolIn);
    bool set_path(String pathIn);
    
    String ask_for_location();
    
    Array<String> get_symbols();
    String get_symbol(int orderIndexIn);
    String get_path();
    
    bool write();
    
};
