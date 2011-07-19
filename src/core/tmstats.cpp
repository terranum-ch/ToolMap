/***************************************************************************
 tmstats.cpp
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

#include "tmstats.h"
#include "tmstatsevent.h"
#include "../database/database_tm.h"
#include "tmstatsrecord.h"

// event declared in tmstatsevent.h
DEFINE_EVENT_TYPE(tmEVT_STAT_CLICK);
DEFINE_EVENT_TYPE(tmEVT_STAT_ATTRIB);
DEFINE_EVENT_TYPE(tmEVT_STAT_INTERSECTION);


tmStatsData::tmStatsData() {
	Reset();
}



tmStatsData::~tmStatsData() {
}



void tmStatsData::Reset() {
	m_Id = wxNOT_FOUND;
	m_TimeStart = wxInvalidDateTime;
	ResetPartial();
}


void tmStatsData::ResetPartial(){
	// don't clean Id and start time!
	m_NbClick = 0;
	m_NbAttribution = 0;
	m_NbIntersection = 0;
	m_TimeElapsed = wxTimeSpan();
}



bool tmStatsData::IsOk() const {
	if (m_TimeStart.IsValid() == false) {
		return false;
	}
	
	if (m_TimeElapsed.IsNull() == true) {
		return false;
	}
	return true;
}










void tmStatsManager::AppendToBuffer(long click, long attrib, long intersection) {
	wxLogMessage("Received statistics (C/A/I): %ld, %ld, %ld",click,attrib,intersection);
	
	// Check that statistics is started here!
	if (m_IsStarted == false) {
		return;
	}
	
	m_StatBufferData.m_NbClick = m_StatBufferData.m_NbClick + click;
	m_StatBufferData.m_NbAttribution = m_StatBufferData.m_NbAttribution + attrib;
	m_StatBufferData.m_NbIntersection = m_StatBufferData.m_NbIntersection + intersection;
	
	if (m_StatBufferData.m_NbClick >= m_BufferSize ||
		m_StatBufferData.m_NbAttribution >= m_BufferSize ||
		m_StatBufferData.m_NbIntersection >= m_BufferSize) {
		_FlushBuffer();
	}
}



void tmStatsManager::_FlushBuffer() {
	wxASSERT(m_Database);	
	tmStatsRecords myRecord (m_Database);
	if(myRecord.Add(m_StatBufferData.m_Id, m_StatBufferData)==false){
		wxLogError(_("Error adding data to statistics!"));
	}
	m_StatBufferData.ResetPartial();
}



void tmStatsManager::_StartRecord() {
	wxLogMessage("Starting to record!");
	wxASSERT(m_Database);
	
	// Create record here
	tmStatsRecords myRecord (m_Database);
	m_StatBufferData.Reset();
	m_StatBufferData.m_TimeStart = wxDateTime::Now();
	long myRecordId = myRecord.Create(m_StatBufferData);
	if (myRecordId == wxNOT_FOUND) {
		wxLogError(_("Error starting statistics!"));
		m_IsStarted = false;
		return;
	}
	m_StatBufferData.m_Id = myRecordId;
	m_IsStarted = true;
}


void tmStatsManager::_StopRecord() {
	if (m_IsStarted == false) {
		return;
	}
	
	wxLogMessage("Stoping to record!");
	m_IsStarted = false;
	_FlushBuffer();
}



tmStatsManager::tmStatsManager() {
	Create(NULL);
}



tmStatsManager::~tmStatsManager() {
	_StopRecord();
}



void tmStatsManager::Create(DataBaseTM * database) {
	m_Database = database;
	m_StatBufferData.Reset();
	wxASSERT(m_StatBufferData.IsOk() == false);
	m_IsStarted = false;
}


bool tmStatsManager::IsReady(){
	if (m_Database == NULL) {
		return false;
	}
	return true;
}



void tmStatsManager::ShowStatsDialog(wxWindow * parent) {
	if (IsReady() == false) {
		wxLogError(_("No project open! Statistics unavaillable"));
		return;
	}
	
	if (m_IsStarted == true) {
		_FlushBuffer();
	}
	
	wxASSERT(parent);
	wxASSERT(m_Database);
	tmStatsRecords myRecord (m_Database);
	long myRecordNb = myRecord.GetCount();
	tmStatsData myDataTotal;
	myDataTotal.m_TimeElapsed = wxTimeSpan(0,0,0);
	if (myRecordNb > 0) {
		if (myRecord.LoadTotal(myDataTotal) == false) {
			wxLogError(_("Error loading statistics total!"));
		}
	}
	
	tmStatsData myDataActual;
	if (m_IsStarted==true) {
		myRecord.Load(m_StatBufferData.m_Id, myDataActual);
	}
	
	tmStats_DLG myDlg (parent, &myDataActual, &myDataTotal, myRecordNb);
	myDlg.SetStarted(m_IsStarted);
	
	int myReturnCode = myDlg.ShowModal();
	switch (myReturnCode) {
		case wxID_CANCEL:
			break;
			
		case wxID_EXECUTE:
			_StartRecord();
			break;
			
		case wxID_STOP:
			_StopRecord();
			break;
			
		default:
			wxLogError("Unsupported return code from Statistics dialog!");
			break;
	}
}














BEGIN_EVENT_TABLE(tmStats_DLG, wxDialog)
	EVT_BUTTON (wxID_EXECUTE, tmStats_DLG::OnRecordStart)
	EVT_BUTTON (wxID_STOP, tmStats_DLG::OnRecordStop)
	EVT_BUTTON (wxID_SAVE, tmStats_DLG::OnExport)
END_EVENT_TABLE()


void tmStats_DLG::OnRecordStart(wxCommandEvent & event) {
	EndModal(wxID_EXECUTE);
}



void tmStats_DLG::OnRecordStop(wxCommandEvent & event) {
	EndModal(wxID_STOP);
}



void tmStats_DLG::OnExport(wxCommandEvent & event) {
	wxLogError("Not implemented now!");
}



void tmStats_DLG::_CreateControls() {
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_SessionSizerCtrl = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Actual session") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText1;
	m_staticText1 = new wxStaticText( this, wxID_ANY, _("Mouse clicks:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxEXPAND, 5 );
	
	m_SessionClickCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SessionClickCtrl->Wrap( -1 );
	fgSizer1->Add( m_SessionClickCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText3;
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Attributions:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxEXPAND, 5 );
	
	m_SessionAttribCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SessionAttribCtrl->Wrap( -1 );
	fgSizer1->Add( m_SessionAttribCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText9;
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Intersections:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer1->Add( m_staticText9, 0, wxALL, 5 );
	
	m_SessionIntersectCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SessionIntersectCtrl->Wrap( -1 );
	fgSizer1->Add( m_SessionIntersectCtrl, 0, wxALL, 5 );
	
	wxStaticText* m_staticText11;
	m_staticText11 = new wxStaticText( this, wxID_ANY, _("Elapsed time:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer1->Add( m_staticText11, 0, wxALL, 5 );
	
	m_SessionElapsedTimeCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SessionElapsedTimeCtrl->Wrap( -1 );
	fgSizer1->Add( m_SessionElapsedTimeCtrl, 0, wxALL, 5 );
	
	m_SessionSizerCtrl->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	bSizer2->Add( m_SessionSizerCtrl, 1, wxALL, 5 );
	
	m_TotalSizerCtrl = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, "" ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText12;
	m_staticText12 = new wxStaticText( this, wxID_ANY, _("Mouse clicks:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer11->Add( m_staticText12, 0, wxALL|wxEXPAND, 5 );
	
	m_TotalClickCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TotalClickCtrl->Wrap( -1 );
	fgSizer11->Add( m_TotalClickCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText31;
	m_staticText31 = new wxStaticText( this, wxID_ANY, _("Attributions:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	fgSizer11->Add( m_staticText31, 0, wxALL|wxEXPAND, 5 );
	
	m_TotalAttribCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TotalAttribCtrl->Wrap( -1 );
	fgSizer11->Add( m_TotalAttribCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText91;
	m_staticText91 = new wxStaticText( this, wxID_ANY, _("Intersections:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	fgSizer11->Add( m_staticText91, 0, wxALL, 5 );
	
	m_TotalIntersectCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TotalIntersectCtrl->Wrap( -1 );
	fgSizer11->Add( m_TotalIntersectCtrl, 0, wxALL, 5 );
	
	wxStaticText* m_staticText111;
	m_staticText111 = new wxStaticText( this, wxID_ANY, _("Elapsed time:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	fgSizer11->Add( m_staticText111, 0, wxALL, 5 );
	
	m_TotalElapsedTimeCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TotalElapsedTimeCtrl->Wrap( -1 );
	fgSizer11->Add( m_TotalElapsedTimeCtrl, 0, wxALL, 5 );
	
	m_TotalSizerCtrl->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	bSizer2->Add( m_TotalSizerCtrl, 1, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Recording") ), wxHORIZONTAL );
	
	m_BtnStartCtrl = new wxButton( this, wxID_EXECUTE, _("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( m_BtnStartCtrl, 0, wxALL, 5 );
	
	m_BtnStopCtrl = new wxButton( this, wxID_STOP, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( m_BtnStopCtrl, 0, wxALL, 5 );
	
	bSizer1->Add( sbSizer6, 0, wxEXPAND|wxALL, 5 );
	
	
	bSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_BtnExportCtrl = new wxButton( this, wxID_SAVE, _("Export..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_BtnExportCtrl, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	m_BtnCloseCtrl = new wxButton( this, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	m_BtnCloseCtrl->SetDefault(); 
	bSizer3->Add( m_BtnCloseCtrl, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer3, 0, wxALL|wxEXPAND|wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	this->Centre( wxBOTH );
}




tmStats_DLG::tmStats_DLG(wxWindow * parent,
						 const tmStatsData * actual,
						 const tmStatsData * total,
						 long nbrecords,
						 wxWindowID id, const wxString & title,
						 const wxPoint & pos, const wxSize & size, long style) :
wxDialog(parent, id, title, pos, size,style) {
	m_DataActual = actual;
	m_DataTotal = total;
	m_DataTotalRecord = nbrecords;
	_CreateControls();
	_UpdateControls();
}



tmStats_DLG::~tmStats_DLG(){
}


void tmStats_DLG::SetStarted(bool recordstarted){
	if (recordstarted == true) {
		m_BtnStartCtrl->Enable(false);
	}
	else {
		m_BtnStopCtrl->Enable(false);
	}
}



void tmStats_DLG::_UpdateControls() {
	if (m_DataActual != NULL && m_DataActual->IsOk()) {
		m_SessionClickCtrl->SetLabel(wxString::Format(_T("%ld"),m_DataActual->m_NbClick));
		m_SessionAttribCtrl->SetLabel(wxString::Format(_T("%ld"), m_DataActual->m_NbAttribution));
		m_SessionIntersectCtrl->SetLabel(wxString::Format(_T("%ld"),m_DataActual->m_NbIntersection));
		m_SessionElapsedTimeCtrl->SetLabel(m_DataActual->m_TimeElapsed.Format());
	}
	
	m_TotalSizerCtrl->GetStaticBox()->SetLabel(wxString::Format(_("Total (%ld sessions)"),m_DataTotalRecord));
	if (m_DataTotal != NULL) {
		m_TotalClickCtrl->SetLabel(wxString::Format(_T("%ld"),m_DataTotal->m_NbClick));
		m_TotalAttribCtrl->SetLabel(wxString::Format(_T("%ld"), m_DataTotal->m_NbAttribution));
		m_TotalIntersectCtrl->SetLabel(wxString::Format(_T("%ld"),m_DataTotal->m_NbIntersection));
		m_TotalElapsedTimeCtrl->SetLabel(m_DataTotal->m_TimeElapsed.Format());
	}
	
	this->Layout();
	GetSizer()->Fit( this );
	this->Centre( wxBOTH );
}

