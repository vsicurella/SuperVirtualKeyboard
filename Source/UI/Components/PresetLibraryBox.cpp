/*
  ==============================================================================

    PresetLibraryBox.cpp
    Created: 27 May 2019 12:02:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PresetLibraryBox.h"

PresetLibraryBox::PresetLibraryBox(SuperVirtualKeyboardPluginState* pluginStateIn, String nameIn)
	: ComboBox(nameIn), pluginState(pluginStateIn)
{
	presets = pluginState->getPresets();
	sortMenus();
}

PresetLibraryBox::~PresetLibraryBox()
{
	pluginState = nullptr;
	presets = nullptr;
}

void PresetLibraryBox::sortMenus()
{
	ModeScaleSorter scaleSort;
	ModeModeSorter modeSort;
	ModeFamilySorter famSort;

	Array<Mode*> sorting;
	for (int i = 1; i < presets->size(); i++)
	{
		sorting.add(presets->getUnchecked(i));
	}

	int id = 0;
	PopupMenu* theMenu;
	Mode* theMode;
	int sepProp;
	String name;

	for (int i = 0; i < menus.size(); i++)
	{
		switch (i)
		{
		case scaleSize:
			sorting.sort(scaleSort, false);
			for (int p = 0; p < presets->size(); p++)
			{
				theMenu = menus.getUnchecked(scaleSize);
				theMode = sorting[p];
				
				if (p > 0 && sepProp != theMode->getScaleSize())
					theMenu->addSeparator();
				
				sepProp = theMode->getScaleSize();
				theMenu->addItem(++id, theMode->getScaleDescription());
			}
			break;
		case modeSize:
			sorting.sort(modeSort, false);
			for (int p = 0; p < presets->size(); p++)
			{
				theMenu = menus.getUnchecked(modeSize);
				theMode = sorting[p];

				if (p > 0 && sepProp != theMode->getModeSize())
					theMenu->addSeparator();

				sepProp = theMode->getModeSize();
				theMenu->addItem(++id, theMode->getModeDescription());
			}
			break;
		case familyName:
			sorting.sort(famSort, false);
			for (int p = 0; p < presets->size(); p++)
			{
				theMenu = menus.getUnchecked(modeSize);
				theMode = sorting[p];

				if (p > 0 && name != theMode->getFamily())
					theMenu->addSeparator();

				name = theMode->getFamily();
				theMenu->addItem(++id, theMode->getDescription());
			}
			break;
		case user:
			sorting.sort(scaleSort, false);
			sorting.sort(modeSort, false);
			for (int p = 0; p < presets->size(); p++)
			{
				theMenu = menus.getUnchecked(scaleSize);
				theMode = sorting[p];

				if (p > 0 && sepProp != theMode->getScaleSize())
					theMenu->addSeparator();

				sepProp = theMode->getScaleSize();
				theMenu->addItem(++id, theMode->getScaleDescription());
			}
			break;
		default:
			break;
		}
	}
}

void PresetLibraryBox::showPopup()
{
	if (!allSorted)
		sortMenus;

	PopupMenu* root = getRootMenu();

}