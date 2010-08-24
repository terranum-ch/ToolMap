/***************************************************************************
 tmimportwizpages.cpp
 
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


#include "tmimportwizpages.h"
#include "tmimportwiz.h"
#include <wx/spinctrl.h>
#include "../gis/tmimportgis.h"
#include "../gis/tmimportcsv.h"



//
////////////// INTRODUCTION WIZARD PAGE ////////////////
//
void ImportWizIntro::_CreateControls() {
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxString m_FileTypeCtrlChoices[] = { _("ESRI's shapefiles"), _("CSV files (semi-colon separated)") };
	int m_FileTypeCtrlNChoices = sizeof( m_FileTypeCtrlChoices ) / sizeof( wxString );
	m_FileTypeCtrl = new wxRadioBox( this, wxID_ANY, _("File type:"), wxDefaultPosition, wxSize(250, -1), m_FileTypeCtrlNChoices, m_FileTypeCtrlChoices, 1, wxRA_SPECIFY_COLS );
	bSizer2->Add( m_FileTypeCtrl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	SetSizer( bSizer2 );
	GetSizer()->Fit(this);
}

ImportWizIntro::ImportWizIntro(ImportWizard * parent, wxWizardPage * next) : 
wxWizardPageSimple(parent, NULL, next){
	m_Parent = parent;
	_CreateControls();
	m_Parent->SetImport(new tmImportGIS());
}

ImportWizIntro::~ImportWizIntro() {
}


bool ImportWizIntro::TransferDataToWindow() {
	m_FileTypeCtrl->SetSelection(m_Parent->GetImport()->GetFileType());
	return true;
}


bool ImportWizIntro::TransferDataFromWindow() {
	tmImport * myImport = m_Parent->GetImport();
	wxDELETE(myImport);
	if (m_FileTypeCtrl->GetSelection() == (int) tmIMPORT_TYPE_SHP) {
		m_Parent->SetImport(new tmImportGIS());
	}
	else if(m_FileTypeCtrl->GetSelection() == (int) tmIMPORT_TYPE_CSV){
		m_Parent->SetImport(new tmImportCSV());
	}
	else {
		wxFAIL;
	}
	return true;
}




//
////////////// INFO WIZARD PAGE ////////////////
//
BEGIN_EVENT_TABLE(ImportWizInfo, wxWizardPageSimple)
EVT_FILEPICKER_CHANGED(wxID_OPEN, ImportWizInfo::OnOpenFile)
END_EVENT_TABLE()

void ImportWizInfo::_CreateControls() {
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("Select a file:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxALL, 5 );
	
	m_FileCtrl = new wxFilePickerCtrl( this, wxID_OPEN, wxEmptyString, _("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizer3->Add( m_FileCtrl, 0, wxEXPAND|wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("File informations") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_InfoLabelCtrl1 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_InfoLabelCtrl1->Wrap( -1 );
	fgSizer1->Add( m_InfoLabelCtrl1, 0, wxALL, 5 );
	
	m_InfoValueCtrl1 = new wxStaticText( this, wxID_ANY, _("14"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InfoValueCtrl1->Wrap( -1 );
	fgSizer1->Add( m_InfoValueCtrl1, 0, wxALL, 5 );
	
	m_InfoLabelCtrl2 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_InfoLabelCtrl2->Wrap( -1 );
	fgSizer1->Add( m_InfoLabelCtrl2, 0, wxALL, 5 );
	
	m_InfoValueCtrl2 = new wxStaticText( this, wxID_ANY, _("15"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InfoValueCtrl2->Wrap( -1 );
	fgSizer1->Add( m_InfoValueCtrl2, 0, wxALL, 5 );
	
	sbSizer2->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	bSizer3->Add( sbSizer2, 0, wxEXPAND|wxALL, 5 );
	
	SetSizer( bSizer3 );
	bSizer3->Fit(this);
}

void ImportWizInfo::OnOpenFile(wxFileDirPickerEvent & event) {
	tmImport * myImport = m_Parent->GetImport();
	wxASSERT(myImport);

	if(myImport->Open(event.GetPath())==false){
		return;
	}
	
	m_InfoValueCtrl2->SetLabel(wxString::Format(_T("%ld"), myImport->GetFeatureCount()));
	
	if (myImport->GetFileType() == tmIMPORT_TYPE_SHP) {
		m_InfoValueCtrl1->SetLabel(TM_GIS_SPATIAL_TYPES_STRING[myImport->GetGeometryType()]);
	}else if (myImport->GetFileType() == tmIMPORT_TYPE_CSV) {
		m_InfoValueCtrl1->SetLabel(wxString::Format(_T("%ld"), myImport->GetFieldCount()));
	}
	else {
		wxFAIL;
	}
	
	
}

ImportWizInfo::ImportWizInfo(ImportWizard * parent) : wxWizardPage(parent) {
	m_Parent = parent;
	m_Prev = NULL;
	m_Next = NULL;
	m_PageTarget = new ImportWizTarget(parent, NULL, NULL);
	m_PageCSVOptions = new ImportWizCSVOptions (parent, this, m_PageTarget);
	m_PageTarget->SetPrev(m_PageCSVOptions);
	_CreateControls();
}

ImportWizInfo::~ImportWizInfo() {
	wxDELETE(m_PageTarget);
	wxDELETE(m_PageCSVOptions);
}

wxWizardPage * ImportWizInfo::GetPrev() const {
	return m_Prev;
}

wxWizardPage * ImportWizInfo::GetNext() const {
	return m_PageCSVOptions;
}

bool ImportWizInfo::TransferDataToWindow() {
	wxFilePickerCtrl * myPicker = NULL;
	if (m_Parent->GetImport()->GetFileType() == tmIMPORT_TYPE_SHP) {
		myPicker = new wxFilePickerCtrl( this, wxID_OPEN, wxEmptyString, _("Select a SHP file"), _("ESRI's shapefile (*.shp)|*.shp"));
		m_InfoLabelCtrl1->SetLabel(_("Geometry type:"));
		m_InfoLabelCtrl2->SetLabel(_("Number of features:"));
	}else if (m_Parent->GetImport()->GetFileType() == tmIMPORT_TYPE_CSV) {
		myPicker = new wxFilePickerCtrl( this, wxID_OPEN, wxEmptyString, _("Select a CSV file"), wxT("Semi-colon separeted file (*.csv)|*.csv"));
		m_InfoLabelCtrl1->SetLabel(_("Number of columns:"));
		m_InfoLabelCtrl2->SetLabel(_("Number of rows:"));
	}
	else {
		wxFAIL;
	}
	GetSizer()->Replace(m_FileCtrl, myPicker);
	this->RemoveChild(m_FileCtrl);
	wxDELETE(m_FileCtrl);
	m_FileCtrl = myPicker;
	Layout();
	
	
	
	
	return true;
}

bool ImportWizInfo::TransferDataFromWindow() {
	return true;
}




//
////////////// CSV OPTIONS WIZARD PAGE ////////////////
//
void ImportWizCSVOptions::_CreateControls() {
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText7;
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("Select columns for import"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer5->Add( m_staticText7, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText8;
	m_staticText8 = new wxStaticText( this, wxID_ANY, _("X column:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer2->Add( m_staticText8, 0, wxALL, 5 );
	
	wxArrayString m_XColCtrlChoices;
	m_XColCtrl = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_XColCtrlChoices, 0 );
	m_XColCtrl->SetSelection( 0 );
	fgSizer2->Add( m_XColCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText10;
	m_staticText10 = new wxStaticText( this, wxID_ANY, _("Y column:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer2->Add( m_staticText10, 0, wxALL, 5 );
	
	wxArrayString m_YColCtrlChoices;
	m_YColCtrl = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_YColCtrlChoices, 0 );
	m_YColCtrl->SetSelection( 0 );
	fgSizer2->Add( m_YColCtrl, 0, wxALL|wxEXPAND, 5 );
	
	bSizer5->Add( fgSizer2, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer5 );
	bSizer5->Fit(this);
}

ImportWizCSVOptions::ImportWizCSVOptions(ImportWizard * parent,wxWizardPage * prev, wxWizardPage * next) : 
wxWizardPageSimple(parent, prev, next){
	m_Parent = parent;
	_CreateControls();
}

ImportWizCSVOptions::~ImportWizCSVOptions() {
}

bool ImportWizCSVOptions::TransferDataToWindow() {
	return true;
}

bool ImportWizCSVOptions::TransferDataFromWindow() {
	return true;
}




//
////////////// TARGET WIZARD PAGE ////////////////
//
void ImportWizTarget::_CreateControls() {
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText12;
	m_staticText12 = new wxStaticText( this, wxID_ANY, _("Select import target:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer6->Add( m_staticText12, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText13;
	m_staticText13 = new wxStaticText( this, wxID_ANY, _("Import into:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer7->Add( m_staticText13, 0, wxALL, 5 );
	
	wxArrayString m_TargetCtrlChoices;
	m_TargetCtrl = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_TargetCtrlChoices, 0 );
	m_TargetCtrl->SetSelection( 0 );
	bSizer7->Add( m_TargetCtrl, 1, wxALL, 5 );
	
	bSizer6->Add( bSizer7, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer6 );
	bSizer6->Fit(this);
}

ImportWizTarget::ImportWizTarget(ImportWizard * parent, wxWizardPage * prev, wxWizardPage * next) :
wxWizardPageSimple(parent, prev, next){
	m_Parent = parent;
	_CreateControls();
}

ImportWizTarget::~ImportWizTarget() {
}

bool ImportWizTarget::TransferDataToWindow() {
	return true;
}

bool ImportWizTarget::TransferDataFromWindow() {
	return true;
}

