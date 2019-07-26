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
}

SvkPresetManager::~SvkPresetManager()
{
	removeAllChangeListeners();
}

SvkPreset* SvkPresetManager::getPresetLoaded(int slotNumIn)
{
	slotNumIn = slotNumIn % presetsLoaded.size();

	SvkPreset* preset = &presetsLoaded.getReference(slotNumIn);

	if (!preset)
	{
		presetsLoaded.set(slotNumIn, SvkPreset());
	}

	return preset;
}

int SvkPresetManager::getPresetLoadedId(int slotNumIn)
{
	SvkPreset* preset = getPresetLoaded(slotNumIn);
    return preset->parentNode[IDs::libraryIndexOfMode];
}

ValueTree SvkPresetManager::getPresetLoadedNode(int slotNumIn)
{
	SvkPreset* preset = getPresetLoaded(slotNumIn);
	return preset->parentNode;
}

Array<Array<ValueTree>>* SvkPresetManager::getPresetsSorted()
{
	return &modesSorted;
}

int SvkPresetManager::getNumModesLoaded()
{
	return numberOfModes;
}

int SvkPresetManager::getNumModesInFavorites()
{
	return favoriteModes.size();
}

int SvkPresetManager::getNumMenuItems()
{
	int totalMenuItems = scaleSizeMenu->getNumItems() + 
		modeSizeMenu->getNumItems() + 
		familyMenu->getNumItems() + 
		userMenu->getNumItems() + 
		favMenu->getNumItems() + 
		slotsMenu->getNumItems();

	return totalMenuItems;
}


ValueTree SvkPresetManager::getModeInLibrary(int indexIn)
{
    if (indexIn < 0)
        indexIn = 0;
    
	int subMenu = indexIn / numberOfModes;
	int index = indexIn % numberOfModes;
    
    if (subMenu < modesSorted.size())
        if (index < modesSorted.getUnchecked(subMenu).size())
            return modesSorted.getUnchecked(subMenu).getUnchecked(index);
    
    return ValueTree();
}

Mode* SvkPresetManager::getModeInSlots(int presetNumIn, int slotNumIn)
{
	SvkPreset* preset = &presetsLoaded.getReference(presetNumIn);

	if (preset && preset->getModeSlotsSize() > slotNumIn)
	{
		return modeSlots.getUnchecked(presetNumIn)->getUnchecked(slotNumIn);
	}
	else
	{
		return modeCustom.get();
	}
}

Mode* SvkPresetManager::getModeCustom()
{
	return modeCustom.get();
}

PopupMenu* SvkPresetManager::getModeMenu()
{
	return modeMenu.get();
}

void SvkPresetManager::buildSlotsMenu()
{
    slotsMenu->clear();
    OwnedArray<Mode>* slot = modeSlots.getUnchecked(0);
    
    for (int i = 0; i < modeSlots.size(); i++)
    {
        slotsMenu->addItem(modeMenu->getNumItems(), slot->getUnchecked(i)->getName());
    }
}

void SvkPresetManager::buildFavoritesMenu()
{
    
}

Mode* SvkPresetManager::setModeCustom(ValueTree modeNodeIn)
{
	modeCustom.reset(new Mode(modeNodeIn));
	return modeCustom.get();
}

Mode* SvkPresetManager::setModeCustom(String stepsIn, String familyIn, int rootNoteIn, String nameIn, String infoIn)
{
	modeCustom.reset(new Mode(stepsIn, familyIn, rootNoteIn, nameIn, infoIn));
    return modeCustom.get();
}

void SvkPresetManager::replaceModeInPreset(int presetSlotNum, int modeSlotNum, int modeLibraryIndexIn)
{
	ValueTree modeNode = getModeInLibrary(modeLibraryIndexIn);
	presetsLoaded.getReference(presetSlotNum).setModeSlot(modeNode, modeSlotNum);
	
	loadModeIntoSlot(presetSlotNum, modeSlotNum, modeNode);
}

