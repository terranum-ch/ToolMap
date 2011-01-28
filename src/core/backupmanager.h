/***************************************************************************
 backupmanager.h
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

#ifndef _BACKUPMANAGER_H
#define _BACKUPMANAGER_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/filename.h>
#include <wx/xrc/xmlres.h>
#include <wx/zipstrm.h>




class ProjectManager;

class BackupFile {
private:
    wxFileName m_OutFileName;
    wxFileName m_InDirectory;
    wxString m_Comment;
    wxString m_Author;
    wxDateTime m_Date;
 
    
public:
    BackupFile();
    virtual ~BackupFile();
    
    bool IsValid() const;
    
    inline const wxFileName GetOutputName() const;
     void SetOutputName(wxFileName value);
    
    inline const wxFileName GetInputDirectory() const;
    void SetInputDirectory(wxFileName value);
    
    inline const wxString GetComment() const;
    void SetComment(wxString value);
    
    inline const wxString GetAuthor() const;
    void SetAuthor(wxString value);
    
    inline const wxDateTime GetDate() const;
    void SetDate(wxDateTime value);
    
};



inline const wxFileName BackupFile::GetOutputName() const {
    return m_OutFileName;
}



inline const wxFileName BackupFile::GetInputDirectory() const {
    return m_InDirectory;
}



inline const wxString BackupFile::GetComment() const {
    return m_Comment;
}



inline const wxString BackupFile::GetAuthor() const {
    return m_Author;
}



inline const wxDateTime BackupFile::GetDate() const {
    return m_Date;
}



/***************** BACKUP MANAGER **********************/
class BackupManager {
private:
    ProjectManager * m_ProjectManager;
    
    void _ListMySQLFiles(const wxString & directory, wxArrayString & files);
    
public:
    BackupManager(ProjectManager * prjmanager);
    virtual ~BackupManager();
    
    bool Backup(const BackupFile & fileinfo);
    bool Restore(const BackupFile & fileinfo);
    bool GetFileInfo(const wxFileName & file, BackupFile & fileinfo);
    bool SetMetadata(const BackupFile & fileinfo, wxZipOutputStream * zip);

    
};

#endif
