/***************************************************************************
								tmupdate_dlg.cpp
						Display dialog for web update
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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

// comment doxygen

#include "tmupdate_dlg.h"



tmUpdate_DLG::tmUpdate_DLG( wxWindow* parent, wxWindowID id, 
						   const wxString& title, const wxPoint& pos,
						   const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	//this->SetSizeHints( wxSize( 320,200 ), wxDefaultSize );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );
	
	tmUpdate_Panel_Sucess = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText8;
	m_staticText8 = new wxStaticText( tmUpdate_Panel_Sucess, wxID_ANY, _("A new version of ToolMap is available"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer29->Add( m_staticText8, 0, wxALL, 5 );
	
	m_TextVersion = new wxStaticText( tmUpdate_Panel_Sucess, wxID_ANY, _("2.0.700"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE|wxRAISED_BORDER );
	m_TextVersion->Wrap( -1 );
	m_TextVersion->SetFont( wxFont( 20, 70, 90, 92, false, wxEmptyString ) );
	
	bSizer29->Add( m_TextVersion, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText10;
	m_staticText10 = new wxStaticText( tmUpdate_Panel_Sucess, wxID_ANY, _("Click on the button below to download the new version.  Once downloaded, close ToolMap, run the installer  and follow the instructions."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( 300 );
	bSizer29->Add( m_staticText10, 0, wxALL|wxEXPAND, 5 );
	
	m_ChangesHyperlink = new wxHyperlinkCtrl( tmUpdate_Panel_Sucess, wxID_ANY, _("See what has changes"), wxT("http://www.wxformbuilder.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer29->Add( m_ChangesHyperlink, 0, wxALL, 5 );
	
	wxStaticLine* m_staticline3;
	m_staticline3 = new wxStaticLine( tmUpdate_Panel_Sucess, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer29->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxHORIZONTAL );
	
	wxButton* m_button25;
	m_button25 = new wxButton( tmUpdate_Panel_Sucess, ID_DLGUP_DOWNLOAD, _("Download"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button25->SetDefault(); 
	bSizer30->Add( m_button25, 0, wxALL, 5 );
	
	wxButton* m_button26;
	m_button26 = new wxButton( tmUpdate_Panel_Sucess, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( m_button26, 0, wxALL, 5 );
	
	
	bSizer30->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxButton* m_button27;
	m_button27 = new wxButton( tmUpdate_Panel_Sucess, ID_DLGUP_OPTIONS1, _("Options >>"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( m_button27, 0, wxALL, 5 );
	
	bSizer29->Add( bSizer30, 0, wxEXPAND, 5 );
	
	tmUpdate_Panel_Sucess->SetSizer( bSizer29 );
	tmUpdate_Panel_Sucess->Layout();
	bSizer29->Fit( tmUpdate_Panel_Sucess );
	bSizer28->Add( tmUpdate_Panel_Sucess, 1, wxEXPAND|wxALL, 5 );
	
	tmUpdate_Panel_Option = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	m_UpdateCheckbox = new wxCheckBox( tmUpdate_Panel_Option, ID_DLGUP_OPT_STARTUP, _("Check for updates during startup"), wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer31->Add( m_UpdateCheckbox, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( tmUpdate_Panel_Option, wxID_ANY, _("Proxy") ), wxVERTICAL );
	
	m_ProxyCheckbox = new wxCheckBox( tmUpdate_Panel_Option, ID_DLGUP_OPT_PROXY, _("Use a proxy for accessing Internet"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sbSizer3->Add( m_ProxyCheckbox, 0, wxALL, 5 );
	
	m_ProxyText1 = new wxStaticText( tmUpdate_Panel_Option, wxID_ANY, _("Proxy informations "), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProxyText1->Wrap( -1 );
	sbSizer3->Add( m_ProxyText1, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_ProxyTextCtrl = new wxTextCtrl( tmUpdate_Panel_Option, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer3->Add( m_ProxyTextCtrl, 0, wxALL|wxEXPAND, 5 );
	
	m_ProxyText2 = new wxStaticText( tmUpdate_Panel_Option, wxID_ANY, _("Please use the following form : mysampleproxy.ch:8080 "), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProxyText2->Wrap( -1 );
	sbSizer3->Add( m_ProxyText2, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	bSizer31->Add( sbSizer3, 1, wxEXPAND|wxTOP, 5 );
	
	tmUpdate_Panel_Option->SetSizer( bSizer31 );
	tmUpdate_Panel_Option->Layout();
	bSizer31->Fit( tmUpdate_Panel_Option );
	bSizer28->Add( tmUpdate_Panel_Option, 0, wxEXPAND | wxALL, 5 );
	
	tmUpdate_Panel_Failure = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer301;
	bSizer301 = new wxBoxSizer( wxVERTICAL );
	
	m_ErrorText = new wxStaticText( tmUpdate_Panel_Failure, wxID_ANY, _("Internet problem. ToolMap wasn't able to connect to the web. Please use the options button bellow if you are behind a proxy."), wxDefaultPosition, wxDefaultSize, 0 );
	m_ErrorText->Wrap( 350 );
	bSizer301->Add( m_ErrorText, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer301->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer311;
	bSizer311 = new wxBoxSizer( wxHORIZONTAL );
	
	wxButton* m_button261;
	m_button261 = new wxButton( tmUpdate_Panel_Failure, ID_DLGUP_CLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button261->SetDefault(); 
	bSizer311->Add( m_button261, 0, wxALL, 5 );
	
	
	bSizer311->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxButton* m_button271;
	m_button271 = new wxButton( tmUpdate_Panel_Failure, ID_DLGUP_OPTIONS2, _("Options >>"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer311->Add( m_button271, 0, wxALL, 5 );
	
	bSizer301->Add( bSizer311, 0, wxEXPAND, 5 );
	
	tmUpdate_Panel_Failure->SetSizer( bSizer301 );
	tmUpdate_Panel_Failure->Layout();
	bSizer301->Fit( tmUpdate_Panel_Failure );
	bSizer28->Add( tmUpdate_Panel_Failure, 0, wxEXPAND | wxALL, 5 );
	
	//tmUpdate_Panel_Failure->Hide();
	
	this->SetSizer( bSizer28 );
	this->Layout();
	bSizer28->Fit( this );
}

tmUpdate_DLG::~tmUpdate_DLG()
{
}