int SvkPresetManager::addModeToNewSlot(int presetSlotNum, int modeLibraryIndexIn)
{
	ValueTree modeNode = getModeInLibrary(modeLibraryIndexIn);
	int slotNumberOut = presetsLoaded.getUnchecked(presetSlotNum).addMode(modeNode);

	loadModeIntoSlot(presetSlotNum, slotNumberOut, modeNode);

	return slotNumberOut;
}

int SvkPresetManager::addModeToNewSlot(int presetSlotNum, ValueTree modePresetNodeIn)
{
	int slotNumberOut = presetsLoaded.getUnchecked(presetSlotNum).addMode(modePresetNodeIn);

	loadModeIntoSlot(presetSlotNum, slotNumberOut, modePresetNodeIn);

	return slotNumberOut;
}

Mode* SvkPresetManager::loadModeIntoSlot(int presetSlotNum, int modeSlotNum, ValueTree modeNode)
{
	SvkPreset* preset = &presetsLoaded.getReference(presetSlotNum);
	preset->setModeSlot(modeNode, modeSlotNum);
	DBG("Preset Mode slot size: " + String(preset->getModeSlotsSize()));

	OwnedArray<Mode>* slot = modeSlots.getUnchecked(presetSlotNum);
	slot->set(modeSlotNum, new Mode(modeNode));
	
	return slot->getUnchecked(modeSlotNum);
}

Mode* SvkPresetManager::loadModeIntoSlot(int presetSlotNum, int modeSlotNum, int modeLibraryIndexIn)
{
	return loadModeIntoSlot(presetSlotNum, modeSlotNum, getModeInLibrary(modeLibraryIndexIn));
}

void SvkPresetManager::removeMode(int presetSlotNum, int modeSlotNum)
{
    SvkPreset* preset = &presetsLoaded.getReference(presetSlotNum);
    OwnedArray<Mode>* slot = modeSlots[presetSlotNum];
    
    if (preset && slot)
    {
        slot->remove(modeSlotNum);
        
        // Decrement mode source slot numbers if a mode in the middle was deleted
        if (slot->size() > modeSlotNum)
        {
            for (int i = 0; i < slot->size(); i ++)
            {
                int ind = preset->getSlotNumberBySelector(i);
                
                if (ind > modeSlotNum)
                    preset->setModeSelectorSlotNum(i, ind-1);
            }
        }
    }
}

void SvkPresetManager::resetModeSlot(int presetSlotNum)
{
    SvkPreset* preset = &presetsLoaded.getReference(presetSlotNum);
    OwnedArray<Mode>* slot = modeSlots[presetSlotNum];
    
    if (preset && slot)
    {
        slot->clear();

        for (int i = 0; i < slot->size(); i ++)
        {
            preset->setModeSelectorSlotNum(i, 0);
        }
        
        addModeToNewSlot(presetSlotNum, Mode::createNode("1"));
    }
}

void SvkPresetManager::handleModeSelection(int presetSlotNum, int modeBoxNumber, int idIn)
{
	DBG("ID Selected: " + String(idIn));

    SvkPreset& preset = presetsLoaded.getReference(presetSlotNum);
    OwnedArray<Mode>* slot = modeSlots.getUnchecked(presetSlotNum);
    
	int factoryModeTotalMenuSize = scaleSizeMenu->getNumItems() + modeSizeMenu->getNumItems() + familyMenu->getNumItems();

    int modeLibraryIndex = idIn - 1;
    int favIdx = modeLibraryIndex - factoryModeTotalMenuSize;
    int slotIdx = favIdx - favoriteModes.size();

	int modeSlotNumber = modeBoxNumber;

    ValueTree modeSelected;
    
    if (modeLibraryIndex < factoryModeTotalMenuSize)
    {
        modeSelected = getModeInLibrary(modeLibraryIndex);
    }
    else if (favIdx < favoriteModes.size())
    {
        modeSelected = favoriteModes[favIdx];
    }
	else if (slotIdx < slot->size() - 2)
	{
		modeSelected = slot->getUnchecked(slotIdx)->modeNode;
		modeSlotNumber = slotIdx;
	}
	else
	{
		modeSelected = getModeCustom()->modeNode;
		modeSlotNumber = slot->size();
		DBG("Custom mode selected");
	}
    
    if (modeSelected.isValid())
    {
        loadModeIntoSlot(presetSlotNum, modeBoxNumber, modeSelected);
    }
    else // if slot number exceeds the slot size, the custom mode will be used
    {
        preset.setModeSelectorSlotNum(modeBoxNumber, slotIdx);
		modeSlotNumber = preset.getModeSlotsSize();
    }
    
	preset.setModeSelectorSlotNum(modeBoxNumber, modeSlotNumber);
    //buildSlotsMenu();
}


