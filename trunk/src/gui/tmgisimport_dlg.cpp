/***************************************************************************
								tmgisimport_dlg.cpp
                    Dialog for importing gis data into project
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

#include "tmgisimport_dlg.h"



tmGISImport_DLG::tmGISImport_DLG( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	CreateCtrls(parent);
}

tmGISImport_DLG::~tmGISImport_DLG()
{
}

void tmGISImport_DLG::CreateCtrls (wxWindow * parent)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxVERTICAL );
	
	bSizer32->SetMinSize( wxSize( 500,-1 ) ); 
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText13;
	m_staticText13 = new wxStaticText( this, wxID_ANY, _("File Name :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer33->Add( m_staticText13, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxFilePickerCtrl* m_filePicker1;
	m_filePicker1 = new wxFilePickerCtrl( this, ID_DLGIG_IMPORT_BTN, wxEmptyString, _("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	bSizer33->Add( m_filePicker1, 1, wxALL, 5 );
	
	bSizer32->Add( bSizer33, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* staticsizer;
	staticsizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("File informations") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_Type_Label;
	m_Type_Label = new wxStaticText( this, wxID_ANY, _("Geometry type :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Type_Label->Wrap( -1 );
	fgSizer2->Add( m_Type_Label, 0, wxALL, 5 );
	
	m_Type_Value = new wxStaticText( this, wxID_ANY, _("Unable to acces file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Type_Value->Wrap( -1 );
	//m_Type_Value->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_Type_Value->SetForegroundColour(*wxRED);
	
	fgSizer2->Add( m_Type_Value, 0, wxALL, 5 );
	
	wxStaticText* m_FeatureCount_Label;
	m_FeatureCount_Label = new wxStaticText( this, wxID_ANY, _("Number of features :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_FeatureCount_Label->Wrap( -1 );
	fgSizer2->Add( m_FeatureCount_Label, 0, wxALL, 5 );
	
	m_FeatureCount_Type = new wxStaticText( this, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_FeatureCount_Type->Wrap( -1 );
	fgSizer2->Add( m_FeatureCount_Type, 0, wxALL, 5 );
	
	staticsizer->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	bSizer32->Add( staticsizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText18;
	m_staticText18 = new wxStaticText( this, wxID_ANY, _("Import into :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer34->Add( m_staticText18, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_ImportChoiceChoices;
	m_ImportChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_ImportChoiceChoices, 0 );
	m_ImportChoice->SetSelection( 0 );
	bSizer34->Add( m_ImportChoice, 1, wxALL|wxEXPAND, 5 );
	
	bSizer32->Add( bSizer34, 0, wxEXPAND, 5 );
	
	wxStaticLine* m_staticline4;
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer32->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxHORIZONTAL );
	
	wxButton* m_button28;
	m_button28 = new wxButton( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer35->Add( m_button28, 0, wxALL, 5 );
	
	m_Btn_Import = new wxButton( this, wxID_OK, _("&Import"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Btn_Import->SetDefault(); 
	bSizer35->Add( m_Btn_Import, 0, wxALL, 5 );
	
	bSizer32->Add( bSizer35, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer32 );
	this->Layout();
	bSizer32->Fit( this );
}

