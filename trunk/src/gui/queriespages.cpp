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




QueriesPageIntro::QueriesPageIntro(QueriesWizard * parent, DataBaseTM * database) : wxWizardPage(parent) {
	wxASSERT(database);
	m_Parent = parent;
	m_pDB = database;
	CreateControls();
	
	m_PageSelectionAttribut = new QueriesPageAttribut(m_Parent, m_pDB, NULL, NULL);
	m_PageExpertSQL = new QueriesPageSQL(m_Parent);
	
	m_PageName = new QueriesPageName(m_Parent, this, NULL);
	m_PageLayer = new QueriesPageLayer(m_Parent,m_pDB, this,m_PageName);
	m_PageGeneric = new QueriesPageGeneric(m_Parent,this,m_PageName);
	m_PageSelection = new QueriesPageSelection(m_Parent, m_pDB, this, m_PageSelectionAttribut);
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
	m_radiobtn->SetSelection(m_Parent->GetData()->m_QueryType);
	
	if (m_Parent->GetData()->m_QueryObjectGeomID == wxNOT_FOUND){
		m_radiobtn->Enable(QUERY_SELECTED, false);
	}
	return true;
}



bool QueriesPageIntro::TransferDataFromWindow() {
	m_Parent->GetData()->m_QueryType = 
	(tmQUERIES_TYPE) m_radiobtn->GetSelection();
	return true;
}






QueriesPageLayer::QueriesPageLayer(QueriesWizard * parent, 
								   DataBaseTM * database,
								   wxWizardPage * prev,
								   wxWizardPage * next) : 
wxWizardPageSimple(parent, prev, next)
{
	wxASSERT(database);
	m_pDB = database;
	m_Parent = parent;
	wxASSERT(next);
	CreateControls();
}



QueriesPageLayer::~QueriesPageLayer() {
}



bool QueriesPageLayer::TransferDataToWindow() {
	
	if (m_ListLayers->IsEmpty()) {
		m_Layers.Clear();
		if(m_Parent->GetData()->GetLayers(m_pDB, m_Layers)==true)
		{
			m_ListLayers->Freeze();
			for (unsigned int i = 0; i<m_Layers.GetCount(); i++) {
				m_ListLayers->Append(m_Layers.Item(i).m_LayerName);
			}	
			m_ListLayers->SetSelection(0);
			m_ListLayers->Thaw();
		}
		else{
			wxLogError(_T("Unable to get the layers list"));
		}
	}

	m_ListLayers->SetFocus();
	return true;
}



