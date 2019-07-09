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

ValueTree SvkPresetManager::getMode(int indexIn)
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

PopupMenu* SvkPresetManager::getModeMenu()
{
    return modeMenu.get();
}

bool SvkPresetManager::loadMode(int presetSlotNum, int modeSlotNum, ValueTree presetNodeIn,  bool sendChangeSignal)
{
    if (presetNodeIn.isValid())
    {
		presetsLoaded[presetSlotNum]; // TODO
                
        if (sendChangeSignal)
            sendChangeMessage();
    }
    
	return presetNodeIn.isValid();
}

bool SvkPresetManager::loadMode(int presetSlotNum,  int modeSlotNum, SvkPreset* presetIn, bool sendChangeSignal)
{
	return loadPreset(slotNumber, presetIn->parentNode, sendChangeSignal);
}

bool SvkPresetManager::loadMode(int presetSlotNum, int modeSlotNum, bool sendChangeSignal)
{
	return loadPreset(slotNumber, presetFromFile(pluginSettingsNode[IDs::presetDirectory]), sendChangeSignal);
}

bool SvkPresetManager::loadPreset(int slotNumber, ValueTree presetNodeIn, bool sendChangeSignal)
{
	if (presetNodeIn.isValid())
	{
		presetsLoaded.set(slotNumber, new SvkPreset(presetNodeIn.createCopy()));

		if (sendChangeSignal)
			sendChangeMessage();
	}

	return presetNodeIn.isValid();
}

bool SvkPresetManager::loadPreset(int slotNumber, int indexIn, bool sendChangeSignal)
{
	return loadPreset(slotNumber, getMode(indexIn), sendChangeSignal);
}

bool SvkPresetManager::loadPreset(int slotNumber, SvkPreset* presetIn, bool sendChangeSignal)
{
	return loadPreset(slotNumber, presetIn->parentNode, sendChangeSignal);
}

bool SvkPresetManager::loadPreset(int slotNumber, bool sendChangeSignal)
{
	return loadPreset(slotNumber, presetFromFile(pluginSettingsNode[IDs::presetDirectory]), sendChangeSignal);
}


bool SvkPresetManager::savePreset(int slotNumber, String absolutePath)
{
    File fileOut = File(absolutePath);
        
    if (!fileOut.getParentDirectory().exists())
    {
        FileChooser chooser("Save your preset",
                            pluginSettingsNode[IDs::presetDirectory].toString(),
                            "*.svk");
        
        chooser.browseForFileToSave(true);
        fileOut = chooser.getResult();
    }
    
    if (fileOut.getParentDirectory().exists())
    {
		SvkPreset* preset = getPresetLoaded(slotNumber);
        std::unique_ptr<XmlElement> xml(preset->parentNode.createXml());
        return xml->writeToFile(fileOut, "");
    }
    
    return false;
}

ValueTree SvkPresetManager::presetFromFile(String absoluteFilePath)
{
	ValueTree nodeIn;
	File fileIn = File(absoluteFilePath);
	
	if (!fileIn.existsAsFile())
	{
		std::unique_ptr<FileChooser> chooser;

		if (fileIn.isDirectory())
			chooser.reset(new FileChooser("Open preset", fileIn, "*.svk"));
		else
			chooser.reset(new FileChooser("Open preset", File::getSpecialLocation(File::userDocumentsDirectory), "*.svk"));
		
		chooser->browseForFileToOpen();
		fileIn = chooser->getResult();
	}

	if (fileIn.existsAsFile())
	{
		std::unique_ptr<XmlElement> xml = parseXML(fileIn);
		nodeIn = ValueTree::fromXml(*(xml.get()));

        bool hasChild;
        if (Mode::isValidMode(nodeIn, hasChild))
		{
            return nodeIn;
		}
	}

	return ValueTree();
}

