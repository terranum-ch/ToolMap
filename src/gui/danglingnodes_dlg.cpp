/***************************************************************************
							danglingnodes_dlg.cpp
						Display Dangling nodes Dialog
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

#include "danglingnodes_dlg.h"



DanglingNode_DLG::DanglingNode_DLG( wxWindow* parent, wxWindowID id,
								   const wxString& title, const wxPoint& pos,
								   const wxSize& size, long style ) : 
wxDialog( parent, id, title, pos, size, style )
{
	CreateControls();
}



DanglingNode_DLG::~DanglingNode_DLG()
{
}



void DanglingNode_DLG::CreateControls()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText6;
	m_staticText6 = new wxStaticText( this, wxID_ANY,
									 _("Search dangling nodes on:"),
									 wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer24->Add( m_staticText6, 0, wxTOP|wxRIGHT|wxLEFT, 10 );
	
	wxArrayString m_LayerChoiceChoices;
	m_LayerChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 300,-1 ), m_LayerChoiceChoices, 0 );
	m_LayerChoice->SetSelection( 0 );
	bSizer24->Add( m_LayerChoice, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer24->Add( 0, 20, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	wxButton* m_button21;
	m_button21 = new wxButton( this, ID_DLGDN_CLEAR, _("Clear"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	bSizer25->Add( m_button21, 2, wxALL, 5 );
	
	wxButton* m_button22;
	m_button22 = new wxButton( this, ID_DLGDN_SEARCH, _("Search"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button22->SetDefault(); 
	bSizer25->Add( m_button22, 1, wxALL, 5 );
	
	bSizer24->Add( bSizer25, 0, wxALIGN_RIGHT|wxTOP|wxBOTTOM, 5 );
	
	this->SetSizer( bSizer24 );
	this->Layout();
	bSizer24->Fit( this );

}