bool SvkPresetManager::loadPreset(int presetSlotNum, ValueTree presetNodeIn, bool sendChangeSignal)
{
	if (presetNodeIn.isValid())
	{
		DBG("Loading Preset:" + presetNodeIn.toXmlString());
	
		presetsLoaded.set(presetSlotNum, SvkPreset(presetNodeIn));
        SvkPreset* preset = &presetsLoaded.getReference(presetSlotNum);

		OwnedArray<Mode>* modes = modeSlots[presetSlotNum];
        modes->clear();

		for (int i = 0; i < preset->getModeSlotsSize(); i++)
		{
			modes->set(i, new Mode(preset->getModeInSlot(i)));
		}

		if (sendChangeSignal)
			sendChangeMessage();
	}

	return presetNodeIn.isValid();
}

bool SvkPresetManager::loadPreset(int presetSlotNum, int indexIn, bool sendChangeSignal)
{
	return loadPreset(presetSlotNum, presetsLoaded.getUnchecked(indexIn).parentNode, sendChangeSignal);
}

bool SvkPresetManager::loadPreset(int presetSlotNum, SvkPreset* presetIn, bool sendChangeSignal)
{
	return loadPreset(presetSlotNum, presetIn->parentNode, sendChangeSignal);
}

bool SvkPresetManager::loadPreset(int presetSlotNum, bool sendChangeSignal)
{
	return loadPreset(presetSlotNum, presetFromFile(pluginSettingsNode[IDs::presetDirectory]), sendChangeSignal);
}

bool SvkPresetManager::saveNodeToFile(ValueTree nodeToSave, String saveMsg, String fileEnding, String absolutePath)
{
	File fileOut = File(absolutePath);

	if (!fileOut.getParentDirectory().exists())
	{
		FileChooser chooser(saveMsg,
			pluginSettingsNode[IDs::presetDirectory].toString(),
			fileEnding);

		chooser.browseForFileToSave(true);
		fileOut = chooser.getResult();
	}

	if (fileOut.getParentDirectory().exists())
	{
		std::unique_ptr<XmlElement> xml(nodeToSave.createXml());
		return xml->writeToFile(fileOut, "");
	}

	return false;
}

bool SvkPresetManager::savePresetToFile(int presetSlotNum, String absolutePath)
{
	SvkPreset* preset = getPresetLoaded(presetSlotNum);

	return saveNodeToFile(preset->parentNode, "Save preset", "*.svk", pluginSettingsNode[IDs::presetDirectory]);
}

bool SvkPresetManager::saveModeToFile(int presetSlotNum, int modeSlotNumber, String absolutePath)
{
	Mode* mode;

	if (modeSlotNumber < modeSlots.size())
		mode = getModeInSlots(presetSlotNum, modeSlotNumber);
	else
		mode = modeCustom.get();

	return saveNodeToFile(mode->modeNode, "Save mode", "*.svk", pluginSettingsNode[IDs::modeDirectory]);
}

