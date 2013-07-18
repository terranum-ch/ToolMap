/***************************************************************************
 beziersettings_dlg.cpp
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
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
#include "beziersettings_dlg.h"
#include "../gis/tmeditmanager.h"
#include "../gis/tmrenderer.h"


BezierSettings_DLG::BezierSettings_DLG( wxWindow* parent, tmEditManager * editmanager, tmRenderer * renderer, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style ){
    m_PreviewCtrl = NULL;
    m_EditManager = editmanager;
    m_Renderer = renderer;
    m_PreviousData.agg_approximation = wxNOT_FOUND;
    
    _CreateControls();
    
    // Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( BezierSettings_DLG::OnIdlePreview ) );
	m_PreviewCtrl->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BezierSettings_DLG::OnCheckPreview ), NULL, this );
	m_PreviewCtrl->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( BezierSettings_DLG::OnUpdateUIPreview ), NULL, this );
}



BezierSettings_DLG::~BezierSettings_DLG()
{
    wxASSERT(m_EditManager);
    m_EditManager->ArcClear();
    m_Renderer->Refresh();
    m_Renderer->Update();
    
    // Disconnect Events
    this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( BezierSettings_DLG::OnIdlePreview ) );
	m_PreviewCtrl->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BezierSettings_DLG::OnCheckPreview ), NULL, this );
    m_PreviewCtrl->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( BezierSettings_DLG::OnUpdateUIPreview ), NULL, this );

    
}


bool BezierSettings_DLG::TransferDataToWindow(){
    m_NotebookMethodCtrl->SetSelection(m_Data.method);
    m_ApproximationCtrl->SetValue(m_Data.agg_approximation);
    m_WidthToleranceCtrl->SetValue(m_Data.ethz_width);
    m_MaxNumPointsCtrl->SetValue(m_Data.ethz_max_points);
    return true;
}


bool BezierSettings_DLG::TransferDataFromWindow(){
    _GetDataFromControl(&m_Data);
    return true;
}


void BezierSettings_DLG::_GetDataFromControl (BezierSettingsData * data){
    if (data == NULL) {
        return;
    }
    
    data->method = BezierSettingsData::AGG;
    if (m_NotebookMethodCtrl->GetSelection() != 0){
        data->method = BezierSettingsData::ETHZ;
    }
    data->agg_approximation = m_ApproximationCtrl->GetValue();
    data->ethz_width = m_WidthToleranceCtrl->GetValue();
    data->ethz_max_points = m_MaxNumPointsCtrl->GetValue();
}


void BezierSettings_DLG::OnIdlePreview( wxIdleEvent& event ){
    if (m_PreviewCtrl == NULL || m_PreviewCtrl->IsChecked() == false) {
      return;
    }
    
    _GetDataFromControl(&m_Data);
    if (m_Data == m_PreviousData) {
        return;
    }
    
    // display bezier preview
    wxASSERT(m_EditManager);
    m_EditManager->ArcClear();
    m_EditManager->BezierToLine(m_Data);
    
    m_Renderer->Refresh();
    m_Renderer->Update();
    
    m_PreviousData = m_Data;
}



void BezierSettings_DLG::OnCheckPreview( wxCommandEvent& event ){
    if (event.IsChecked() == false) {
        wxASSERT(m_EditManager);
        m_EditManager->ArcClear();
        m_Renderer->Refresh();
        m_Renderer->Update();
    }
}



void BezierSettings_DLG::OnUpdateUIPreview( wxUpdateUIEvent& event ) {
    if (m_EditManager == NULL || m_EditManager->IsBezierToLinePreviewAllowed() == false) {
        event.Enable(false);
        return;
    }
    event.Enable(true);
}



void BezierSettings_DLG::_CreateControls(){
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_NotebookMethodCtrl = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wxPanel* m_panel1;
	m_panel1 = new wxPanel( m_NotebookMethodCtrl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, _("Approximation scale") ), wxVERTICAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( m_panel1, wxID_ANY, _("A high scale value implies high accuracy\nand a large number of vertices"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	sbSizer1->Add( m_staticText2, 0, wxALL, 5 );
	
	m_ApproximationCtrl = new wxSpinCtrlDouble( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), wxSP_ARROW_KEYS, 0, 10, 0, 0.1 );
	sbSizer1->Add( m_ApproximationCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	bSizer2->Add( sbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_NotebookMethodCtrl->AddPage( m_panel1, _("AGG"), false );
	wxPanel* m_panel2;
	m_panel2 = new wxPanel( m_NotebookMethodCtrl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("Parameters") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText3;
	m_staticText3 = new wxStaticText( m_panel2, wxID_ANY, _("Width tolerance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_WidthToleranceCtrl = new wxSpinCtrlDouble( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), wxSP_ARROW_KEYS, 0, 1000, 0, 0.1 );
	fgSizer1->Add( m_WidthToleranceCtrl, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxStaticText* m_staticText4;
	m_staticText4 = new wxStaticText( m_panel2, wxID_ANY, _("Maximum number of points\nfor each Bezier segment"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer1->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_MaxNumPointsCtrl = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition,  wxSize( 100,-1 ), wxSP_ARROW_KEYS, 0, 10000, 0 );
	fgSizer1->Add( m_MaxNumPointsCtrl, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	sbSizer3->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	
	bSizer3->Add( sbSizer3, 1, wxEXPAND|wxALL, 5 );
	
	
	m_panel2->SetSizer( bSizer3 );
	m_panel2->Layout();
	bSizer3->Fit( m_panel2 );
	m_NotebookMethodCtrl->AddPage( m_panel2, _("ETHZ"), false );
	
	bSizer1->Add( m_NotebookMethodCtrl, 1, wxEXPAND | wxALL, 5 );
	
	m_PreviewCtrl = new wxCheckBox( this, wxID_ANY, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_PreviewCtrl, 0, wxALL, 5 );
	
	wxStdDialogButtonSizer* m_sdbSizer1;
	wxButton* m_sdbSizer1OK;
	wxButton* m_sdbSizer1Cancel;
	wxButton* m_sdbSizer1Help;
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1Help = new wxButton( this, wxID_HELP );
	m_sdbSizer1->AddButton( m_sdbSizer1Help );
	m_sdbSizer1->Realize();
	
	bSizer1->Add( m_sdbSizer1, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
}

