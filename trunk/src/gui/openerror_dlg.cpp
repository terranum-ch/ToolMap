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


tmOpenError_DLG::tmOpenError_DLG( wxWindow* parent, 
						   int iActDBVersion,
						   const wxString& title,
						   wxWindowID id,
						   const wxPoint& pos,
						   const wxSize& size, long style ) : 
							wxDialog( parent, id, _("Error opening project"), pos, size, style )
{
	// TODO: Set new code for dialog here
	int iNeededDBVersion = 220;
	wxString szErrorMsg2 = _("Project isn't a ToolMap project.\nPlease select a ToolMap project");
	if (iActDBVersion < iNeededDBVersion && iActDBVersion > OPEN_NOT_GOOD_VERSION)
	{
		szErrorMsg2 = wxString::Format(_("Detected project version is : %d\nNeeded project version is : %d\nPlease migrate your project first"),
									  iActDBVersion, iNeededDBVersion);
	}
	if (iActDBVersion > iNeededDBVersion)
	{
		szErrorMsg2 = wxString::Format(_("Detected project version is : %d\nNeeded project version is : %d\nPlease install latest version of ToolMap"),
									   iActDBVersion, iNeededDBVersion);
	}
	
	wxString szErrorMsg1 = _("Error opening : ") + title;
	

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	wxBoxSizer* item1;
	item1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* item2;
	item2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBitmap bmpError = wxArtProvider::GetBitmap(wxART_ERROR, wxART_MESSAGE_BOX);
	m_dlgbmp = new wxStaticBitmap( this, ID_OPENERROR_BMP, bmpError, wxDefaultPosition, wxDefaultSize, 0 );
	item2->Add( m_dlgbmp, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* item3;
	item3 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* item4;
	item4 = new wxStaticText( this, wxID_ANY, szErrorMsg1, wxDefaultPosition, wxDefaultSize, 0 );
	item4->Wrap( -1 );
	item3->Add( item4, 0, wxALL, 5 );
	
	m_ErrorTxt = new wxStaticText( this, wxID_ANY, szErrorMsg2, wxDefaultPosition, wxDefaultSize, 0 );
	m_ErrorTxt->Wrap( -1 );
	item3->Add( m_ErrorTxt, 0, wxALL, 5 );
	
	item2->Add( item3, 1, wxEXPAND, 5 );
	
	item1->Add( item2, 1, wxEXPAND, 5 );
	
	wxStaticLine* item5;
	item5 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	item1->Add( item5, 0, wxEXPAND | wxALL, 5 );
	
	m_OkBtn = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OkBtn->SetDefault(); 
	item1->Add( m_OkBtn, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	this->SetSizer( item1 );
	this->Layout();
	item1->Fit( this );
	
	this->Centre( wxBOTH );
}

tmOpenError_DLG::~tmOpenError_DLG()
{
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


