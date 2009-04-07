/***************************************************************************
								information_dlg.h
				Display informations dialog for layers and features
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

// comment doxygen

#include "information_dlg.h"


InformationDLG::InformationDLG( wxWindow* parent, wxWindowID id,
								 const wxString& title, const wxPoint& pos,
								 const wxSize& size, long style ) : 
								wxDialog( parent, id, title, pos, size, style )
{
	InitMembersValue();
	CreateControls();
	
	
}




InformationDLG::~InformationDLG()
{
	
}



void InformationDLG::InitMembersValue()
{
	m_notebook = NULL;
	m_LayerInfo = NULL;
	m_CloseBtn = NULL;
}



void InformationDLG::CreateControls()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook = new wxNotebook( this, wxID_ANY, 
								wxDefaultPosition, wxDefaultSize, 0 );
	wxPanel* m_panel4;
	m_panel4 = new wxPanel( m_notebook, wxID_ANY, 
						   wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	m_LayerInfo = new wxHtmlWindow( m_panel4, wxID_ANY, wxDefaultPosition,
								   wxSize( 350,250 ), wxHW_SCROLLBAR_AUTO );
	//m_LayerInfo->SetMinSize( wxSize( 150,100 ) );
	
	bSizer23->Add( m_LayerInfo, 1, wxEXPAND, 5 );
	
	m_panel4->SetSizer( bSizer23 );
	m_panel4->Layout();
	bSizer23->Fit( m_panel4 );
	m_notebook->AddPage( m_panel4, _("Layer"), false );
	wxPanel* m_panel5;
	m_panel5 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition,
						   wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook->AddPage( m_panel5, _("Feature"), false );
	
	bSizer22->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
	m_CloseBtn = new wxButton( this, wxID_CANCEL, _("&Close"),
							  wxDefaultPosition, wxDefaultSize, 0 );
	bSizer22->Add( m_CloseBtn, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer22 );
	this->Layout();
	bSizer22->Fit( this );
	
	this->Center(wxBOTH);
	

}



/***************************************************************************//**
 @brief Set Layer metadata
 @param metadata HTML string containing metadata
 @author Lucien Schreiber (c) CREALP 2009
 @date 07 April 2009
 *******************************************************************************/
void InformationDLG::SetMetaData (const wxString & metadata)
{
	m_LayerInfo->SetPage(metadata);
}
