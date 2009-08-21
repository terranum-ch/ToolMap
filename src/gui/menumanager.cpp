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
MenuManager::MenuManager(wxMenuBar * menubar, wxFileConfig * configfile)
{
	m_MenuStatus = MENU_DB_CLOSED;
	m_MenuBar = menubar;
	bMenu_DB_IsOpen = FALSE;
	m_pFilesHistory = NULL;
	m_pConfig = configfile;
	
	// prepare for recent documents
	InitializeRecentFilesHistory();
}

/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
 *******************************************************************************/
MenuManager::~MenuManager()
{
	TerminateRecentFilesHistory();
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
	UpdateMenuData();
	UpdateMenuView();
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


void MenuManager::UpdateMenuData ()
{
	m_MenuBar->Enable(ID_MENU_ADD_SPATIAL_DATA, bMenu_DB_IsOpen);
	m_MenuBar->Enable(ID_MENU_IMPORT_GIS_DATA, bMenu_DB_IsOpen);
}


void MenuManager::UpdateMenuView ()
{
	for (int i = ID_MENU_ZOOM_PREVIOUS_EXTEND; i <= ID_MENU_SELECTED_LAYER ; i++)
	{
		m_MenuBar->Enable(i, bMenu_DB_IsOpen);
	}
}


/***************************************************************************//**
 @brief Init the Recent files
 @details This function search for the #ID_MENU_RECENT menu and set it as the
 menu for recent files
 @author Lucien Schreiber (c) CREALP 2007
 @date 07 April 2008
 *******************************************************************************/
void MenuManager::InitializeRecentFilesHistory()
{
	TerminateRecentFilesHistory();
	
	if (m_MenuBar)
	{
		wxMenuItem * miFound = m_MenuBar->FindItem(ID_MENU_RECENT);
		if (miFound->IsSubMenu())
		{
			wxMenu * menu = miFound->GetSubMenu();
			if (menu)
			{
				m_pFilesHistory = new wxFileHistory(5);
				m_pFilesHistory->Load(*m_pConfig);
				m_pFilesHistory->UseMenu(menu);
				m_pFilesHistory->AddFilesToMenu();
	
			}
			
		}
		
	}
	
}


/***************************************************************************//**
 @brief Uninit the Recent files
 @details This function save the actual File history on the disk and delete the
 file history object
 @author Lucien Schreiber (c) CREALP 2007
 @date 07 April 2008
 *******************************************************************************/
void MenuManager::TerminateRecentFilesHistory()
{
	// save the recent to the config file and delete 
	// the object.
	
	if (m_pFilesHistory)
    {

		m_pFilesHistory->Save(*m_pConfig);

		delete m_pFilesHistory;
		m_pFilesHistory = 0;
	}
	
}


/***************************************************************************//**
 @brief Add a path to the recent file menu
 @details Call this function for adding a recent path to the Recent menu
 @param spath a wxString containing the path we want to store into the recent
 files
 @author Lucien Schreiber (c) CREALP 2007
 @date 07 April 2008
 *******************************************************************************/
void MenuManager::AddFileToRecent (const wxString & spath)
{
	if (m_pFilesHistory)
	{
		m_pFilesHistory->AddFileToHistory(spath);
	}
	
}


/***************************************************************************//**
 @brief Remove an item from the recent files menu
 @details Remove the specified item from the recent files menu.
 @param fileid index of element to remove
 @author Lucien Schreiber (c) CREALP 2007
 @date 08 April 2008
 *******************************************************************************/
void MenuManager::RemoveFileFromRecent (int fileid)
{
	if(m_pFilesHistory)
	{
		m_pFilesHistory->RemoveFileFromHistory(fileid);
	}
}


/***************************************************************************//**
 @brief Get a path from the recent file menu
 @details Call this function for getting the path linked to an Menu event. 
 Pay
 attention fileid is an index and not the Menu ID sending the event see code
 behind :
 @code ...
 MenuManager * m_MManager
 m_MManager->GetRecentFile(myPath, event.GetId() - wxID_FILE1);
 ...
 @endcode
 @param filepath wxString filled with filepath if funtion return TRUE otherwise
 unchanged
 @param fileid the index of the item we want to retrieve from the file history
 @return true if an item was found in the file history and FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2007
 @date 07 April 2008
 *******************************************************************************/
bool MenuManager::GetRecentFile (wxString & filepath, int fileid)
{
	
	if (m_pFilesHistory)
	{
		filepath = m_pFilesHistory->GetHistoryFile(fileid);
		if (!filepath.IsEmpty())
		{
			return true;
		}
		wxLogError(_T("Recent path is empty"));
		
	}
	return false;
}



void MenuManager::EditingStatus(bool started)
{
	m_MenuBar->Enable(ID_MENU_DRAW, started);
	//m_MenuBar->Enable(ID_MENU_EDIT_VERTEX_POS, started);
	m_MenuBar->Enable(ID_MENU_MODIFY,started);
	
}




/********************************* TOOLBAR MANAGER ********************************/
ToolbarManager::ToolbarManager(wxToolBar * toolbar)
{
	wxASSERT(toolbar);
	m_TBar = toolbar;
}


ToolbarManager::~ToolbarManager()
{
	
}


void ToolbarManager::EditingStatus(bool started)
{
	m_TBar->EnableTool(ID_MENU_DRAW, started);
	m_TBar->EnableTool(ID_MENU_MODIFY, started);
}

