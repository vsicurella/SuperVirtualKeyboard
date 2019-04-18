/*
  ==============================================================================

    PluginState.cpp
    Created: 7 Apr 2019 12:27:01pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PluginState.h"

struct ModeScaleSorter
{
	// Scale first, then mode, then family

	static int compareElements(const ModeLayout* t1, const ModeLayout* t2)
	{
		int sz1 = t1->scaleSize;
		int sz2 = t2->scaleSize;

		int m1 = t1->modeSize;
		int m2 = t2->modeSize;

		String f1 = t1->family;
		String f2 = t2->family;

		if (sz1 < sz2) return -1;
		else if (sz1 > sz2) return 1;
		else
		{
			if (m1 < m2) return -1;
			else if (m1 > m2) return 1;
			else
			{
				if (f1 < f2) return  -1;
				else if (f1 > f2) return 1;
				else return 0;
			}
		}
	}
};

struct ModeModeSorter
{
	// Mode first, then scale, then family

	static int compareElements(const ModeLayout* t1, const ModeLayout* t2)
	{
		int sz1 = t1->scaleSize;
		int sz2 = t2->scaleSize;

		int m1 = t1->modeSize;
		int m2 = t2->modeSize;

		String f1 = t1->family;
		String f2 = t2->family;

		if (m1 < m2) return -1;
		else if (m1 > m2) return 1;
		else
		{
			if (sz1 < sz2) return -1;
			else if (sz1 > sz2) return 1;
			else
			{
				if (f1 < f2) return  -1;
				else if (f1 > f2) return 1;
				else return 0;
			}
		}
	}
};
struct ModeFamilySorter
{
	// Family first, then scale, then mode

	static int compareElements(const ModeLayout* t1, const ModeLayout* t2)
	{
		int sz1 = t1->scaleSize;
		int sz2 = t2->scaleSize;

		int m1 = t1->modeSize;
		int m2 = t2->modeSize;

		String f1 = t1->family;
		String f2 = t2->family;

		if (f1 < f2) return -1;
		else if (f1 > f2) return 1;
		else
		{
			if (sz1 < sz2) return -1;
			else if (sz1 > sz2) return 1;
			else
			{
				if (m1 < m2) return  -1;
				else if (m1 > m2) return 1;
				else return 0;
			}
		}
	}
};

UndoManager* SuperVirtualKeyboardPluginState::get_undo_mgr()
{
	return undoManager.get();
}

OwnedArray<ModeLayout>* SuperVirtualKeyboardPluginState::get_presets()
{
	return &presets;
}

Array<Array<ModeLayout*>>* SuperVirtualKeyboardPluginState::get_presets_sorted()
{
	return &presetsSorted;
}

ValueTree SuperVirtualKeyboardPluginState::get_current_preset()
{
	return presetCurrentNode;
}

int SuperVirtualKeyboardPluginState::get_current_preset_index()
{
	ModeLayout* mode;

	for (int i = 0; i < presets.size(); i++)
	{
		mode = presets.getUnchecked(i);

		if (mode == modeCurrent)
			return i;
	}

	return -1;
}

ModeLayout* SuperVirtualKeyboardPluginState::get_current_mode()
{
	return modeCurrent;
}

int SuperVirtualKeyboardPluginState::is_mode_in_presets(ModeLayout* modeIn)
{
	int index = 0;

	for (auto mode : presets)
	{
		if (modeIn->strSteps == mode->strSteps)
			return index;
		index++;
	}

	return 0;
}

void SuperVirtualKeyboardPluginState::set_current_mode(int presetIndexIn)
{
	ModeLayout* mode = presets.getUnchecked(presetIndexIn);
	
	if (mode)
	{
		modeCurrent = mode;
		presetCurrentNode.removeChild(0, undoManager.get());
		presetCurrentNode.setProperty(IDs::indexOfMode, presetIndexIn, undoManager.get());
		presetCurrentNode.addChild(modeCurrent->modeLayoutNode.createCopy(), 0, undoManager.get());
	}
}

void SuperVirtualKeyboardPluginState::set_current_mode(ModeLayout* modeIn)
{
	if (modeIn)
	{
		modeCurrent = modeIn;
		presets.set(0, modeCurrent, true);
		presetCurrentNode.removeChild(0, undoManager.get());
		presetCurrentNode.setProperty(IDs::indexOfMode, 0, undoManager.get());
		presetCurrentNode.addChild(modeCurrent->modeLayoutNode.createCopy(), 0, undoManager.get());
	}
}

void SuperVirtualKeyboardPluginState::set_current_key_settings(ValueTree pianoNodeIn)
{
	if (pianoNodeIn.isValid())
	{
		presetCurrentNode.removeChild(1, undoManager.get());
		presetCurrentNode.addChild(pianoNodeIn.createCopy(), 1, undoManager.get());
	}
}

//==============================================================================

void SuperVirtualKeyboardPluginState::createPresets()
{
	presets.clear();
	presets.add(new ModeLayout(Array<int>({ 1 }), "Custom"));
	presets.add(new ModeLayout(Array<int>({ 1, 2, 1, 2, 1 }), "Mavila"));
	presets.add(new ModeLayout(Array<int>({ 2, 1, 2, 2, 1 }), "Father"));
	presets.add(new ModeLayout(Array<int>({ 1, 1, 2, 1, 1, 1, 2 }), "Mavila"));
	presets.add(new ModeLayout(Array<int>({ 1, 2, 1, 2, 1, 2, 1 }), "Dicot"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 2, 2, 2, 1 }), "Machine"));
	presets.add(new ModeLayout(Array<int>({ 2, 1, 2, 1, 2, 1, 2 }), "Orgone"));
	presets.add(new ModeLayout(Array<int>({ 3, 1, 3, 1, 3, 1 }), "Augmented"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 1, 2, 2, 2, 1 }), "Meantone"));
	presets.add(new ModeLayout(Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1 }), "Diminished"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 1, 2, 2, 1, 2, 1 }), "Father"));
	presets.add(new ModeLayout(Array<int>({ 2, 1, 2, 1, 2, 1, 2, 1, 1 }), "Orwell"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 2, 2, 1, 2, 2, 1 }), "Hedgehog"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 1, 2, 1, 2, 1, 2, 1 }), "Titanium"));
	presets.add(new ModeLayout(Array<int>({ 3, 3, 3, 3, 3 }), "Blackwood"));
	presets.add(new ModeLayout(Array<int>({ 3, 1, 3, 1, 3, 1, 3 }), "Orgone"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 2, 2, 2, 2, 2, 1 }), "Porcupine"));
	presets.add(new ModeLayout(Array<int>({ 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1 }), "Orgone"));
	presets.add(new ModeLayout(Array<int>({ 3, 3, 1, 3, 3, 3 }), "Gorgo"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 2, 1, 2, 2, 2, 2, 1 }), "Mavila"));
	presets.add(new ModeLayout(Array<int>({ 2, 1, 2, 1, 2, 2, 1, 2, 1, 2 }), "Lemba"));
	presets.add(new ModeLayout(Array<int>({ 3, 3, 1, 3, 3, 3, 1 }), "Meantone"));
	presets.add(new ModeLayout(Array<int>({ 3, 2, 3, 2, 3, 2, 2 }), "Maqamic"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 1, 2, 2, 1, 2, 2, 2, 1 }), "Maqamic"));
	presets.add(new ModeLayout(Array<int>({ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2 }), "Machine"));
	presets.add(new ModeLayout(Array<int>({ 4, 4, 1, 4, 4, 1 }), "Bicycle"));
	presets.add(new ModeLayout(Array<int>({ 3, 1, 3, 3, 1, 3, 3, 1 }), "Father"));
	presets.add(new ModeLayout(Array<int>({ 3, 3, 2, 3, 3, 3, 2 }), "Meantone"));
	presets.add(new ModeLayout(Array<int>({ 2, 3, 2, 2, 3, 2, 3, 2 }), "Keemun"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 2, 2, 3, 2, 2, 2, 2 }), "Negri"));
	presets.add(new ModeLayout(Array<int>({ 5, 2, 2, 5, 2, 2, 2 }), "Mavila"));
	presets.add(new ModeLayout(Array<int>({ 3, 1, 3, 1, 3, 1, 3, 1, 3, 1 }), "Blackwood"));
	presets.add(new ModeLayout(Array<int>({ 2, 3, 2, 2, 3, 2, 2, 3, 2 }), "Miracle"));
	presets.add(new ModeLayout(Array<int>({ 4, 4, 1, 4, 4, 4, 1 }), "Superpyth"));
	presets.add(new ModeLayout(Array<int>({ 3, 3, 3, 3, 2, 3, 3, 2 }), "Hedgehog"));
	presets.add(new ModeLayout(Array<int>({ 3, 2, 3, 2, 3, 2, 3, 2, 2 }), "Orwell"));
	presets.add(new ModeLayout(Array<int>({ 3, 3, 3, 1, 3, 3, 3, 3, 1 }), "Mavila"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 2, 3, 2, 2, 3, 2, 2, 3 }), "Sephiroth"));
	presets.add(new ModeLayout(Array<int>({ 5, 5, 4, 5, 5 }), "Godzilla"));
	presets.add(new ModeLayout(Array<int>({ 5, 3, 5, 3, 5, 3 }), "Triforce"));
	presets.add(new ModeLayout(Array<int>({ 4, 3, 3, 4, 3, 3, 4 }), "Dastgah-e Sehgah / Maqam Nairuz"));
	presets.add(new ModeLayout(Array<int>({ 4, 4, 2, 4, 4, 4, 2 }), "Meantone"));
	presets.add(new ModeLayout(Array<int>({ 4, 3, 4, 3, 4, 3, 3 }), "Mohajira"));
	presets.add(new ModeLayout(Array<int>({ 4, 4, 3, 4, 4, 4, 3 }), "Meantone"));
	presets.add(new ModeLayout(Array<int>({ 2, 3, 2, 2, 3, 2, 3, 2, 2, 3, 2 }), "Orgone"));
	presets.add(new ModeLayout(Array<int>({ 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 3 }), "Injera"));
	presets.add(new ModeLayout(Array<int>({ 5, 5, 3, 5, 5, 5, 3 }), "Meantone"));
	presets.add(new ModeLayout(Array<int>({ 4, 3, 4, 3, 4, 3, 4, 3, 3, }), "Orwell"));
	presets.add(new ModeLayout(Array<int>({ 2, 1, 1, 2, 1, 2, 1, 2, 1 }), "BP Lambda"));
	presets.add(new ModeLayout(Array<int>({ 2, 1, 1, 2, 1, 2, 1, 1, 2 }), "BP Dur II"));

	// Connect to ValueTree structure
	for (int i = 0; i < presets.size(); i++)
	{
		modeLibraryNode.addChild(presets.getUnchecked(i)->modeLayoutNode, i, nullptr);
	}

	presetsSorted.clear();
	presetsSorted.resize(3);

	ModeScaleSorter scaleSort;
	ModeModeSorter modeSort;
	ModeFamilySorter famSort;

	Array<ModeLayout*> sorting;
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