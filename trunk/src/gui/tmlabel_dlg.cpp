/***************************************************************************
 tmlabel_dlg
 -------------------
 copyright            : (C) 2014 CREALP Lucien Schreiber
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

#include "tmlabel_dlg.h"
#include "../gis/tmlayerproperties.h"
#include "../gis/tmgisdatavectorshp.h"

tmLabelDLG::tmLabelDLG( wxWindow* parent, tmLayerProperties * properties, const wxPoint& pos, wxWindowID id, const wxString& title, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style ){
    
    m_LayerProperties = properties;
    m_GisData = (tmGISDataVectorSHP*) tmGISData::LoadLayer(m_LayerProperties);
    
    _CreateControls();
	
	// Connect Events
	m_AddFieldBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmLabelDLG::OnAddField ), NULL, this );
	m_AddTextBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmLabelDLG::OnAddText ), NULL, this );
	m_ClearBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmLabelDLG::OnClear ), NULL, this );
}



tmLabelDLG::~tmLabelDLG(){
	// Disconnect Events
	m_AddFieldBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmLabelDLG::OnAddField ), NULL, this );
	m_AddTextBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmLabelDLG::OnAddText ), NULL, this );
	m_ClearBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tmLabelDLG::OnClear ), NULL, this );
	
}


void tmLabelDLG::_CreateControls(){
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Definition") ), wxVERTICAL );
	
    m_CheckActiveLabelCtrl = new wxCheckBox( this, wxID_ANY, _("Enable labels for this layer"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer9->Add( m_CheckActiveLabelCtrl, 0, wxALL, 5 );
    
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer6->AddGrowableCol( 1 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText16;
	m_staticText16 = new wxStaticText( this, wxID_ANY, _("Field"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer6->Add( m_staticText16, 0, wxALL, 5 );
	
	wxArrayString myFieldsName;
    wxASSERT(m_GisData);
    m_GisData->GetFieldsName(myFieldsName);

	m_FieldCtrl = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), myFieldsName, 0 );
	m_FieldCtrl->SetSelection( 0 );
	//m_FieldCtrl->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer6->Add( m_FieldCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_AddFieldBtn = new wxButton( this, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_AddFieldBtn, 0, wxALL, 5 );
	
	wxStaticText* m_staticText17;
	m_staticText17 = new wxStaticText( this, wxID_ANY, _("Text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	fgSizer6->Add( m_staticText17, 0, wxALL, 5 );
	
	m_TextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_TextCtrl, 0, wxALL|wxEXPAND, 5 );
	
	m_AddTextBtn = new wxButton( this, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_AddTextBtn, 0, wxALL, 5 );
	
	
	sbSizer9->Add( fgSizer6, 0, wxEXPAND, 5 );
	
	m_ClearBtn = new wxButton( this, wxID_ANY, _("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer9->Add( m_ClearBtn, 0, wxALL, 5 );
	
	
	bSizer14->Add( sbSizer9, 0, wxEXPAND|wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Preview") ), wxVERTICAL );
	
	m_PreviewCtrl = new wxHtmlWindow( this, wxID_ANY, wxDefaultPosition, wxSize (-1, 100), wxHW_SCROLLBAR_AUTO );
	//m_PreviewCtrl->SetMinSize( wxSize( -1,100 ) );
	
	sbSizer8->Add( m_PreviewCtrl, 1, wxEXPAND, 5 );
	
	
	bSizer14->Add( sbSizer8, 1, wxEXPAND|wxALL, 5 );
	
	wxStdDialogButtonSizer* m_sdbSizer3;
	wxButton* m_sdbSizer3OK;
	wxButton* m_sdbSizer3Cancel;
	m_sdbSizer3 = new wxStdDialogButtonSizer();
	m_sdbSizer3OK = new wxButton( this, wxID_OK );
	m_sdbSizer3->AddButton( m_sdbSizer3OK );
	m_sdbSizer3Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer3->AddButton( m_sdbSizer3Cancel );
	m_sdbSizer3->Realize();
	
	bSizer14->Add( m_sdbSizer3, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer14 );
	this->Layout();
	bSizer14->Fit( this );
	
	this->Centre( wxBOTH );
}


