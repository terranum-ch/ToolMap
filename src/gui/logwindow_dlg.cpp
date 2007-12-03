/***************************************************************************
								logwindow_dlg.cpp
							Display the log windows
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


#include "logwindow_dlg.h"


IMPLEMENT_DYNAMIC_CLASS( ImportLog, wxDialog )


///*!
// * ImportLog event table definition
// */
//
//BEGIN_EVENT_TABLE( ImportLog, wxDialog )
//
//////@begin ImportLog event table entries
//////@end ImportLog event table entries
//
//END_EVENT_TABLE()


/*!
 * ImportLog constructors
 */
ImportLog::ImportLog()
{
    Init();
}

ImportLog::ImportLog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * ImportLog creator
 */
bool ImportLog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();

    return true;
}


/*!
 * ImportLog destructor
 */
ImportLog::~ImportLog()
{

}


/*!
 * Member initialisation
 */
void ImportLog::Init()
{

}


/*!
 * Control creation for ImportLog
 */
void ImportLog::CreateControls()
{    

    ImportLog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Program log"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 1, wxGROW|wxALL, 5);

    wxTextCtrl* itemTextCtrl4 = new wxTextCtrl( itemDialog1, ID_DLGLOG_TXT, _T(""), wxDefaultPosition, wxSize(400, 200), wxTE_MULTILINE );
    itemStaticBoxSizer3->Add(itemTextCtrl4, 1, wxGROW|wxALL, 5);

    wxButton* itemButton5 = new wxButton( itemDialog1, wxID_CLOSE, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton5->SetDefault();
    itemBoxSizer2->Add(itemButton5, 0, wxALIGN_RIGHT|wxALL, 5);

}