bool SvkPresetManager::saveModeToFile(String absolutePath)
{
	return saveNodeToFile(modeCustom->modeNode, "Save mode", "*.svk", pluginSettingsNode[IDs::modeDirectory]);
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

bool SvkPresetManager::commitPreset(int slotNumber, ValueTree nodeIn)
{
	SvkPreset* preset = &presetsLoaded.getReference(slotNumber);

	if (preset && SvkPreset::isValidPresetNode(nodeIn))
	{
		preset->restoreFromNode(nodeIn);
		return true;
	}

	return false;
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
    buildModeMenu();

	modeSlots.add(new OwnedArray<Mode>());

	setModeCustom("1");
    
	loadPreset(0, ValueTree(IDs::presetNode), false);
    handleModeSelection(0, 0, 9);
    handleModeSelection(0, 1, 9);
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

                factoryMode = Mode::createNode(steps, family, "", info, true);
				
				addAndSortMode(factoryMode);
                loadedFactoryModes.add(factoryMode);
		 	}

			factoryMode = ValueTree();
		}
	}
}

void SvkPresetManager::loadModeDirectory()
{
	// TODO: Add checking for duplicate modes

	if ((bool)pluginSettingsNode[IDs::createPresetFolder])
	{
		File modeDirectory = File(pluginSettingsNode[IDs::presetDirectory]);
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
		numberOfModes = modeLibraryNode.getNumChildren();

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

void SvkPresetManager::buildModeMenu()
{
    modeMenu.reset(new PopupMenu());
    subMenus.clear();
    
    scaleSizeMenu = subMenus.add(new PopupMenu());
	modeSizeMenu = subMenus.add(new PopupMenu());
    familyMenu = subMenus.add(new PopupMenu());
    userMenu = subMenus.add(new PopupMenu());
    
    favMenu = subMenus.add(new PopupMenu());
    slotsMenu = subMenus.add(new PopupMenu());
    
    ValueTree presetIn;
    String displayName;
    var separatorProperty;
    
    int subMenuIndex = 0;
    
    for (int subMenu = 0; subMenu < SortType::user; subMenu++)
    {
        for (int presetNum = 0; presetNum < modeLibraryNode.getNumChildren(); presetNum++)
        {
            presetIn = modesSorted.getUnchecked(subMenu).getUnchecked(presetNum);
            
            switch (subMenu)
            {
                case (SortType::scaleSize):
                    if (separatorProperty != presetIn[IDs::scaleSize])
                        scaleSizeMenu->addSeparator();
                    separatorProperty = presetIn[IDs::scaleSize];
                    displayName = presetIn[IDs::modeName];
                    scaleSizeMenu->addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::modeSize):
                    if (separatorProperty != presetIn[IDs::modeSize])
						modeSizeMenu->addSeparator();
                    separatorProperty = presetIn[IDs::modeSize];
                    displayName = presetIn[IDs::modeName];
					modeSizeMenu->addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::familyName):
                    if (separatorProperty != presetIn[IDs::family])
                        familyMenu->addSeparator();
                    separatorProperty = presetIn[IDs::family];
                    displayName = presetIn[IDs::modeName];
                    familyMenu->addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::user):
                    if (presetIn[IDs::factoryPreset])
                        continue;
                    if (separatorProperty != presetIn[IDs::scaleSize])
                        userMenu->addSeparator();
                    separatorProperty = presetIn[IDs::scaleSize];
                    displayName = presetIn[IDs::modeName];
                    userMenu->addItem(++subMenuIndex, displayName);
                    break;
            }
        }
    }

	modeMenu->addSubMenu("by Scale Size", *scaleSizeMenu);
	modeMenu->addSubMenu("by Mode Size", *modeSizeMenu);
	modeMenu->addSubMenu("by Family", *familyMenu);
	modeMenu->addSubMenu("User", *userMenu);
	modeMenu->addSubMenu("Favorites", *favMenu);

	modeMenu->addSeparator();
	modeMenu->addSubMenu("Slots", *slotsMenu);
	modeMenu->addSeparator();

	modeMenu->addItem(++subMenuIndex, "Custom Mode");
}
