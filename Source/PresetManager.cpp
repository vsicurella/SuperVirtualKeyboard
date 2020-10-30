/*
  ==============================================================================

    PresetManager.cpp
    Created: 27 May 2019 10:57:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PresetManager.h"

SvkPresetManager::SvkPresetManager(ValueTree pluginSettingsNodeIn)
{
    pluginSettingsNode = pluginSettingsNodeIn;
    initializeModePresets();
    
    loadPreset(SvkPreset::getDefaultPreset().getPresetNode(), false);
}

SvkPresetManager::~SvkPresetManager()
{
    removeAllChangeListeners();
}

SvkPreset& SvkPresetManager::getPreset()
{
    return svkPresetWorking;
}

int SvkPresetManager::getNumMenuItems(bool withFactoryMenu , bool withUserMenu, bool withFavMenu, bool withSlots)
{
    int totalMenuItems = 0;
    
    if (withFactoryMenu)
    {
        int factoryMenuSize = 0;
        
        for (auto sortedArray : modesSorted)
            factoryMenuSize += sortedArray.size();
        
        totalMenuItems += factoryMenuSize;
        //modesSorted[0].size() + modesSorted[1].size() + modesSorted[2].size();
    }
    
    if (withUserMenu)
        totalMenuItems += loadedUserModes.size();

    if (withFavMenu)
        totalMenuItems += favoriteModes.size();

    if (withSlots)
        totalMenuItems += modeSlots.size();

    return totalMenuItems;
}

int SvkPresetManager::getNumModeSlots() const
{
    return modeSlots.size();
}

Array<int> SvkPresetManager::getModeSlotsInUse() const
{
    return svkPresetWorking.getSlotNumbersInUse();
}

int SvkPresetManager::getModeSlotOfSelector(int modeSelectorNumIn) const
{
    return svkPresetWorking.getSlotNumberBySelector(modeSelectorNumIn);
}

ValueTree SvkPresetManager::getModeInLibrary(int indexIn)
{
    if (indexIn < 0)
        indexIn = 0;
    
    int numModes = loadedFactoryModes.size() + loadedUserModes.size();
    
    int subMenu = indexIn / numModes;
    int index = indexIn % numModes;
    
    if (subMenu < modesSorted.size())
        if (index < modesSorted.getUnchecked(subMenu).size())
            return modesSorted.getUnchecked(subMenu).getUnchecked(index);
    
    return ValueTree();
}

Mode* SvkPresetManager::getModeInSlot(int modeSlotNumIn)
{
    int slotIndex = svkPresetWorking.getSlotNumberIndex(modeSlotNumIn);

    if (slotIndex >= 0)
    {
        jassert(slotIndex < modeSlots.size());

        Mode* mode = modeSlots[slotIndex];
        DBG("GRABBING MODE " + String(modeSlotNumIn) + " which is: " + mode->getDescription());

        return mode;
    }
    else if (modeSlotNumIn > 0)
    {
        return modeCustom.get();
    }
    
    return nullptr;
}

Mode* SvkPresetManager::getModeBySelector(int selectorNumber)
{
    int slotNumber = svkPresetWorking.getSlotNumberBySelector(selectorNumber);
    if (svkPresetWorking.isSlotNumberInUse(slotNumber))
    {
        return modeSlots[svkPresetWorking.getSlotNumberIndex(slotNumber)];
    }

    else if (slotNumber > MAX_MODE_SLOTS_INDEX)
    {
        return modeCustom.get();
    }

    return nullptr;
}

Mode* SvkPresetManager::getModeCustom()
{
    return modeCustom.get();
}

Mode* SvkPresetManager::setModeCustom(ValueTree modeNodeIn)
{
    svkPresetWorking.setCustomMode(modeNodeIn);
    modeCustom.reset(new Mode(svkPresetWorking.getCustomMode(), false));
    return modeCustom.get();
}

Mode* SvkPresetManager::setModeCustom(String stepsIn, String familyIn, String nameIn, String infoIn, int rootNoteIn)
{
    return setModeCustom(Mode::createNode(stepsIn, familyIn, nameIn, infoIn, rootNoteIn));
}

int SvkPresetManager::setSlotToMode(int modeSlotNum, ValueTree modeNode)
{
    int slotIndex = svkPresetWorking.setModeSlot(modeNode, modeSlotNum);
    modeNode = svkPresetWorking.getModeInSlot(modeSlotNum);
    
    if (slotIndex >= 0)
    {
        if (slotIndex >= modeSlots.size())
            modeSlots.add(new Mode(modeNode, false));

        else
            modeSlots.set(slotIndex, new Mode(modeNode, false));
    }

    return slotIndex;
}
int SvkPresetManager::addSlot(ValueTree modeNode)
{
    int slotNumber = svkPresetWorking.addModeSlot(modeNode);
    modeSlots.add(new Mode(modeNode));

    return slotNumber;
}

int SvkPresetManager::setSlotAndSelection(int modeSlotNum, int modeSelectorNum, ValueTree modeNode)
{
    int slotIndex = setSlotToMode(modeSlotNum, modeNode);
    svkPresetWorking.setModeSelectorSlotNum(modeSelectorNum, modeSlotNum);
    return slotIndex;
}

int SvkPresetManager::addSlotAndSetSelection(int modeSelectorNumber, ValueTree modeNode)
{
    int modeSlotNum = addSlot(modeNode);
    svkPresetWorking.setModeSelectorSlotNum(modeSelectorNumber, modeSlotNum);
    return modeSlotNum;
}

void SvkPresetManager::removeMode(int modeSlotNum)
{
    int slotIndex = svkPresetWorking.removeModeSlot(modeSlotNum);
    modeSlots.remove(slotIndex);
}

void SvkPresetManager::resetModeSlots()
{
    svkPresetWorking.resetModeSlots();
    modeSlots.clear();
    
    modeSlots.add(new Mode(svkPresetWorking.getMode1()));
    modeSlots.add(new Mode(svkPresetWorking.getMode2()));
}

/*
    This operation should be completely revised, as it's piecing together two systems without full implementation.
    Currently, modes can be inserted in one of 128 slots, and mode selectors can point to a certain slot.
    But inserting modes into different slots is not fully implemented yet, so the mode slot containing a mode that
    the mode selector is pointed to will be replaced by the mode chosen in calling this function.
*/
void SvkPresetManager::handleModeSelection(int selectorNumber, int idIn)
{
    int numSortedModes = getNumMenuItems(true, false, false, false);

    int modeLibraryIndex = idIn - 1;
    int userModesIndex = modeLibraryIndex - numSortedModes + loadedUserModes.size(); // issue here, subtracting loaded user modes is a workaround
    int favIdx = userModesIndex - loadedUserModes.size();
    int slotIdx = favIdx - favoriteModes.size();
    
    int modeSlotNumber = svkPresetWorking.getSlotNumberBySelector(selectorNumber);

    // if selector was set to a custom mode, change the slot to the selector's number
    if (modeSlotNumber > MAX_MODE_SLOTS_INDEX)
        modeSlotNumber = selectorNumber;

    ValueTree modeSelected;
    
    if (modeLibraryIndex < numSortedModes)
    {
        modeSelected = getModeInLibrary(modeLibraryIndex).createCopy();
    }
    else if (userModesIndex < loadedUserModes.size())
    {
        modeSelected = loadedUserModes[userModesIndex].createCopy();
    }
    else if (favIdx < favoriteModes.size())
    {
        modeSelected = favoriteModes[favIdx].createCopy();
    }
    else if (slotIdx < modeSlots.size())
    {
        // Currently the only way to switch a selector's mode slot pointer
        modeSelected = ValueTree();
        modeSlotNumber = slotIdx;
    }
    else
    {
        DBG("Custom mode selected");
        modeSelected = modeCustom->modeNode;
        modeSlotNumber = MAX_MODE_SLOTS_INDEX + 1;
    }
    
    // Replaces the mode selector's mode slot with new mode
    if (modeSelected.isValid() && selectorNumber < 2 && modeSlotNumber <= MAX_MODE_SLOTS_INDEX)
    {
        setSlotToMode(modeSlotNumber, modeSelected);
    }
    
    svkPresetWorking.setModeSelectorSlotNum(selectorNumber, modeSlotNumber);
    getModeInSlot(modeSlotNumber)->setRootNote(svkPresetWorking.getModeSelectorRootNote(selectorNumber));
}

