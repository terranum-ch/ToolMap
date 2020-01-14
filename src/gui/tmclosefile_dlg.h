/***************************************************************************
 tmclosefile.h
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TM_CLOSE_FILE_DLG_H_
#define _TM_CLOSE_FILE_DLG_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

const int ID_BTN_ALL = wxID_HIGHEST + 14;
const int ID_BTN_NONE = wxID_HIGHEST + 15;
const int ID_BTN_INVERT = wxID_HIGHEST + 16;


class tmCloseFile_DLG : public wxDialog
{
private:
    wxCheckListBox *m_FileListCtrl;
    wxButton *m_BtnAllCtrl;
    wxButton *m_BtnInvertCtrl;
    wxButton *m_BtnNoneCtrl;

    void _CreateControls();

    void OnBtnAll(wxCommandEvent &event);

    void OnUpdateUIAll(wxUpdateUIEvent &event);

    void OnBtnInvert(wxCommandEvent &event);

    void OnBtnNone(wxCommandEvent &event);

    void OnUpdateUINone(wxUpdateUIEvent &event);

DECLARE_EVENT_TABLE()

public:

    tmCloseFile_DLG(wxWindow *parent, const wxArrayString &choices, wxWindowID id = wxID_ANY,
                    const wxString &title = _("Unlink data"), const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

    ~tmCloseFile_DLG();

    wxArrayInt GetSelections();
};


#endif
