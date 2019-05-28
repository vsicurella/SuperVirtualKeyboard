/*
  ==============================================================================

    PresetManager.cpp
    Created: 27 May 2019 10:57:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PresetManager.h"


SvkPreset* SvkPresetManager::presetFromFile(String absoluteFilePath)
{
	ValueTree nodeIn;
	File fileIn = File(absoluteFilePath);
	SvkPreset* presetOut = nullptr;

	if (!fileIn.exists())
	{
		FileChooser chooser("Open preset",
			File::getSpecialLocation(File::userDocumentsDirectory),
			"*.svk");

		chooser.browseForFileToOpen();
		fileIn = chooser.getResult();
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
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 1 }), "Custom", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 2, 1, 2, 1 }), "Mavila", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 2, 1 }), "Father", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 1, 2, 1, 1, 1, 2 }), "Mavila", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 2, 1, 2, 1, 2, 1 }), "Dicot", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 2, 2, 1 }), "Machine", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 1, 2, 1, 2 }), "Orgone", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 1, 3, 1, 3, 1 }), "Augmented", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 1, 2, 2, 2, 1 }), "Meantone", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1 }), "Diminished", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 1, 2, 2, 1, 2, 1 }), "Father", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1, 1 }), "Orwell", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 2, 1, 2, 2, 1 }), "Hedgehog", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 1, 2, 1, 2, 1, 2, 1 }), "Titanium", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 3, 3, 3 }), "Blackwood", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 1, 3, 1, 3, 1 }), "Hanson", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 1, 3, 1, 3, 1, 3 }), "Orgone", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 2, 2, 2, 2, 1 }), "Porcupine", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1 }), "Orgone", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 4, 1, 4, 1, 4, 1 }), "Magic", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 1, 3, 3, 3 }), "Gorgo", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 1, 2, 2, 2, 2, 1 }), "Mavila", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 1, 2, 2, 1, 2, 1, 2 }), "Lemba", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 1, 3, 3, 3, 1 }), "Superpyth", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 2, 3, 2, 3, 2, 2 }), "Maqamic", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 1, 2, 2, 1, 2, 2, 2, 1 }), "Maqamic", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2 }), "Machine", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 4, 1, 4, 4, 1 }), "Bicycle", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 1, 3, 3, 1, 3, 3, 1 }), "Father", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 2, 3, 3, 3, 2 }), "Meantone", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 3, 2, 2, 3, 2, 3, 2 }), "Keemun", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 2, 3, 2, 2, 2, 2 }), "Negri", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 1, 4, 1, 4, 1, 4 }), "Hanson", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 2, 2, 5, 2, 2, 2 }), "Mavila", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 1, 3, 1, 3, 1, 3, 1, 3, 1 }), "Blackwood", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 3, 2, 2, 3, 2, 2, 3, 2 }), "Miracle", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 4, 1, 4, 4, 4, 1 }), "Superpyth", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 3, 3, 2, 3, 3, 2 }), "Hedgehog", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 2, 3, 2, 3, 2, 3, 2, 2 }), "Orwell", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 3, 1, 3, 3, 3, 3, 1 }), "Mavila", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 3, 2, 2, 3, 2, 2, 3 }), "Sephiroth", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 5, 4, 5, 5 }), "Godzilla", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 3, 5, 3, 5, 3 }), "Triforce", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 3, 4, 3, 3, 4 }), "Dastgah-e Sehgah / Maqam Nairuz", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 4, 2, 4, 4, 4, 2 }), "Meantone", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 4, 3, 4, 3, 3 }), "Mohajira", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 4, 3, 4, 4, 4, 3 }), "Meantone", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 3, 2, 2, 3, 2, 3, 2, 2, 3, 2 }), "Orgone", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 3 }), "Injera", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 5, 1, 5, 5, 5, 1 }), "Superpyth", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 4, 4, 4, 4, 4, 7 }), "Nusecond", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 5, 3, 5, 5, 5, 3 }), "Meantone", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 2, 3, 2, 3, 3, 2, 3, 2, 3, 2 }), "Meantone", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 4, 3, 4, 3, 4, 3, 3 }), "Orwell", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 1 }), "Mothra", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 5, 4, 5, 4, 5, 4, 4 }), "Mohajira", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1 }), "Miracle", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 7, 7, 3, 7, 7, 7, 3 }), "Schismatic", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 4, 3, 3, 4, 3, 4, 3, 4, 3, 3 }), "Schismatic", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 4, 3, 4, 3, 4, 3, 3 }), "Miracle", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 1 }), "Magic", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 3, 3, 3, 5, 3, 3, 3, 5, 3, 3, 3, 5, 3, 3, 5 }), "Hanson", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 4, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3 }), "Tricot", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 1, 2, 1, 2, 1, 2, 1 }), "BP Lambda", true), -1, nullptr);
	presetLibraryNode.addChild(Mode::createNode(Array<int>({ 2, 1, 1, 2, 1, 2, 1, 1, 2 }), "BP Dur II", true), -1, nullptr);


	if ((bool)pluginSettingsNode[IDs::createPresetFolder])
	{
		File presetDirectory = File(pluginSettingsNode[IDs::presetDirectory].toString());
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
