/***************************************************************************
								aattribbatch_dlg.cpp
						Display dialog for batch advanced attribution
                             -------------------
    copyright            : (C) 2010 CREALP Lucien Schreiber 
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

#include "aattribbatch_dlg.h"

///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE( AAttribBatch_DLG, wxDialog )
	EVT_LISTBOX( ID_DLGAATB_LIST_TYPE, AAttribBatch_DLG::OnTypeChange )
	EVT_LISTBOX( ID_DLGAATB_LIST_FIELDS, AAttribBatch_DLG::OnFieldsChange )
	EVT_BUTTON( wxID_APPLY, AAttribBatch_DLG::OnApply )
	EVT_BUTTON( wxID_SAVE, AAttribBatch_DLG::OnSave )
END_EVENT_TABLE()



AAttribBatch_DLG::AAttribBatch_DLG( wxWindow* parent, wxWindowID id, const wxString& title,
								   const wxPoint& pos, const wxSize& size, long style ) :
wxDialog( parent, id, title, pos, size, style ){
	this->SetSizeHints( wxSize( 360,200 ), wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText1;
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer4->Add( m_staticText1, 0, wxALL, 5 );
	
	m_ListType = new wxListBox( this, ID_DLGAATB_LIST_TYPE, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer4->Add( m_ListType, 1, wxALL|wxEXPAND, 5 );
	
	bSizer2->Add( bSizer4, 1, wxEXPAND, 5 );
	
	wxStaticLine* m_staticline1;
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer2->Add( m_staticline1, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* statictext;
	statictext = new wxStaticText( this, wxID_ANY, wxT("Fields:"), wxDefaultPosition, wxDefaultSize, 0 );
	statictext->Wrap( -1 );
	bSizer3->Add( statictext, 0, wxALL, 5 );
	
	m_ListFields = new wxListBox( this, ID_DLGAATB_LIST_FIELDS, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer3->Add( m_ListFields, 1, wxALL|wxEXPAND, 5 );
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	wxStaticLine* m_staticline3;
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer2->Add( m_staticline3, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText4;
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Set Value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer6->Add( m_staticText4, 0, wxALL, 5 );
	
	m_PanelValue = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText5;
	m_staticText5 = new wxStaticText( m_PanelValue, wxID_ANY, wxT("Hint: Select an object type then a field to be able to set value to selected objects "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( 100 );
	bSizer7->Add( m_staticText5, 0, wxALL, 5 );
	
	m_PanelValue->SetSizer( bSizer7 );
	m_PanelValue->Layout();
	bSizer7->Fit( m_PanelValue );
	bSizer6->Add( m_PanelValue, 1, wxEXPAND | wxALL, 5 );
	
	bSizer2->Add( bSizer6, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	m_Btn = new wxStdDialogButtonSizer();
	m_BtnSave = new wxButton( this, wxID_SAVE );
	m_Btn->AddButton( m_BtnSave );
	m_BtnApply = new wxButton( this, wxID_APPLY );
	m_Btn->AddButton( m_BtnApply );
	m_BtnCancel = new wxButton( this, wxID_CANCEL );
	m_Btn->AddButton( m_BtnCancel );
	m_Btn->Realize();
	bSizer1->Add( m_Btn, 0, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

AAttribBatch_DLG::~AAttribBatch_DLG()
{
}
