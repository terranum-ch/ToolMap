/***************************************************************************
								queriespages.cpp
                    Pages definition for the QueryWizard
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

#include "queriespages.h"



QueriesPageIntro::QueriesPageIntro(QueriesWizard * parent) : wxWizardPage(parent) {
	m_Parent = parent;
	CreateControls();
	
	m_PageLayer = new QueriesPageLayer(m_Parent, this, NULL);

	
}



QueriesPageIntro::~QueriesPageIntro() {
}



wxWizardPage* QueriesPageIntro::GetPrev() const {
	return NULL;
}



wxWizardPage* QueriesPageIntro::GetNext() const {
	return m_PageLayer;
}



void QueriesPageIntro::CreateControls() {
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText1;
	m_staticText1 = new wxStaticText( this, wxID_ANY, 
									 _("Choose which type of query you want to add"),
									 wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer1->Add( m_staticText1, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxString m_radiobtnChoices[] = { 
		_("Layers query"), 
		_("From selected feature"), 
		_("Generic query"), 
		_("Hand made using SQL (expert)") };
	int m_radiobtnNChoices = sizeof( m_radiobtnChoices ) / sizeof( wxString );
	m_radiobtn = new wxRadioBox( this, wxID_ANY,
								wxEmptyString, wxDefaultPosition,
								wxDefaultSize, m_radiobtnNChoices,
								m_radiobtnChoices, 1, wxRA_SPECIFY_COLS );
	bSizer2->Add( m_radiobtn, 0, wxALIGN_CENTER|wxALL, 5);
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 10 );
	
	SetSizer(bSizer1);
	bSizer1->Fit(this);
		
}



bool QueriesPageIntro::TransferDataToWindow() {
	m_radiobtn->SetSelection(m_Parent->m_QueryType);
	
	if (m_Parent->m_QueryObjectGeomID == wxNOT_FOUND){
		m_radiobtn->Enable(QUERY_SELECTED, false);
	}
	return true;
}



bool QueriesPageIntro::TransferDataFromWindow() {
	m_Parent->m_QueryType = (tmQUERIES_TYPE) m_radiobtn->GetSelection();
	return true;
}






QueriesPageLayer::QueriesPageLayer(QueriesWizard * parent, 
								   wxWizardPage * prev,
								   wxWizardPage * next) : 
wxWizardPageSimple(parent, prev, next)
{
	m_Parent = parent;
	CreateControls();
}



QueriesPageLayer::~QueriesPageLayer() {
}



bool QueriesPageLayer::TransferDataToWindow() {
	return true;
}



bool QueriesPageLayer::TransferDataFromWindow() {
	return true;
}



void QueriesPageLayer::CreateControls() {
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, 
									 wxT("Add a query for selecting all objects belonging\n")
									 wxT("to the following layer"),
									 wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxALL, 5 );
	
	m_ListLayers = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE ); 
	bSizer3->Add( m_ListLayers, 1, wxALL|wxEXPAND, 5 );
	
	SetSizer(bSizer3);
	bSizer3->Fit(this);
}




