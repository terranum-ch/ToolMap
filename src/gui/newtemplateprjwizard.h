/***************************************************************************
 newtemplateprjwizard.h
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef _NEWTEMPLATEPRJWIZARD_H
#define _NEWTEMPLATEPRJWIZARD_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/wizard.h>
#include <wx/filepicker.h>
#include <wx/filefn.h>

#include "../core/backupmanager.h"

const int ID_NEWPRJ_SOURCEDIRCTRL = wxID_HIGHEST + 3;
const int ID_NEWPRJ_DESTDIRCTRL = wxID_HIGHEST + 4;
const int ID_NEWPRJ_DESTNAMECTRL = wxID_HIGHEST + 5;


class NewTemplatePrjWizard : public wxWizard
{
private:
    wxWizardPageSimple *m_SourcePage;
    wxWizardPageSimple *m_DestinationPage;

    wxFilePickerCtrl *m_SourceFileCtrl;
    wxDirPickerCtrl *m_DestDirCtrl;
    wxTextCtrl *m_DestNameCtrl;
    wxStaticText *m_DateCtrl;
    wxStaticText *m_CommentCtrl;

    BackupFile *m_BackupFileData;

    void OnUpdateNextBtn(wxIdleEvent &event);

    void OnSourceDirChanged(wxFileDirPickerEvent &event);

    void _CreateControls();

DECLARE_EVENT_TABLE();

public:
    NewTemplatePrjWizard(wxWindow *parent, int id, const wxString &title);

    virtual ~NewTemplatePrjWizard();

    inline BackupFile *GetBackupFileData() const;

    virtual bool TransferDataFromWindow();

    int ShowWizard();
};


inline BackupFile *NewTemplatePrjWizard::GetBackupFileData() const
{
    return m_BackupFileData;
}

#endif