bool SvkPresetManager::loadPreset(ValueTree presetNodeIn, bool sendChangeSignal)
{
    if (presetNodeIn.hasType(IDs::presetNode))
    {
        DBG("Loading Preset:" + presetNodeIn.toXmlString());
        
        svkPresetSaved.restoreFromNode(presetNodeIn, true);
        svkPresetWorking = SvkPreset(svkPresetSaved);
        
        modeSlots.clear();
        for (auto mode : svkPresetWorking.getModeSlots())
        {
            modeSlots.add(new Mode(mode.getChild(0), false));
        }

        ValueTree customModeNode = svkPresetWorking.getCustomMode();
        modeCustom.reset(new Mode(customModeNode, false));

        if (sendChangeSignal)
            sendChangeMessage();

        return true;
    }

    DBG("Problem loading preset.");

    return false;
}

bool SvkPresetManager::saveNodeToFile(ValueTree nodeToSave, String saveMsg, String fileEnding, String absolutePath)
{
    File fileOut = File(absolutePath);
    
    if (fileOut.isDirectory())
    {
        FileChooser chooser(saveMsg,
            fileOut, fileEnding);

        chooser.browseForFileToSave(true);
        fileOut = chooser.getResult();
    }
    else if (!fileOut.exists())
    {
        FileChooser chooser(saveMsg,
            File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory),
            fileEnding);
        
        chooser.browseForFileToSave(true);
        fileOut = chooser.getResult();
    }

    if (fileOut.getParentDirectory().exists())
    {
        std::unique_ptr<XmlElement> xml(nodeToSave.createXml());
        return xml->writeTo(fileOut);
    }

    return false;
}

