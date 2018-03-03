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
#include <wx/zipstrm.h>
#include <wx/regex.h>
#include <wx/wfstream.h>
#include <wx/sstream.h>
#include <wx/progdlg.h>
#include <wx/xml/xml.h>


class DataBaseTM;

class BackupFile
{
private:
    wxFileName m_OutFileName;
    wxFileName m_InDirectory;
    wxString m_Comment;
    wxString m_Author;
    wxDateTime m_Date;
    bool m_UseDate;

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

    inline const bool IsUsingDate() const;

    void SetUseDate(bool value);
};


inline const wxFileName BackupFile::GetOutputName() const
{
    return m_OutFileName;
}


inline const wxFileName BackupFile::GetInputDirectory() const
{
    return m_InDirectory;
}


inline const wxString BackupFile::GetComment() const
{
    return m_Comment;
}


inline const wxString BackupFile::GetAuthor() const
{
    return m_Author;
}


inline const wxDateTime BackupFile::GetDate() const
{
    return m_Date;
}


inline const bool BackupFile::IsUsingDate() const
{
    return m_UseDate;
}


/***************** BACKUP MANAGER **********************/
class BackupManager
{
private:
    DataBaseTM *m_Database;

    void _ListMySQLFiles(const wxString &directory, wxArrayString &files);

public:
    BackupManager(DataBaseTM *database);

    virtual ~BackupManager();

    bool Backup(const BackupFile &fileinfo, wxWindow *progressparent = NULL);

    bool Restore(const BackupFile &fileinfo, wxWindow *progressparent = NULL);

    bool GetFileInfo(const wxFileName &file, BackupFile &fileinfo);

    bool SetMetadata(const BackupFile &fileinfo, wxZipOutputStream *zip);

    inline DataBaseTM *GetDatabase();

};


inline DataBaseTM *BackupManager::GetDatabase()
{
    return m_Database;
}


#endif
