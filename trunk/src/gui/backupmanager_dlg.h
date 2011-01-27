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
#include "../core/datalistreportctrl.h"

class BackupManager;

class BackupManagerDLG : public wxDialog {
  private:
    void _CreateControls();

    void OnButtonClose(wxCommandEvent & event);
    void OnButtonRestore(wxCommandEvent & event);
    void OnButtonDelete(wxCommandEvent & event);
    void OnListColumnClick(wxListEvent & event);

  public:
    BackupManagerDLG(wxWindow * parent, wxWindowID id, const wxString & title, BackupManager * bckmanager);
    virtual ~BackupManagerDLG();

};
#endif