bool SvkPresetManager::savePresetToFile(String absolutePath)
{
    commitPreset();
    DBG("Saving the following node to " + absolutePath + ": " + svkPresetWorking.toString());
    return saveNodeToFile(svkPresetWorking.getPresetNode(), "Save preset", "*.svk", absolutePath);
}

bool SvkPresetManager::saveModeToFile(int modeSlotNumber, String absolutePath)
{
    Mode* mode;

    if (modeSlotNumber < modeSlots.size())
        mode = getModeInSlot(modeSlotNumber);
    else
        mode = modeCustom.get();
    
    bool saved = saveNodeToFile(mode->modeNode, "Save mode", "*.svk", absolutePath);

    if (saved)
    {
        ValueTree newCopy = mode->modeNode.createCopy();
        addAndSortMode(newCopy);
        loadedUserModes.add(newCopy);

        // Reset custom mode, since saved mode is a user mode now
        // Not sure if this should be done like that...
        if (mode == modeCustom.get())
            modeCustom.reset(new Mode("1"));
    }

    return saved;
}

ValueTree SvkPresetManager::nodeFromFile(String openMsg, String fileEnding, String absoluteFilePath)
{
    ValueTree nodeIn;
    File fileIn = File(absoluteFilePath);

    if (!fileIn.existsAsFile())
    {
        std::unique_ptr<FileChooser> chooser;

        if (fileIn.isDirectory())
            chooser.reset(new FileChooser(openMsg, fileIn, fileEnding));
        else
            chooser.reset(new FileChooser(openMsg, File::getSpecialLocation(File::userDocumentsDirectory), fileEnding));

        chooser->browseForFileToOpen();
        fileIn = chooser->getResult();
    }

    if (fileIn.existsAsFile())
    {
        std::unique_ptr<XmlElement> xml = parseXML(fileIn);
        nodeIn = ValueTree::fromXml(*(xml.get()));
        
        if (nodeIn.isValid())
            return nodeIn;
    }

    return ValueTree();
}

ValueTree SvkPresetManager::modeFromFile(String absoluteFilePath)
{
    ValueTree nodeIn = nodeFromFile("Open mode", "*.svk", absoluteFilePath);

    if (nodeIn.hasType(IDs::modePresetNode))
        return nodeIn;

    return ValueTree();
}

ValueTree SvkPresetManager::presetFromFile(String absoluteFilePath)
{
    ValueTree nodeIn = nodeFromFile("Open preset", "*.svk", absoluteFilePath);

    if (nodeIn.hasType(IDs::presetNode))
        return nodeIn;
    
    return ValueTree();
}

