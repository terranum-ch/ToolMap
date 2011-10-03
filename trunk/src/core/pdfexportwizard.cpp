/***************************************************************************
 pdfexportwizard.cpp
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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

#include "pdfexportwizard.h"

void PdfExportWizard::_CreateControls() {
    // Page Layout
    m_PageLayout = new wxWizardPageSimple(this);
    wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_PageLayout, wxID_ANY, _("Export type") ), wxVERTICAL );
	
	m_ExportOnePageCtrl = new wxRadioButton( m_PageLayout, wxID_ANY, _("One Page"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    m_ExportOnePageCtrl->SetValue(true);
	sbSizer1->Add( m_ExportOnePageCtrl, 0, wxALL, 5 );
	
	m_ExportMultiPageCtrl = new wxRadioButton( m_PageLayout, wxID_ANY, _("Multiple pages"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_ExportMultiPageCtrl, 0, wxALL, 5 );
	
	bSizer1->Add( sbSizer1, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_PageLayout, wxID_ANY, _("Export Layout") ), wxVERTICAL );
	
	m_ExportOneColCtrl = new wxRadioButton( m_PageLayout, wxID_ANY, _("One column"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	sbSizer2->Add( m_ExportOneColCtrl, 0, wxALL, 5 );
	
	m_ExportTwoColCtrl = new wxRadioButton( m_PageLayout, wxID_ANY, _("Two Columns"), wxDefaultPosition, wxDefaultSize, 0 );
    m_ExportTwoColCtrl->SetValue(true);
	sbSizer2->Add( m_ExportTwoColCtrl, 0, wxALL, 5 );
	
	bSizer1->Add( sbSizer2, 0, wxALL|wxEXPAND, 5 );
	
	m_PageLayout->SetSizer( bSizer1 );
	m_PageLayout->Layout();
    
    
    
    // page paper
    m_PagePaper = new wxWizardPageSimple(this);
    wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_PagePaper, wxID_ANY, _("Page Size") ), wxVERTICAL );
	
	wxString m_PaperSizeCtrlChoices[] = { _("A4"), _("A3"), _("A2"), _("A1"), _("A0") };
	int m_PaperSizeCtrlNChoices = sizeof( m_PaperSizeCtrlChoices ) / sizeof( wxString );
	m_PaperSizeCtrl = new wxChoice( m_PagePaper, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PaperSizeCtrlNChoices, m_PaperSizeCtrlChoices, 0 );
	m_PaperSizeCtrl->SetSelection( 0 );
	sbSizer3->Add( m_PaperSizeCtrl, 0, wxALL, 5 );
	
	bSizer2->Add( sbSizer3, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_PagePaper, wxID_ANY, _("Page orientation") ), wxVERTICAL );
	
	m_PaperPortraitCtrl = new wxRadioButton( m_PagePaper, wxID_ANY, _("Portrait"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_PaperPortraitCtrl->SetValue( true ); 
	sbSizer4->Add( m_PaperPortraitCtrl, 0, wxALL, 5 );
	
	m_PaperLandscapeCtrl = new wxRadioButton( m_PagePaper, wxID_ANY, _("Landscape"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_PaperLandscapeCtrl, 0, wxALL, 5 );
	
	bSizer2->Add( sbSizer4, 0, wxALL|wxEXPAND, 5 );
	
	m_PagePaper->SetSizer( bSizer2 );
	m_PagePaper->Layout();
    
    
    // page decoration
    m_PageDecoration = new wxWizardPageSimple(this);
    wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_PageDecoration, wxID_ANY, _("Decorations") ), wxVERTICAL );
	
	m_DecorationSimpleCtrl = new wxCheckBox( m_PageDecoration, wxID_ANY, _("Use very simple decorations"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer5->Add( m_DecorationSimpleCtrl, 0, wxALL, 5 );
	
	bSizer3->Add( sbSizer5, 0, wxEXPAND|wxALL, 5 );
	
	m_PageDecoration->SetSizer( bSizer3 );
	m_PageDecoration->Layout();
}



PdfExportWizard::PdfExportWizard(wxWindow * parent, wxWindowID id, const wxString & title)
: wxWizard(parent, id,title){
    m_PageLayout = NULL;
    m_PagePaper = NULL;
    m_PageDecoration = NULL;
    
    _CreateControls();
    wxWizardPageSimple::Chain(m_PageLayout, m_PagePaper);
    wxWizardPageSimple::Chain(m_PagePaper, m_PageDecoration);
    
    // specifiy size
	GetPageAreaSizer()->Add(m_PageLayout);
	GetPageAreaSizer()->Add(m_PagePaper);
    GetPageAreaSizer()->Add(m_PageDecoration);
	wxSize mySize = GetPageAreaSizer()->CalcMin();
	SetMinSize(mySize);
	SetPageSize(mySize);
}



PdfExportWizard::~PdfExportWizard() {
}



int PdfExportWizard::ShowWizard() {
    if (RunWizard(m_PageLayout)==true) {
		return wxID_OK;
	}
	return wxID_CANCEL;
}



bool PdfExportWizard::GetWizardChoices(PdfDocument & document) {
    return false;
}

