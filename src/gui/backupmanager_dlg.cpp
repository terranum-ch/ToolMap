/***************************************************************************
 backupmanager_dlg.cpp
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


#include "backupmanager_dlg.h"
#include "../core/backupmanager.h"

void BackupManagerDLG::_CreateControls() {
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
    int myListWidth = 650;
    int myBigColSize = wxRound((myListWidth - 26) / 3.0);
    m_ListBackup = new DataListReportCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( myListWidth,300 ));
	
    // insert columns
	m_ListBackup->InsertColumn(0, "");
    m_ListBackup->InsertColumn(1, _("File Name"));
    m_ListBackup->InsertColumn(2, _("Date"));
    m_ListBackup->InsertColumn(3, _("Hour"));
	m_ListBackup->InsertColumn(4, _("Comment"));

    // size columns
 	m_ListBackup->SetColumnWidth(0, 26);
    m_ListBackup->SetColumnWidth(1, myBigColSize);
    m_ListBackup->SetColumnWidth(2, wxRound(myBigColSize / 2.0));
    m_ListBackup->SetColumnWidth(3, wxRound(myBigColSize / 2.0));
	m_ListBackup->SetColumnWidth(4, myBigColSize);
    bSizer3->Add( m_ListBackup, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_BtnDelete = new wxButton( this, wxID_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_BtnDelete, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_BtnRestore = new wxButton( this, ID_BTN_RESTORE, _("Restore"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_BtnRestore, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxButton* myButton;
	myButton = new wxButton( this, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	myButton->SetDefault(); 
	bSizer4->Add( myButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );	
    
    m_StatusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP | wxNO_BORDER);
    bSizer3->Add(m_StatusBar, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 0);

	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );
	this->Centre( wxBOTH );
}



void BackupManagerDLG::OnButtonClose(wxCommandEvent & event) {
}



void BackupManagerDLG::OnButtonRestore(wxCommandEvent & event) {
}



void BackupManagerDLG::OnButtonDelete(wxCommandEvent & event) {
}



void BackupManagerDLG::OnListColumnClick(wxListEvent & event) {
}



BackupManagerDLG::BackupManagerDLG(wxWindow * parent, wxWindowID id, const wxString & title, BackupManager * bckmanager): 
wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER){
    wxASSERT(bckmanager);
    m_BackupManager = bckmanager;
    _CreateControls();
}



BackupManagerDLG::~BackupManagerDLG() {
}