bool SvkPresetManager::commitPreset()
{
    return loadPreset(svkPresetWorking.getPresetNode(), false);
}

bool SvkPresetManager::resetToSavedPreset()
{
    DBG("PRESET MANAGER: Resetting to last saved preset.");
    return loadPreset(svkPresetSaved.getPresetNode(), false);
}

void SvkPresetManager::initializeModePresets()
{
    modeLibraryNode = ValueTree(IDs::modeLibraryNode);
    loadedFactoryModes.clear();
    loadedUserModes.clear();
    modesSorted.clear();
    for (int m = 0; m < numSortTypes; m++)
        modesSorted.add(Array<ValueTree>());

    createFactoryModes();
    loadModeDirectory();
}


void SvkPresetManager::createFactoryModes()
{
    if (!(bool) pluginSettingsNode[IDs::saveFactoryModes] || !(bool)pluginSettingsNode[IDs::createPresetFolder])
    {
        const char* factoryModes = BinaryData::FactoryModes_txt;
        int size = BinaryData::FactoryModes_txtSize;

        loadedFactoryModes.clear();

        if (factoryModes && size > 0)
        {
            MemoryInputStream instream(factoryModes, size, false);

            String line, steps, family, info, name;
            ValueTree factoryMode;

            while (instream.getNumBytesRemaining() > 0)
            {
                line = instream.readNextLine();
                steps = line.upToFirstOccurrenceOf(", ", false, true);
                
                family = line.upToFirstOccurrenceOf("; ", false, true);
                family = family.substring(steps.length() + 2, family.length());
                
                info = line.fromFirstOccurrenceOf("; ", false, true);

                factoryMode = Mode::createNode(steps, family, "", info, 60, true);

                addAndSortMode(factoryMode);
                loadedFactoryModes.add(factoryMode);
             }

            factoryMode = ValueTree();
        }
    }
    
    DBG("Amt of factory modes:" + String(loadedFactoryModes.size()));
}

void SvkPresetManager::loadModeDirectory()
{
    // TODO: Add checking for duplicate modes

    if ((bool)pluginSettingsNode[IDs::createPresetFolder])
    {
        File modeDirectory = File(pluginSettingsNode[IDs::modeDirectory]);
        Array<File> filesToLoad = modeDirectory.findChildFiles(File::TypesOfFileToFind::findFiles, true, "*.svk");

        std::unique_ptr<XmlElement> xml;
        ValueTree modeNodeIn;

        while (filesToLoad.size() > 0)
        {
            xml = parseXML(filesToLoad.removeAndReturn(0));
            modeNodeIn = ValueTree::fromXml(*(xml.get()));

            if (modeNodeIn.hasType(IDs::modePresetNode))
            {
                addAndSortMode(modeNodeIn);

                if ((bool)modeNodeIn[IDs::factoryPreset])
                {
                    loadedFactoryModes.add(modeNodeIn);
                }
                else
                {
                    loadedUserModes.add(modeNodeIn);
                }
            }
        }
    }
    
    DBG("New amt of Factory Modes: " + String(loadedFactoryModes.size()));
    DBG("Amt User Modes loaded: " + String(loadedUserModes.size()));
}


void SvkPresetManager::resortModeLibrary()
{
    modesSorted.clear();
    for (int m = 0; m < numSortTypes; m++)
        modesSorted.add(Array<ValueTree>());

    ValueTree modeToSort;

    for (int i = 0; i < modeLibraryNode.getNumChildren(); i++)
    {
        modeToSort = modeLibraryNode.getChild(i);
        addModeToSort(modeToSort);
    }
}

int SvkPresetManager::addModeToLibrary(ValueTree modeNodeIn)
{
    if (modeNodeIn.hasType(IDs::modePresetNode))
    {
        int libraryIndex = modeLibraryNode.getNumChildren();
        modeNodeIn.setProperty(IDs::libraryIndexOfMode, libraryIndex, nullptr);
        
        modeLibraryNode.appendChild(modeNodeIn, nullptr);

        return libraryIndex;
    }

    return -1;
}

