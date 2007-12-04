/***************************************************************************
							TocWindowDlg.cpp
				Display the TOC, try using the wxDrawerWindow class
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

#include "tocwindow_dlg.h"

TocWindowDlg::TocWindowDlg(wxWindow * parent, wxWindowID id, const wxString & title) 
	: wxDrawerWindow(parent,id,title,SYMBOL_TOCWINDOW_DLG_SIZE)
{
	CreateControls();
}

TocWindowDlg::TocWindowDlg()
{

}

TocWindowDlg::~TocWindowDlg()
{

}

IMPLEMENT_DYNAMIC_CLASS( TocWindowDlg, wxDialog )


/*!
 * TocWindowDlg event table definition
 */






/*!
 * Member initialisation
 */

void TocWindowDlg::Init()
{

}


/*!
 * Control creation for TocWindowDlg
 */
void TocWindowDlg::CreateControls()
{    


	TocWindowDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxTreeCtrl* itemTreeCtrl3 = new wxTreeCtrl( itemDialog1, ID_TREECTRL1, wxDefaultPosition, wxSize(100, 300), wxTR_SINGLE );
    itemBoxSizer2->Add(itemTreeCtrl3, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_LEFT|wxALL, 5);

    wxToggleButton* itemToggleButton5 = new wxToggleButton( itemDialog1, ID_DLGTOC_ADD, _("+"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemToggleButton5->SetValue(false);
    itemBoxSizer4->Add(itemToggleButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxToggleButton* itemToggleButton6 = new wxToggleButton( itemDialog1, ID_DLGTOC_REMOVE, _("-"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemToggleButton6->SetValue(false);
    itemBoxSizer4->Add(itemToggleButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);


}
