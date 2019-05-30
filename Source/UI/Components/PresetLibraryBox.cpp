/*
  ==============================================================================

    PresetLibraryBox.cpp
    Created: 27 May 2019 12:02:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PresetLibraryBox.h"

PresetLibraryBox::PresetLibraryBox(SvkPresetManager* presetManagerIn)
	: ComboBox("PresetLibraryBox"), presetManager(presetManagerIn)
{
	presetsLibraryNode = presetManager->presetLibraryNode;
	presetsSorted = presetManager->getPresetsSorted();
    populateMenu();
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
	var separatorProperty;
	int scaleSize;

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

	getRootMenu()->addSubMenu("by Scale Size", *menus->getUnchecked(0), true);
	getRootMenu()->addSubMenu("by Mode Size", *modeSubMenu, true);
	getRootMenu()->addSubMenu("by Family", *familySubMenu, true);
	getRootMenu()->addSubMenu("User", *userSubMenu, true);

}

void PresetLibraryBox::showPopup()
{
	ComboBox::showPopup();
}
