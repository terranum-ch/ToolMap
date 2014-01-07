/***************************************************************************
 preference_dlg.h
 Display Preference dialog
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

// comment doxygen

#include "preference_dlg.h"


PreferenceDLG::PreferenceDLG( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	_CreateControls();
}



PreferenceDLG::~PreferenceDLG()
{
}



void PreferenceDLG::_CreateControls(){
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxNotebook* m_notebook2;
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wxPanel* m_panel2;
	m_panel2 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText5;
	m_staticText5 = new wxStaticText( m_panel2, wxID_ANY, _("Selection color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer2->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_SelColourCtrl = new wxColourPickerCtrl( m_panel2, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer2->Add( m_SelColourCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_SelOutlineCtrl = new wxCheckBox( m_panel2, wxID_ANY, _("Display outline"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_SelOutlineCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
    fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_RelPathCtrl = new wxCheckBox( m_panel2, wxID_ANY, _("Save relative paths"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_RelPathCtrl, 0, wxALL, 5 );
    
    fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
    
    m_BuildOverviewCtrl = new wxCheckBox( m_panel2, wxID_ANY, _("Build overviews"));
    fgSizer2->Add(m_BuildOverviewCtrl, 0, wxALL, 5);
    
    
	bSizer3->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer3 );
	m_panel2->Layout();
	bSizer3->Fit( m_panel2 );
	m_notebook2->AddPage( m_panel2, _("General"), false );
	wxPanel* m_panel3;
	m_panel3 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_UpdateCheckCtrl = new wxCheckBox( m_panel3, wxID_ANY, _("Check for updates at startup"), wxDefaultPosition, wxDefaultSize, 0 );
	m_UpdateCheckCtrl->SetValue(true); 
	bSizer6->Add( m_UpdateCheckCtrl, 0, wxALL, 5 );
	
	wxStaticText* m_staticText7;
	m_staticText7 = new wxStaticText( m_panel3, wxID_ANY, _("Proxy informations:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer6->Add( m_staticText7, 0, wxALL, 5 );
	
	m_ProxyInfoCtrl = new wxTextCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_ProxyInfoCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText8;
	m_staticText8 = new wxStaticText( m_panel3, wxID_ANY, _("Please use the following form: myproxy.com:8080"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer6->Add( m_staticText8, 0, wxALL, 5 );
	
	m_panel3->SetSizer( bSizer6 );
	m_panel3->Layout();
	bSizer6->Fit( m_panel3 );
	m_notebook2->AddPage( m_panel3, _("Updates"), false );
	
    wxPanel* m_panelwebraster;
	m_panelwebraster = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_panelwebraster, wxID_ANY, _("Internet waiting time before refreshing [ms]") ), wxVERTICAL );
	
	m_WaitingTimeCtrl = new wxSlider( m_panelwebraster, wxID_ANY, 250, 0, 500, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	sbSizer1->Add( m_WaitingTimeCtrl, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer4->Add( sbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panelwebraster, wxID_ANY, _("Location for temporary web raster") ), wxVERTICAL );
	
	m_RamCtrl = new wxRadioButton( m_panelwebraster, wxID_ANY, _("RAM"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_RamCtrl->SetValue( true );
	sbSizer2->Add( m_RamCtrl, 0, wxALL, 5 );
	
	m_MemoryCtrl = new wxRadioButton( m_panelwebraster, wxID_ANY, _("File"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_MemoryCtrl, 0, wxALL, 5 );
	
	
	bSizer4->Add( sbSizer2, 0, wxEXPAND|wxALL, 5 );
	
	
	m_panelwebraster->SetSizer( bSizer4 );
	m_panelwebraster->Layout();
	bSizer4->Fit( m_panelwebraster );
	m_notebook2->AddPage( m_panelwebraster, _("Web raster"), false );
    
    
	bSizer5->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	wxStdDialogButtonSizer* m_sdbSizer2;
	wxButton* m_sdbSizer2OK;
	wxButton* m_sdbSizer2Cancel;
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();
	bSizer5->Add( m_sdbSizer2, 0, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer5 );
	this->Layout();
	bSizer5->Fit( this );
	
	this->Centre( wxBOTH );
}


bool PreferenceDLG::TransferDataToWindow(){
	wxConfigBase * myConfig =  wxConfigBase::Get(false);
    wxASSERT(myConfig);
    myConfig->SetPath("UPDATE");
	bool bCheckStartup = myConfig->ReadBool("check_on_start", true);
    wxString myProxyInfo = myConfig->Read("proxy_info", wxEmptyString);
    myConfig->SetPath("..");
	
	myConfig->SetPath("GENERAL");
	wxString mySelColorText = myConfig->Read("selection_color", wxEmptyString);
	bool mySelHalo = myConfig->ReadBool("selection_halo", false);
    bool myRelPath = myConfig->ReadBool("relative_path", true);
    
    bool isUsingRam = myConfig->ReadBool("using_ram", true);
    long myInternetWaitTime = myConfig->ReadLong("internet_wait_time", 250);
    myConfig->SetPath("..");
	
	wxColour mySelColor = *wxRED;
	if (mySelColorText != wxEmptyString) {
		mySelColor.Set(mySelColorText);
	}
	
    myConfig->SetPath("SPATIAL_INDEX");
	bool bCreateIndex = myConfig->ReadBool("create_index", true);
    myConfig->SetPath("..");
    
    m_BuildOverviewCtrl->SetValue(bCreateIndex);
    
	m_SelColourCtrl->SetColour(mySelColor);
	m_SelOutlineCtrl->SetValue(mySelHalo);
    m_RelPathCtrl->SetValue(myRelPath);
    
    m_WaitingTimeCtrl->SetValue((int) myInternetWaitTime);
    m_RamCtrl->SetValue(isUsingRam);
    m_MemoryCtrl->SetValue(!isUsingRam);
	
	m_UpdateCheckCtrl->SetValue(bCheckStartup);
	m_ProxyInfoCtrl->SetValue(myProxyInfo);
	return true;
}



bool PreferenceDLG::TransferDataFromWindow(){
	wxConfigBase * myConfig =  wxConfigBase::Get(false);
    wxASSERT(myConfig);
    myConfig->SetPath("UPDATE");
	myConfig->Write("check_on_start", m_UpdateCheckCtrl->GetValue());
    myConfig->Write("proxy_info", m_ProxyInfoCtrl->GetValue());
    myConfig->SetPath("..");
	
	myConfig->SetPath("GENERAL");
	myConfig->Write("selection_color", m_SelColourCtrl->GetColour().GetAsString());
	myConfig->Write("selection_halo", m_SelOutlineCtrl->GetValue());
    myConfig->Write("relative_path", m_RelPathCtrl->GetValue());
    
    myConfig->Write("using_ram", m_RamCtrl->GetValue());
    myConfig->Write("internet_wait_time", m_WaitingTimeCtrl->GetValue());
    myConfig->SetPath("..");
    
    myConfig->SetPath("SPATIAL_INDEX");
	myConfig->Write("create_index", m_BuildOverviewCtrl->GetValue());
    myConfig->SetPath("..");
	return true;
}
