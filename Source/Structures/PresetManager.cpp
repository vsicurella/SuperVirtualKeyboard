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
    intializePresets();
}

SvkPresetManager::~SvkPresetManager()
{
	presetLoaded.release();
	removeAllChangeListeners();
}

SvkPreset* SvkPresetManager::getPresetLoaded()
{
	return presetLoaded.get();
}

Array<Array<ValueTree>>* SvkPresetManager::getPresetsSorted()
{
	return &presetsSorted;
}


bool SvkPresetManager::loadPreset(ValueTree presetNodeIn)
{
	std::unique_ptr<SvkPreset> newPreset(new SvkPreset(presetNodeIn));
    bool shouldLoad = true;
    
	if (newPreset.get())
	{
		/*
		if (presetCurrent->theMidiSettingsNode.isValid())
		{
			midiProcessor->restoreFromNode(presetCurrent->theMidiSettingsNode);
		}
		*/
		// this needs to be updated to address the root note
        
		bool dummy;
        shouldLoad *= Mode::isValidMode(newPreset->theModeNode, dummy);
        
		/*
		if (presetLoaded->theKeyboardNode.isValid())
		{
			pianoNode.copyPropertiesAndChildrenFrom(presetCurrent->theKeyboardNode, nullptr);
		}
		*/
        
       if (shouldLoad)
       {
        presetLoaded.swap(newPreset);
		presetNode.copyPropertiesAndChildrenFrom(presetLoaded->parentNode, nullptr);
		sendChangeMessage();
		return true;
       }
	}
    return false;
}

bool SvkPresetManager::loadPreset(int libraryIndexIn)
{
    int numPresets = presetLibraryNode.getNumChildren();
    int subMenu = libraryIndexIn / numPresets;
    int index = libraryIndexIn % numPresets;
    ValueTree presetToLoad = presetsSorted.getUnchecked(subMenu).getUnchecked(index);
	return loadPreset(presetToLoad);
}

bool SvkPresetManager::loadPreset(SvkPreset* presetIn)
{
	return loadPreset(presetIn->parentNode);
}

bool SvkPresetManager::loadPreset()
{
	return loadPreset(presetFromFile(pluginSettingsNode[IDs::presetDirectory]));
}

bool SvkPresetManager::savePreset(String absolutePath)
{
	return presetLoaded->writeToFile(absolutePath);
}


SvkPreset* SvkPresetManager::presetFromFile(String absoluteFilePath)
{
	ValueTree nodeIn;
	SvkPreset* presetOut = nullptr;

	File fileIn = File(absoluteFilePath);
	
	if (!fileIn.exists())
	{
		std::unique_ptr<FileChooser> chooser;

		if (fileIn.isDirectory())
			chooser.reset(new FileChooser("Open preset", fileIn, "*.svk"));
		else
			chooser.reset(new FileChooser("Open preset", File::getSpecialLocation(File::userDocumentsDirectory), "*.svk"));
		
		chooser->browseForFileToOpen();
		fileIn = chooser->getResult();
	}

	if (fileIn.exists())
	{
		std::unique_ptr<XmlElement> xml = parseXML(fileIn);
		nodeIn = ValueTree::fromXml(*(xml.get()));

		if (nodeIn.hasType(IDs::presetNode))
		{
			presetOut = new SvkPreset(nodeIn);
		}
	}

	return presetOut;
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
		{	// (const void*)
			MemoryInputStream instream(factoryPresets, size, false);

			String line, steps, family, name;
			ValueTree factoryMode;

			while (instream.getNumBytesRemaining() > 0)
			{
				line = instream.readNextLine();
				steps = line.upToFirstOccurrenceOf(", ", false, true);
				family = line.fromFirstOccurrenceOf(", ", false, true);

                factoryMode = Mode::createNode(steps, family, true);
				
				addAndSortPreset(factoryMode);
                loadedFactoryPresets.add(factoryMode);
			}
		}
	}
}

void SvkPresetManager::loadPresetDirectory()
{
	// Add checking for duplicate modes
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
				if ((bool)presetInNode[IDs::factoryPreset])
				{
					addAndSortPreset(presetInNode);
					loadedFactoryPresets.add(presetInNode);
				}
				else
				{
					addAndSortPreset(presetInNode);
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

void SvkPresetManager::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
	int subMenuIndex = comboBoxThatHasChanged->getSelectedId();

	ValueTree presetToLoad = presetLibraryNode.getChild(subMenuIndex % presetLibraryNode.getNumChildren());

	loadPreset(presetToLoad);
}
