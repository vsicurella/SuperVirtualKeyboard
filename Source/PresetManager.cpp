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
	presetLoaded = SvkPreset();

    intializePresets();
}

SvkPresetManager::~SvkPresetManager()
{
	removeAllChangeListeners();
}

SvkPreset* SvkPresetManager::getPresetLoaded()
{
	return &presetLoaded;
}

int SvkPresetManager::getPresetLoadedId()
{
    return presetLoaded.parentNode[IDs::libraryIndexOfMode];
}


Array<Array<ValueTree>>* SvkPresetManager::getPresetsSorted()
{
	return &presetsSorted;
}

ValueTree SvkPresetManager::getPreset(int indexIn)
{
    if (indexIn < 0)
        indexIn = 0;
    
	int subMenu = indexIn / numberOfPresets;
	int index = indexIn % numberOfPresets;
    
    if (subMenu < presetsSorted.size())
        if (index < presetsSorted.getUnchecked(subMenu).size())
            return presetsSorted.getUnchecked(subMenu).getUnchecked(index);
    
    return ValueTree();
}

ValueTree SvkPresetManager::getMode(int indexIn)
{
	bool hasModeChild;
	ValueTree preset = getPreset(indexIn);

	if (Mode::isValidMode(preset, hasModeChild))
		if (hasModeChild)
			return preset.getChildWithName(IDs::modePresetNode);
	
	return preset;
}

// Put inside a shared pointer
PopupMenu* SvkPresetManager::getPresetMenu()
{
    return presetMenu.get();
}

bool SvkPresetManager::loadPreset(ValueTree presetNodeIn, bool sendChangeSignal)
{
	presetLoaded = SvkPreset(presetNodeIn.createCopy());

    presetNode = presetLoaded.parentNode;
			
	if (sendChangeSignal)
		sendChangeMessage();

	return presetLoaded.theModeNode.isValid();
}

bool SvkPresetManager::loadPreset(int indexIn, bool sendChangeSignal)
{
	return loadPreset(getPreset(indexIn), sendChangeSignal);
}

bool SvkPresetManager::loadPreset(SvkPreset* presetIn, bool sendChangeSignal)
{
	return loadPreset(presetIn->parentNode, sendChangeSignal);
}

bool SvkPresetManager::loadPreset(bool sendChangeSignal)
{
	return loadPreset(presetFromFile(pluginSettingsNode[IDs::presetDirectory]), sendChangeSignal);
}

