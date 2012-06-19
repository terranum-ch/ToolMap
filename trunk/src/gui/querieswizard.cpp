/***************************************************************************
								querieswizard.cpp
                    Main part of the new query system
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

#include "querieswizard.h"
#include "../database/database_tm.h"








int QueriesWizard::ShowWizard() {
	if (RunWizard(m_PageIntro)==true){
		return wxID_OK;
	}
	else {
		return wxID_CANCEL;
	}

}



QueriesWizard::QueriesWizard(wxWindow * parent, DataBaseTM * database, int id) :
wxWizard(parent, id, _("Query Wizard"), wxNullBitmap, wxDefaultPosition){
	m_pDB = database;
	wxASSERT(m_pDB);
	m_PageIntro = new QueriesPageIntro(this, m_pDB);
	
	m_QueryData = new QueriesData();
	
	GetPageAreaSizer()->Add(m_PageIntro);
	wxSize mySize = GetPageAreaSizer()->CalcMin();
	SetMinSize(mySize);
	SetPageSize(mySize);
}


QueriesWizard::~QueriesWizard() {
	delete m_QueryData;
	this->Destroy();
}
