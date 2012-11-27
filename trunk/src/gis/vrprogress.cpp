/***************************************************************************
				vrprogress.cpp
                    
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

#include "vrprogress.h"

vrProgress::vrProgress() {
}

vrProgress::~vrProgress() {
}




int CPL_STDCALL GDALUpdateSimple(double dfComplete, const char * pszMessage, void * pProgressArg)
{
	wxASSERT(pProgressArg);
	vrProgressSimple * mypSimple = (vrProgressSimple*) pProgressArg;
	mypSimple->GetPercent()->SetValue(dfComplete * 100.0);
	
	mypSimple->UpdateProgress();
	if (mypSimple->GetContinue() == false) {
		return FALSE;
	}
	return TRUE;
}



vrProgressSimple::vrProgressSimple(wxWindow * parent, wxString title, wxString message) {
	m_ProgressWnd = new wxProgressDialog(title, message, 100, parent,
									 wxPD_AUTO_HIDE|wxPD_APP_MODAL|wxPD_CAN_ABORT);
	m_Continue = true;
	m_Percent.Create(100);
}



vrProgressSimple::~vrProgressSimple() {
	wxDELETE(m_ProgressWnd);
}



void vrProgressSimple::UpdateProgress() {
	wxASSERT(m_ProgressWnd);
	if (m_Percent.IsNewStep()) {
		wxLogMessage("percent : %d", m_Percent.GetPercent());
		m_Continue = m_ProgressWnd->Update(m_Percent.GetPercent());
	}
}


