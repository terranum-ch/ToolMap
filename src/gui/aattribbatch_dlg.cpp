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



AAttribBatch_DLG::AAttribBatch_DLG( wxWindow* parent,
								   tmAAttribBatchManager * manager,
								   wxWindowID id, const wxString& title,
								   const wxPoint& pos, const wxSize& size, long style ) :
wxDialog( parent, id, title, pos, size, style ){
	wxASSERT(manager);
	m_pBatchManager = manager;	
	
	this->SetSizeHints( wxSize( 360,200 ), wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText1;
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
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
	statictext->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	statictext->Wrap( -1 );
	bSizer3->Add( statictext, 0, wxALL, 5 );
	
	m_ListFields = new wxListBox( this, ID_DLGAATB_LIST_FIELDS, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer3->Add( m_ListFields, 1, wxALL|wxEXPAND, 5 );
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Value") ), wxVERTICAL );
	m_CtrlSizer = new wxBoxSizer( wxVERTICAL );
	sbSizer1->Add( m_CtrlSizer, 0, wxALL|wxEXPAND, 0 );
	
	_SetControl(NULL);
	
	bSizer1->Add( sbSizer1, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
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


bool AAttribBatch_DLG::TransferDataToWindow(){
	wxASSERT(m_pBatchManager);
	
	PrjMemObjectsArray myObjets;
	wxArrayInt myNumber;
	if(m_pBatchManager->GetTypes(myObjets, myNumber, m_ArrayIds)==false){
		wxLogError(_("Getting Types failed for batch attribution"));
		return true;
	}
	
	m_ListType->Freeze();
	for (unsigned int i = 0; i < myObjets.GetCount(); i++) {
		wxString myText;
		myText.Append(myObjets.Item(i).m_ObjectName);
		myText.Append(wxString::Format(_T(" (%d)"), myNumber.Item(i)));
		m_ListType->Append(myText);
	}
	m_ListType->Thaw();
	
	
	return true;
}



void AAttribBatch_DLG::OnTypeChange( wxCommandEvent& event ){
	m_ListFields->Clear();
	_SetControl(NULL);
	
	if (m_ArrayIds.IsEmpty() == true) {
		wxLogWarning(_("Types are empty, getting fields isn't possible"));
		return;
	}
	
	wxLogMessage(_T("Selected type item index : %d"), event.GetSelection());
	if (event.GetSelection() >= (signed int) m_ArrayIds.GetCount()) {
		wxLogError(_("Error getting field for item %d!"), event.GetSelection());
		return;
	}
	
	if (m_pBatchManager->GetFields(m_ArrayIds.Item(event.GetSelection()), m_Fields)==false) {
		wxLogError(_("Getting fields for item %d failed"), event.GetSelection());
		return;
	}
	
	m_ListFields->Freeze();
	for (unsigned int i = 0; i< m_Fields.GetCount(); i++) {
		m_ListFields->Append(m_Fields.Item(i)->m_Fieldname);
	}
	m_ListFields->Thaw();
	
}



void AAttribBatch_DLG::OnFieldsChange( wxCommandEvent& event ){
	if (m_Fields.IsEmpty() == true) {
		wxLogWarning(_("Fields are empty, Advanced batch attribution isn't possible"));
		return;
	}
	
	wxLogMessage(_T("Selected field item index : %d"), event.GetSelection());
	if (event.GetSelection() >= (signed int) m_Fields.GetCount()) {
		wxLogError(_("Error showing field for item %d!"), event.GetSelection());
		return;
	}
	
	tmAAttribCtrl * myCtrl = m_pBatchManager->GetValueControl(*(m_Fields.Item(event.GetSelection())),this);
	if (myCtrl == NULL) {
		wxLogError(_("Getting Control for Field %d failed"), event.GetSelection());
		return;
	}
	
	// setting control
	_SetControl(myCtrl);
}


void AAttribBatch_DLG::_SetControl(tmAAttribCtrl * ctrl){
	m_ValueCtrl = NULL;
	// setting default ctrl
	if (ctrl == NULL) {
		ProjectDefMemoryFields myUnusedField;
		ctrl = new tmAAttribInfoPanel(this, myUnusedField);
		wxASSERT(ctrl);
	}
	
	
#ifdef __WXOSX__
	ctrl->SetPanelColour(wxColour(222,222,222));
#else
	wxStaticBox myBox;
	ctrl->SetPanelColour(myBox.GetBackgroundColour());
#endif
	
	wxASSERT(m_CtrlSizer);
	wxSizerItemList myItemList = m_CtrlSizer->GetChildren();
	if (myItemList.IsEmpty() == false) {
		wxSizerItemList::iterator iter;
		iter = myItemList.begin();
		wxSizerItem * myItem = *iter;
		wxASSERT(myItem);
		myItem->DeleteWindows();
		m_CtrlSizer->Remove(0);
	}
	
	m_ValueCtrl = ctrl;
	m_CtrlSizer->Add( m_ValueCtrl, 1, wxEXPAND | wxALL, 5);
	this->Layout();
}


void AAttribBatch_DLG::OnSave( wxCommandEvent& event ) {
	wxASSERT(m_ValueCtrl);
	wxString myValue = m_ValueCtrl->GetControlValue();
	if (myValue.IsEmpty() == true || myValue.IsNull()) {
		wxLogError(_("Attribution failed, please select a Type, a Field and then set a Value!"));
		return;
	}
	
	wxWindowDisabler mydisabler;
	int myNumber = wxNOT_FOUND;
	// to delete the busy cursor
	{
		wxBusyCursor myBusyCursor;
		myNumber = m_pBatchManager->Attribute(m_ArrayIds.Item(m_ListType->GetSelection()),
											  *(m_Fields.Item(m_ListFields->GetSelection())),
											  myValue);
		if (myNumber == wxNOT_FOUND) {
			wxLogError(_("Batch attribution Failed"));
			return;
		}
	}

	Close();
}

void AAttribBatch_DLG::OnApply( wxCommandEvent& event ) {
	wxASSERT(m_ValueCtrl);
	wxString myValue = m_ValueCtrl->GetControlValue();
	if (myValue.IsEmpty() == true || myValue.IsNull()) {
		wxLogError(_("Attribution failed, please select a Type, a Field and then set a Value!"));
		return;
	}
	
	wxWindowDisabler mydisabler;
	int myNumber = wxNOT_FOUND;
	// to delete the busy cursor
	{
		wxBusyCursor myBusyCursor;
		myNumber = m_pBatchManager->Attribute(m_ArrayIds.Item(m_ListType->GetSelection()),
												  *(m_Fields.Item(m_ListFields->GetSelection())),
												  myValue);
		if (myNumber == wxNOT_FOUND) {
			wxLogError(_("Batch attribution Failed"));
			return;
		}
	}
		
	wxMessageBox(wxString::Format(_("%s = %s set for %d features"),
								  m_Fields.Item(m_ListFields->GetSelection())->m_Fieldname.c_str(),
								  myValue.c_str(),
								  myNumber),
				 _("Batch attribution succeed"),
				 wxICON_INFORMATION | wxOK, this);
}



