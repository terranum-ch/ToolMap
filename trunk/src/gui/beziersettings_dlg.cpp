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
    m_ApproximationValue = 0.2;
    m_ApproximationValuePrevious = 0;
    m_PreviewCtrl = NULL;
    m_EditManager = editmanager;
    m_Renderer = renderer;
    
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
    m_ApproximationCtrl->SetValue(m_ApproximationValue);
    return true;
}


bool BezierSettings_DLG::TransferDataFromWindow(){
    m_ApproximationValue = m_ApproximationCtrl->GetValue();
    return true;
}



void BezierSettings_DLG::OnIdlePreview( wxIdleEvent& event ){
    if (m_PreviewCtrl == NULL || m_PreviewCtrl->IsChecked() == false) {
      return;
    }
    
    if (wxIsSameDouble(m_ApproximationCtrl->GetValue(), m_ApproximationValuePrevious) == true) {
        return;
    }
    double myApproxActualValue = m_ApproximationCtrl->GetValue();
    
    wxASSERT(m_EditManager);
    m_EditManager->ArcClear();
    m_EditManager->BezierToLine(myApproxActualValue);
    m_ApproximationValuePrevious = myApproxActualValue;
    
    m_Renderer->Refresh();
    m_Renderer->Update();
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
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Approximation scale") ), wxVERTICAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("A small scale value implies high accuracy\nand a large number of vertices"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	sbSizer1->Add( m_staticText2, 0, wxALL, 5 );
	
	m_ApproximationCtrl = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), wxSP_ARROW_KEYS, 0, 100, 0, 0.1 );
	sbSizer1->Add( m_ApproximationCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	bSizer1->Add( sbSizer1, 1, wxEXPAND|wxALL, 5 );
	
	m_PreviewCtrl = new wxCheckBox( this, wxID_ANY, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_PreviewCtrl, 0, wxALL, 5 );
	
	wxStdDialogButtonSizer* m_sdbSizer1;
	wxButton* m_sdbSizer1OK;
	wxButton* m_sdbSizer1Cancel;
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	bSizer1->Add( m_sdbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
}

