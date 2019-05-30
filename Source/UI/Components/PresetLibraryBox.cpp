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
}

PresetLibraryBox::~PresetLibraryBox()
{
	presetManager = nullptr;
}

void PresetLibraryBox::populateMenu()
{
	getRootMenu()->clear();
	menus.reset(new OwnedArray<PopupMenu>());

	PopupMenu* scaleSubMenu = menus->add(new PopupMenu());
	PopupMenu* modeSubMenu = menus->add(new PopupMenu());
	PopupMenu* familySubMenu = menus->add(new PopupMenu());
	PopupMenu* userSubMenu = menus->add(new PopupMenu());
	
	ValueTree presetIn;
	String displayName;
	var scaleSize, modeSize, family, separatorProperty;
	int subMenuIndex = 0;

	for (int subMenu = 0; subMenu < SortType::user; subMenu++)
	{
		for (int presetNum = 0; presetNum < presetsLibraryNode.getNumChildren(); presetNum++)
		{
			presetIn = presetsSorted->getUnchecked(subMenu).getUnchecked(presetNum);

			switch (subMenu)
			{
			case (SortType::scaleSize):
				if (separatorProperty != presetIn[IDs::scaleSize])
					addSeparator();
				separatorProperty = presetIn[IDs::scaleSize];
				break;

			case (SortType::modeSize):
				if (separatorProperty != presetIn[IDs::modeSize])
					addSeparator();
				separatorProperty = presetIn[IDs::modeSize];
				break;

			case (SortType::familyName):
				if (separatorProperty != presetIn[IDs::family])
					addSeparator();
				separatorProperty = presetIn[IDs::family];
				break;

			case (SortType::user):
				if (presetIn[IDs::factoryPreset])
					continue;
				if (separatorProperty != presetIn[IDs::scaleSize])
					addSeparator();
				separatorProperty = presetIn[IDs::scaleSize];
				break;
			}

			displayName = presetIn[IDs::modeName];
			scaleSubMenu->addItem(++subMenuIndex, displayName);
		}
	}

	getRootMenu()->addSubMenu("by Scale Size", *scaleSubMenu);
	getRootMenu()->addSubMenu("by Mode Size", *modeSubMenu);
	getRootMenu()->addSubMenu("by Family", *familySubMenu);
	getRootMenu()->addSubMenu("User", *userSubMenu);

}

void PresetLibraryBox::showPopup()
{
	populateMenu();
	ComboBox::showPopup();
}