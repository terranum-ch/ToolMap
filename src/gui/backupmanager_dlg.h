/***************************************************************************
 backupmanager_dlg.h
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

#ifndef _BACKUPMANAGER_DLG_H
#define _BACKUPMANAGER_DLG_H
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/wupdlock.h>
#include "../core/datalistreportctrl.h"

class BackupManager;

const int ID_BTN_RESTORE = wxID_HIGHEST + 1;
const int ID_LIST_BACKUPS = wxID_HIGHEST + 2;

class BackupManagerDLG : public wxDialog {
  private:
    wxImageList * m_ImgList;
    DataListReportCtrl * m_ListBackup;
    bool m_AscendingOrder;
	int m_ClassedCol;
    
    wxButton* m_BtnDelete;
    wxButton* m_BtnRestore;
    wxStatusBar* m_StatusBar;
    BackupManager * m_BackupManager;
    wxString m_BackupPath;

    
    void _CreateControls();
    bool _LoadData();
    void _UpdateStatusbar(const unsigned int & bcknumber);

    void OnButtonRestore(wxCommandEvent & event);
    void OnButtonDelete(wxCommandEvent & event);
    void OnListColumnClick(wxListEvent & event);
    
    void OnUpdateUIDelete(wxUpdateUIEvent & event);
    void OnUpdateUIRestore(wxUpdateUIEvent & event);
    
    DECLARE_EVENT_TABLE();

  public:
    BackupManagerDLG(wxWindow * parent, wxWindowID id, const wxString & title, BackupManager * bckmanager);
    virtual ~BackupManagerDLG();

};
#endif
