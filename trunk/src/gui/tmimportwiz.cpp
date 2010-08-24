/***************************************************************************
 tmimportwiz.cpp
 
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


#include "tmimportwiz.h"
#include "../gis/tmimport.h"
#include "tmimportwizpages.h"

ImportWizard::ImportWizard(wxWindow * window, wxWindowID id) : 
wxWizard(window, id, _("Import data")) {
	m_Import = NULL;
	m_PageInfo = new ImportWizInfo(this);
	m_PageIntro = new ImportWizIntro(this, m_PageInfo);
	m_PageInfo->SetPreviousPage(m_PageIntro);
	
	GetPageAreaSizer()->Add(m_PageIntro);
	wxSize mySize = GetPageAreaSizer()->CalcMin();
	SetMinSize(mySize);
	SetPageSize(mySize);
}



ImportWizard::~ImportWizard() {
	wxDELETE(m_Import);
	this->Destroy();
}



int ImportWizard::ShowWizard() {
	if (RunWizard(m_PageIntro)==true){
		return wxID_OK;
	}
	else {
		return wxID_CANCEL;
	}	
}


void ImportWizard::SetEnableControl(wxWindowID id, bool status) {
	wxWindow * myWnd = FindWindowById(id);
	if (myWnd) {
		myWnd->Enable(status);
	}
}


void ImportWizard::SetImport(tmImport * value) {
  m_Import = value;
}

