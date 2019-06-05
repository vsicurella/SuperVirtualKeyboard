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
    // old code that might not be necessary
	presetsLibraryNode = presetManager->presetLibraryNode;
	presetsSorted = presetManager->getPresetsSorted();
    
    rootMenu = std::make_shared<PopupMenu>(presetManager->getPresetMenu());
    populateMenu(*rootMenu);
}

PresetLibraryBox::~PresetLibraryBox()
{
	presetManager = nullptr;
}

void PresetLibraryBox::populateMenu()
{
    PopupMenu* currentMenu = getRootMenu();
    *currentMenu = createMenu(presetManager);
}

void PresetLibraryBox::populateMenu(const PopupMenu& menuToReference)
{
    PopupMenu* currentMenu = getRootMenu();
    *currentMenu = menuToReference;
}

PopupMenu PresetLibraryBox::createMenu(SvkPresetManager* presetManagerIn)
{
    ValueTree presetsLibraryNode = presetManagerIn->presetLibraryNode;
    Array<Array<ValueTree>>* presetsSorted = presetManagerIn->getPresetsSorted();
    
    PopupMenu menuOut;
    Array<PopupMenu> menus = Array<PopupMenu>({PopupMenu(), PopupMenu(), PopupMenu(), PopupMenu()});
    
    PopupMenu& scaleSubMenu = menus.getReference(0);
    PopupMenu& modeSubMenu = menus.getReference(1);
    PopupMenu& familySubMenu = menus.getReference(2);
    PopupMenu& userSubMenu = menus.getReference(3);
    
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
                        scaleSubMenu.addSeparator();
                    separatorProperty = presetIn[IDs::scaleSize];
                    displayName = presetIn[IDs::modeName];
                    scaleSubMenu.addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::modeSize):
                    if (separatorProperty != presetIn[IDs::modeSize])
                        modeSubMenu.addSeparator();
                    separatorProperty = presetIn[IDs::modeSize];
                    displayName = presetIn[IDs::modeName];
                    modeSubMenu.addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::familyName):
                    if (separatorProperty != presetIn[IDs::family])
                        familySubMenu.addSeparator();
                    separatorProperty = presetIn[IDs::family];
                    displayName = presetIn[IDs::modeName];
                    familySubMenu.addItem(++subMenuIndex, displayName);
                    break;
                    
                case (SortType::user):
                    if (presetIn[IDs::factoryPreset])
                        continue;
                    if (separatorProperty != presetIn[IDs::scaleSize])
                        userSubMenu.addSeparator();
                    separatorProperty = presetIn[IDs::scaleSize];
                    displayName = presetIn[IDs::modeName];
                    userSubMenu.addItem(++subMenuIndex, displayName);
                    break;
            }
        }
    }
    
    menuOut.addSubMenu("by Scale Size", scaleSubMenu, true);
    menuOut.addSubMenu("by Mode Size", modeSubMenu, true);
    menuOut.addSubMenu("by Family", familySubMenu, true);
    menuOut.addSubMenu("User", userSubMenu, true);
    
    return menuOut;
}

void PresetLibraryBox::showPopup()
{
	ComboBox::showPopup();
}
