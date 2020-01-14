/***************************************************************************
 logwindow_dlg.cpp
  Display the log windows
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


#include "logwindow_dlg.h"


IMPLEMENT_DYNAMIC_CLASS(ImportLogDLG, wxDialog)


BEGIN_EVENT_TABLE(ImportLogDLG, wxDialog)
                EVT_BUTTON(wxID_CLOSE, ImportLogDLG::OnCloseLogWindow)
END_EVENT_TABLE()

void ImportLogDLG::OnCloseLogWindow(wxCommandEvent &event)
{
    this->Hide();
}

/*!
 * ImportLog constructors
 */
ImportLogDLG::ImportLogDLG()
{
    Init();
}

ImportLogDLG::ImportLogDLG(wxWindow *parent, wxWindowID id, const wxString &caption, const wxPoint &pos,
                           const wxSize &size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * ImportLogDLG creator
 */
bool
ImportLogDLG::Create(wxWindow *parent, wxWindowID id, const wxString &caption, const wxPoint &pos, const wxSize &size,
                     long style)
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent, id, caption, pos, size, style);

    CreateControls();
    if (GetSizer()) {
        GetSizer()->SetSizeHints(this);
    }
    Centre();

    return true;
}


/*!
 * ImportLogDLG destructor
 */
ImportLogDLG::~ImportLogDLG()
{

}


/*!
 * Member initialisation
 */
void ImportLogDLG::Init()
{

}


/*!
 * Control creation for ImportLogDLG
 */
void ImportLogDLG::CreateControls()
{

    ImportLogDLG *itemDialog1 = this;

    wxBoxSizer *itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox *itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Program log"));
    wxStaticBoxSizer *itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 1, wxGROW | wxALL, 5);

    wxTextCtrl *itemTextCtrl4 = new wxTextCtrl(itemDialog1, ID_DLGLOG_TXT, _T(""), wxDefaultPosition, wxSize(400, 200),
                                               wxTE_MULTILINE);
    itemStaticBoxSizer3->Add(itemTextCtrl4, 1, wxGROW | wxALL, 5);

    wxButton *itemButton5 = new wxButton(itemDialog1, wxID_CLOSE, _("&Close"), wxDefaultPosition, wxDefaultSize, 0);
    itemButton5->SetDefault();
    itemBoxSizer2->Add(itemButton5, 0, wxALIGN_RIGHT | wxALL, 5);

}

