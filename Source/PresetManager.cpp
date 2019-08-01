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

	SvkPreset* preset = presetsLoaded[0];

	if (!preset)
	{
		presetsLoaded.set(slotNumIn, new SvkPreset());
	}

	return preset;
}

int SvkPresetManager::getNumPresetsLoaded()
{
    return presetsLoaded.size();
}

int SvkPresetManager::getNumMenuItems(bool withFactoryMenu , bool withUserMenu, bool withFavMenu, bool withSlots)
{
	int totalMenuItems = 0;
	
	if (withFactoryMenu)
		totalMenuItems += 
		modesSorted[0].size() + modesSorted[1].size() + modesSorted[2].size();
	
	if (withUserMenu)
		totalMenuItems += loadedUserModes.size();

	if (withFavMenu)
		totalMenuItems += favoriteModes.size();

	if (withSlots)
		totalMenuItems += modeSlots.size();

	return totalMenuItems;
}

ValueTree SvkPresetManager::getModeInLibrary(int indexIn)
{
    if (indexIn < 0)
        indexIn = 0;
    
	int subMenu = indexIn / getNumMenuItems(true, true, false, false);
	int index = indexIn % getNumMenuItems(true, true, false, false);
    
    if (subMenu < modesSorted.size())
        if (index < modesSorted.getUnchecked(subMenu).size())
            return modesSorted.getUnchecked(subMenu).getUnchecked(index);
    
    return ValueTree();
}