bool QueriesPageLayer::TransferDataFromWindow() {
	int iNum = m_ListLayers->GetSelection();
	if (m_Layers.GetCount() > 0) {
		
		m_Parent->GetData()->m_QueryLayerID =  m_Layers.Item(iNum).m_LayerID;
		m_Parent->GetData()->m_QueryLayerType = (TOC_GENERIC_NAME) m_Layers.Item(iNum).m_LayerType;
	}
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



QueriesPageSelection::QueriesPageSelection(QueriesWizard * parent,
										   DataBaseTM * database,
										   wxWizardPage * prev,
										   wxWizardPageSimple * next) :
wxWizardPageSimple(parent,prev,next){
	wxASSERT(database);
	_CreateControls();
	m_Parent = parent;
	m_pDB = database;
	
}



QueriesPageSelection::~QueriesPageSelection() {
}



bool QueriesPageSelection::TransferDataToWindow() {
	
	if (m_SelTypeList->IsEmpty()) {
		if(m_Parent->GetData()->GetTypes(m_pDB, m_Types)){
			m_SelTypeList->Freeze();
			for (unsigned int i = 0; i< m_Types.GetCount(); i++) {
				m_SelTypeList->Append(m_Types.Item(i).m_ObjectName);
			}
			m_SelTypeList->SetSelection(0);
			m_SelTypeList->Thaw();
		}
	}
	m_SelTypeList->SetFocus();
	return true;
}



bool QueriesPageSelection::TransferDataFromWindow() {
	unsigned int mySel = m_SelTypeList->GetSelection();
	
	if (m_Types.GetCount() > 0) {
		wxASSERT(mySel < m_Types.GetCount());
		
		m_Parent->GetData()->m_QueryObjectID = m_Types.Item(mySel).m_ObjectID;
	}
	
	return true;
}




void QueriesPageAttribut::_CreateControls() {
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_CheckAdvAttrib = new wxCheckBox( this, wxID_ANY, _("Use advanced attribution"), wxDefaultPosition, wxDefaultSize, 0 );
	m_CheckAdvAttrib->SetValue(true);
	
	bSizer12->Add( m_CheckAdvAttrib, 0, wxALL, 5 );
	
	m_AdvSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Advanced attributs") ), wxVERTICAL );
	
	m_AdvText = new wxStaticText( this, wxID_ANY, _("Delete any unneeded attribute"), wxDefaultPosition, wxDefaultSize, 0 );
	m_AdvText->Wrap( -1 );
	m_AdvSizer->Add( m_AdvText, 0, wxALL, 5 );
	
	m_AdvAttributs = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_AdvSizer->Add( m_AdvAttributs, 1, wxALL|wxEXPAND, 5 );
	
	m_ReloadButton = new wxFlatButton( this, wxID_ANY, _("Reload attributs"), wxDefaultSize );
	m_AdvSizer->Add( m_ReloadButton, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	bSizer12->Add( m_AdvSizer, 1, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer12 );
	bSizer12->Fit(this);
		
}


void QueriesPageAttribut::_EnableCtrls(bool enable){
	m_AdvText->Enable(enable);
	m_AdvAttributs->Enable(enable);
	m_ReloadButton->Enable(enable);
}


void QueriesPageAttribut::OnReloadAttributs(wxCommandEvent & event ){
	_LoadAttributs();
	event.Skip();
}


void QueriesPageAttribut::OnUseAdvancedAttributs(wxCommandEvent& event){
	_EnableCtrls(event.IsChecked());
	event.Skip();
}

void QueriesPageAttribut::_LoadAttributs() {
	wxASSERT(m_Parent);
	wxASSERT(m_pDB);
	
	m_AdvAttributs->Freeze();
	m_AdvAttributs->Clear();
	
	long myLayerID = wxNOT_FOUND;
	
	if(m_Parent->GetData()->GetParentLayer(m_pDB, myLayerID)==false){
		m_AdvAttributs->Thaw();
		return;
	}
	
	if (m_Parent->GetData()->GetFieldsValues(m_pDB, myLayerID, 
											 m_Parent->GetData()->m_QueryFields,
											 m_Parent->GetData()->m_QueryFieldsValues)==false) 
	{
		m_AdvAttributs->Thaw();
		return;
	}
	
	// adding to list
	QueriesData * myData = m_Parent->GetData();
	wxASSERT(myData);
	wxString myText = wxEmptyString;
	
	for (unsigned int i = 0; i<myData->m_QueryFields.GetCount() ; i++){
		myText.Clear();
		myText.Append(myData->m_QueryFields.Item(i).m_Fieldname);
		myText.Append(_T(" = "));
		if (myData->m_QueryFieldsValues.Item(i) == wxEmptyString) {
			myText.Append(_("Empty"));
		}
		else {
			myText.Append(myData->m_QueryFieldsValues.Item(i));
		}
		
		m_AdvAttributs->Append(myText);
		
	}
	
	m_AdvAttributs->Thaw();
}



void QueriesPageAttribut::OnDeleteAttribut( wxKeyEvent& event ){ 
	if (event.GetKeyCode() == WXK_BACK || event.GetKeyCode() == WXK_DELETE)
	{
		if (m_AdvAttributs->GetSelection() == wxNOT_FOUND) {
			event.Skip();
			return;
		}
		
		if (m_Parent->GetData()->DeleteFieldsValue(m_AdvAttributs->GetSelection())==false){
			event.Skip();
			return;
		}
		
		m_AdvAttributs->Delete(m_AdvAttributs->GetSelection());
		
		if (m_AdvAttributs->GetCount() == 0) {
			m_CheckAdvAttrib->SetValue(false);
		}
		
	}
	event.Skip(); 
}


QueriesPageAttribut::QueriesPageAttribut(QueriesWizard * parent, DataBaseTM * database,
										 wxWizardPageSimple * prev, wxWizardPageSimple * next):
wxWizardPageSimple(parent, prev, next){
	m_Parent = parent;
	m_pDB = database;
	
	_CreateControls();
	
	// Connect Events
	m_AdvAttributs->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( QueriesPageAttribut::OnDeleteAttribut ),
							NULL, this );
	m_CheckAdvAttrib->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED,
							  wxCommandEventHandler( QueriesPageAttribut::OnUseAdvancedAttributs ),
							  NULL, this );
	m_ReloadButton->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
							wxCommandEventHandler( QueriesPageAttribut::OnReloadAttributs ),
							NULL, this );

}



