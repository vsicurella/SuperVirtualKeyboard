/*
  ==============================================================================

    PluginState.cpp
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PluginState.h"


UndoManager* SuperVirtualKeyboardPluginState::getUndoManager()
{
	return undoManager.get();
}

OwnedArray<Mode>* SuperVirtualKeyboardPluginState::get_presets()
{
	return &presets;
}

Array<Array<Mode*>>* SuperVirtualKeyboardPluginState::get_presets_sorted()
{
	return &presetsSorted;
}

SvkPreset* SuperVirtualKeyboardPluginState::getCurrentPreset()
{
	return presetCurrent.get();
}

int SuperVirtualKeyboardPluginState::get_current_preset_index()
{
	Mode* mode;

	for (int i = 0; i < presets.size(); i++)
	{
		mode = presets.getUnchecked(i);

		if (mode == modeCurrent)
			return i;
	}

	return -1;
}

Mode* SuperVirtualKeyboardPluginState::getCurrentMode()
{
	return modeCurrent;
}

int SuperVirtualKeyboardPluginState::is_mode_in_presets(String stepsStringIn)
{
	int index = 0;

	for (auto mode : presets)
	{
		if (stepsStringIn == mode->getStepsString())
			return index;
		index++;
	}

	return 0;
}

void SuperVirtualKeyboardPluginState::setCurrentMode(int presetIndexIn)
{
	Mode* mode = presets.getUnchecked(presetIndexIn);
	
	if (mode)
	{
		modeCurrent = mode;

		if (presetCurrent->theKeyboardNode.isValid())
			presetCurrent->theKeyboardNode.setProperty(IDs::pianoHasCustomColor, false, nullptr);

		presetCurrent->updateModeNode(modeCurrent->modeNode);
		presetCurrent->parentNode.setProperty(IDs::libraryIndexOfMode, presetIndexIn, undoManager.get());

		pluginStateNode.removeChild(1, nullptr);
		modePresetNode.copyPropertiesAndChildrenFrom(modeCurrent->modeNode, nullptr);
		pluginStateNode.addChild(modePresetNode, 1, nullptr);
	}
}

void SuperVirtualKeyboardPluginState::setCurrentMode(Mode* modeIn)
{
	if (modeIn)
	{
		modeCurrent = modeIn;
		presets.set(0, modeCurrent, true);

		if (presetCurrent->theKeyboardNode.isValid())
			presetCurrent->theKeyboardNode.setProperty(IDs::pianoHasCustomColor, false, nullptr);

		presetCurrent->updateModeNode(modeCurrent->modeNode);
		presetCurrent->parentNode.setProperty(IDs::libraryIndexOfMode, 0, undoManager.get());

		pluginStateNode.removeChild(1, nullptr);
		modePresetNode = modeCurrent->modeNode;
		pluginStateNode.addChild(modePresetNode, 1, nullptr);
	}
}

void SuperVirtualKeyboardPluginState::updateModeOffset(int offsetIn)
{
	modeCurrent->setOffset(offsetIn);
	presetCurrent->updateModeNode(modeCurrent->modeNode);
}


void SuperVirtualKeyboardPluginState::updateKeyboardSettingsPreset()
{
	if (pianoNode.isValid())
	{
		presetCurrent->updateKeyboardNode(pianoNode);
	}
}

bool SuperVirtualKeyboardPluginState::savePreset()
{
	presetCurrent->theModeNode.copyPropertiesAndChildrenFrom(modePresetNode, nullptr);
	return presetCurrent->writeToFile();
}


bool SuperVirtualKeyboardPluginState::loadPreset()
{
	SvkPreset newPreset;

	if (newPreset.parentNode.isValid())
	{
		presetCurrent.reset(new SvkPreset(SvkPreset::loadFromFile()));
		modeCurrent->restore_from_node(presetCurrent->theModeNode);
		modePresetNode = modeCurrent->modeNode;
		return true;
	}

	return false;
}


//==============================================================================

void SuperVirtualKeyboardPluginState::createPresets()
{
	presets.clear();
	presets.add(new Mode(Array<int>({ 1 }), "Custom"));
	presets.add(new Mode(Array<int>({ 1, 2, 1, 2, 1 }), "Mavila"));
	presets.add(new Mode(Array<int>({ 2, 1, 2, 2, 1 }), "Father"));
	presets.add(new Mode(Array<int>({ 1, 1, 2, 1, 1, 1, 2 }), "Mavila"));
	presets.add(new Mode(Array<int>({ 1, 2, 1, 2, 1, 2, 1 }), "Dicot"));
	presets.add(new Mode(Array<int>({ 2, 2, 2, 2, 2, 1 }), "Machine"));
	presets.add(new Mode(Array<int>({ 2, 1, 2, 1, 2, 1, 2 }), "Orgone"));
	presets.add(new Mode(Array<int>({ 3, 1, 3, 1, 3, 1 }), "Augmented"));
	presets.add(new Mode(Array<int>({ 2, 2, 1, 2, 2, 2, 1 }), "Meantone"));
	presets.add(new Mode(Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1 }), "Diminished"));
	presets.add(new Mode(Array<int>({ 2, 2, 1, 2, 2, 1, 2, 1 }), "Father"));
	presets.add(new Mode(Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1, 1 }), "Orwell"));
	presets.add(new Mode(Array<int>({ 2, 2, 2, 2, 1, 2, 2, 1 }), "Hedgehog"));
	presets.add(new Mode(Array<int>({ 2, 2, 1, 2, 1, 2, 1, 2, 1 }), "Titanium"));
	presets.add(new Mode(Array<int>({ 3, 3, 3, 3, 3 }), "Blackwood"));
	presets.add(new Mode(Array<int>({ 3, 1, 3, 1, 3, 1, 3 }), "Orgone"));
	presets.add(new Mode(Array<int>({ 2, 2, 2, 2, 2, 2, 2, 1 }), "Porcupine"));
	presets.add(new Mode(Array<int>({ 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1 }), "Orgone"));
	presets.add(new Mode(Array<int>({ 3, 3, 1, 3, 3, 3 }), "Gorgo"));
	presets.add(new Mode(Array<int>({ 2, 2, 2, 1, 2, 2, 2, 2, 1 }), "Mavila"));
	presets.add(new Mode(Array<int>({ 2, 1, 2, 1, 2, 2, 1, 2, 1, 2 }), "Lemba"));
	presets.add(new Mode(Array<int>({ 3, 3, 1, 3, 3, 3, 1 }), "Meantone"));
	presets.add(new Mode(Array<int>({ 3, 2, 3, 2, 3, 2, 2 }), "Maqamic"));
	presets.add(new Mode(Array<int>({ 2, 2, 1, 2, 2, 1, 2, 2, 2, 1 }), "Maqamic"));
	presets.add(new Mode(Array<int>({ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2 }), "Machine"));
	presets.add(new Mode(Array<int>({ 4, 4, 1, 4, 4, 1 }), "Bicycle"));
	presets.add(new Mode(Array<int>({ 3, 1, 3, 3, 1, 3, 3, 1 }), "Father"));
	presets.add(new Mode(Array<int>({ 3, 3, 2, 3, 3, 3, 2 }), "Meantone"));
	presets.add(new Mode(Array<int>({ 2, 3, 2, 2, 3, 2, 3, 2 }), "Keemun"));
	presets.add(new Mode(Array<int>({ 2, 2, 2, 2, 3, 2, 2, 2, 2 }), "Negri"));
	presets.add(new Mode(Array<int>({ 5, 2, 2, 5, 2, 2, 2 }), "Mavila"));
	presets.add(new Mode(Array<int>({ 3, 1, 3, 1, 3, 1, 3, 1, 3, 1 }), "Blackwood"));
	presets.add(new Mode(Array<int>({ 2, 3, 2, 2, 3, 2, 2, 3, 2 }), "Miracle"));
	presets.add(new Mode(Array<int>({ 4, 4, 1, 4, 4, 4, 1 }), "Superpyth"));
	presets.add(new Mode(Array<int>({ 3, 3, 3, 3, 2, 3, 3, 2 }), "Hedgehog"));
	presets.add(new Mode(Array<int>({ 3, 2, 3, 2, 3, 2, 3, 2, 2 }), "Orwell"));
	presets.add(new Mode(Array<int>({ 3, 3, 3, 1, 3, 3, 3, 3, 1 }), "Mavila"));
	presets.add(new Mode(Array<int>({ 2, 2, 2, 3, 2, 2, 3, 2, 2, 3 }), "Sephiroth"));
	presets.add(new Mode(Array<int>({ 5, 5, 4, 5, 5 }), "Godzilla"));
	presets.add(new Mode(Array<int>({ 5, 3, 5, 3, 5, 3 }), "Triforce"));
	presets.add(new Mode(Array<int>({ 4, 3, 3, 4, 3, 3, 4 }), "Dastgah-e Sehgah / Maqam Nairuz"));
	presets.add(new Mode(Array<int>({ 4, 4, 2, 4, 4, 4, 2 }), "Meantone"));
	presets.add(new Mode(Array<int>({ 4, 3, 4, 3, 4, 3, 3 }), "Mohajira"));
	presets.add(new Mode(Array<int>({ 4, 4, 3, 4, 4, 4, 3 }), "Meantone"));
	presets.add(new Mode(Array<int>({ 2, 3, 2, 2, 3, 2, 3, 2, 2, 3, 2 }), "Orgone"));
	presets.add(new Mode(Array<int>({ 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 3 }), "Injera"));
	presets.add(new Mode(Array<int>({ 5, 5, 3, 5, 5, 5, 3 }), "Meantone"));
	presets.add(new Mode(Array<int>({ 4, 3, 4, 3, 4, 3, 4, 3, 3, }), "Orwell"));
	presets.add(new Mode(Array<int>({ 2, 1, 1, 2, 1, 2, 1, 2, 1 }), "BP Lambda"));
	presets.add(new Mode(Array<int>({ 2, 1, 1, 2, 1, 2, 1, 1, 2 }), "BP Dur II"));

	// Connect to ValueTree structure
	for (int i = 0; i < presets.size(); i++)
	{
		modeLibraryNode.addChild(presets.getUnchecked(i)->modeNode, i, nullptr);
		presets.getUnchecked(i)->modeNode.setProperty(IDs::factoryPreset, true, nullptr);
	}

	presetsSorted.clear();
	presetsSorted.resize(3);

	ModeScaleSorter scaleSort;
	ModeModeSorter modeSort;
	ModeFamilySorter famSort;

	Array<Mode*> sorting;
	for (int i = 1; i < presets.size(); i++)
	{
		sorting.add(presets.getUnchecked(i));
	}

	sorting.sort(scaleSort, false);
	presetsSorted.getReference(SortType::scaleSizeSort).addArray(sorting);

	sorting.sort(modeSort, false);
	presetsSorted.getReference(SortType::modeSizeSort).addArray(sorting);

	sorting.sort(famSort, false);
	presetsSorted.getReference(SortType::familySort).addArray(sorting);
}