void SvkPresetManager::addModeToSort(ValueTree modeNodeIn)
{
    if (modeNodeIn.hasType(IDs::modePresetNode))
    {
        modesSorted.getReference(SortType::scaleSize).addSorted(scaleSizeSort, modeNodeIn);
        modesSorted.getReference(SortType::modeSize).addSorted(modeSizeSort, modeNodeIn);
        modesSorted.getReference(SortType::familyName).addSorted(familyNameSort, modeNodeIn);

        if (!(bool)modeNodeIn[IDs::factoryPreset])
        {
            modesSorted.getReference(SortType::user).addSorted(scaleSizeSort, modeNodeIn);
        }
    }
}

int SvkPresetManager::addAndSortMode(ValueTree modeNodeIn)
{
    int ind = addModeToLibrary(modeNodeIn);
    addModeToSort(modeNodeIn);

    return ind;
}

void SvkPresetManager::requestModeMenu(PopupMenu* menuToUse)
{
    menuToUse->clear();
    
    PopupMenu scaleSizeMenu;
    PopupMenu modeSizeMenu;
    PopupMenu familyMenu;
    PopupMenu userMenu;
    PopupMenu favMenu;
    PopupMenu slotsMenu;
    
    ValueTree presetIn;
    String displayName;
    var separatorProperty;
    
    int numModes = 0;
    int subMenuIndex = 0;
    
    for (int subMenu = 0; subMenu <= SortType::user; subMenu++)
    {
        
        if (subMenu < SortType::user)
            numModes = modeLibraryNode.getNumChildren();
        else
            numModes = loadedUserModes.size();
        
        for (int presetNum = 0; presetNum < numModes; presetNum++)
        {
            presetIn = modesSorted.getUnchecked(subMenu).getUnchecked(presetNum);
            
            switch (subMenu)
            {
                case (SortType::scaleSize):
                    if (separatorProperty != presetIn[IDs::scaleSize])
                        scaleSizeMenu.addSeparator();
                    separatorProperty = presetIn[IDs::scaleSize];
                    displayName = presetIn[IDs::modeName];
                    scaleSizeMenu.addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::modeSize):
                    if (separatorProperty != presetIn[IDs::modeSize])
                        modeSizeMenu.addSeparator();
                    separatorProperty = presetIn[IDs::modeSize];
                    displayName = presetIn[IDs::modeName];
                    modeSizeMenu.addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::familyName):
                    if (separatorProperty != presetIn[IDs::family])
                        familyMenu.addSeparator();
                    separatorProperty = presetIn[IDs::family];
                    displayName = presetIn[IDs::modeName];
                    familyMenu.addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::user):
                    if (presetIn[IDs::factoryPreset])
                        continue;
                    if (separatorProperty != presetIn[IDs::scaleSize])
                        userMenu.addSeparator();
                    separatorProperty = presetIn[IDs::scaleSize];
                    displayName = presetIn[IDs::modeName];
                    userMenu.addItem(++subMenuIndex, displayName);
                    break;
            }
        }
    }
    
    // USERS
//    for (int i = 0; i < loadedUserModes.size(); i++)
//    {
//        userMenu.addItem(++subMenuIndex, loadedUserModes[i][IDs::modeName].toString());
//    }

    // FAVORITES
    for (int i = 0; i < favoriteModes.size(); i++)
    {
        favMenu.addItem(++subMenuIndex, favoriteModes[i][IDs::modeName].toString());
    }

    // SLOTS
    Mode* mode;

    for (int i = 0; i < modeSlots.size(); i++)
    {
        mode = modeSlots[i];
        slotsMenu.addItem(++subMenuIndex, mode->getName());
    }

    menuToUse->addSubMenu("by Scale Size", scaleSizeMenu);
    menuToUse->addSubMenu("by Mode Size", modeSizeMenu);
    menuToUse->addSubMenu("by Family", familyMenu);
    menuToUse->addSubMenu("User", userMenu);
    menuToUse->addSubMenu("Favorites", favMenu);

    menuToUse->addSeparator();
    menuToUse->addSubMenu("Slots", slotsMenu);
    menuToUse->addSeparator();

    String customModeName = modeCustom->getName();

    if (customModeName == "undefined[1] 1")
        customModeName = "Custom Mode";

    menuToUse->addItem(++subMenuIndex, customModeName);
}
