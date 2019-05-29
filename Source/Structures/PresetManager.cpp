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
	createFactoryPresets();

	loadPreset(8);
}

SvkPresetManager::~SvkPresetManager()
{
	presetLoaded.release();
}

SvkPreset* SvkPresetManager::getPresetLoaded()
{
	return presetLoaded.get();
}

Mode* SvkPresetManager::getModeLoaded()
{
	return modeLoaded.get();
}


bool SvkPresetManager::loadPreset(ValueTree presetNodeIn)
{
	std::unique_ptr<SvkPreset> newPreset(new SvkPreset(presetNodeIn));

	if (newPreset.get())
	{
		presetLoaded.swap(newPreset);

		/*
		if (presetCurrent->theMidiSettingsNode.isValid())
		{
			midiProcessor->restoreFromNode(presetCurrent->theMidiSettingsNode);
		}
		*/
		// this needs to be updated to address the root note
		if (presetLoaded->theModeNode.isValid())
		{
			modeLoaded.reset(new Mode(presetLoaded->theModeNode));
			modeNode = modeLoaded->modeNode;
		}
		/*
		if (presetLoaded->theKeyboardNode.isValid())
		{
			pianoNode.copyPropertiesAndChildrenFrom(presetCurrent->theKeyboardNode, nullptr);
		}
		*/
		return true;
	}
}

bool SvkPresetManager::loadPreset(int libraryIndexIn)
{
	return loadPreset(presetLibraryNode.getChild(libraryIndexIn));
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
	presetLoaded->writeToFile(absolutePath);
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

void SvkPresetManager::createFactoryPresets()
{
	const char* factoryPresets;
	int size;
	factoryPresets = BinaryData::getNamedResource("Factory Presets.txt", size);

	if (factoryPresets && size > 0)
	{	// (const void*)
		MemoryInputStream instream(factoryPresets, size, false);

		String line, steps, family, name;

		while (instream.getNumBytesRemaining() > 0)
		{
			line = instream.readNextLine();
			steps = line.upToFirstOccurrenceOf(", ", false, true);
			family = line.fromFirstOccurrenceOf(", ", false, true);
			presetLibraryNode.addChild(Mode::createNode(steps, family, true), -1, nullptr);
		}
	}
	
	if ((bool)pluginSettingsNode[IDs::createPresetFolder])
	{
		File presetDirectory = File(pluginSettingsNode[IDs::presetDirectory]);
		Array<File> filesToLoad = presetDirectory.findChildFiles(File::TypesOfFileToFind::findFiles, true, "*.svk");

		std::unique_ptr<XmlElement> xml;
		ValueTree loadedPresetNode;

		while (filesToLoad.size > 0)
		{
			xml = parseXML(filesToLoad.removeAndReturn(0));
			loadedPresetNode = ValueTree::fromXml(*(xml.get()));

			if (loadedPresetNode.hasType(IDs::modePresetNode))
			{
				presetLibraryNode.addChild(loadedPresetNode, -1, nullptr);
			}
		}
	}
}

void SvkPresetManager::sortPresetLibrary()
{

}

void SvkPresetManager::addPresetToLibrary(ValueTree presetNodeIn)
{

}