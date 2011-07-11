/***************************************************************************
 openerror_dlg.cpp
 Display the error message dialog
 -------------------
 copyright            : (C) 2008 CREALP Lucien Schreiber 
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

#include "openerror_dlg.h"
#include "../database/database_tm.h"
#include "../components/tmupdate/update.h"
#include "../core/backupmanager.h"
#include "../core/tmprojectupdater.h"



void tmOpenError_DLG::_CreateControls(const wxString & projname, int iActDBVersion){
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_ErrPanelDB = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	
	wxString myErrTxt = wxString::Format(_("Opening '%s' project failed!"), projname);
	m_ErrDBTextCtrl = new wxStaticText( m_ErrPanelDB, wxID_ANY, myErrTxt , wxDefaultPosition, wxDefaultSize, 0 );
	m_ErrDBTextCtrl->Wrap( -1 );
	bSizer6->Add( m_ErrDBTextCtrl, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_ErrPanelDB, wxID_ANY, _("Error:") ), wxVERTICAL );
	
	m_ErrDBLogCtrl = new wxTextCtrl( m_ErrPanelDB, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300,50 ), 0 );
	sbSizer1->Add( m_ErrDBLogCtrl, 1, wxEXPAND, 5 );
	
	bSizer6->Add( sbSizer1, 1, wxEXPAND|wxALL, 5 );
	
	m_ErrPanelDB->SetSizer( bSizer6 );
	m_ErrPanelDB->Layout();
	bSizer6->Fit( m_ErrPanelDB );
	bSizer5->Add( m_ErrPanelDB, 1, wxEXPAND | wxALL, 5 );
	
	m_ErrPanelProjVersion = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxString myErrTxt2 = wxString::Format(_("The project '%s' is no longer supported (version %d).\nWould you like to convert it to the newer version (version %d)?\n\nConverted projects can no longer be loader by previous version of ToolMap."),
										  projname,
										  iActDBVersion,
										  TM_DATABASE_VERSION);
										  
	m_ErrVerTextCtrl = new wxStaticText( m_ErrPanelProjVersion, wxID_ANY, myErrTxt2, wxDefaultPosition, wxDefaultSize, 0 );
	m_ErrVerTextCtrl->Wrap( -1 );
	bSizer7->Add( m_ErrVerTextCtrl, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_ErrPanelProjVersion, wxID_ANY, _("Operations") ), wxVERTICAL );
	
	m_ErrVerBackupConvertCtrl = new wxButton( m_ErrPanelProjVersion, wxID_ANY, _("Backup and convert..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_ErrVerBackupConvertCtrl->SetDefault(); 
	sbSizer2->Add( m_ErrVerBackupConvertCtrl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ErrVerConvertCtrl = new wxButton( m_ErrPanelProjVersion, wxID_ANY, _("Convert only..."), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_ErrVerConvertCtrl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer7->Add( sbSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	m_ErrPanelProjVersion->SetSizer( bSizer7 );
	m_ErrPanelProjVersion->Layout();
	bSizer7->Fit( m_ErrPanelProjVersion );
	bSizer5->Add( m_ErrPanelProjVersion, 1, wxEXPAND | wxALL, 5 );
	
	m_ErrPanelTMVersion = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	wxString myErrTxt3 = wxString::Format(_("The project '%s' is not supported by this version of ToolMap! (project version: %d)\nPlease consider upgrading ToolMap to a newer version."),
										  projname,
										  iActDBVersion);
	m_ErrTMTextCtrl = new wxStaticText( m_ErrPanelTMVersion, wxID_ANY, myErrTxt3, wxDefaultPosition, wxDefaultSize, 0 );
	m_ErrTMTextCtrl->Wrap( -1 );
	bSizer8->Add( m_ErrTMTextCtrl, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_ErrPanelTMVersion, wxID_ANY, _("Upgrades") ), wxVERTICAL );
	
	m_ErrTMDownloadCtrl = new wxButton( m_ErrPanelTMVersion, wxID_ANY, _("Download new version of ToolMap..."), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer3->Add( m_ErrTMDownloadCtrl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer8->Add( sbSizer3, 1, wxEXPAND|wxALL, 5 );
	
	m_ErrPanelTMVersion->SetSizer( bSizer8 );
	m_ErrPanelTMVersion->Layout();
	bSizer8->Fit( m_ErrPanelTMVersion );
	bSizer5->Add( m_ErrPanelTMVersion, 1, wxEXPAND | wxALL, 5 );
	
	wxStdDialogButtonSizer* m_sdbSizer2;
	wxButton* m_sdbSizer2Cancel;
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();
	bSizer5->Add( m_sdbSizer2, 0, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer5 );
}


tmOpenError_DLG::tmOpenError_DLG( wxWindow* parent, 
								 int iActDBVersion,
								 const wxString& projname,
								 DataBaseTM * database,
								 wxWindowID id,
								 const wxPoint& pos,
								 const wxSize& size, long style ) : 
wxDialog( parent, id, _("Open failed"), pos, size, style ){
	m_pDB = database;
	wxASSERT(m_pDB);
	
	_CreateControls(projname, iActDBVersion);
	
	
	if (iActDBVersion > tmDB_OPEN_FAILED_WRONG_VERSION) {
		// Project Need Update
		if (iActDBVersion < TM_DATABASE_VERSION) {
			m_ErrPanelDB->Hide();
			m_ErrPanelTMVersion->Hide();
		}
		// ToolMap Need Update
		else {
			m_ErrPanelDB->Hide();
			m_ErrPanelProjVersion->Hide();
		}

	}
	// generic problem (mainly Database)
	else {
		m_ErrPanelProjVersion->Hide();
		m_ErrPanelTMVersion->Hide();
		if (iActDBVersion == tmDB_OPEN_FAILED_NOT_TM_DB) {
			m_ErrDBLogCtrl->SetValue(wxString::Format(_("'%s' isn't a ToolMap project!"),projname));
		}else {
			m_ErrDBLogCtrl->SetValue(_("Project is corrupted or MySQL encounter an error!\n") +
									 m_pDB->DataBaseGetLastError());
		}
	}
	
	this->Layout();
	GetSizer()->Fit( this );
	this->Centre( wxBOTH );
	
	m_ErrVerBackupConvertCtrl->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmOpenError_DLG::OnConvertBackup ), NULL, this );
	m_ErrVerConvertCtrl->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmOpenError_DLG::OnConvert ), NULL, this );
	m_ErrTMDownloadCtrl->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmOpenError_DLG::OnDownload ), NULL, this );
}



tmOpenError_DLG::~tmOpenError_DLG()
{
	m_ErrVerBackupConvertCtrl->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmOpenError_DLG::OnConvertBackup ), NULL, this );
	m_ErrVerConvertCtrl->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmOpenError_DLG::OnConvert ), NULL, this );
	m_ErrTMDownloadCtrl->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmOpenError_DLG::OnDownload ), NULL, this );	
}


void tmOpenError_DLG::OnConvertBackup( wxCommandEvent& event ){
	//1. Get backup path
	wxString myBkpPath = wxEmptyString;
	if(m_pDB->GetProjectBackupPath(myBkpPath)!= PATH_OK){
		// backup path not specified, choose one now
		wxDirDialog myPathDlg (this, _("Select a Backup Path"), "",wxDD_DEFAULT_STYLE);
		if (myPathDlg.ShowModal() == wxID_CANCEL) {
			return;
		}
		myBkpPath = myPathDlg.GetPath();
	}
	
	//2. Try backuping
	BackupFile myBckFile;
    myBckFile.SetInputDirectory(wxFileName(m_pDB->DataBaseGetPath(),
                                           m_pDB->DataBaseGetName()));
    myBckFile.SetDate(wxDateTime::Now());
    myBckFile.SetOutputName(wxFileName(myBkpPath,
									   m_pDB->DataBaseGetName(),
									   "tmbk"));
	myBckFile.SetComment(wxString::Format(_("Automatic backup before converting project to version %d"), 
										  TM_DATABASE_VERSION));
	wxBeginBusyCursor();
	BackupManager  myBM (m_pDB);
	if (myBM.Backup(myBckFile) == false) {
		wxLogError(_("Backup : '%s' Failed !"), myBckFile.GetOutputName().GetFullName());
		wxEndBusyCursor();
		return;
	}
	wxEndBusyCursor();
	
	// 3. convert
	wxCommandEvent myUnusedEvent;
	OnConvert(myUnusedEvent);
}



void tmOpenError_DLG::OnConvert( wxCommandEvent& event ){
	tmProjectUpdater myPrjUpd (m_pDB);
	if (myPrjUpd.DoUpdate() != tmPRJ_UPD_ERROR_OK) {
		wxLogError(_("Converting project '%s' failed!"), m_pDB->DataBaseGetName());
		return;
	}
	
	// converting done! quit the dialog
	EndModal(wxID_OK);
}


void tmOpenError_DLG::OnDownload( wxCommandEvent& event ){
	wxLaunchDefaultBrowser(WEBUPDATE_SERVER_DOWNLOAD);	
}









void tmOpenRecentError_DLG::_CreateControls(const wxString & prjname) {
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxString myMessage = wxString::Format(_("The project '%s' was not found!\nIt was maybe moved or deleted. \nPlease try opening another project"),
										  prjname);
	
	m_ErrorMsgCtrl = new wxStaticText( this, wxID_ANY, myMessage, wxDefaultPosition, wxDefaultSize, 0 );
	m_ErrorMsgCtrl->Wrap( -1 );
	bSizer4->Add( m_ErrorMsgCtrl, 0, wxALL|wxEXPAND, 5 );
	
	m_RemoveCheckCtrl = new wxCheckBox( this, wxID_ANY, _("Remove this project from recent projects"),
									   wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_RemoveCheck));
	m_RemoveCheckCtrl->SetValue(m_RemoveCheck); 
	bSizer4->Add( m_RemoveCheckCtrl, 0, wxALL|wxEXPAND, 5 );
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

	
	wxStdDialogButtonSizer* m_sdbSizer1;
	wxButton* m_sdbSizer1OK;
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1->Realize();
	bSizer4->Add( m_sdbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
	bSizer4->Fit( this );
	
	this->Centre( wxBOTH );
}




tmOpenRecentError_DLG::tmOpenRecentError_DLG(wxWindow * parent, wxWindowID id, const wxString & title,
											 const wxString & prjname,
											 const wxPoint & pos, const wxSize & size, long style) :
wxDialog(parent, id, title, pos, size,style) {
	m_RemoveCheck = true;
	_CreateControls(prjname);
}


tmOpenRecentError_DLG::~tmOpenRecentError_DLG(){
}


