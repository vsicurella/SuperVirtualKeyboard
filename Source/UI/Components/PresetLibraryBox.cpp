/*
  ==============================================================================

    PresetLibraryBox.cpp
    Created: 27 May 2019 12:02:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PresetLibraryBox.h"

PresetLibraryBox::PresetLibraryBox(SvkPresetManager* presetManagerIn, String nameIn)
	: ComboBox(nameIn), presetManager(presetManagerIn)
{
	presetsLibraryNode = presetManager->presetLibraryNode;
	presetsSorted = presetManager->getPresetsSorted();
	sortedIndicies = presetManager->getSortedIndicies();
}

PresetLibraryBox::~PresetLibraryBox()
{
	presetManager = nullptr;
}

void PresetLibraryBox::populateMenu()
{
	getRootMenu()->clear();
	menus.reset(new OwnedArray<PopupMenu>());

	PopupMenu* subMenu;
	ValueTree presetIn;
	String displayName;

	var separatorProperty;
	// can definitely improve this
	for (int menuIndex = 1; menuIndex <= 4; menuIndex++)
	{	
		subMenu = menus->add(new PopupMenu());

		for (int sortIndex = 1; sortIndex <= presetsSorted->getUnchecked(menuIndex - 1).size(); sortIndex++)
		{
			presetIn = presetsSorted->getUnchecked(menuIndex - 1).getUnchecked(sortIndex - 1);
			displayName = presetIn[IDs::modeName];

			switch (menuIndex - 1)
			{
			case(SortType::scaleSize):
				if (separatorProperty != presetIn[IDs::scaleSize])
					addSeparator();
				separatorProperty = presetIn[IDs::scaleSize];
				subMenu->addItem(sortIndex, displayName);
				break;
			case(SortType::modeSize):
				if (separatorProperty != presetIn[IDs::modeSize])
					addSeparator();
				separatorProperty = presetIn[IDs::modeSize];
				subMenu->addItem(sortIndex, displayName);
				break;
			case(SortType::familyName):
				if (separatorProperty != presetIn[IDs::family])
					addSeparator();
				separatorProperty = presetIn[IDs::family];
				subMenu->addItem(sortIndex, displayName);
				break;
			case(SortType::user):
				if (separatorProperty != presetIn[IDs::scaleSize])
					addSeparator();
				separatorProperty = presetIn[IDs::scaleSize];
				subMenu->addItem(sortIndex, displayName);
				break;
			default:
				break;
			}
		}

		switch (menuIndex - 1)
		{
		case(SortType::scaleSize):
			getRootMenu()->addSubMenu("by Scale Size", *subMenu);
				break;
		case(SortType::modeSize):
			getRootMenu()->addSubMenu("by Mode Size", *subMenu);
			break;
		case(SortType::familyName):
			getRootMenu()->addSubMenu("by Family", *subMenu);
			break;
		case(SortType::user):
			getRootMenu()->addSubMenu("User", *subMenu);
			break;
		default:
			break;
		}
	}

}

void PresetLibraryBox::showPopup()
{
	populateMenu();
	ComboBox::showPopup();
}