/***************************************************************************
 menumanager.cpp
 Deals with menu activities (enable / disable)
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************/ /**
  @file menumanager.cpp
  @brief Deals with menu activation
  @details This class is the main point for enabling / disabling menu based on
  events.
  @author Lucien Schreiber (c) CREALP 2007
  @date 12 March 2008
  *******************************************************************************/

#include "menumanager.h"

IMPLEMENT_CLASS(MenuManager, wxObject);

/***************************************************************************/ /**
  @brief constructor
  @author Lucien Schreiber (c) CREALP 2007
  @date 12 March 2008
  *******************************************************************************/
MenuManager::MenuManager(wxMenuBar* menubar) {
    m_MenuBar = menubar;
    m_pFilesHistory = nullptr;

    // prepare for recent documents
    InitializeRecentFilesHistory();
}

/***************************************************************************/ /**
  @brief Destructor
  @author Lucien Schreiber (c) CREALP 2007
  @date 12 March 2008
  *******************************************************************************/
MenuManager::~MenuManager() {
    TerminateRecentFilesHistory();
}

/***************************************************************************/ /**
  @brief Init the Recent files
  @details This function search for the #ID_MENU_RECENT menu and set it as the
  menu for recent files
  @author Lucien Schreiber (c) CREALP 2007
  @date 07 April 2008
  *******************************************************************************/
void MenuManager::InitializeRecentFilesHistory() {
    TerminateRecentFilesHistory();
    wxConfigBase* pconfig = wxFileConfig::Get();
    pconfig->SetPath("RECENT_PRJ");

    if (m_MenuBar) {
        wxMenuItem* miFound = m_MenuBar->FindItem(ID_MENU_RECENT);
        if (miFound->IsSubMenu()) {
            wxMenu* menu = miFound->GetSubMenu();
            if (menu) {
                m_pFilesHistory = new wxFileHistory(5);
                m_pFilesHistory->Load(*pconfig);
                m_pFilesHistory->UseMenu(menu);
                m_pFilesHistory->AddFilesToMenu();
            }
        }
    }
    pconfig->SetPath("..");
}

/***************************************************************************/ /**
  @brief Uninit the Recent files
  @details This function save the actual File history on the disk and delete the
  file history object
  @author Lucien Schreiber (c) CREALP 2007
  @date 07 April 2008
  *******************************************************************************/
void MenuManager::TerminateRecentFilesHistory() {
    // save the recent to the config file and delete
    // the object.
    wxConfigBase* pconfig = wxFileConfig::Get();
    pconfig->SetPath("RECENT_PRJ");

    if (m_pFilesHistory) {
        m_pFilesHistory->Save(*pconfig);

        delete m_pFilesHistory;
        m_pFilesHistory = 0;
    }
    pconfig->SetPath("..");
}

/***************************************************************************/ /**
  @brief Add a path to the recent file menu
  @details Call this function for adding a recent path to the Recent menu
  @param spath a wxString containing the path we want to store into the recent
  files
  @author Lucien Schreiber (c) CREALP 2007
  @date 07 April 2008
  *******************************************************************************/
void MenuManager::AddFileToRecent(const wxString& spath) {
    if (m_pFilesHistory) {
        m_pFilesHistory->AddFileToHistory(spath);
    }
}

/***************************************************************************/ /**
  @brief Remove an item from the recent files menu
  @details Remove the specified item from the recent files menu.
  @param fileid index of element to remove
  @author Lucien Schreiber (c) CREALP 2007
  @date 08 April 2008
  *******************************************************************************/
void MenuManager::RemoveFileFromRecent(int fileid) {
    if (m_pFilesHistory) {
        m_pFilesHistory->RemoveFileFromHistory(fileid);
    }
}

/***************************************************************************/ /**
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
bool MenuManager::GetRecentFile(wxString& filepath, int fileid) {
    if (m_pFilesHistory) {
        filepath = m_pFilesHistory->GetHistoryFile(fileid);
        if (!filepath.IsEmpty()) {
            return true;
        }
        wxLogError(_T("Recent path is empty"));
    }
    return false;
}