Mode* SvkPresetManager::getModeInSlots(int presetNumIn, int slotNumIn)
{
	SvkPreset* preset = presetsLoaded[presetNumIn];

	if (preset && preset->getModeSlotsSize() > slotNumIn)
	{
		Mode* mode = modeSlots.getUnchecked(presetNumIn)->getUnchecked(slotNumIn);
		//DBG("GRABBING MODE: " + String(slotNumIn) + " which is " + mode->getDescription());
		return mode;
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

Mode* SvkPresetManager::setModeCustom(Mode* modeIn)
{
	modeCustom.reset(modeIn);
	return modeCustom.get();
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

int SvkPresetManager::setSlotToMode(int presetSlotNum, int modeSlotNum, ValueTree modeNode)
{
	presetsLoaded[presetSlotNum]->setModeSlot(modeNode, modeSlotNum);
	modeSlots.getUnchecked(presetSlotNum)->set(modeSlotNum, new Mode(modeNode, false));
	return modeSlotNum;
}
int SvkPresetManager::addSlot(int presetSlotNum, ValueTree modeNode)
{
	return setSlotToMode(presetSlotNum, presetsLoaded[presetSlotNum]->getModeSlotsSize(), modeNode);
}

int SvkPresetManager::setSlotAndSelection(int presetSlotNum, int modeSlotNum, int modeSelectorNum, ValueTree modeNode)
{
	modeSlotNum = setSlotToMode(presetSlotNum, modeSlotNum, modeNode);
	presetsLoaded[presetSlotNum]->setModeSelectorSlotNum(modeSelectorNum, modeSlotNum);
	return modeSlotNum;
}

int SvkPresetManager::addSlotAndSetSelection(int presetSlotNum, int modeSelectorNumber, ValueTree modeNode)
{
	int modeSlotNum = addSlot(presetSlotNum, modeNode);
	presetsLoaded[presetSlotNum]->setModeSelectorSlotNum(modeSelectorNumber, modeSlotNum);
	return modeSlotNum;
}


void SvkPresetManager::removeMode(int presetSlotNum, int modeSlotNum)
{
    SvkPreset* preset = presetsLoaded[presetSlotNum];
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
    SvkPreset* preset = presetsLoaded[presetSlotNum];
    OwnedArray<Mode>* slot = modeSlots[presetSlotNum];
    
    if (preset && slot)
    {
        slot->clear();

        for (int i = 0; i < slot->size(); i ++)
        {
            preset->setModeSelectorSlotNum(i, 0);
        }
        
        addSlot(presetSlotNum, Mode::createNode("1"));
    }
}

void SvkPresetManager::refreshModeSlot(int presetSlotNum)
{
	SvkPreset* preset = presetsLoaded[presetSlotNum];
	OwnedArray<Mode>* slot = modeSlots[presetSlotNum];

	if (preset && slot)
	{
		slot->clear();

		for (int i = 0; i < preset->getModeSlotsSize(); i++)
		{
			slot->add(new Mode(preset->getModeInSlot(i), false));
		}
	}
}

void SvkPresetManager::handleModeSelection(int presetSlotNum, int modeBoxNumber, int idIn)
{
    SvkPreset* preset = presetsLoaded[presetSlotNum];
    OwnedArray<Mode>* slot = modeSlots.getUnchecked(presetSlotNum);
    
	int numSortedModes = getNumMenuItems(true, false, false, false);

    int modeLibraryIndex = idIn - 1;
    int userModesIndex = modeLibraryIndex - numSortedModes;
    int favIdx = userModesIndex - loadedUserModes.size();
    int slotIdx = favIdx - favoriteModes.size();
    
//    DBG("Sorted Index: " + String(modeLibraryIndex) + " out of " + String(numSortedModes));
//    DBG("User Index: " + String(userModesIndex) + " out of " + String(loadedUserModes.size()));
//    DBG("Fav Index: " + String(favIdx) + " out of " + String(favoriteModes.size()));
//    DBG("Slot Index: " + String(slotIdx) + " out of " + String(modeSlots.size()));

	int modeSlotNumber = modeBoxNumber;

    ValueTree modeSelected;
    
    if (modeLibraryIndex < numSortedModes)
    {
        modeSelected = getModeInLibrary(modeLibraryIndex);
    }
    else if (userModesIndex < loadedUserModes.size())
    {
        modeSelected = loadedUserModes[userModesIndex];
    }
    else if (favIdx < favoriteModes.size())
    {
        modeSelected = favoriteModes[favIdx];
    }
	else if (slotIdx < slot->size())
	{
		modeSelected = ValueTree();
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
        setSlotToMode(presetSlotNum, modeBoxNumber, modeSelected);
    }
    
	preset->setModeSelectorSlotNum(modeBoxNumber, modeSlotNumber);
}


bool SvkPresetManager::loadPreset(int presetSlotNum, ValueTree presetNodeIn, bool sendChangeSignal)
{
	if (presetNodeIn.isValid())
	{
		DBG("Loading Preset:" + presetNodeIn.toXmlString());
	
		presetsLoaded.set(presetSlotNum, new SvkPreset(presetNodeIn));
        SvkPreset* preset = presetsLoaded[presetSlotNum];

		OwnedArray<Mode>* modes = modeSlots[presetSlotNum];
        modes->clear();

		for (int i = 0; i < preset->getModeSlotsSize(); i++)
		{
			modes->set(i, new Mode(preset->getModeInSlot(i)));
		}

		ValueTree customModeNode = preset->parentNode.getChildWithName(IDs::modeCustomNode);
		setModeCustom(customModeNode.getChildWithName(IDs::modePresetNode));

		if (sendChangeSignal)
			sendChangeMessage();
	}

	return presetNodeIn.isValid();
}

bool SvkPresetManager::loadPreset(int presetSlotNum, int indexIn, bool sendChangeSignal)
{
	return loadPreset(presetSlotNum, presetsLoaded[indexIn]->parentNode, sendChangeSignal);
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
	SvkPreset* preset = presetsLoaded[presetSlotNum];
	DBG("Saving this to " + absolutePath + ": " + preset->parentNode.toXmlString());
	return saveNodeToFile(preset->parentNode, "Save preset", "*.svk", absolutePath);
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
	SvkPreset* preset = presetsLoaded[slotNumber];

	if (preset && SvkPreset::isValidPresetNode(nodeIn))
	{
		preset->restoreFromNode(nodeIn);// , true);
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
    //requestModeMenu();

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
    for (int i = 0; i < loadedUserModes.size(); i++)
    {
        userMenu.addItem(++subMenuIndex, loadedUserModes[i][IDs::modeName].toString());
    }

	// FAVORITES
	for (int i = 0; i < favoriteModes.size(); i++)
	{
		favMenu.addItem(++subMenuIndex, favoriteModes[i][IDs::modeName].toString());
	}

	// SLOTS
	OwnedArray<Mode>* slot = modeSlots.getUnchecked(0);
	Mode* mode;

	for (int i = 0; i < slot->size(); i++)
	{
		mode = slot->getUnchecked(i);
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

    String customModeName = "Custom Mode";

    if (modeCustom->getFamily() != "undefined")
        customModeName = modeCustom->getDescription();

    menuToUse->addItem(++subMenuIndex, customModeName);
}
