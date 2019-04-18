/*
  ==============================================================================

    ReaperWriter.cpp
    Created: 18 Apr 2019 12:22:34pm
    Author:  Soundtoys Quality Assurance

  ==============================================================================
*/

#include "ReaperWriter.h"

ReaperWriter::ReaperWriter(ModeLayout* modeIn)
{
    mode = modeIn;
    
    // check if reaper resource path exists
    
    if ((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::MacOSX) != 0)
    {
        file = File(File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("Application Support").getChildFile("REAPER").getChildFile("MIDINoteNames"));
    }
    else if ((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::Windows) != 0)
    {
        file = File(File::getSpecialLocation(File::userHomeDirectory).getChildFile("AppData").getChildFile("Roaming").getChildFile("REAPER").getChildFile("MIDINoteNames"));
    }
    
    if (!file.isDirectory())
       file = File::getSpecialLocation(File::userDocumentsDirectory);

    FileChooser chooser("Save as", file, "*.txt");
    chooser.browseForFileToSave(true);
    file = chooser.getResult();
}

ReaperWriter::~ReaperWriter() {}

void ReaperWriter::set_mode(ModeLayout* modeIn)
{
    mode = modeIn;
}

void ReaperWriter::set_symbol(int orderIndex, char symbolIn)
{
    orderSymbols.set(orderIndex % orderSymbols.size(), symbolIn);
}

bool ReaperWriter::set_path(String pathIn)
{
    file = File(pathIn);
    return file.isDirectory();
}

String ReaperWriter::ask_for_location()
{
    return "";
}

ModeLayout* ReaperWriter::get_mode()
{
    return mode;
}

Array<char> ReaperWriter::get_symbols()
{
    return orderSymbols;
}

char ReaperWriter::get_symbol(int orderIndexIn)
{
    return orderSymbols[orderIndexIn];
}

String ReaperWriter::get_path()
{
    return file.getFullPathName();
}

bool ReaperWriter::write_file()
{
    for (int i = 0; i < 128; i++)
    {
        
    }
    
    return true;
}