bool SvkPresetManager::savePreset(String absolutePath)
{
	return presetLoaded.writeToFile(absolutePath);
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

bool SvkPresetManager::commitPresetNode(ValueTree nodeIn)
{
    if (nodeIn.isValid())
    {
        ValueTree nodeChild;
        
        // MODE SETTINGS
        nodeChild = nodeIn.getChildWithName(IDs::modePresetNode);
        commitModeNode(nodeChild);
        
        // KEYBOARD SETTINGS
        nodeChild = nodeIn.getChildWithName(IDs::pianoNode);
        commitKeyboardNode(nodeChild);
        
        // MIDI MAPS
        nodeChild = nodeIn.getChildWithName(IDs::midiMapNode);
        commitMapNode(nodeChild);
        
        return true;
    }
    
    return false;
}

bool SvkPresetManager::commitModeNode(ValueTree modeNodeIn)
{
    if (modeNodeIn.hasType(IDs::modePresetNode))
    {
        presetLoaded.updateModeNode(modeNodeIn);
        return true;
    }
    
    return false;
}

bool SvkPresetManager::commitKeyboardNode(ValueTree keyboardNodeIn)
{
    if (keyboardNodeIn.hasType(IDs::pianoNode))
    {
        presetLoaded.updateKeyboardNode(keyboardNodeIn);
        return true;
    }
    
    return false;
}

bool SvkPresetManager::commitMapNode(ValueTree mapNodeIn)
{
    if (mapNodeIn.hasType(IDs::midiMapNode))
    {
        presetLoaded.updateMapNode(mapNodeIn);
        return true;
    }
    
    return false;
}

void SvkPresetManager::intializePresets()
{
	presetLibraryNode = ValueTree(IDs::presetLibraryNode);
	presetNode = ValueTree(IDs::presetNode);
	loadedFactoryPresets.clear();
	loadedUserPresets.clear();
	presetsSorted.clear();
	for (int m = 0; m < numSortTypes; m++)
		presetsSorted.add(Array<ValueTree>());

	createFactoryPresets();
	loadPresetDirectory();
    buildPresetMenu();
}


void SvkPresetManager::createFactoryPresets()
{
	// I would like to add some more error checking to this and making it so that
	//  it creates missing factory presets if it notices them gone from the directory
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
				
				addAndSortPreset(factoryMode);
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
				addAndSortPreset(presetInNode);

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


void SvkPresetManager::resortPresetLibrary()
{
	presetsSorted.clear();
	for (int m = 0; m < numSortTypes; m++)
		presetsSorted.add(Array<ValueTree>());

	ValueTree presetToSort;

	for (int i = 0; i < presetLibraryNode.getNumChildren(); i++)
	{
		presetToSort = presetLibraryNode.getChild(i);
		addPresetToSort(presetToSort);
	}
}

int SvkPresetManager::addPresetToLibrary(ValueTree presetNodeIn)
{
	if (presetNodeIn.hasType(IDs::presetNode) || presetNodeIn.hasType(IDs::modePresetNode))
	{
		int libraryIndex = presetLibraryNode.getNumChildren();
		presetNodeIn.setProperty(IDs::libraryIndexOfMode, libraryIndex, nullptr);
		presetLibraryNode.appendChild(ValueTree(presetNodeIn), nullptr);
		numberOfPresets = presetLibraryNode.getNumChildren();

		return libraryIndex;
	}

	return -1;
}

void SvkPresetManager::addPresetToSort(ValueTree presetNodeIn)
{
	if (presetNodeIn.hasType(IDs::presetNode) || presetNodeIn.hasType(IDs::modePresetNode))
	{
		presetsSorted.getReference(SortType::scaleSize).addSorted(scaleSizeSort, ValueTree(presetNodeIn));
		presetsSorted.getReference(SortType::modeSize).addSorted(modeSizeSort, ValueTree(presetNodeIn));
		presetsSorted.getReference(SortType::familyName).addSorted(familyNameSort, ValueTree(presetNodeIn));

		if (!(bool)presetNodeIn[IDs::factoryPreset])
		{
			presetsSorted.getReference(SortType::user).addSorted(scaleSizeSort, ValueTree(presetNodeIn));
		}
	}
}

int SvkPresetManager::addAndSortPreset(ValueTree presetNodeIn)
{
	int ind = addPresetToLibrary(presetNodeIn);
	addPresetToSort(presetNodeIn);

	return ind;
}

void SvkPresetManager::buildPresetMenu()
{
    presetMenu.reset(new PopupMenu());
    presetSubMenus.clear();
    
    PopupMenu* scaleSubMenu = presetSubMenus.add(new PopupMenu());
    PopupMenu* modeSubMenu = presetSubMenus.add(new PopupMenu());
    PopupMenu* familySubMenu = presetSubMenus.add(new PopupMenu());
    PopupMenu* userSubMenu = presetSubMenus.add(new PopupMenu());
    
    ValueTree presetIn;
    String displayName;
    var separatorProperty;
    
    int subMenuIndex = 0;
    
    for (int subMenu = 0; subMenu < SortType::user; subMenu++)
    {
        for (int presetNum = 0; presetNum < presetLibraryNode.getNumChildren(); presetNum++)
        {
            presetIn = presetsSorted.getUnchecked(subMenu).getUnchecked(presetNum);
            
            switch (subMenu)
            {
                case (SortType::scaleSize):
                    if (separatorProperty != presetIn[IDs::scaleSize])
                        scaleSubMenu->addSeparator();
                    separatorProperty = presetIn[IDs::scaleSize];
                    displayName = presetIn[IDs::modeName];
                    scaleSubMenu->addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::modeSize):
                    if (separatorProperty != presetIn[IDs::modeSize])
                        modeSubMenu->addSeparator();
                    separatorProperty = presetIn[IDs::modeSize];
                    displayName = presetIn[IDs::modeName];
                    modeSubMenu->addItem(++subMenuIndex, displayName);
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

    presetMenu->addSubMenu("by Scale Size", *scaleSubMenu, true);
    presetMenu->addSubMenu("by Mode Size", *modeSubMenu, true);
    presetMenu->addSubMenu("by Family", *familySubMenu, true);
    presetMenu->addSubMenu("User", *userSubMenu, true);
}


void SvkPresetManager::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
	int subMenuIndex = comboBoxThatHasChanged->getSelectedId();

	ValueTree presetToLoad = presetLibraryNode.getChild(subMenuIndex % presetLibraryNode.getNumChildren());

	loadPreset(presetToLoad);
}
