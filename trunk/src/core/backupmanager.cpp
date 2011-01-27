/***************************************************************************
 backupmanager.cpp
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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


#include "backupmanager.h"
#include "projectmanager.h"

BackupFile::BackupFile() {
    m_OutFileName.Clear();
    m_InDirectory.Clear();
    m_Comment = wxEmptyString;
    m_Author = wxEmptyString;
    m_Date = wxDateTime();
    wxASSERT(m_Date.IsValid() == false);
}




BackupFile::~BackupFile() {
}



bool BackupFile::IsValid() {
    if (m_OutFileName.IsOk() == false) {
        return false;
    }
    
    if (m_InDirectory.IsOk() == false) {
        return false;
    }
    
    if (m_Date.IsValid() == false) {
        return false;
    }
    return true;
}



void BackupFile::SetOutputName(wxFileName value) {
  m_OutFileName = value;
}



void BackupFile::SetInputDirectory(wxFileName value) {
  m_InDirectory = value;
}



void BackupFile::SetComment(wxString value) {
  m_Comment = value;
}



void BackupFile::SetAuthor(wxString value) {
  m_Author = value;
}



void BackupFile::SetDate(wxDateTime value) {
  m_Date = value;
}






/***************** BACKUP MANAGER **********************/
BackupManager::BackupManager(ProjectManager * prjmanager) {
}

BackupManager::~BackupManager() {
}

bool BackupManager::Backup(const BackupFile & fileinfo) {
}

bool BackupManager::Restore(const BackupFile & fileinfo) {
}

bool BackupManager::GetFileInfo(const wxFileName & file, BackupFile & fileinfo) {
}

