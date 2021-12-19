/*
  ==============================================================================

    ReaperWriter.cpp
    Created: 18 Apr 2019 12:22:34pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ReaperWriter.h"

ReaperWriter::ReaperWriter(const Mode* modeIn)
{
    mode = modeIn;
    
    // check if reaper resource path exists
    File defaultFolder;
    if ((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::MacOSX) != 0)
    {
        defaultFolder = File(File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("Application Support").getChildFile("REAPER").getChildFile("MIDINoteNames"));
    }
    else if ((SystemStats::getOperatingSystemType() & SystemStats::OperatingSystemType::Windows) != 0)
    {
        defaultFolder = File(File::getSpecialLocation(File::userHomeDirectory).getChildFile("AppData").getChildFile("Roaming").getChildFile("REAPER").getChildFile("MIDINoteNames"));
    }
    
    if (!defaultFolder.isDirectory())
       defaultFolder = File::getSpecialLocation(File::userDocumentsDirectory);

    setup_default_symbols();

    chooser = std::make_unique<FileChooser>("Save as", defaultFolder, "*.txt");
    chooser->launchAsync(
        FileBrowserComponent::FileChooserFlags::saveMode | FileBrowserComponent::FileChooserFlags::warnAboutOverwriting,
            [&](const FileChooser& chooser)
            {
                fileToWrite = chooser.getResult();
                if (fileToWrite.getParentDirectory().exists())
                    write(fileToWrite);
            });

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
    fileToWrite = File(pathIn);
    return fileToWrite.getParentDirectory().isDirectory();
}

String ReaperWriter::ask_for_location()
{
    return "";
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
    return fileToWrite.getFullPathName();
}

void ReaperWriter::setup_default_symbols()
{
    orderSymbols.clear();

    CharPointer_UTF8 block = CharPointer_UTF8("\xe2\x96\x88");
    String symbol;
    int orderMax = mode->getMaxStep();
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

bool ReaperWriter::write(File fileOut)
{
    Array<int> modeOrders = mode->getOrders();
    Array<float> modeDegrees = Mode::ordersToModalDegrees(modeOrders);
    
    if (!fileOut.getParentDirectory().exists())
        return false;
    
    std::unique_ptr<FileOutputStream> outStream(fileOut.createOutputStream());

    if (!outStream->openedOk())
        return false;
    
    outStream->setPosition(0);
    outStream->truncate();

    // Header
    outStream->writeText("# MIDI note / CC name map\n", false, false, nullptr);

    int scaleSize = mode->getScaleSize();
    int modeSize = mode->getModeSize();
    
    int order;
    int index;
    float degree;
    String noteName;
    
    for (int i = 0; i < 128; i++)
    {
        index = 127 - i;
        order = modeOrders[index];
        degree = modeDegrees[index];
        noteName = String(degree - ((int) degree / modeSize) * modeSize);

        outStream->writeText(String(index) + " ", false, false, nullptr);

        if (order != 0)
        {
            outStream->writeText(orderSymbols[order-1], false, false, nullptr);
        }
        
        outStream->writeText(" " + noteName, false, false, nullptr);
        outStream->write("\n", 1);
    }

    outStream->flush();
    return true;
}