bool SvkPresetManager::commitPresetNode(int slotNumber, ValueTree nodeIn)
{
    if (nodeIn.isValid())
    {
        ValueTree nodeChild;
        
        // MODE SETTINGS
        nodeChild = nodeIn.getChildWithName(IDs::modePresetNode);
        commitModeNode(slotNumber, nodeChild);
        
        // KEYBOARD SETTINGS
        nodeChild = nodeIn.getChildWithName(IDs::pianoNode);
        commitKeyboardNode(slotNumber, nodeChild);
        
        // MIDI MAPS
        nodeChild = nodeIn.getChildWithName(IDs::midiMapNode);
        commitMapNode(slotNumber, nodeChild);
        
        return true;
    }
    
    return false;
}

bool SvkPresetManager::commitModeNode(int slotNumber, ValueTree modeNodeIn)
{
    if (modeNodeIn.hasType(IDs::modePresetNode))
    {
		SvkPreset* preset = getPresetLoaded(slotNumber);
		preset->updateModeNode(modeNodeIn);
        return true;
    }
    
    return false;
}

bool SvkPresetManager::commitKeyboardNode(int slotNumber, ValueTree keyboardNodeIn)
{
    if (keyboardNodeIn.hasType(IDs::pianoNode))
    {
		SvkPreset* preset = getPresetLoaded(slotNumber);
        preset->updateKeyboardNode(keyboardNodeIn);
        return true;
    }
    
    return false;
}

bool SvkPresetManager::commitMapNode(int slotNumber, ValueTree mapNodeIn)
{
    if (mapNodeIn.hasType(IDs::midiMapNode))
    {
		SvkPreset* preset = getPresetLoaded(slotNumber);
        preset->updateMidiNode(mapNodeIn);
        return true;
    }
    
    return false;
}

void SvkPresetManager::initializeModePresets()
{
	modeLibraryNode = ValueTree(IDs::modeLibraryNode);
	loadedFactoryPresets.clear();
	loadedUserPresets.clear();
	modesSorted.clear();
	for (int m = 0; m < numSortTypes; m++)
		modesSorted.add(Array<ValueTree>());

	createFactoryModes();
	loadPresetDirectory();
    buildPresetMenu();
    
    loadPreset(0, 8);
}


void SvkPresetManager::createFactoryModes()
{
	if (!(bool) pluginSettingsNode[IDs::saveFactoryPresets] || !(bool)pluginSettingsNode[IDs::createPresetFolder])
	{
		const char* factoryPresets = BinaryData::FactoryModes_txt;
		int size = BinaryData::FactoryModes_txtSize;

		loadedFactoryPresets.clear();

		if (factoryPresets && size > 0)
		{
			MemoryInputStream instream(factoryPresets, size, false);

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
                loadedFactoryPresets.add(factoryMode);
			}
		}
	}
}

void SvkPresetManager::loadPresetDirectory()
{
	// TODO: Add checking for duplicate modes

	if ((bool)pluginSettingsNode[IDs::createPresetFolder])
	{
		File presetDirectory = File(pluginSettingsNode[IDs::presetDirectory]);
		Array<File> filesToLoad = presetDirectory.findChildFiles(File::TypesOfFileToFind::findFiles, true, "*.svk");

		std::unique_ptr<XmlElement> xml;
		ValueTree presetInNode;

		while (filesToLoad.size() > 0)
		{
			xml = parseXML(filesToLoad.removeAndReturn(0));
			presetInNode = ValueTree::fromXml(*(xml.get()));

			if (presetInNode.hasType(IDs::modePresetNode))
			{
				addAndSortMode(presetInNode);

				if ((bool)presetInNode[IDs::factoryPreset])
				{
					loadedFactoryPresets.add(presetInNode);
				}
				else
				{
					loadedUserPresets.add(presetInNode);
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

void SvkPresetManager::buildPresetMenu()
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

    modeMenu->addSubMenu("by Scale Size", *scaleSizeSubMenu, true);
    modeMenu->addSubMenu("by Mode Size", *modeSizeSubMenu, true);
    modeMenu->addSubMenu("by Family", *familySubMenu, true);
    modeMenu->addSubMenu("User", *userSubMenu, true);
}
