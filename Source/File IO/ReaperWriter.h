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
    
    Array<String> orderSymbols;
 
    File filePath;
	File fileOut;

	void setup_default_symbols();
    
public:
    
    ReaperWriter(ModeLayout* modeIn);
    ~ReaperWriter();
    
    void set_mode(ModeLayout* modeIn);
    void set_symbol(int orderIndex, String symbolIn);
    bool set_path(String pathIn);
    
    String ask_for_location();
    
    ModeLayout* get_mode();
    Array<String> get_symbols();
	String get_symbol(int orderIndexIn);
    String get_path();
    
    bool write_file();
    
};
