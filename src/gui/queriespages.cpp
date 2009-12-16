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
	
	m_PageSelectionAttribut = new QueriesPageAttribut(m_Parent, NULL, NULL);
	m_PageExpertSQL = new QueriesPageSQL(m_Parent);
	
	m_PageName = new QueriesPageName(m_Parent, this, NULL);
	m_PageLayer = new QueriesPageLayer(m_Parent, this,m_PageName);
	m_PageGeneric = new QueriesPageGeneric(m_Parent,this,m_PageName);
	m_PageSelection = new QueriesPageSelection(m_Parent,this,m_PageSelectionAttribut);
	m_PageExpert = new QueriesPageExpert(m_Parent,this,m_PageExpertSQL);
	
}



QueriesPageIntro::~QueriesPageIntro() {
	delete m_PageLayer;
	delete m_PageName;
	delete m_PageGeneric;
	delete m_PageSelection;
	delete m_PageExpert;
	
	delete m_PageSelectionAttribut;
	delete m_PageExpertSQL;
	
}



wxWizardPage* QueriesPageIntro::GetPrev() const {
	return NULL;
}



wxWizardPage* QueriesPageIntro::GetNext() const {
	switch (m_radiobtn->GetSelection())
	{
		case QUERY_LAYERS:
			m_PageName->SetPrev(m_PageLayer);
			return m_PageLayer;
			break;
			
			case QUERY_GENERIC:
			m_PageName->SetPrev(m_PageGeneric);
			return m_PageGeneric;
			break;
			
			case QUERY_SELECTED:
			m_PageName->SetPrev(m_PageSelectionAttribut);
			wxWizardPageSimple::Chain(m_PageSelection, m_PageSelectionAttribut);
			wxWizardPageSimple::Chain(m_PageSelectionAttribut, m_PageName);
			return m_PageSelection;
			break;
			
			case QUERY_SQL:
			m_PageName->SetPrev(m_PageExpertSQL);
			wxWizardPageSimple::Chain(m_PageExpert, m_PageExpertSQL);
			wxWizardPageSimple::Chain(m_PageExpertSQL, m_PageName);
			return m_PageExpert;
			break;

		
	}
	return m_PageName;
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
	//m_radiobtn->SetSelection(m_Parent->m_QueryType);
	
	//if (m_Parent->m_QueryObjectGeomID == wxNOT_FOUND){
	//	m_radiobtn->Enable(QUERY_SELECTED, false);
	//}
	return true;
}



bool QueriesPageIntro::TransferDataFromWindow() {
	//m_Parent->m_QueryType = (tmQUERIES_TYPE) m_radiobtn->GetSelection();
	return true;
}






QueriesPageLayer::QueriesPageLayer(QueriesWizard * parent, 
								   wxWizardPage * prev,
								   wxWizardPage * next) : 
wxWizardPageSimple(parent, prev, next)
{
	m_Parent = parent;
	wxASSERT(next);
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





void QueriesPageSelection::_CreateControls() {
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText7;
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Add a query for selecting all objects\nhaving the following value"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer10->Add( m_staticText7, 0, wxALL, 5 );
	
	wxArrayString m_SelTypeListChoices;
	m_SelTypeList = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_SelTypeListChoices, 0 );
	m_SelTypeList->SetSelection( 0 );
	bSizer10->Add( m_SelTypeList, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALL, 15 );
	
	this->SetSizer( bSizer10 );
	bSizer10->Fit(this);
}



QueriesPageSelection::QueriesPageSelection(QueriesWizard * parent, wxWizardPage * prev,
										   wxWizardPageSimple * next) :
wxWizardPageSimple(parent,prev,next){
	_CreateControls();
	m_Parent = parent;
}



QueriesPageSelection::~QueriesPageSelection() {
}



bool QueriesPageSelection::TransferDataToWindow() {
	return true;
}



bool QueriesPageSelection::TransferDataFromWindow() {
	return true;
}




