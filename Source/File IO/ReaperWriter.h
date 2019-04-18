/*
  ==============================================================================

    ReaperWriter.h
    Created: 18 Apr 2019 12:22:34pm
    Author:  Soundtoys Quality Assurance

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Structures/ModeLayout.h"

class ReaperWriter
{
    
    ModeLayout* mode;
    //Scale* scale;
    
    Array<char> orderSymbols;
    
    File file;
    
public:
    
    ReaperWriter(ModeLayout* modeIn);
    ~ReaperWriter();
    
    void set_mode(ModeLayout* modeIn);
    void set_symbol(int orderIndex, char symbolIn);
    bool set_path(String pathIn);
    
    String ask_for_location();
    
    ModeLayout* get_mode();
    Array<char> get_symbols();
    char get_symbol(int orderIndexIn);
    String get_path();
    
    bool write_file();
    
};