QueriesPageAttribut::~QueriesPageAttribut() {
	// Disconnect Events
	m_AdvAttributs->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( QueriesPageAttribut::OnDeleteAttribut ),
							   NULL, this );
	m_CheckAdvAttrib->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED,
								 wxCommandEventHandler( QueriesPageAttribut::OnUseAdvancedAttributs ),
								 NULL, this );
	m_ReloadButton->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
							   wxCommandEventHandler( QueriesPageAttribut::OnReloadAttributs),
							   NULL, this );
}



bool QueriesPageAttribut::TransferDataToWindow() {
	// add fields in every case
	_LoadAttributs();
	if (m_AdvAttributs->IsEmpty() || m_Parent->GetData()->HasFieldsValues() == false){
		_EnableCtrls(false);
		m_CheckAdvAttrib->SetValue(false);
		m_CheckAdvAttrib->Enable(false);
	}
	return true;
}



bool QueriesPageAttribut::TransferDataFromWindow() {
	m_Parent->GetData()->m_QueryUseFields = m_CheckAdvAttrib->IsChecked();
	return true;
}




void QueriesPageGenericData::_Add(int layertarger, const wxString & description, const wxString & sql) {
	m_TargetLayer.Add(layertarger);
	m_Description.Add(description);
	m_SQL.Add(sql);
}


QueriesPageGenericData::QueriesPageGenericData() {
	_Add(TOC_NAME_LINES, _("Line(s) without attributions"),
		 _T("SELECT l.OBJECT_ID FROM generic_lines l WHERE l.OBJECT_ID NOT IN (SELECT generic_aat.OBJECT_GEOM_ID FROM generic_aat)"));
	_Add(TOC_NAME_LINES, _("Line(s) with more than one attribut"),
		 _T("SELECT l.OBJECT_ID FROM generic_lines l WHERE OBJECT_ID IN (SELECT OBJECT_GEOM_ID  FROM generic_aat GROUP BY OBJECT_GEOM_ID HAVING COUNT(*) > 1)"));
}

QueriesPageGenericData::~QueriesPageGenericData() {
}

void QueriesPageGenericData::GetData(int index, int & layertarget, wxString & description, wxString & sql) {
	layertarget = m_TargetLayer.Item(index);
	description = m_Description.Item(index);
	sql = m_SQL.Item(index);
}

wxArrayString QueriesPageGenericData::GetDescription(){
	return m_Description;
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
	m_GenericData = NULL;
	_CreateControls();
	m_Parent = parent;
}

QueriesPageGeneric::~QueriesPageGeneric() {
}

bool QueriesPageGeneric::TransferDataToWindow() {
	m_GenericData = new QueriesPageGenericData();
	
	
	if (m_ListGeneric->IsEmpty()){
		m_ListGeneric->Append(m_GenericData->GetDescription());
	}
	
	if (m_ListGeneric->GetSelection() == wxNOT_FOUND ) {
		m_ListGeneric->SetSelection(0);
	}
	m_ListGeneric->SetFocus();
	
	return true;
}

bool QueriesPageGeneric::TransferDataFromWindow() {
	
	// get data back from generic list
	int iTarget = wxNOT_FOUND;
	wxString myDesc = wxEmptyString;
	wxString mySQL = wxEmptyString;
	
	int iSelected = m_ListGeneric->GetSelection();
	m_GenericData->GetData(iSelected, iTarget, myDesc, mySQL);
	
	// set data to the parent
	m_Parent->GetData()->m_QueryLayerType = (TOC_GENERIC_NAME) iTarget;
	m_Parent->GetData()->m_QuerySQL = mySQL;
	
	// propose a name if name is empty
	if (m_Parent->GetData()->m_QueryName.IsEmpty()) {
		m_Parent->GetData()->m_QueryName = myDesc;
	}
		
	
	delete m_GenericData;
	m_GenericData = NULL;
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
	m_Parent->GetData()->m_QueryLayerType = (TOC_GENERIC_NAME) m_LayerType->GetSelection();
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
	m_Parent->GetData()->m_QuerySQL = m_SQLText->GetValue();
	return true;
}



QueriesPageName::QueriesPageName(QueriesWizard * parent, wxWizardPage * prev, wxWizardPage * next) :
wxWizardPageSimple(parent, prev, next){
	_CreateControls();
	m_Parent = parent;
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
	m_TextName->SetValue(m_Parent->GetData()->m_QueryName);
	return true;
}

bool QueriesPageName::TransferDataFromWindow() {
	m_Parent->GetData()->m_QueryName = m_TextName->GetValue();
	return true;
}