void QueriesPageAttribut::_CreateControls() {
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_CheckAdvAttrib = new wxCheckBox( this, wxID_ANY, wxT("Use advanced attribution"), wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer12->Add( m_CheckAdvAttrib, 0, wxALL, 5 );
	
	m_AdvSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Advanced attributs") ), wxVERTICAL );
	
	m_AdvText = new wxStaticText( this, wxID_ANY, wxT("Delete any unneeded attribute"), wxDefaultPosition, wxDefaultSize, 0 );
	m_AdvText->Wrap( -1 );
	m_AdvSizer->Add( m_AdvText, 0, wxALL, 5 );
	
	m_AdvAttributs = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_AdvSizer->Add( m_AdvAttributs, 1, wxALL|wxEXPAND, 5 );
	
	bSizer12->Add( m_AdvSizer, 1, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer12 );
	bSizer12->Fit(this);
}

QueriesPageAttribut::QueriesPageAttribut(QueriesWizard * parent, wxWizardPageSimple * prev,
										 wxWizardPageSimple * next) :
wxWizardPageSimple(parent, prev, next){
	m_Parent = parent;
	_CreateControls();
}

QueriesPageAttribut::~QueriesPageAttribut() {
}

bool QueriesPageAttribut::TransferDataToWindow() {
	return true;
}

bool QueriesPageAttribut::TransferDataFromWindow() {
	return true;
}





void QueriesPageGeneric::_CreateControls() {
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Choose the generic query you want to add"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxALL, 5 );
	
	m_ListGeneric = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE ); 
	bSizer3->Add( m_ListGeneric, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer3 );
	bSizer3->Fit(this);
}

QueriesPageGeneric::QueriesPageGeneric(QueriesWizard * parent, wxWizardPage * prev, wxWizardPage * next):
wxWizardPageSimple(parent, prev, next){
	_CreateControls();
}

QueriesPageGeneric::~QueriesPageGeneric() {
}

bool QueriesPageGeneric::TransferDataToWindow() {
	return true;
}

bool QueriesPageGeneric::TransferDataFromWindow() {
	return true;
}






void QueriesPageExpert::_CreateControls() {
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText8;
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Choose on which build-up layer the query\nwill apply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer11->Add( m_staticText8, 0, wxALL, 5 );
	
	wxString m_LayerTypeChoices[] = { wxT("Lines"), wxT("Points"), wxT("Labels"), wxT("Notes") };
	int m_LayerTypeNChoices = sizeof( m_LayerTypeChoices ) / sizeof( wxString );
	m_LayerType = new wxRadioBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), m_LayerTypeNChoices, m_LayerTypeChoices, 1, wxRA_SPECIFY_COLS );
	m_LayerType->SetSelection( 0 );
	bSizer11->Add( m_LayerType, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( bSizer11 );
	bSizer11->Fit(this);
}


QueriesPageExpert::QueriesPageExpert(QueriesWizard * parent, wxWizardPage * prev,
									 wxWizardPageSimple * next) :
wxWizardPageSimple(parent, prev, next){
	_CreateControls();
	m_Parent = parent;
}

QueriesPageExpert::~QueriesPageExpert() {
}

bool QueriesPageExpert::TransferDataToWindow() {
	return true;
}

bool QueriesPageExpert::TransferDataFromWindow() {
	return true;
}




void QueriesPageSQL::_CreateControls() {
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText8;
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("SQL code for the query"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer9->Add( m_staticText8, 0, wxALL, 5 );
	
	m_SQLText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer9->Add( m_SQLText, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer9 );
	bSizer9->Fit(this);
}

QueriesPageSQL::QueriesPageSQL(QueriesWizard * parent, wxWizardPageSimple * prev,
							   wxWizardPageSimple * next) :
wxWizardPageSimple(parent, prev, next){
	m_Parent = parent;
	_CreateControls();
}

QueriesPageSQL::~QueriesPageSQL() {
}

bool QueriesPageSQL::TransferDataToWindow() {
	return true;
}

bool QueriesPageSQL::TransferDataFromWindow() {
	return true;
}



QueriesPageName::QueriesPageName(QueriesWizard * parent, wxWizardPage * prev, wxWizardPage * next) :
wxWizardPageSimple(parent, prev, next){
	_CreateControls();
}

QueriesPageName::~QueriesPageName() {
}

void QueriesPageName::_CreateControls() {
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText3;
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Name of the query"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer4->Add( m_staticText3, 0, wxALL, 5 );
	
	m_TextName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_TextName->SetMaxLength( 200 ); 
	bSizer4->Add( m_TextName, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer4 );
	bSizer4->Fit(this);
}


bool QueriesPageName::TransferDataToWindow() {
	return true;
}

bool QueriesPageName::TransferDataFromWindow() {
	return true;
}



