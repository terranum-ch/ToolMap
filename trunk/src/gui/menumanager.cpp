/***************************************************************************
								menumanager.cpp
				Deals with menu activities (enable / disable)
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
    email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************//**
 @file menumanager.cpp
 @brief Deals with menu activation
 @details This class is the main point for enabling / disabling menu based on
 events.
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
 *******************************************************************************/


#include "menumanager.h"

IMPLEMENT_CLASS(MenuManager, wxObject);
/***************************************************************************//**
 @brief constructor
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
 *******************************************************************************/
MenuManager::MenuManager(wxMenuBar * menubar)
{
	m_MenuStatus = MENU_DB_CLOSED;
	m_MenuBar = menubar;
	bMenu_DB_IsOpen = FALSE;
}

/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
 *******************************************************************************/
MenuManager::~MenuManager()
{
	
}


/***************************************************************************//**
 @brief Change status of menus
 @details Call this function for changing status of menu in response of event.
 @param flags a #MENUSTATUS item reflecting the change to take into account
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
 *******************************************************************************/
void MenuManager::SetStatus(MENUSTATUS flags)
{
	m_MenuStatus = flags; //m_MenuStatus ^ flags; //XOR
	
	// change status for database opened
	bMenu_DB_IsOpen = m_MenuStatus & MENU_DB_OPENED;
}


/***************************************************************************//**
 @brief Update menu 
 @details Call this function during idle time for updating menu items. The
 updated items are linked to the SetStatus() function
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
 *******************************************************************************/
void MenuManager::UpdateMenusStatus()
{
	UpdateMenuProject();
}


/***************************************************************************//**
 @brief Update project menu
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
 *******************************************************************************/
void MenuManager::UpdateMenuProject()
{
	for (int i = ID_MENU_BACKUP_PRJ; i <= ID_MENU_PRJ_SETTINGS ; i++)
	{
		m_MenuBar->Enable(i, bMenu_DB_IsOpen);
	}

}

