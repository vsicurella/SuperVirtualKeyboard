/*
  ==============================================================================

    PresetLibraryBox.cpp
    Created: 27 May 2019 12:02:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ReferencedComboBox.h"

ReferencedComboBox::ReferencedComboBox(const String& nameIn, const PopupMenu* menuIn)
    : ComboBox(nameIn), rootMenu(PopupMenu())
{
    if (menuIn)
        setMenu(*menuIn);
}

ReferencedComboBox::~ReferencedComboBox()
{
}

void ReferencedComboBox::setMenu(const PopupMenu& menuIn)
{
    rootMenu = menuIn;
    
    PopupMenu& currentMenu = *getRootMenu();
    currentMenu = rootMenu;
}
