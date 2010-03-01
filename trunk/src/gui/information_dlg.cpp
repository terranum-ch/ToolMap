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
#include "../gis/tmtocctrl.h"
#include "../gis/tmselecteddatamemory.h"

#include "../gis/tmgisdata.h"



/*BEGIN_EVENT_TABLE(InformationDLG, wxDialog)
	EVT_CLOSE(InformationDLG::OnCloseDlg)
END_EVENT_TABLE();*/


void InformationDLG::_CreateControls() {
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	wxNotebook * m_notebook = new wxNotebook( this, wxID_ANY, 
								wxDefaultPosition, wxDefaultSize, 0 );
	wxPanel* m_panel4;
	m_panel4 = new wxPanel( m_notebook, wxID_ANY, 
						   wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	m_LayerCtrl = new tmLayerInfoCtrl( m_panel4, wxID_ANY, wxDefaultPosition, wxSize( 350,250 ));
	bSizer23->Add( m_LayerCtrl, 1, wxEXPAND, 5 );
	
	m_panel4->SetSizer( bSizer23 );
	m_panel4->Layout();
	bSizer23->Fit( m_panel4 );
	m_notebook->AddPage( m_panel4, _("Layer"), false );
	wxPanel* m_panel5;
	m_panel5 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition,
						   wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook->AddPage( m_panel5, _("Feature"), false );
	
	bSizer22->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer22 );
	this->Layout();
	bSizer22->Fit( this );
	
	this->Center(wxBOTH);
}

void InformationDLG::OnSelectionChanged(wxCommandEvent & event) {
}



void InformationDLG::OnLayerChanged(wxCommandEvent & event) {
	wxLogMessage(_T("Event received"));
	wxASSERT(m_TOC);
	m_LayerCtrl->Update(m_TOC);
}



void InformationDLG::OnCloseDlg(wxCloseEvent & event) {
	wxLogMessage(_T("Destroying dialog"));
	Destroy();
	
}



InformationDLG::InformationDLG(wxWindow * parent, tmTOCCtrl * toc,
							   tmSelectedDataMemory * sel,
							   wxWindowID id, const wxString & title,
							   const wxPoint & pos, const wxSize & size,
							   long style) : 
wxDialog(parent, id, title, pos, size, style) {
	m_TOC = toc,
	m_Selected = sel;
	
	_CreateControls();
	
	
	// connect event
	
	Connect(tmEVT_TOC_SELECTION_CHANGED, wxCommandEventHandler(InformationDLG::OnLayerChanged));
	Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler(InformationDLG::OnCloseDlg));
}



InformationDLG::~InformationDLG() {
	// disconnect event
	Disconnect(tmEVT_TOC_SELECTION_CHANGED, wxCommandEventHandler(InformationDLG::OnLayerChanged));
	Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler(InformationDLG::OnCloseDlg));
}



bool InformationDLG::TransferDataToWindow() {
	wxASSERT(m_TOC);
	m_LayerCtrl->Update(m_TOC);
	
	
	return true;
}






/***************************************************************************//**
Layer informations (wxHtmlWindow)
author Lucien Schreiber (c) CREALP 2010
date 01 mars 2010
*******************************************************************************/
tmLayerInfoCtrl::tmLayerInfoCtrl(wxWindow * parent, wxWindowID id,
								 const wxPoint & pos, const wxSize & size):
wxHtmlWindow(parent, id, pos, size){
	
}

tmLayerInfoCtrl::~tmLayerInfoCtrl() {
}

void tmLayerInfoCtrl::Update(tmTOCCtrl * toc) {
	
	// load layer informations only if dialog is visible
	tmLayerProperties * itemProp = toc->GetSelectionLayer();
	if (!itemProp)
	{
		wxLogDebug(_T("No layer selected, select a layer"));
		return;
	}
	
	
	tmGISData * myData = tmGISData::LoadLayer(itemProp);
	if (!myData)
	{
		wxLogError(_("Error loading GIS data for metadata"));
		return;
	}
	wxString myMetaData = myData->GetMetaDataAsHtml();
	SetPage(myMetaData);
}










#if (0)

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
	
//	m_CloseBtn = new wxButton( this, wxID_CANCEL, _("&Close"),
//							  wxDefaultPosition, wxDefaultSize, 0 );
//	bSizer22->Add( m_CloseBtn, 0, wxALL|wxALIGN_RIGHT, 5 );
	
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


#endif