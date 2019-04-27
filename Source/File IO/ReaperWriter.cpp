/*
  ==============================================================================

    ReaperWriter.cpp
    Created: 18 Apr 2019 12:22:34pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ReaperWriter.h"

ReaperWriter::ReaperWriter(Mode* modeIn)
{
    mode = modeIn;
    
    // check if reaper resource path exists
    
    if ((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::MacOSX) != 0)
    {
        filePath = File(File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("Application Support").getChildFile("REAPER").getChildFile("MIDINoteNames"));
    }
    else if ((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::Windows) != 0)
    {
        filePath = File(File::getSpecialLocation(File::userHomeDirectory).getChildFile("AppData").getChildFile("Roaming").getChildFile("REAPER").getChildFile("MIDINoteNames"));
    }
    
    if (!filePath.isDirectory())
       filePath = File::getSpecialLocation(File::userDocumentsDirectory);

    FileChooser chooser("Save as", filePath, "*.txt");
    chooser.browseForFileToSave(true);
    fileOut = chooser.getResult();

	setup_default_symbols();
}

ReaperWriter::~ReaperWriter()
{
}

void ReaperWriter::set_mode(Mode* modeIn)
{
    mode = modeIn;
}

void ReaperWriter::set_symbol(int orderIndex, String symbolIn)
{
    orderSymbols.set(orderIndex % orderSymbols.size(), symbolIn);
}

bool ReaperWriter::set_path(String pathIn)
{
    fileOut = File(pathIn);
    return fileOut.getParentDirectory().isDirectory();
}

String ReaperWriter::ask_for_location()
{
    return "";
}

Mode* ReaperWriter::get_mode()
{
    return mode;
}

Array<String> ReaperWriter::get_symbols()
{
    return orderSymbols;
}

String ReaperWriter::get_symbol(int orderIndexIn)
{
    return orderSymbols[orderIndexIn];
}

String ReaperWriter::get_path()
{
    return fileOut.getFullPathName();
}

void ReaperWriter::setup_default_symbols()
{
	orderSymbols.clear();

	CharPointer_UTF8 block = CharPointer_UTF8("\xe2\x96\x88");
	String symbol;
	int orderMax = mode->getMaxStep();

	for (int i = 0; i < 8; i++)
	{
		symbol = "";

		for (int o = 0; o < (8 - i); o++)
		{
			symbol += *block;
		}

		orderSymbols.add(symbol);
	}
}

bool ReaperWriter::write_file()
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

	int order;
	int index;
    for (int i = 0; i < 128; i++)
    {
		index = 127 - i;
		order = mode->getOrders()[index];

		if (order != 0)
		{
			outStream->writeText(String(index) + " ", false, false, nullptr);
			outStream->writeText(orderSymbols[order-1], false, false, nullptr);
			outStream->write("\n", 1);
		}
    }

	outStream->flush();
	return true;
}
