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
	presetsLoaded = OwnedArray<SvkPreset>();

    initializeModePresets();
}

SvkPresetManager::~SvkPresetManager()
{
	removeAllChangeListeners();
}

SvkPreset* SvkPresetManager::getPresetLoaded(int slotNumIn)
{
	SvkPreset* preset = presetsLoaded[slotNumIn];

	if (!preset)
	{
		preset = new SvkPreset();
		presetsLoaded.set(slotNumIn, preset);
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
	SvkPreset* preset = presetsLoaded[presetNumIn];

	if (preset && preset->getNumModes() > slotNumIn)
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
	presetsLoaded[presetSlotNum]->setModeSlot(modeNode, modeSlotNum);
	
	loadModeIntoSlot(presetSlotNum, modeSlotNum, modeNode);
}

int SvkPresetManager::addModeToNewSlot(int presetSlotNum, int modeLibraryIndexIn)
{
	ValueTree modeNode = getModeInLibrary(modeLibraryIndexIn);
	int slotNumberOut = presetsLoaded[presetSlotNum]->addMode(modeNode);

	loadModeIntoSlot(presetSlotNum, slotNumberOut, modeNode);

	return slotNumberOut;
}

int SvkPresetManager::addModeToNewSlot(int presetSlotNum, ValueTree modePresetNodeIn)
{
	int slotNumberOut = presetsLoaded[presetSlotNum]->addMode(modePresetNodeIn);

	loadModeIntoSlot(presetSlotNum, slotNumberOut, modePresetNodeIn);

	return slotNumberOut;
}

Mode* SvkPresetManager::loadModeIntoSlot(int presetSlotNum, int modeSlotNum, ValueTree modeNode)
{
	modeSlots[presetSlotNum]->ensureStorageAllocated(modeSlotNum + 1);
	return modeSlots[presetSlotNum]->set(modeSlotNum, new Mode(modeNode));
}

bool SvkPresetManager::loadPreset(int presetSlotNum, ValueTree presetNodeIn, bool sendChangeSignal)
{
	if (presetNodeIn.isValid())
	{
		presetsLoaded.ensureStorageAllocated(presetSlotNum + 1);
		modeSlots.ensureStorageAllocated(presetSlotNum + 1);

		SvkPreset* preset = presetsLoaded.set(presetSlotNum, new SvkPreset(presetNodeIn.createCopy()));
		
		OwnedArray<Mode>* modes = modeSlots[presetSlotNum];

		if (modes)
			modes->clear();
		else
			modes = modeSlots.set(presetSlotNum, new OwnedArray<Mode>());

		for (int i = 0; i < preset->getNumModes(); i++)
			modes->add(new Mode(preset->getModeInSlot(i)));

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
	SvkPreset* preset = getPresetLoaded(presetSlotNum);
	preset->commitPreset();

	return saveNodeToFile(preset->parentNode, "Save preset", ".svk", pluginSettingsNode[IDs::presetDirectory]);
}

bool SvkPresetManager::saveModeToFile(int presetSlotNum, int modeSlotNumber, String absolutePath)
{
	Mode* mode;

	if (modeSlotNumber < modeSlots.size())
		mode = getModeInSlots(presetSlotNum, modeSlotNumber);
	else
		mode = modeCustom.get();

	return saveNodeToFile(mode->modeNode, "Save mode", ".svk", pluginSettingsNode[IDs::modeDirectory]);
}

bool SvkPresetManager::saveModeToFile(String absolutePath)
{
	return saveNodeToFile(modeCustom->modeNode, "Save mode", ".svk", pluginSettingsNode[IDs::modeDirectory]);
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
    
	loadPreset(0, new SvkPreset(), false);
	addModeToNewSlot(0, 8);
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
    modeSubMenu.clear();
    
    PopupMenu* scaleSizeSubMenu = modeSubMenu.add(new PopupMenu());
	PopupMenu* modeSizeSubMenu = modeSubMenu.add(new PopupMenu());
    PopupMenu* familySubMenu = modeSubMenu.add(new PopupMenu());
    PopupMenu* userSubMenu = modeSubMenu.add(new PopupMenu());
    
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
                        scaleSizeSubMenu->addSeparator();
                    separatorProperty = presetIn[IDs::scaleSize];
                    displayName = presetIn[IDs::modeName];
                    scaleSizeSubMenu->addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::modeSize):
                    if (separatorProperty != presetIn[IDs::modeSize])
						modeSizeSubMenu->addSeparator();
                    separatorProperty = presetIn[IDs::modeSize];
                    displayName = presetIn[IDs::modeName];
					modeSizeSubMenu->addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::familyName):
                    if (separatorProperty != presetIn[IDs::family])
                        familySubMenu->addSeparator();
                    separatorProperty = presetIn[IDs::family];
                    displayName = presetIn[IDs::modeName];
                    familySubMenu->addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::user):
                    if (presetIn[IDs::factoryPreset])
                        continue;
                    if (separatorProperty != presetIn[IDs::scaleSize])
                        userSubMenu->addSeparator();
                    separatorProperty = presetIn[IDs::scaleSize];
                    displayName = presetIn[IDs::modeName];
                    userSubMenu->addItem(++subMenuIndex, displayName);
                    break;
            }
        }
    }

	modeMenu->addSubMenu("by Scale Size", *scaleSizeSubMenu);
	modeMenu->addSubMenu("by Mode Size", *modeSizeSubMenu);
	modeMenu->addSubMenu("by Family", *familySubMenu);
	modeMenu->addSubMenu("User", *userSubMenu);
	modeMenu->addSubMenu("Favorites", PopupMenu());

	modeMenu->addSeparator();
	modeMenu->addSubMenu("Slots", PopupMenu());
	modeMenu->addSeparator();

	modeMenu->addItem(modeMenu->getNumItems(), "Custom Mode");
}
