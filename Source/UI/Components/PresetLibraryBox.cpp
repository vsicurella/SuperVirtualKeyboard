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
	var scaleSize, modeSize, family;
	int subMenuIndex;

		for (int presetNum = 0; presetNum < presetsLibraryNode.getNumChildren(); presetNum++)
		{
			presetIn = presetsSorted->getUnchecked(SortType::scaleSize).getUnchecked(presetNum);

			if (scaleSize != presetIn[IDs::scaleSize])
				addSeparator();
			scaleSize = presetIn[IDs::scaleSize];

			subMenuIndex = presetNum + presetNum * SortType::scaleSize;
			scaleSubMenu->addItem(subMenuIndex, displayName);

			presetIn = presetsSorted->getUnchecked(SortType::modeSize).getUnchecked(presetNum);

			if (modeSize != presetIn[IDs::modeSize])
				addSeparator();
			modeSize = presetIn[IDs::modeSize];

			subMenuIndex = presetNum + presetNum * SortType::modeSize;
			modeSubMenu->addItem(subMenuIndex, displayName);

			presetIn = presetsSorted->getUnchecked(SortType::familyName).getUnchecked(presetNum);

			if (family != presetIn[IDs::family])
				addSeparator();
			family = presetIn[IDs::family];

			subMenuIndex = presetNum + presetNum * SortType::familyName;
			familySubMenu->addItem(subMenuIndex, displayName);
		}



	for (int subMenu = 0; subMenu < SortType::user; subMenu++)
	{
		for (int presetNum = 0; presetNum < presetsLibraryNode.getNumChildren(); presetNum++)
		{
			presetIn = presetsSorted->getUnchecked(subMenu).getUnchecked(presetNum);

			switch (subMenu)
			{
			case (SortType::scaleSize):
				if (scaleSize != presetIn[IDs::scaleSize])
					addSeparator();
				scaleSize = presetIn[IDs::scaleSize];
				break;

			case (SortType::modeSize):
				if (modeSize != presetIn[IDs::modeSize])
					addSeparator();
				modeSize = presetIn[IDs::modeSize];
				break;

			case (SortType::familyName):
				if (family != presetIn[IDs::family])
					addSeparator();
				family = presetIn[IDs::family];
				break;
			}

			scaleSubMenu->addItem(++subMenuIndex, displayName);
		}
	}
}

void PresetLibraryBox::showPopup()
{
	populateMenu();
	ComboBox::showPopup();
}